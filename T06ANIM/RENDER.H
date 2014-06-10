/* FILENAME: RENDER.H
 * PROGRAMMER: II2
 * PURPOSE: Render handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "matr.h"

typedef struct tagii2OBJ
{
  VEC *V;
  INT NumOfV;
  INT (*F)[3];
  INT NumOfF;
} ii2OBJ;

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} ii2CAMERA;

/* Функция получения точки на экране. */
POINT II2_WorldtoScreen( VEC V );

/* Функция загрузки геометрического объекта. */
BOOL II2_RndObjLoad( ii2OBJ *Obj, CHAR *FileName );

/* Функция освобождения геометрического объекта. */
VOID II2_RndObjFree( ii2OBJ *Obj );

/* Функция отрисовки геометрического объекта. */
VOID II2_RndObjDraw( ii2OBJ *Obj, ii2ANIM *Ani );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
