/* FILENAME: UNIT.C
 * PROGRAMMER: II2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */
#include <stdio.h>

#include "units.h"

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( ii2UNIT *Unit, ii2ANIM *Ani )
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
} /* End of 'II2_AnimUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ii2UNIT *) ��������� �� ��������� ������ ��������.
 */
ii2UNIT *II2_InfoUnitCreate( VOID )
{
  ii2UNIT *Unit;

  if ((Unit = II2_AnimUnitCreate(sizeof(ii2UNIT))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'II2_InfoUnitCreate' function */
