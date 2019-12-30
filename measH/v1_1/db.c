#include "../ccydalib/sh_ccydalib.h"
#include "db.h"

static struct itimerval newval;
static sigset_t maska;
static struct sigaction xa;
struct k_opis buf;
struct measSetData write_par,cur_par;
static short i, coord[4], was_err,first_run; // was_err - код ошибки внутри большого цикла
static short tab_ids[num_of_tabs];
static unsigned int j, Nc, up_count;

static int time_wait(int ms)              // задержка в миллисекундах
  {
  int sig;
   newval.it_value.tv_sec=ms/1000;        // секунды
   newval.it_value.tv_usec=ms%1000*1000;  // микросекунды
   if(setitimer(ITIMER_REAL,&newval,0))
     {
      perror("setitimer");
      return(1);
     }
   if(sigwait(&maska,&sig))
     {
      perror("sigwait");
      return(1);
     }
   newval.it_value.tv_sec=0;
   newval.it_value.tv_usec=0;
   if(setitimer(ITIMER_REAL,&newval,0))  // стоп таймер
     {
      perror("setitimer2");
      return(1);
     }
   return(0);
  }

static void handler(int a)
  {
   switch (a)
     {
      case SIGALRM:
      break;
     }
  }

int read_controls()  // чтение столбцов таблицы M_PD_ECS_MF_CNTR
{
  int db_status=0;
//   время в секундах
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=1;  // первый столбец
  coord[3]=1;  // последний столбец
  i=sh_drtb((char *)cur_par.tim ,sizeof(cur_par.tim), coord, tab_ids[1]);  // M_PD_ECS_MF_CNTR
  if(i<0)
  {
    sh_put_data_er(i);
    printf("1.Ошибка %x чтения таблицы M_PD_ECS_MF_CNTR\n",i);
    db_status|=2;
  }
//   стартовый импульс
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=2;  // первый столбец
  coord[3]=2;  // последний столбец
  i=sh_drtb((char *)cur_par.pulseStart ,sizeof(cur_par.pulseStart), coord, tab_ids[1]);  // M_PD_ECS_MF_CNTR
  if(i<0)
  {
    sh_put_data_er(i);
    printf("2.Ошибка %x чтения таблицы M_PD_ECS_MF_CNTR\n",i);
    db_status|=4;
  }
  return(db_status);
}
// write values and status into table num - number of meas 1,2
void set_state(unsigned int num, double *field, char *state)
{
  // первый столбец feald первая строка первое измерение, вторая строка второе измерение
  // второй столбец status первая строка первое измерение, вторая строка второе измерение

  coord[0]=num;  // первая строка
  coord[1]=num;  // последняя строка
  coord[2]=1;    // первый столбец
  coord[3]=1;    // последний столбец
  was_err=0;
// write field
  i=sh_dwtb((char *)field , sizeof(*field), coord, tab_ids[0]);  // M_PD_ECS_MF_MEAS
  if(i<0) {
    sh_put_data_er(i);
    printf("Ошибка %x записи поля в таблицу M_PD_ECS_MF_MEAS\n",i);
    was_err=i;
  }
// write status
  coord[0]=num;  // первая строка
  coord[1]=num;  // последняя строка
  coord[2]=2;    // первый столбец
  coord[3]=2;    // последний столбец

  i=sh_dwtb(state, sizeof(state), coord, tab_ids[0]);  // M_PD_ECS_MF_MEAS
  if(i<0) {
    sh_put_data_er(i);
    printf("Ошибка %x записи в таблицу M_PD_ECS_MF_MEAS\n",i);
    was_err=i;
  }

  if(!was_err)
    sh_put_meas_er(0);      // подтверждение обновления измерений в БД
  else
    sh_put_meas_er(was_err);
}

int db_init(void)
{
  int db_status=0;
  newval.it_interval.tv_sec=0;
  newval.it_interval.tv_usec=0;
  newval.it_value.tv_sec=0;
  newval.it_value.tv_usec=0;
  if(setitimer(ITIMER_REAL,&newval,0))
  {
    syslog(LOG_NOTICE,":test:setitimer:%s\n",strerror(errno));
    perror("setitimer");
    exit(0);
  }
  if(sigemptyset(&maska))          // SIGALRM используется с таймерами
  {
    syslog(LOG_NOTICE,":test:sigemptyset:%s\n",strerror(errno));
    perror("sigemptyset");
    exit(0);
  }
  if(sigaddset(&maska,SIGALRM))
  {
    syslog(LOG_NOTICE,":test:sigaddset:%s\n",strerror(errno));
    perror("sigaddset");
    exit(0);
  }
  xa.sa_handler=handler;
  xa.sa_flags=SA_RESTART;
  if(sigaction(SIGALRM,&xa,NULL))  // set handler
  {
    syslog(LOG_NOTICE,":test:sigaction:%s\n",strerror(errno));
    perror("sigaction");
    exit(0);
  }
  if(i=API_ECSini())  // инициализация связи со связной программой
  {
    printf("Ошибка при вызове API_ECSini = %d\n", i);
    if(i=API_ECSini())
    {
      syslog(LOG_NOTICE,":test:Вторая ошибка при вызове API_ECSini = %d\n", i);
      printf("Вторая ошибка при вызове API_ECSini = %d\n", i);
      exit(0);
    }
  }
  for(j=0;j<num_of_tabs;j++)  // открыть все таблицы
  {
    i=sh_optb(tb_names[j]);
    if(i<0)
    {
      tab_ids[j]=0;
      sh_put_meas_er(i);   // код ошибки
      sh_put_data_er(i);
      syslog(LOG_NOTICE,"Ошибка %x открытия таблицы %s\n",i,tb_names[j]);
      printf("Ошибка %x открытия таблицы %s\n",i,tb_names[j]);
      db_status|=1;
      continue;
    }
    tab_ids[j]=i;
  }
  //  однократная инициализация измеряемых данных
/*
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=1;  // первый столбец
  oord[3]=1;  // последний столбец

  field[0]=323.4;
  field[1]=345.6;
  i=sh_dwtb((char *)field , sizeof(field), coord, tab_ids[0]);  // M_PD_ECS_MF_MEAS
  if(i<0)
  {
    sh_put_data_er(i);
    printf("Ошибка %x записи поля в таблицу M_PD_ECS_MF_MEAS\n",i);
    exit(0);
  }
  coord[2]=2;  // первый столбец
  coord[3]=2;  // последний столбец

  state[0]=0x2;//0x83;
  state[1]=1;
  i=sh_dwtb(state, sizeof(state), coord, tab_ids[0]);  // M_PD_ECS_MF_MEAS
  if(i<0)
  {
    sh_put_data_er(i);
    printf("Ошибка %x записи в таблицу M_PD_ECS_MF_MEAS\n",i);
    exit(0);
  }
  sh_put_meas_er(0);  // подтверждение обновления измерений в БД
*/

  db_status|=read_controls() ;
  write_par=cur_par;
  //memcpy(tim_, cur_tim_, sizeof(cur_tim_));
  //memcpy(puls , cur_puls, sizeof(cur_puls));

  //  открыты все таблицы, управляющие данные прочитаны --------------------------------------------------
  //  ЗДЕСЬ надо прописать аппаратуру последними данными, которые были сохранены в БД

  was_err=0;
  Nc=0;
  first_run=1;

  up_count=sh_get_count(); // циклический счётчик действий сверху
  if(db_status==0) sh_put_data_er(0); //error not found
  //  sh_put_mess("Задача measH начинает работу");
  return(db_status);
}

int db_process(unsigned int *update)
{
  int was_change=first_run;
  *update=0;

  if(first_run){
    *update=0xffffff; //update all parameters of btimer
  }
  first_run=0;
  was_err=0;
  //time_wait(300);        // ~33 раза за 10 секунд
  j=sh_get_count();      // циклический счётчик действий сверху
  if(j!=up_count)        // есть новые данные сверху
   {
    up_count=j;
    int db_errcode;
    if((db_errcode=read_controls())) { // прочитал все управляющие данные в cur_***
      printf("Ошибка %x чтения таблицы M_PD_ECS_TIMIT\n",db_errcode);
      return(0);    // Ошибка БД - это криминал don't write data into HW
    }
    for(j=0;j<2;j++)     // для 2-x значений
      {
       if(cur_par.tim[j] != write_par.tim[j])
         {
          printf("Управление %d: время было %lg стало %lg\n", j+1,write_par.tim[j], cur_par.tim[j]);
          write_par.tim[j]=cur_par.tim[j];
          *update|=(1<<(j));
          was_change=1;
         }
       if(cur_par.pulseStart[j] != write_par.pulseStart[j])
         {
          printf("Управление  %d: стартовый импульс был %d стал %d\n", j+1, write_par.pulseStart[j],cur_par.pulseStart[j]);
          write_par.pulseStart[j]=cur_par.pulseStart[j];
          *update|=(1<<(j+2));
          was_change=1;
         }
      }

// здесь нужна проверка введённых данных

    if(!was_err)
     sh_put_data_er(0);  // подтверждение правильности введённых данных
   }
  return(was_change);
}
