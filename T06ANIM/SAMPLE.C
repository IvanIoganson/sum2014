/* FILENAME: SAMPLE.C
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

/* ��������� �������� ������� �������� */
typedef struct tagii2UNIT_COW
{
  II2_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  CHAR *FN;
  ii2OBJ Obj;
} ii2UNIT_COW;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitInit( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
  II2_RndObjLoad( &Unit->Obj, Unit->FN);
} /* End of 'CowUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitClose( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
  II2_RndObjFree(&Unit->Obj);
} /* End of 'CowUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitResponse( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
  MatrProjection( -II2_RndWp / 2, II2_RndWp / 2, -II2_RndHp / 2, II2_RndHp / 2, II2_RndProjDist, 1000 );
} /* End of 'CowUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitRender( ii2UNIT_COW *Unit, ii2ANIM *Ani )
{
  II2_RndObjDraw( &Unit->Obj, Ani );
} /* End of 'II2_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ii2UNIT *) ��������� �� ��������� ������ ��������.
 */
ii2UNIT * II2_CowUnitCreate( VOID )
{
  ii2UNIT_COW *Unit;

  if ((Unit = (ii2UNIT_COW *)II2_AnimUnitCreate(sizeof(ii2UNIT_COW))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  Unit->FN = "cow.object";
  return (ii2UNIT *)Unit;
} /* End of 'II2_CowUnitCreate' function */

/***********************************************************/

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

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'II2_AnimUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ii2UNIT *) ��������� �� ��������� ������ ��������.
 */
ii2UNIT * II2_InfoUnitCreate( VOID )
{
  ii2UNIT *Unit;

  if ((Unit = II2_AnimUnitCreate(sizeof(ii2UNIT))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'II2_InfoUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */
