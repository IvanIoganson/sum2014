/* FILENAME: UNIT.C
 * PROGRAMMER: II2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */
#include <stdio.h>

#include "units.h"

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitRender( ii2UNIT *Unit, ii2ANIM *Ani )
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
} /* End of 'II2_AnimUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ii2UNIT *) указатель на созданный объект анимации.
 */
ii2UNIT *II2_InfoUnitCreate( VOID )
{
  ii2UNIT *Unit;

  if ((Unit = II2_AnimUnitCreate(sizeof(ii2UNIT))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'II2_InfoUnitCreate' function */
