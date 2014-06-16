/* FILENAME: COW.C
 * PROGRAMMER: II2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"
#include "geom.h"

/* Структура описания объекта анимации */
typedef struct tagii2UNIT_COW
{
  II2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  CHAR *FN;
  ii2GEOM Obj;
} ii2UNIT_COW;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitInit( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
  II2_GeomLoad( &Unit->Obj, Unit->FN);
} /* End of 'CowUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitClose( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
  II2_GeomFree(&Unit->Obj);
} /* End of 'CowUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitResponse( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitRender( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
  II2_GeomDraw(&Unit->Obj);
} /* End of 'II2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ii2UNIT *) указатель на созданный объект анимации.
 */
ii2UNIT *II2_CowUnitCreate( VOID )
{
  ii2UNIT_COW *Unit;

  if ((Unit = (ii2UNIT_COW *)II2_AnimUnitCreate(sizeof(ii2UNIT_COW))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  Unit->FN = "house1.object";
  return (ii2UNIT *)Unit;
} /* End of 'II2_CowUnitCreate' function */

 /* END OF 'COW.C' FILE */
