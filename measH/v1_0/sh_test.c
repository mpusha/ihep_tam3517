#include "/usr/usera/voevodin/EqContStation/sh_ccydalib/sh_ccydalib.h"

static struct itimerval newval;
static sigset_t maska;
static struct sigaction xa;

// данные измеряемые
static double field[2];     // поле измеренное
static char   state[2];     // ссылки на O.K., Синхр, Приём, Измер

// данные управляющие
static double tim_[2], cur_tim_[2];      // время в секундах
static char   puls[2], cur_puls[2];      // стартовый импульс

//
static struct k_opis {
   char r_name[20];
   unsigned short n_tup;    // число строк
   unsigned short n_attr;   // число столбцов
   unsigned short tup_len;  // длина строки
   unsigned short last_act; // последнее действие 0 - чтение, 0xcccc - запись связной, 0xaaaa - запись управляющей
  } buf;
static short i, coord[4], was_err; // was_err - код ошибки внутри большого цикла
static char *tb_names[]={
	"M_PD_ECS_MF_MEAS",     // 2 параметра измерения 
	"M_PD_ECS_MF_CNTR"      // 2 параметра управления
      };
#define num_of_tabs sizeof(tb_names)/sizeof(tb_names[0])

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

void read_controls()  // чтение столбцов таблицы M_PD_ECS_MF_CNTR
  {
//   время в секундах
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=1;  // первый столбец
   coord[3]=1;  // последний столбец
   i=sh_drtb((char *)cur_tim_ ,sizeof(cur_tim_), coord, tab_ids[1]);  // M_PD_ECS_MF_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("1.Ошибка %x чтения таблицы M_PD_ECS_MF_CNTR\n",i);
      exit(0);
     }
//   стартовый импульс
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=2;  // первый столбец
   coord[3]=2;  // последний столбец
   i=sh_drtb((char *)cur_puls ,sizeof(cur_puls), coord, tab_ids[1]);  // M_PD_ECS_MF_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("2.Ошибка %x чтения таблицы M_PD_ECS_MF_CNTR\n",i);
      exit(0);
     }
  }

main()
  {
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
         printf("Ошибка %x открытия таблицы %s\n",i,tb_names[j]);
         continue;
        }
      tab_ids[j]=i;
     }
//  однократная инициализация измеряемых данных
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=1;  // первый столбец
   coord[3]=1;  // последний столбец

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
//
   read_controls();
   memcpy(tim_, cur_tim_, sizeof(cur_tim_));
   memcpy(puls , cur_puls, sizeof(cur_puls));

//  открыты все таблицы, управляющие данные прочитаны
//  ЗДЕСЬ надо прописать аппаратуру последними данными, которые были сохранены в БД

   was_err=0;

   up_count=sh_get_count(); // циклический счётчик действий сверху
   Nc=0;
   sh_put_mess("Тестовая задача sh_test начинает работу");
   while(1)                 // большой постоянно действующий цикл !!!
    {
     was_err=0;
     time_wait(300);        // ~33 раза за 10 секунд
     j=sh_get_count();      // циклический счётчик действий сверху
     if(j!=up_count)        // есть новые данные сверху
      {
       up_count=j;
       read_controls();     // прочитал все управляющие данные в cur_***
       for(j=0;j<2;j++)     // для 2-x значений
         {
          if(cur_tim_[j] != tim_[j])
            {
             printf("Управление %d: время было %lg стало %lg\n", j+1,tim_[j], cur_tim_[j]);
             tim_[j]=cur_tim_[j];
            }
          if(cur_puls[j] != puls[j])
            {
             printf("Управление  %d: стартовый импульс был %d стал %d\n", j+1, puls[j], cur_puls[j]);
             puls[j]=cur_puls[j];
            }
         }

// здусь нужна проверка введённых данных 

       if(!was_err)
        sh_put_data_er(0);  // подтверждение правильности введённых данных
      }

    }

  }

