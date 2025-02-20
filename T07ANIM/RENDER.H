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

/* ��� ������������� ������ � ������������ */
typedef struct
{
  VEC
    Loc,             /* ����� ������������ */
    Dir, Up, Right,  /* �������������� ������� */
    At;              /* ����� ���������� */
} ii2CAMERA;

/* ������� �������� ��������������� �������. */
BOOL II2_ObjLoad( ii2OBJ *Obj, CHAR *FileName );

/* ������� ������������ ��������������� �������. */
VOID II2_ObjFree( ii2OBJ *Obj );

/* ������� ��������� ��������������� �������. */
VOID II2_ObjDraw( ii2OBJ *Obj, ii2ANIM *Ani );

/* ������� �������������� �� ������� ������� ��������� � ����. */
MATR II2_VieverCamera( VEC Loc, VEC At, VEC Upaprox );

/* Camera rotation by camera-axes-Up function. */
VOID II2_RndCameraRotateUp( ii2CAMERA *Cam, DBL Angle );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
