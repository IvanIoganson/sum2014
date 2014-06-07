/* FILENAME: ANIM.H
 * PROGRAMMER: VG4
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Максимальное количество элементов анимации */
#define VG4_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagvg4UNIT vg4UNIT;

/* Структура хранения контекста анимации */
typedef struct tagvg4ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  vg4UNIT *Units[VG4_MAX_UNITS]; 
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
} vg4ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( vg4UNIT *Unit, vg4ANIM *Ani );
 */
#define VG4_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( vg4UNIT *Unit, vg4ANIM *Ani );       \
  VOID (*Close)( vg4UNIT *Unit, vg4ANIM *Ani );      \
  VOID (*Response)( vg4UNIT *Unit, vg4ANIM *Ani );   \
  VOID (*Render)( vg4UNIT *Unit, vg4ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagvg4UNIT
{
  VG4_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
VOID VG4_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID VG4_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID VG4_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID VG4_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID VG4_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID VG4_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID VG4_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID VG4_AnimAddUnit( vg4UNIT *Unit );

/* Функция создания объекта анимации */
vg4UNIT * VG4_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
