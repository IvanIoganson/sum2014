/* FILENAME: UNIT.C
 * PROGRAMMER: II2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"
#include "render.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID II2_AnimUnitInit( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID II2_AnimUnitClose( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID II2_AnimUnitResponse( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID II2_AnimUnitRender( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ii2UNIT *) ��������� �� ��������� ������ ��������.
 */
ii2UNIT *II2_AnimUnitCreate( INT Size )
{
  ii2UNIT *Unit;

  if (Size < sizeof(ii2UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* ��������� ���� ��-��������� */
  Unit->Size = Size;
  Unit->Init = II2_AnimUnitInit;
  Unit->Close = II2_AnimUnitClose;
  Unit->Response = II2_AnimUnitResponse;
  Unit->Render = II2_AnimUnitRender;
  return Unit;
} /* End of 'II2_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

