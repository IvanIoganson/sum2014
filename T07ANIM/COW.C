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

/* ��������� �������� ������� �������� */
typedef struct tagii2UNIT_COW
{
  II2_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  CHAR *FN;
  ii2GEOM Obj;
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
  II2_GeomLoad( &Unit->Obj, Unit->FN);
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
  II2_GeomFree(&Unit->Obj);
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
  II2_GeomDraw(&Unit->Obj);
} /* End of 'II2_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ii2UNIT *) ��������� �� ��������� ������ ��������.
 */
ii2UNIT *II2_CowUnitCreate( VOID )
{
  ii2UNIT_COW *Unit;

  if ((Unit = (ii2UNIT_COW *)II2_AnimUnitCreate(sizeof(ii2UNIT_COW))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  Unit->FN = "house1.object";
  return (ii2UNIT *)Unit;
} /* End of 'II2_CowUnitCreate' function */

 /* END OF 'COW.C' FILE */
