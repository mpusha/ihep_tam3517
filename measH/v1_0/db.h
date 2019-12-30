#ifndef DB_H
#define DB_H

// данные управляющие
//static double tim_[2], cur_tim_[2];      // время в секундах
//static char   puls[2], cur_puls[2];      // стартовый импульс

struct measSetData {
  double tim[2];
  char pulseStart[2];
};

//
struct k_opis {
   char r_name[20];
   unsigned short n_tup;    // число строк
   unsigned short n_attr;   // число столбцов
   unsigned short tup_len;  // длина строки
   unsigned short last_act; // последнее действие 0 - чтение, 0xcccc - запись связной, 0xaaaa - запись управляющей
  } ;

static char *tb_names[]={
    "M_PD_ECS_MF_MEAS",     // 2 параметра измерения
    "M_PD_ECS_MF_CNTR"      // 2 параметра управления
      };
#define num_of_tabs sizeof(tb_names)/sizeof(tb_names[0])

#endif
