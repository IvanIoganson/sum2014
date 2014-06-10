/* FILENAME: RENDER.C
 * PROGRAMMER: II2
 * PURPOSE: Render handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"
#include "render.h"

ii2CAMERA II2_RndCam;

/* Параметры проецирования */
DBL
  II2_RndWs = 30, II2_RndHs = 30,   /* размеры кадра в точках */
  II2_RndWp = 1, II2_RndHp = 1,     /* размеры обрасти проецирования */
  II2_RndProjDist = 1;              /* расстояние до плоскости проекции */

/* Матрицы */
MATR
  II2_RndMatrWorld = II2_UNIT_MATR, /* матрица преобразования мировой СК */
  II2_RndMatrView = II2_UNIT_MATR;  /* матрица преобразования видовой СК */

/* Функция преобразования из мировой системы коорлинат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT II2_WorldtoScreen( VEC P )
{
  POINT Ps;
  VEC Pp = P;

  /* преобразование СК */
  //Pp = MatrMulVec(MatrMulMatr(II2_RndMatrWorld, II2_RndMatrView), P);
  
  /* проецирование */
  Pp.X *= II2_RndProjDist / Pp.Z;
  Pp.Y *= II2_RndProjDist / Pp.Z;

  Ps.x = ( Pp.X + II2_RndWp / 2) / II2_RndWp * (II2_RndWs - 1) * 25;
  Ps.y = (-Pp.Y + II2_RndHp / 2) / II2_RndHp * (II2_RndHs - 1) * 15;
  
  /*Ps.x = ((P.X) * II2_RndProjDist / P.Z + 0.5) / II2_RndWp * II2_RndWs;
  Ps.y = ((P.Y) * II2_RndProjDist / P.Z + 0.5) / II2_RndHp * II2_RndHs;*/

  return Ps;
} /* End of 'II2_RndWorldToScreen' function */

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ii2OBJ *Obj;
 *   - имя файла (*.OBJ):
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL II2_RndObjLoad( ii2OBJ *Obj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* обнулили данные */
  memset(Obj, 0, sizeof(ii2OBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* считаем количество вершин и граней */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }
  /* выделяем память под вершины и грани как единый участок памяти
   * (memory bulk) */
  Obj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  Obj->F = (INT (*)[3])(Obj->V + nv);
  Obj->NumOfV = nv;
  Obj->NumOfF = nf;

  /* считываем данные */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Obj->V[nv++] = VecSet(x + 6, y - 4, z + 2);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%i%i%i", &a, &b, &c);
      Obj->F[nf][0] = a - 1;
      Obj->F[nf][1] = b - 1;
      Obj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'II2_RndObjLoad' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ii2OBJ *Obj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_RndObjFree( ii2OBJ *Obj )
{
  if (Obj->V != NULL)
    free(Obj->V);
  memset(Obj, 0, sizeof(ii2OBJ));
} /* End of 'II2_RndObjFree' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ii2OBJ *Obj;
 *   - контекст устройства вывода:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_RndObjDraw( ii2OBJ *Obj, ii2ANIM *Ani )
{
  INT i, j, s = 1;
  //static DBL cx, cy, cz;
  POINT pt[3];

  for (i = 0; i < Obj->NumOfV; i++)
  {
    Obj->V[i] = MatrMulVec(MatrRotate( Ani->JX * II2_PI / 10, 5, 0, 2), Obj->V[i]);
    Obj->V[i].Z += Ani->MsWheel / 10;
    //cx = 

    if (Obj->V[i].Z < II2_RndProjDist)
      continue;

    pt[0] = II2_WorldtoScreen(Obj->V[i]);

    Ellipse(Ani->hDC, pt[0].x - s, pt[0].y - s, pt[0].x + s, pt[0].y + s);
  }
  for (i = 0; i < Obj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      pt[j] = II2_WorldtoScreen(Obj->V[Obj->F[i][j]]);
    
    if (Obj->V[Obj->F[i][0]].Z < II2_RndProjDist || Obj->V[Obj->F[i][1]].Z < II2_RndProjDist ||
        Obj->V[Obj->F[i][2]].Z < II2_RndProjDist)
      continue;
    MoveToEx(Ani->hDC, pt[0].x, pt[0].y, NULL);
    LineTo(Ani->hDC, pt[1].x, pt[1].y);
    LineTo(Ani->hDC, pt[2].x, pt[2].y);
    LineTo(Ani->hDC, pt[0].x, pt[0].y);
  }
} /* End of 'II2_RndObjDraw' function */

/* END OF 'RENDER.C' FILE */
