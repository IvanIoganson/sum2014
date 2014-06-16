/* FILENAME: GEOM.H
 * PROGRAMMER: II2
 * PURPOSE: Geometry handle functions
 * LAST UPDATE: 10.06.2014
 */

#ifndef __GEOM_H_
#define __GEOM_H_

#include "anim.h"

/***
 * Работа с геометрическими объектами
 ***/

/* Тип представления текстурных координат */
typedef struct tagii2UV
{
  FLT U, V; /* компоненты текстурных координат */
} ii2UV;

/* Функиця инициализации текстурных координат */
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

/* Теп представления вершины */
typedef struct tagii2VERTEX
{
  VEC P;    /* координаты */
  ii2UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} ii2VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagii2PRIM_TYPE
{
  II2_PRIM_TRIMESH, II2_PRIM_GRID
} ii2PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagii2PRIM
{
  ii2PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  ii2VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} ii2PRIM;

/* Цвет по-умолчанию */
extern COLOR II2_DefaultColor;

/* Функция создания примитива */
BOOL II2_PrimCreate( ii2PRIM *P, ii2PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL II2_PrimCreateSphere( ii2PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID II2_PrimFree( ii2PRIM *P );

/* Функция отрисовки примитива */
VOID II2_PrimDraw( ii2PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagii2MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} ii2MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagii2GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  ii2MATERIAL *Mtls;         /* таблица материалов */
  ii2PRIM *Prims;            /* массив примитивов */
} ii2GEOM;

/* Функция добавления материала к объекту */
INT II2_GeomAddMaterial( ii2GEOM *G,
                         ii2MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT II2_GeomAddPrim( ii2GEOM *G, ii2PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID II2_GeomFree( ii2GEOM *G );

/* Функция рисования */
VOID II2_GeomDraw( ii2GEOM *G );

/* Функция загрузки геометрического объекта */
BOOL II2_GeomLoad( ii2GEOM *G, CHAR *FileName );

#endif /*__GEOM_H_*/