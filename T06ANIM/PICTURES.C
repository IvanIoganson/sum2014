/* FILENAME: PIC.C
 * PROGRAMMER: II2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <math.h>

#include "anim.h"
#include "image.h"

/* Структура описания объекта анимации */
typedef struct tagii2UNIT_Pic
{
  II2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL X, Y;             /* Смещение */
  IMAGE AndImg, XorImg; /* Картинки*/
} ii2UNIT_Pic;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_Pic *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PicUnitInit( ii2UNIT_Pic *Unit, ii2ANIM *Ani )
{
} /* End of 'PicUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_Pic *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PicUnitClose( ii2UNIT_Pic *Unit, ii2ANIM *Ani )
{
  ImageFree(&Unit->AndImg);
  ImageFree(&Unit->XorImg);
} /* End of 'PicUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_Pic *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PicUnitResponse( ii2UNIT_Pic *Unit, ii2ANIM *Ani )
{
} /* End of 'PicUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_Pic *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PicUnitRender( ii2UNIT_Pic *Unit, ii2ANIM *Ani )
{
  HDC MemhDC = CreateCompatibleDC(Ani->hDC);

  if (!Ani->IsPause)
  {
    Unit->X += Ani->JX / Ani->W * 4;
    Unit->Y += Ani->JY / Ani->H * 4;
    if (Unit->X < 0 || Unit->X > 1)
       Unit->X = (INT)Unit->X;
    if (Unit->Y < 0 || Unit->Y > 1)
       Unit->Y = (INT)Unit->Y;
  }
  SelectObject(MemhDC, Unit->AndImg.hBm);
  StretchBlt(Ani->hDC, Unit->X * Ani->W, Unit->Y * Ani->H, Unit->AndImg.W, Unit->AndImg.H, MemhDC, 0, 0, Unit->AndImg.W, Unit->AndImg.H, SRCAND);
  
  SelectObject(MemhDC, Unit->XorImg.hBm);
  StretchBlt(Ani->hDC, Unit->X * Ani->W, Unit->Y * Ani->H, Unit->XorImg.W, Unit->XorImg.H, MemhDC, 0, 0, Unit->XorImg.W, Unit->XorImg.H, SRCINVERT);
  DeleteObject(MemhDC);
} /* End of 'II2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ii2UNIT *) указатель на созданный объект анимации.
 */
ii2UNIT *II2_PicUnitCreate( CHAR *FNameAnd, CHAR *FNameXor )
{
  ii2UNIT_Pic *Unit;

  if ((Unit = (ii2UNIT_Pic *)II2_AnimUnitCreate(sizeof(ii2UNIT_Pic))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)PicUnitInit;
  Unit->Close = (VOID *)PicUnitClose;
  Unit->Response = (VOID *)PicUnitResponse;
  Unit->Render = (VOID *)PicUnitRender;
  if (ImageLoad(&Unit->AndImg, FNameAnd) == FALSE)
    free(Unit);
  if (ImageLoad(&Unit->XorImg, FNameXor) == FALSE)
  {
    ImageFree(&Unit->AndImg);
    free(Unit);
  }
  Unit->X = 0.8;
  Unit->Y = 0;
  return (ii2UNIT *)Unit;
} /* End of 'II2_PicUnitCreate' function */

/* END OF 'PIC.C' FILE */
