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

extern ii2CAMERA II2_RndCam;
extern DBL
  II2_RndWs, II2_RndHs, /* ������� ����� � ������ */
  II2_RndWp, II2_RndHp, /* ������� ������� ������������� */
  II2_RndProjDist;      /* ���������� �� ��������� �������� */


/* ������� ��������� ����� �� ������. */
POINT II2_WorldtoScreen( VEC V );

/* ������� �������� ��������������� �������. */
BOOL II2_RndObjLoad( ii2OBJ *Obj, CHAR *FileName );

/* ������� ������������ ��������������� �������. */
VOID II2_RndObjFree( ii2OBJ *Obj );

/* ������� ��������� ��������������� �������. */
VOID II2_RndObjDraw( ii2OBJ *Obj, ii2ANIM *Ani );

/* ������� ���������� ������� ��������. */
VOID CameraUnitRender( ii2CAMERA *Unit, ii2ANIM *Ani );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
