/* FILENAME: CLOCK.C
 * PROGRAMMER: II2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <math.h>
#include <time.h>

#include "anim.h"

/* Структура описания объекта анимации */
typedef struct tagii2UNIT_CLOCK
{
  II2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL X, Y;             /* Смещение */
  SYSTEMTIME Time;      /* Время */
} ii2UNIT_CLOCK;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitInit( ii2UNIT_CLOCK *Unit, ii2ANIM *Ani )
{
  GetSystemTime(&Unit->Time);
} /* End of 'ClockUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitClose( ii2UNIT_CLOCK *Unit, ii2ANIM *Ani )
{
} /* End of 'ClockUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitResponse( ii2UNIT_CLOCK *Unit, ii2ANIM *Ani )
{
} /* End of 'ClockUnitResponse' function */



/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitRender( ii2UNIT_CLOCK *Unit, ii2ANIM *Ani )
{
  DBL alf;
  static DBL k = 1.0;
  static INT MsX, MsY, MsDeltaX, MsDeltaY;

  if (!Ani->IsPause)
    GetLocalTime(&Unit->Time);
 
  if (!Ani->IsPause)
  {
    if (Ani->MsWheel > 0)
      k *= 2;
    if (Ani->MsWheel < 0)
      k /= 2;

    Unit->X += Ani->JR / Ani->W * 4;
    Unit->Y += Ani->JZ / Ani->H * 4;
    if (Unit->X < 0 || Unit->X > 1)
       Unit->X = (INT)Unit->X;
    if (Unit->Y < 0 || Unit->Y > 1)
       Unit->Y = (INT)Unit->Y;
  }

  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(55, 55, 255));

  MoveToEx(Ani->hDC, Unit->X * Ani->W, Unit->Y * Ani->H, NULL);
  alf = (DBL)(Unit->Time.wHour * 60 + Unit->Time.wMinute) / 360 * II2_PI;
  LineTo(Ani->hDC, Ani->W * Unit->X + cos(alf) * Ani->W / 8 * k, Ani->H * Unit->Y + sin(alf) * Ani->H / 8 * k);

  MoveToEx(Ani->hDC, Unit->X * Ani->W, Unit->Y * Ani->H, NULL);
  alf = (DBL)(Unit->Time.wMinute * 60 + Unit->Time.wSecond) / 1800 * II2_PI;
  LineTo(Ani->hDC, Ani->W * Unit->X + cos(alf) * Ani->W / 6 * k, Ani->H * Unit->Y + sin(alf) * Ani->H / 6 * k);

  MoveToEx(Ani->hDC, Unit->X * Ani->W, Unit->Y * Ani->H, NULL);
  alf = (DBL)(Unit->Time.wSecond * 1000 + Unit->Time.wMilliseconds) / 30000 * II2_PI;
  LineTo(Ani->hDC, Ani->W * Unit->X + cos(alf) * Ani->W / 4 * k, Ani->H * Unit->Y + sin(alf) * Ani->H / 4 * k);
} /* End of 'II2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ii2UNIT *) указатель на созданный объект анимации.
 */
ii2UNIT *II2_ClockUnitCreate( VOID )
{
  ii2UNIT_CLOCK *Unit;

  if ((Unit = (ii2UNIT_CLOCK *)II2_AnimUnitCreate(sizeof(ii2UNIT_CLOCK))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)ClockUnitInit;
  Unit->Close = (VOID *)ClockUnitClose;
  Unit->Response = (VOID *)ClockUnitResponse;
  Unit->Render = (VOID *)ClockUnitRender;
  Unit->X = 0.5;
  Unit->Y = 0.5;
  return (ii2UNIT *)Unit;
} /* End of 'II2_ClockUnitCreate' function */

/* END OF 'CLOCK.C' FILE */
