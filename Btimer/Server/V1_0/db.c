
#include "../ccydalib/sh_ccydalib.h"
#include "db.h"

static struct itimerval newval;
static sigset_t maska;
static struct sigaction xa;


struct Btimer cur_par, write_par;

struct k_opis buf;
static short i, coord[4], was_err,first_run; // was_err - код ошибки внутри большого цикла

static short tab_ids[num_of_tabs];
static unsigned int j, up_count;

static int time_wait(int ms)              // задержка в миллисекундах
{
  int sig;
  newval.it_value.tv_sec=ms/1000;        // секунды
  newval.it_value.tv_usec=ms%1000*1000;  // микросекунды
  if(setitimer(ITIMER_REAL,&newval,0)) {
      perror("setitimer");
      return(1);
    }
  if(sigwait(&maska,&sig)) {
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

void set_state(short my_cy)
{
//    Статус системы

  coord[0]=1;  // первая строка
  coord[1]=1;  // последняя строка
  coord[2]=6;  // первый столбец
  coord[3]=6;  // последний столбец
  if(my_cy) b_status=0x81;else b_status=0; // 0 -Норма, 0x81 - Ошибка арасным цветом
  i=sh_dwtb(&b_status, sizeof(b_status), coord, tab_ids[0]);  // M_PD_ECS_BTIM_MEAS
  if(i<0) {
    sh_put_data_er(i);
    printf("5.Ошибка %x записи в таблицу M_PD_ECS_BTIM_MEAS\n",i);
    was_err=i;
  }
  if(!was_err)
    sh_put_meas_er(0);     // сообщаю о нормальном состоянии системы
  else
    sh_put_meas_er(was_err);
}
int read_controls()  // чтение столбцов таблицы M_PD_ECS_BTIM_CNTR
{
  int db_status=0;
//   поле в каналах В-таймера
  coord[0]=1;  // первая строка
  coord[1]=6;  // последняя строка
  coord[2]=1;  // первый столбец
  coord[3]=1;  // последний столбец
  i=sh_drtb((char *)cur_par.fBt ,sizeof(cur_par.fBt), coord, tab_ids[1]);  //  M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("1.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=2;
  }
//   поле привязка
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=2;  // первый столбец
  coord[3]=2;  // последний столбец
  i=sh_drtb((char *)cur_par.fB1_2 ,sizeof(cur_par.fB1_2 ), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("2.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=4;
  }
//   поле промежуточный стол
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=3;  // первый столбец
  coord[3]=3;  // последний столбец
  i=sh_drtb((char *)cur_par.fBIMiddle ,sizeof(cur_par.fBIMiddle), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("3.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=8;
  }
//   поле основной стол
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=4;  // первый столбец
  coord[3]=4;  // последний столбец
  i=sh_drtb((char *)cur_par.fBITop ,sizeof(cur_par.fBITop), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("4.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=16;
  }
//   состояние каналов В-таймера
  coord[0]=1;  // первая строка
  coord[1]=6;  // последняя строка
  coord[2]=5;  // первый столбец
  coord[3]=5;  // последний столбец
  i=sh_drtb(cur_par.onOffBt ,sizeof(cur_par.onOffBt), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("5.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=32;
  }
//  состояние поле привязка
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=6;  // первый столбец
  coord[3]=6;  // последний столбец
  i=sh_drtb(cur_par.onOffB1_2 ,sizeof(cur_par.onOffB1_2), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("6.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=64;
  }
//   состояние промежуточный стол
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=7;  // первый столбец
  coord[3]=7;  // последний столбец
  i=sh_drtb(cur_par.onOffBIMiddle ,sizeof(cur_par.onOffBIMiddle), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("7.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=128;
  }
//   состояние основной стол
  coord[0]=1;  // первая строка
  coord[1]=2;  // последняя строка
  coord[2]=8;  // первый столбец
  coord[3]=8;  // последний столбец
  i=sh_drtb(cur_par.onOffBITop ,sizeof(cur_par.onOffBITop), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
  if(i<0) {
    sh_put_data_er(i);
    printf("8.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
    db_status|=256;
  }
  return(db_status);
}

int db_init(void)
{
  int db_status=0;
  
  newval.it_interval.tv_sec=0;
  newval.it_interval.tv_usec=0;
  newval.it_value.tv_sec=0;
  newval.it_value.tv_usec=0;
  if(setitimer(ITIMER_REAL,&newval,0)) {
    syslog(LOG_NOTICE,":test:setitimer:%s\n",strerror(errno));
    perror("setitimer");
	sleep(1);
    return(EXIT_FAILURE);
  }
  if(sigemptyset(&maska))          // SIGALRM используется с таймерами
  {
    syslog(LOG_NOTICE,":test:sigemptyset:%s\n",strerror(errno));
    perror("sigemptyset");
    sleep(1);
    return(EXIT_FAILURE);
  }
  if(sigaddset(&maska,SIGALRM))
  {
    syslog(LOG_NOTICE,":test:sigaddset:%s\n",strerror(errno));
    perror("sigaddset");
	sleep(1);
    return(EXIT_FAILURE);
  } 
  xa.sa_handler=handler;
  xa.sa_flags=SA_RESTART;
  if(sigaction(SIGALRM,&xa,NULL))  // set handler
  {
    syslog(LOG_NOTICE,":test:sigaction:%s\n",strerror(errno));
    perror("sigaction");
    sleep(1);
    return(EXIT_FAILURE);
  }

  if(i=API_ECSini())  // инициализация связи со связной программой
  {
    printf("Error in call API_ECSini = %d\n", i);
    if(i=API_ECSini()) {
       syslog(LOG_NOTICE,":test:Second error in call API_ECSini = %d\n", i);
       printf("Second erro rin call API_ECSini = %d\n", i);
       sleep(1);
       return(EXIT_FAILURE);
    }
  }
  for(j=0;j<num_of_tabs;j++)  // открыть все таблицы
  {
    i=sh_optb(tb_names[j]);
    if(i<0) {
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
  //  тексты по 8 байт
     coord[2]=1;  // первый столбец
     coord[3]=1;  // последний столбец
     for(j=0;j< sizeof(b_tim);j++)
       {
        coord[0]=coord[1]=j+1;  // первая/последняя строка
        i=sh_dwtb(b_comm[j] , 8, coord, tab_ids[0]);  // M_PD_ECS_BTIM_MEAS
        if(i<0)
          {
           sh_put_data_er(i);
           printf("Ошибка %x записи в таблицу M_PD_ECS_BTIM_MEAS j=%d\n",i,j);
           exit(0);
          }
       }
  //   ссылки на имена каналов
     coord[0]=1;  // первая строка
     coord[1]=6;  // последняя строка
     coord[2]=2;  // первый столбец
     coord[3]=2;  // последний столбец
     for(i=0;i< sizeof(b_tim);i++)
       b_tim[i]=i;
     i=sh_dwtb(b_tim ,sizeof(b_tim), coord, tab_ids[0]);  // M_PD_ECS_BTIM_MEAS
     if(i<0)
       {
        sh_put_data_er(i);
        printf("1.Ошибка %x записи в таблицу M_PD_ECS_BTIM_MEAS\n",i);
        exit(0);
       }
  //   ссылки на имена B1, B2
     coord[0]=1;  // первая строка
     coord[1]=2;  // последняя строка
     coord[2]=3;  // первый столбец
     coord[3]=3;  // последний столбец
     b_b1_b2[0]=6;
     b_b1_b2[1]=7;
     i=sh_dwtb(b_b1_b2 ,sizeof(b_b1_b2), coord, tab_ids[0]);  // M_PD_ECS_BTIM_MEAS
     if(i<0)
       {
        sh_put_data_er(i);
        printf("2.Ошибка %x записи в таблицу M_PD_ECS_BTIM_MEAS\n",i);
        exit(0);
       }
  //   ссылки на имена 1 B, I
     coord[0]=1;  // первая строка
     coord[1]=2;  // последняя строка
     coord[2]=4;  // первый столбец
     coord[3]=4;  // последний столбец
     b_bimf_1[0]=8;
     b_bimf_1[1]=9;
     i=sh_dwtb(b_bimf_1, sizeof(b_bimf_1), coord, tab_ids[0]);  // M_PD_ECS_BTIM_MEAS
     if(i<0)
       {
        sh_put_data_er(i);
        printf("3.Ошибка %x записи в таблицу M_PD_ECS_BTIM_MEAS\n",i);
        exit(0);
       }
  //   ссылки на имена 2 B, I
     coord[0]=1;  // первая строка
     coord[1]=2;  // последняя строка
     coord[2]=5;  // первый столбец
     coord[3]=5;  // последний столбец
     b_bimf_2[0]=10;
     b_bimf_2[1]=11;
     i=sh_dwtb(b_bimf_2, sizeof(b_bimf_2), coord, tab_ids[0]);  // M_PD_ECS_BTIM_MEAS
     if(i<0)
       {
        sh_put_data_er(i);
        printf("4.Ошибка %x записи в таблицу M_PD_ECS_BTIM_MEAS\n",i);
        exit(0);
       }
  //    Статус системы
     coord[0]=1;  // первая строка
     coord[1]=1;  // последняя строка
     coord[2]=6;  // первый столбец
     coord[3]=6;  // последний столбец
     b_status=0x81;   // 0 -Норма, 0x81 - Ошибка арасным цветом
     i=sh_dwtb(&b_status, sizeof(b_status), coord, tab_ids[0]);  // M_PD_ECS_BTIM_MEAS
     if(i<0)
       {
        sh_put_data_er(i);
        printf("5.Ошибка %x записи в таблицу M_PD_ECS_BTIM_MEAS\n",i);
        exit(0);
       }

    sh_put_meas_er(0);  // подтверждение обновления измерений в БД
*/
// end comments of one time initialisation
// -----------------------------------------------------------------------------------------------------
  db_status|=read_controls() ;

//  открыты все таблицы, управляющие данные прочитаны --------------------------------------------------
//  ЗДЕСЬ надо прописать аппаратуру последними данными, которые были сохранены в БД

  first_run=1;
  write_par=cur_par;
  up_count=sh_get_count(); // циклический счётчик действий сверху
  if(db_status==0) sh_put_data_er(0); //error not found
//  sh_put_mess("Задача BtimerU70 начинает работу");
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
  time_wait(300);        // ~33 раза за 10 секунд
  j=sh_get_count();      // циклический счётчик действий сверху
  if(j!=up_count) {      // есть новые данные сверху
    up_count=j;
    int db_errcode;
    if((db_errcode=read_controls())) { // прочитал все управляющие данные в cur_par
      printf("Ошибка %x чтения таблицы M_PD_ECS_TIMIT\n",db_errcode);
      return(0);    // Ошибка БД - это криминал don't write data into HW
    }
    for(j=0;j<6;j++) {     // для 6-ти значений
      if(cur_par.fBt[j] != write_par.fBt[j]) {
        printf("В-таймер: канал %d: поле было %lg стало %lg\n", j+1,write_par.fBt[j], cur_par.fBt[j]);
        write_par.fBt[j]=cur_par.fBt[j];
        *update|=(1<<j);
        was_change=1;
      }
      if(cur_par.onOffBt[j] != write_par.onOffBt[j]) {
        printf("В-таймер: канал %d: состояние было %d стало %d\n", j+1,write_par.onOffBt[j], cur_par.onOffBt[j]);
        write_par.onOffBt[j]=cur_par.onOffBt[j];
        *update|=(1<<(j+6));
        was_change=1;
      }
    }
    for(j=0;j<2;j++) {    // для 2-x значений
      if(cur_par.fB1_2[j] != write_par.fB1_2[j]) {
        printf("Привязка: столбец %d: поле было %lg стало %lg\n", j+1, write_par.fB1_2[j], cur_par.fB1_2[j]);
        write_par.fB1_2[j]=cur_par.fB1_2[j];
        *update|=(1<<(j+12));
        was_change=1;
      }
      if(cur_par.onOffB1_2[j] != write_par.onOffB1_2[j]) {
        printf("Привязка: столбец %d: состояние было %d стало %d\n", j+1, write_par.onOffB1_2[j], cur_par.onOffB1_2[j]);
        write_par.onOffB1_2[j]=cur_par.onOffB1_2[j];
        *update|=(1<<(j+14));
        was_change=1;
      }
      if(cur_par.fBIMiddle[j] != write_par.fBIMiddle[j]) {
        printf("Промежуточный стол: столбец %d: поле было %lg стало %lg\n", j+1, write_par.fBIMiddle[j], cur_par.fBIMiddle[j]);
        write_par.fBIMiddle[j]=cur_par.fBIMiddle[j];
        *update|=(1<<(j+16));
        was_change=1;
      }
      if(cur_par.onOffBIMiddle[j] != write_par.onOffBIMiddle[j]) {
        printf("Промежуточный стол: столбец %d: состояние было %d стало %d\n", j+1, write_par.onOffBIMiddle[j], cur_par.onOffBIMiddle[j]);
        write_par.onOffBIMiddle[j]=cur_par.onOffBIMiddle[j];
        *update|=(1<<(j+18));
        was_change=1;
      }
      if(cur_par.fBITop[j] != write_par.fBITop[j]) {
        printf("Верхний стол: столбец %d: поле было %lg стало %lg\n", j+1, write_par.fBITop[j], cur_par.fBITop[j]);
        write_par.fBITop[j]=cur_par.fBITop[j];
        *update|=(1<<(j+20));
        was_change=1;
      }
      if(cur_par.onOffBITop[j] != write_par.onOffBITop[j]) {
        printf("Верхний стол: столбец %d: состояние было %d стало %d\n", j+1, write_par.onOffBITop[j], cur_par.onOffBITop[j]);
        write_par.onOffBITop[j]=cur_par.onOffBITop[j];
        *update|=(1<<(j+22));
        was_change=1;
      }
    }
 // здесь нужна проверка введённых данных
    sh_put_data_er(0);  // подтверждение правильности введённых данных
  }
  return(was_change);
}	
