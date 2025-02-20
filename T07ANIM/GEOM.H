/* FILENAME: GEOM.H
 * PROGRAMMER: II2
 * PURPOSE: Geometry handle functions
 * LAST UPDATE: 10.06.2014
 */

#ifndef __GEOM_H_
#define __GEOM_H_

#include "anim.h"

/***
 * ������ � ��������������� ���������
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagii2UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} ii2UV;

/* ������� ������������� ���������� ��������� */
__inline ii2UV II2_UVSet( FLT U, FLT V )
{
  ii2UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'II2_UVSet' function */

typedef struct tagCOLOR
{
  FLT r, b, g, a;
} COLOR;

/* ��� ������������� ������� */
typedef struct tagii2VERTEX
{
  VEC P;    /* ���������� */
  ii2UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} ii2VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagii2PRIM_TYPE
{
  II2_PRIM_TRIMESH, II2_PRIM_GRID
} ii2PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagii2PRIM
{
  ii2PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  ii2VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} ii2PRIM;

/* ���� ��-��������� */
extern COLOR II2_DefaultColor;

/* ������� �������� ��������� */
BOOL II2_PrimCreate( ii2PRIM *P, ii2PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL II2_PrimCreateSphere( ii2PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� ��������� */
VOID II2_PrimFree( ii2PRIM *P );

/* ������� ��������� ��������� */
VOID II2_PrimDraw( ii2PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagii2MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} ii2MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagii2GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  ii2MATERIAL *Mtls;         /* ������� ���������� */
  ii2PRIM *Prims;            /* ������ ���������� */
} ii2GEOM;

/* ������� ���������� ��������� � ������� */
INT II2_GeomAddMaterial( ii2GEOM *G,
                         ii2MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT II2_GeomAddPrim( ii2GEOM *G, ii2PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID II2_GeomFree( ii2GEOM *G );

/* ������� ��������� */
VOID II2_GeomDraw( ii2GEOM *G );

/* ������� �������� ��������������� ������� */
BOOL II2_GeomLoad( ii2GEOM *G, CHAR *FileName );

/* ������� �������� ���������.*/
VOID LoadMaterials( ii2GEOM *G, CHAR *FileName );

/* ������� �������� ����� �����. */
BOOL II2_PrimCreateHeightField( ii2PRIM *P, CHAR *FileName, FLT Height, FLT Scale );
#endif /*__GEOM_H_*/