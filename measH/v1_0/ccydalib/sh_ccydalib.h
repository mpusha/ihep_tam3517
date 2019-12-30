// Файл для управляющей программы пользователя и для ccydalib
// Основные структуры и описания БД ССУДА в разделяемой памяти
// В памяти только первая плоскость данных с диска

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <signal.h>
#include <sched.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include "../ccydalib/sh_ccyda.h"



short sh_put_data_er(short );    // из управляющей программы ошибка управляющих данных
short sh_put_meas_er(short);     // из управляющей ошибка измерений
short API_ECSini(); 
short sh_optb(char *);                
short sh_lptb(char *, short ); 
short sh_attb(char *, short , short );  
short sh_drtb(char *, unsigned int , short *, short ); 
short sh_dwtb(char *, unsigned int , short *, short );

/* 
0.unsigned int sh_get_count()            // управляющая читает циклический счётчик обновления данных сверху
   Выход:     счётчик обновлений
1.short sh_put_data_er(short er_code)    // из управляющей код ошибки + завершение действия
   Выход:     0 - О.К.
         0x8000 - ошибка работы с разделяемой памятью
2.short sh_put_meas_er(short er_code)    // из управляющей код ошибки + завершение измерений
   Выход:     0 - О.К.
         0x8000 - ошибка работы с разделяемой памятью
3.short API_ECSini()           // вызывается первой перед началом работы с таблицами в памяти
   Выход: 0 - О.К.
          1 - память не была размечена за время ожидания
          2 - память не создана за время ожидания
4.int   API_wait_ev()                                  // ждать события от связной/управляющей программы
   Выход:   0 - О.К.
         <> 0 - код ошибки из стандартной функции sigwait
5.void sh_put_mess(char *mes)                          // положить сообщение в архив сообщений
                                                       // для каждой СКО хранятся 5 последних сообщений
6.short sh_optb(char *tab_name)                        // открыть таблицу tab_name (возвращает tab_id)
   Выход:   > 0 - идентификатор tab_id таблицы для дальнейших обращений к ней
         0x8000 - нет таблицы с таким именем
7.short sh_lptb(char * buf, short tab_id)               // получить в buf описание таблицы tab_id 
   Выход:     0 - О.К. buf = имя(20байт), число строк(short), столбцов(short), длина строки(short), последнее действие(short)
         0x8000 - нет таблицы с таким идентификатором
8.short sh_attb(char *buf, short N-attr, short tab_id) // получить в buf описание столбца N-attr таблицы tab_id 
   Выход:     0 - О.К. buf=char длина, char тип, short смещение (нет для матрицы!!!)
         0x8000 - нет таблицы с таким идентификатором или нет такого номера столбца
9.short sh_drtb(char *buf, unsigned int length, short *coord, short tab_id) // читать в buf данные таблицы tab_id
   Выход:   > 0 - число прочитанных байт
         0x8000 - ошибка в идентификаторе таблицы, length=0 или в координатах
10.short sh_dwtb(char *buf, unsigned int length, short *coord, short tab_id) // писать из buf данные таблицы tab_id 
   Выход:   > 0 - число записанных байт
         0x8000 -  ошибка в идентификаторе таблицы, length=0 или в координатах

Координаты вырезки данных из таблицы задаются так:
short coord[4] ==> coord[0] - номер первой строки данных с 1
                   coord[1] - номер последней строки данных с 1
                   coord[2] - номер первого столбца данных с 1
                   coord[3] - номер последнего столбца данных с 1
Данные в буфер помещаются подряд построчно!!! 

*/


