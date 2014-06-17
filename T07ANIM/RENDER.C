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
  /*Ani->MatrWorld.A[3][0] -= Ani->JR / 10; 
  Ani->MatrWorld.A[3][1] += Ani->JZ / 10;
  if (Ani->JPOV == 0)
    Ani->MatrWorld.A[3][2] -= 1;
  else if (Ani->JPOV == 4)
    Ani->MatrWorld.A[3][2] += 1;*/
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

/* Cameras getting matrix function.
* ARGUMENTS: None.
* RETURNS: 
*   (MATR) Camera matrix.
*/
static MATR II2_RndCameraGetMatrix( ii2CAMERA *Cam )
{
  MATR RetMatr = 
  {
    {
      { Cam->Dir.X  , Cam->Dir.Y  , Cam->Dir.Z  , 0 },
      { Cam->Up.X   , Cam->Up.Y   , Cam->Up.Z   , 0 },
      { Cam->Right.X, Cam->Right.Y, Cam->Right.Z, 0 },
      {                  0,                  0,                 0 , 1 }
    }
  };
  return RetMatr;
} /* End of 'II2_RndCameraGetMatrix' funciton */

/* Cameras normalizing vectors function.
* ARGUMENTS: None.
* RETURNS: None.
*/
static VOID II2_RndCameraNormalize( ii2CAMERA *Cam )
{
  Cam->At = VecNormalize(Cam->At);
  Cam->Dir = VecNormalize(Cam->Dir);
  Cam->Up = VecNormalize(Cam->Up);
  Cam->Right = VecNormalize(Cam->Right);
} /* End of 'II2_RndCameraGetMatrix' funciton */

/* Camera move by Dir-axes properties 
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID II2_RndCameraMoveDir( ii2CAMERA *Cam, DBL Coef )
{
  Cam->Dir = VecNormalize(VecSubVec(Cam->At, Cam->Loc));
  Cam->Loc = VecAddVec(Cam->Loc, VecMulNum(Cam->Dir, Coef));
  Cam->At = VecAddVec(Cam->At, VecMulNum(Cam->Dir, Coef));
}  /* End of 'II2_RndCameraMoveByDir' funciton */

/* Camera move by Right-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID II2_RndCameraMoveRight( ii2CAMERA *Cam, DBL Coef )
{
  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(Cam->Dir, Cam->Up);
  Cam->Loc = VecAddVec(Cam->Loc, VecMulNum(VecNormalize(Cam->Right), Coef));
  Cam->At = VecAddVec(Cam->At, VecMulNum(VecNormalize(Cam->Right), Coef));
}  /* End of 'II2_RndCameraMoveByDir' funciton */

/* Camera move by Up-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID II2_RndCameraMoveUp( ii2CAMERA *Cam, DBL Coef )
{
  Cam->Loc = VecAddVec(Cam->Loc, VecMulNum(VecNormalize(Cam->Up), Coef));
  Cam->At = VecAddVec(Cam->At, VecMulNum(VecNormalize(Cam->Up), Coef));
}  /* End of 'II2_RndCameraMoveByDir' funciton */

/* Camera rotation by camera-axes-Up function.
 * ARGUMENTS:
 *   - Angle:
 *       DBL Angle;
 * RETURNS: None.
 */
VOID II2_RndCameraRotateUp( ii2CAMERA *Cam, DBL Angle )
{
  MATR RotMat;

  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);

  II2_RndCameraNormalize(Cam);

  RotMat = II2_RndCameraGetMatrix(Cam);
  RotMat = MatrMulMatr(MatrRotateY(Angle), RotMat);

  Cam->At = VecAddVec(Cam->Loc, VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2]));
}  /* End of 'II2_RndCameraRotateUp' funciton */

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

  //srand(30);
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
      //glColor3b(rand() % 256, rand() % 256, rand() % 256);
      glVertex3d(V1.X, V1.Y, V1.Z);
      glVertex3d(V2.X, V2.Y, V2.Z);
      glVertex3d(V3.X, V3.Y, V3.Z);
    glEnd();
  }
  //free(A);
} /* End of 'Ani->ObjDraw' function */

/* Camera rotation by camera-axes-right function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID II2_RndCameraRotateRight( ii2CAMERA *Cam, DBL Angle )
{
  MATR RotMat;
  /*
  if (Angle == 0)
    return;
  */
  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);

  II2_RndCameraNormalize(Cam);

  RotMat = II2_RndCameraGetMatrix(Cam);
  RotMat = MatrMulMatr(MatrRotateZ(Angle), RotMat);

  Cam->At = VecAddVec(Cam->Loc, VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2]));
}  /* End of 'II2_RndCameraRotateRight' funciton */

/* Camera rotation by camera-axes-dir function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID II2_RndCameraRotateDir( ii2CAMERA *Cam, DBL Angle )
{
  MATR RotMat;

  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);
  II2_RndCameraNormalize(Cam);

  RotMat = II2_RndCameraGetMatrix(Cam);
  RotMat = MatrMulMatr(MatrRotateX(Angle), RotMat);

  Cam->At = VecAddVec(Cam->Loc, VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2]));
}  /* End of 'II2_RndCameraRotateDir' funciton */

/* END OF 'RENDER.C' FILE */
