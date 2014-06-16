/* FILENAME: ANIM.H
 * PROGRAMMER: II2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"
#include "matr.h"

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
  HDC hDC;            /* Контекст окна */
  HGLRC hRC;

  /* Массив элементов анимации и их количество */
  ii2UNIT *Units[II2_MAX_UNITS]; 
  INT NumOfUnits;
 
  DBL JX, JY, JZ, JR, JU; /* Джостик */
  INT JPOV, JButs[32];    /* Джостик */

  DBL
  Ws, Hs, /* размеры кадра в точках */
  Wp, Hp, /* размеры обрасти проецирования */
  ProjDist;      /* расстояние до плоскости проекции */

  /* Матрицы */
  MATR
  MatrWorld, /* матрица преобразования мировой СК */
  MatrView,  /* матрица преобразования видовой СК */
  MatrProjection; /* матрица проекции */

  MATR
  MatrWorldViewProj;              /* Итоговая матрица преобразования */

  DBL MsWheel;            /* Колесо мыши */
  INT MsX, MsY, MsDeltaX, MsDeltaY; /* Местонахождение курсора и розность координат*/

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

extern ii2ANIM II2_Anim;

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

#define II2_JOYST(x) ((DBL)((INT)((2.0 * (ji.dw##x##pos - jc.w##x##min) / (jc.w##x##max - jc.w##x##min - 1) - 1) * 100)) / 100)

/* Базовый тип объекта анимации */
struct tagii2UNIT
{
  II2_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
BOOL II2_AnimInit( HWND hWnd );

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
