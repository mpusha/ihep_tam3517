#ifndef DB_H
#define DB_H

// данные измеряемые
static char b_tim[6];     // ссылки на имена каналов 0-5
static char *b_comm[]={   // комментарии до 8 байт текста
       "Ekr",
       "Bamps_St",
       "Ch-3",
       "Ch-4",
       "Ch-5",
       "Ch-6"
      };
static char b_b1_b2[2];   // ссылки на B1, B2
static char b_bimf_1[2];  // ссылки на B, I middle flat
static char b_bimf_2[2];  // ссылки на B, I top flat
static char b_status;     // ссылка на норма/ошибка 0 -Норма, 0x81 - Ошибка арасным цветом
                          // добавление к коду 0x80 ==> текст выводится красным цветом

// данные управляющие
/*static double f_b_tim[6], cur_f_b_tim[6];  // поле в 6-ти каналах В-таймера
static double b_1_2[2], cur_b_1_2[2];      // поле B1, B2
static double b_bi_1[2], cur_b_bi_1[2];    // поле B, I middle flat
static double b_bi_2[2], cur_b_bi_2[2];    // поле B, I top flat
static char s_b_tim[6], cur_s_b_tim[6];    // состояние в 6-ти каналах В-таймера
static char s_b_1_2[2], cur_s_b_1_2[2];    // состояние B1, B2
static char s_b_bi_1[2], cur_s_b_bi_1[2];  // состояние B, I middle flat
static char s_b_bi_2[2], cur_s_b_bi_2[2];  // состояние B, I top flat
*/

struct Btimer {
  double fBt[6];         // поле в 6-ти каналах В-таймера
  char onOffBt[6];       // состояние On/Off в 6-ти каналах В-таймера
  double fB1_2[2];       // поле On/Off B1, B2
  char onOffB1_2[2];     // состояние B1, B2
  double fBIMiddle[2];   // поле B, I middle flat
  double fBITop[2];      // поле B, I top flat
  char onOffBIMiddle[2]; // состояние On/Off B, I middle flat
  char onOffBITop[2];    // состояние On/Off B, I top flat
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
    "M_PD_ECS_BTIM_MEAS",   // 6 параметров измерения
    "M_PD_ECS_BTIM_CNTR"    // 8 параметров управления
      };
#define num_of_tabs sizeof(tb_names)/sizeof(tb_names[0])


#endif
