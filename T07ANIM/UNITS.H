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

/* Функция создания объекта анимации */
ii2UNIT *II2_CowUnitCreate( VOID );

/* Функция создания информационного объекта анимации */
ii2UNIT *II2_InfoUnitCreate( VOID );

/* Функция создания объекта анимации. */
ii2UNIT *II2_ClockUnitCreate( VOID );

/* Функция создания объекта анимации. */
ii2UNIT *II2_PicUnitCreate( CHAR *FNameAnd, CHAR *FNameXor );

/* Функция создания объекта анимации. */
ii2UNIT *II2_EdgeUnitCreate( DBL X, DBL Y, DBL Z );

/* Функция создания объекта анимации. */
ii2UNIT *II2_PlaneUnitCreate( DBL Y, CHAR *FN );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
