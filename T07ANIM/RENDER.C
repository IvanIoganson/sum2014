/* FILENAME: RENDER.C
 * PROGRAMMER: II2
 * PURPOSE: Render handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"
#include "render.h"

/* Функция преобразования из мировой системы коорлинат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC Loc, VEC At, VEC Upaprox;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (MATR) Матрица преоброзаваний камеры.
 */
MATR II2_VieverCamera( VEC Loc, VEC At, VEC Upaprox )
{
  VEC Right, Up, Dir;
  MATR r;

  Dir = VecNormalize(VecSubVec(At, Loc));
  Right = VecNormalize(VecCrossVec(Dir, Upaprox));
  Up = VecCrossVec(Right, Dir);

  r.A[0][0] = Right.X; r.A[0][1] = Up.X; r.A[0][2] = -Dir.X; r.A[0][3] = 0;
  r.A[1][0] = Right.Y; r.A[1][1] = Up.Y; r.A[1][2] = -Dir.Y; r.A[1][3] = 0;
  r.A[2][0] = Right.Z; r.A[2][1] = Up.Z; r.A[2][2] = -Dir.Z; r.A[2][3] = 0;
  r.A[3][0] = -VecDotVec(Loc, Right);
  r.A[3][1] = -VecDotVec(Loc, Up);
  r.A[3][2] = VecDotVec(Loc, Dir);
  r.A[3][3] = 1;


  /*r.A[0][0] = Right.X;
  r.A[1][0] = Up.X;
  r.A[2][0] = -Dir.X;
  r.A[3][0] = 0;
     
  r.A[0][1] = Right.Y;
  r.A[1][1] = Up.Y;
  r.A[2][1] = -Dir.Y;
  r.A[3][1] = 0;
     
  r.A[0][2] = Right.Z;
  r.A[1][2] = Up.Z;
  r.A[2][2] = -Dir.Z;
  r.A[3][2] = 0;
     
  r.A[0][3] = -VecDotVec(Loc, Right);
  r.A[1][3] = -VecDotVec(Loc, Up);
  r.A[2][3] = -VecDotVec(Loc, Dir);
  r.A[3][3] = 1;*/

  return r;
} /* End of 'Ani->WorldToScreen' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2CAMERA *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID CameraUnitRender( ii2CAMERA *Unit, ii2ANIM *Ani )
{
  Ani->MatrWorld.A[3][0] -= Ani->JR / 10; 
  Ani->MatrWorld.A[3][1] += Ani->JZ / 10;
  if (Ani->JPOV == 0)
    Ani->MatrWorld.A[3][2] += 1;
  else if (Ani->JPOV == 4)
    Ani->MatrWorld.A[3][2] -= 1;
  //Ani->MatrWorld = MatrSumMatr(Ani->MatrWorld, MatrRotate(Ani->JX * II2_PI / 10, 0, 0, 0));

} /* End of 'II2_CameraUnitRender' function */

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ii2OBJ *Obj;
 *   - имя файла (*.OBJ):
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL II2_ObjLoad( ii2OBJ *Obj, CHAR *FileName )
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
      Obj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      if(sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
         sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
         sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
         sscanf(Buf + 2, "%i %i %i", &a, &b, &c))
      {
        Obj->F[nf][0] = a - 1;
        Obj->F[nf][1] = b - 1;
        Obj->F[nf][2] = c - 1;
      }
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'Ani->ObjLoad' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ii2OBJ *Obj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_ObjFree( ii2OBJ *Obj )
{
  if (Obj->V != NULL)
    free(Obj->V);
  memset(Obj, 0, sizeof(ii2OBJ));
} /* End of 'Ani->ObjFree' function */

__inline VOID Swap (VEC *V1, VEC *V2)
{
  VEC tmp = *V1;
  *V1 = *V2;
  *V2 = tmp;
}

VOID QuikSort( VEC *V, INT *A, INT N )
{
  VEC V1 = V[N / 2];
  INT i = 0, j = N - 1;

  if (N < 2)
    return;
  
  while (i < j)
  {
    while (V[i].Z < V1.Z)
      i++;
    while (V[j].Z > V1.Z)
      j--;
    if (i <= j)
    {
      if (i != j)
      { 
        INT t = A[i];
        A[i] = A[j];
        A[j] = t;
        Swap(V + i, V + j);
      }
      i++;
      j--;
    }
  }
  QuikSort(V, A, j + 1);
  QuikSort(V + i, A, N - i);
}

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ii2OBJ *Obj;
 *   - контекст устройства вывода:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_ObjDraw( ii2OBJ *Obj, ii2ANIM *Ani )
{
  INT i;
  //INT *A;

  /*if ((pts = malloc(sizeof(POINT) * Obj->NumOfV)) == NULL)
    return;

  for (i = 0; i < Obj->NumOfV; i++)
    pts[i] = (Ani->MatrWorldViewProj, Obj->V[i]);*/

  /*if ((A = malloc(sizeof(INT) * Obj->NumOfV)) == NULL)
    return;

  for(i = 0; i < Obj->NumOfV; i++)
    A[i] = i;

  QuikSort(Obj->V, A, Obj->NumOfV );*/

  srand(30);
  for (i = 0; i < Obj->NumOfF; i++)
  {
    //INT n0 = A[Obj->F[i][0]], n1 = A[Obj->F[i][1]], n2 = A[Obj->F[i][2]];
    INT n0 = Obj->F[i][0], n1 = Obj->F[i][1], n2 = Obj->F[i][2];
    VEC V1 = Obj->V[n0],
      V2 = Obj->V[n1],
      V3 = Obj->V[n2];
    /*if (Obj->V[n0].Z < Ani->ProjDist || Obj->V[n1].Z < Ani->ProjDist ||
        Obj->V[n2].Z < Ani->ProjDist)
      continue;*/
    glBegin(GL_TRIANGLES);
      glColor3b(rand() % 256, rand() % 256, rand() % 256);
      glVertex3d(V1.X, V1.Y, V1.Z);
      glVertex3d(V2.X, V2.Y, V2.Z);
      glVertex3d(V3.X, V3.Y, V3.Z);
    glEnd();
  }
  //free(A);
} /* End of 'Ani->ObjDraw' function */

/* END OF 'RENDER.C' FILE */
