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

/* Функция загрузки геометрического объекта. */
BOOL II2_ObjLoad( ii2OBJ *Obj, CHAR *FileName );

/* Функция освобождения геометрического объекта. */
VOID II2_ObjFree( ii2OBJ *Obj );

/* Функция отрисовки геометрического объекта. */
VOID II2_ObjDraw( ii2OBJ *Obj, ii2ANIM *Ani );

/* Функция преобразования из мировой системы коорлинат в кадр. */
MATR II2_VieverCamera( VEC Loc, VEC At, VEC Upaprox );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
