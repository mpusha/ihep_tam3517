#include "ccydalib/sh_ccydalib.h"

static struct itimerval newval;
static sigset_t maska;
static struct sigaction xa;

// данные измеряемые
static char b_tim[6];     // ссылки на имена каналов 0-5
static char *b_comm[]={   // комментарии до 8 байт текста
       "comment1",
       "comment2",
       "comment3",
       "comment4",
       "comment5",
       "comment6"
      };
static char b_b1_b2[2];   // ссылки на B1, B2
static char b_bimf_1[2];  // ссылки на B, I middle flat
static char b_bimf_2[2];  // ссылки на B, I top flat
static char b_status;     // ссылка на норма/ошибка

// данные управляющие
static double f_b_tim[6], cur_f_b_tim[6];  // поле в 6-ти каналах В-таймера
static double b_1_2[2], cur_b_1_2[2];      // поле B1, B2
static double b_bi_1[2], cur_b_bi_1[2];    // поле B, I middle flat
static double b_bi_2[2], cur_b_bi_2[2];    // поле B, I top flat
static char s_b_tim[6], cur_s_b_tim[6];    // состояние в 6-ти каналах В-таймера
static char s_b_1_2[2], cur_s_b_1_2[2];    // состояние B1, B2
static char s_b_bi_1[2], cur_s_b_bi_1[2];  // состояние B, I middle flat
static char s_b_bi_2[2], cur_s_b_bi_2[2];  // состояние B, I top flat

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
	"M_PD_ECS_BTIM_MEAS",   // 6 параметров измерения 
	"M_PD_ECS_BTIM_CNTR"    // 8 параметров управления
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

void read_controls()  // чтение столбцов таблицы M_PD_ECS_BTIM_CNTR
  {
//   поле в каналах В-таймера
   coord[0]=1;  // первая строка
   coord[1]=6;  // последняя строка
   coord[2]=1;  // первый столбец
   coord[3]=1;  // последний столбец
   i=sh_drtb((char *)cur_f_b_tim ,sizeof(cur_f_b_tim), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("1.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
      exit(0);
     }
//   поле привязка
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=2;  // первый столбец
   coord[3]=2;  // последний столбец
   i=sh_drtb((char *)cur_b_1_2 ,sizeof(cur_b_1_2), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("2.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
      exit(0);
     }
//   поле промежуточный стол
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=3;  // первый столбец
   coord[3]=3;  // последний столбец
   i=sh_drtb((char *)cur_b_bi_1 ,sizeof(cur_b_bi_1), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("3.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
      exit(0);
     }
//   поле основной стол
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=4;  // первый столбец
   coord[3]=4;  // последний столбец
   i=sh_drtb((char *)cur_b_bi_2 ,sizeof(cur_b_bi_2), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("4.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
      exit(0);
     }
//   состояние каналов В-таймера
   coord[0]=1;  // первая строка
   coord[1]=6;  // последняя строка
   coord[2]=5;  // первый столбец
   coord[3]=5;  // последний столбец
   i=sh_drtb(cur_s_b_tim ,sizeof(cur_s_b_tim), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("5.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
      exit(0);
     }
//   поле привязка
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=6;  // первый столбец
   coord[3]=6;  // последний столбец
   i=sh_drtb(cur_s_b_1_2 ,sizeof(cur_s_b_1_2), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("6.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
      exit(0);
     }
//   состояние промежуточный стол
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=7;  // первый столбец
   coord[3]=7;  // последний столбец
   i=sh_drtb(cur_s_b_bi_1 ,sizeof(cur_s_b_bi_1), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("7.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
      exit(0);
     }
//   состояние основной стол
   coord[0]=1;  // первая строка
   coord[1]=2;  // последняя строка
   coord[2]=8;  // первый столбец
   coord[3]=8;  // последний столбец
   i=sh_drtb(cur_s_b_bi_2 ,sizeof(cur_s_b_bi_2), coord, tab_ids[1]);  // M_PD_ECS_BTIM_CNTR
   if(i<0)
     {
      sh_put_data_er(i);
      printf("8.Ошибка %x чтения таблицы M_PD_ECS_BTIM_CNTR\n",i);
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

//
   read_controls();
   memcpy(f_b_tim, cur_f_b_tim, sizeof(cur_f_b_tim));
   memcpy(b_1_2 , cur_b_1_2, sizeof(cur_b_1_2));
   memcpy(b_bi_1 , cur_b_bi_1, sizeof(cur_b_bi_1));
   memcpy(b_bi_2 , cur_b_bi_2, sizeof(cur_b_bi_2));
   memcpy(s_b_tim , cur_s_b_tim, sizeof(cur_s_b_tim));
   memcpy(s_b_1_2 , cur_s_b_1_2, sizeof(cur_s_b_1_2));
   memcpy(s_b_bi_1 , cur_s_b_bi_1, sizeof(cur_s_b_bi_1));
   memcpy(s_b_bi_2 , cur_s_b_bi_2, sizeof(cur_s_b_bi_2));

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
       for(j=0;j<6;j++)     // для 6-ти значений
         {
          if(cur_f_b_tim[j] != f_b_tim[j])
            {
             printf("В-таймер: канал %d: поле было %lg стало %lg\n", j+1,f_b_tim[j], cur_f_b_tim[j]);
             f_b_tim[j]=cur_f_b_tim[j];
            }
          if(cur_s_b_tim[j] != s_b_tim[j])
            {
             printf("В-таймер: канал %d: состояние было %d стало %d\n", j+1, s_b_tim[j], cur_s_b_tim[j]);
             s_b_tim[j]=cur_s_b_tim[j];
            }
         }
       for(j=0;j<2;j++)     // для 2-x значений
         {
          if(cur_b_1_2[j] != b_1_2[j])
            {
             printf("Привязка: столбец %d: поле было %lg стало %lg\n", j+1, b_1_2[j], cur_b_1_2[j]);
             b_1_2[j]=cur_b_1_2[j];
            }
          if(cur_s_b_1_2[j] != s_b_1_2[j])
            {
             printf("Привязка: столбец %d: состояние было %d стало %d\n", j+1, s_b_1_2[j], cur_s_b_1_2[j]);
             s_b_1_2[j]=cur_s_b_1_2[j];
            }
          if(cur_b_bi_1[j] != b_bi_1[j])
            {
             printf("Стол_1: столбец %d: поле было %lg стало %lg\n", j+1, b_bi_1[j], cur_b_bi_1[j]);
             b_bi_1[j]=cur_b_bi_1[j];
            }
          if(cur_s_b_bi_1[j] != s_b_bi_1[j])
            {
             printf("Стол_1: столбец %d: состояние было %d стало %d\n", j+1, s_b_bi_1[j], cur_s_b_bi_1[j]);
             s_b_bi_1[j]=cur_s_b_bi_1[j];
            }
          if(cur_b_bi_2[j] != b_bi_2[j])
            {
             printf("Стол_2: столбец %d: поле было %lg стало %lg\n", j+1, b_bi_2[j], cur_b_bi_2[j]);
             b_bi_2[j]=cur_b_bi_2[j];
            }
          if(cur_s_b_bi_2[j] != s_b_bi_2[j])
            {
             printf("Стол_2: столбец %d: состояние было %d стало %d\n", j+1, s_b_bi_2[j], cur_s_b_bi_2[j]);
             s_b_bi_2[j]=cur_s_b_bi_2[j];
            }
         }

// здусь нужна проверка введённых данных 

       if(!was_err)
        sh_put_data_er(0);  // подтверждение правильности введённых данных
      }

    }

  }

