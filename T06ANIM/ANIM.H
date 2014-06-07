/* FILENAME: ANIM.H
 * PROGRAMMER: II2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Максимальное количество элементов анимации */
#define II2_MAX_UNITS 3000

#define II2_PI 3.1415926535897932384626433832795

/* Объявление типа объектов анимации "вперед" */
typedef struct tagii2UNIT ii2UNIT;

/* Структура хранения контекста анимации */
typedef struct tagii2ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  ii2UNIT *Units[II2_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE Keys[256];     /* Сотояние клавиш клавиатуры и мыши */
} ii2ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( ii2UNIT *Unit, ii2ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( ii2UNIT *Unit, ii2ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( ii2UNIT *Unit, ii2ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( ii2UNIT *Unit, ii2ANIM *Ani );
 */
#define II2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ii2UNIT *Unit, ii2ANIM *Ani );       \
  VOID (*Close)( ii2UNIT *Unit, ii2ANIM *Ani );      \
  VOID (*Response)( ii2UNIT *Unit, ii2ANIM *Ani );   \
  VOID (*Render)( ii2UNIT *Unit, ii2ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagii2UNIT
{
  II2_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
VOID II2_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID II2_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID II2_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID II2_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID II2_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID II2_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID II2_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID II2_AnimAddUnit( ii2UNIT *Unit );

/* Функция создания объекта анимации */
ii2UNIT *II2_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
