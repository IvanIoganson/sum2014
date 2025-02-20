/* FILENAME: UNITS.H
 * PROGRAMMER: II2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "anim.h"

typedef struct tagii2PLANE
{
  II2_UNIT_BASE_FIELDS;
  DBL Y;
  UINT TexNo;
} ii2PLANE;

/* ������� �������� ������� �������� */
ii2UNIT *II2_CowUnitCreate( VOID );

/* ������� �������� ��������������� ������� �������� */
ii2UNIT *II2_InfoUnitCreate( VOID );

/* ������� �������� ������� ��������. */
ii2UNIT *II2_ClockUnitCreate( VOID );

/* ������� �������� ������� ��������. */
ii2UNIT *II2_PicUnitCreate( CHAR *FNameAnd, CHAR *FNameXor );

/* ������� �������� ������� ��������. */
ii2UNIT *II2_EdgeUnitCreate( DBL X, DBL Y, DBL Z );

/* ������� �������� ������� ��������. */
ii2UNIT *II2_PlaneUnitCreate( DBL Y, CHAR *FN );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
