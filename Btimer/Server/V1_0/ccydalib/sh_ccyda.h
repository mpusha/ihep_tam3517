// Описание таблицы
struct r_opis {
   unsigned long next;      // смещение от начала памяти !
   unsigned short flag;     // <> 0 ==> длина элемента в матрице
   char r_name[20];
   unsigned short n_tup;    // число строк
   unsigned short n_attr;   // число столбцов
   unsigned short tup_len;  // длина строки
   unsigned short last_act; // последнее действие 0 - чтение, 0xcccc - запись связной, 0xaaaa - запись управляющей
   unsigned long  tab_data; // смещение данных от начала r_opis !
  };
// описание столбца для всех типов таблиц
struct at_opis {
   unsigned char at_len;
   unsigned char at_type;
   unsigned short at_offset;
  };
// структура начала разделяемой памяти
struct sh_start {
   size_t shmem_size;       // размер выделенной памяти
// Данные от связной программы 
   pid_t comm_pid;
   unsigned int S_put_sig;  // циклический счётчик отправленных сигналов
   unsigned int sh_flag;    // <> 0 ==> разделяемая память назначена и размечена
// Данные для связной программы от управляющей
   pid_t cntr_pid;
   unsigned int C_put_count;  // циклический счётчик попыток обработки управляющих данных
   unsigned int C_count;      // циклический счётчик попыток измерений в управляющей программе
   short err_meas;            // код ошибки измерений = 0 - O.K.
   short err_data;            // код ошибки в управляющих данных = 0 - O.K.
   short Mess_tuple;          // номер строки в U70_ECS_MESSAGES для данной СКО
   short Mess_cur_pl;         // номер текущей плоскости для записи сообщений, циклически от 1 до 5 
// Далее следуют описания таблиц БД ==> структура r_opis + структуры at_opis + данные
  };
//  структура строки таблицы U70_ECS_MESSAGES
struct ECS_mess {
   char ECS_name[52];         // текстовый идентификатор установка/подсистема (25 символов кириллицы)
   char mes_time[32];         // дата/время формирования сообщения
   char messa[252];           // текст сообщения (125 символов кириллицы)
  };

static char name1[]={"EquipmentControlStationMemory"};   // Имя разделяемой памяти
static char name2[]={"tabs_to_shmem.ecs"};               // файл с именами таблиц, копируемых в память
static char name3[]={"U70_ECS_MESSAGES"};                // таблица с сообщениями от СКО (до 5 сообщений на каждый)
short sh_ECSini(short);
/*
 short  sh_ECSini(short flag)            // вызывается первой перед началом работы с таблицами в памяти
   flag = <>0 --> вызов из связной программы
          = 0 --> вызов из управляющей программы 
   Выход: 0 - О.К.
          1 - память не была размечена за время ожидания
          2 - память не создана за время ожидания
*/
