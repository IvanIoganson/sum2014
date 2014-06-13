/* FILENAME: RENDER.C
 * PROGRAMMER: II2
 * PURPOSE: Render handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"
#include "render.h"

ii2CAMERA II2_RndCam = {{0, 0, 2}, {1, 0, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, 0}};

/* Параметры проецирования */
DBL
  II2_RndWs = 30, II2_RndHs = 30,   /* размеры кадра в точках */
  II2_RndWp = 1, II2_RndHp = 1,     /* размеры обрасти проецирования */
  II2_RndProjDist = 3;              /* расстояние до плоскости проекции */

/* Матрицы */
MATR
  II2_RndMatrWorld = II2_UNIT_MATR, /* матрица преобразования мировой СК */
  II2_RndMatrView = II2_UNIT_MATR,  /* матрица преобразования видовой СК */
  II2_RndMatrProjection = II2_UNIT_MATR; /* матрица проекции */

static MATR
  II2_RndMatrWorldViewProj;              /* Итоговая матрица преобразования */


/* Функция перевычисления итоговой матрицы преобразования.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_RndMatrSetup( VOID )
{
  II2_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(II2_RndMatrWorld, II2_RndMatrView),
      II2_RndMatrProjection);
} /* End of 'II2_RndMatrSetup' function */

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

  II2_RndMatrView = r;

  return r;
} /* End of 'II2_RndWorldToScreen' function */

POINT II2_WorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  II2_VieverCamera(II2_RndCam.Loc, II2_RndCam.At, II2_RndCam.Up);

  /* преобразование СК */
  Pp = MatrMulVec(II2_RndMatrWorldViewProj, P);
 
  Pp.X *= II2_RndProjDist / Pp.Z * 30;
  Pp.Y *= II2_RndProjDist / Pp.Z * 30;

  /* проецирование */
  Ps.x = ( Pp.X + 0.5 * II2_RndWp) * (II2_RndWs - 1);
  Ps.y = (-Pp.Y + 0.5 * II2_RndHp) * (II2_RndHs - 1);
  return Ps;
} /* End of 'II2_RndWorldToScreen' function */

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
  II2_RndMatrWorld.A[3][0] -= Ani->JR / 10; 
  II2_RndMatrWorld.A[3][1] += Ani->JZ / 10;
  if (Ani-> == 0)
    II2_RndMatrWorld.A[3][2] += 1;
  else if (Ani->JPOV == 4)
    II2_RndMatrWorld.A[3][2] -= 1;
  //II2_RndMatrWorld = MatrSumMatr(II2_RndMatrWorld, MatrRotate(Ani->JX * II2_PI / 10, 0, 0, 0));

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
  INT i;
  POINT *pts;
  static DBL cx, cy, cz;
  static INT flag = FALSE;

  if ((pts = malloc(sizeof(POINT) * Obj->NumOfV)) == NULL)
    return;

  II2_RndMatrSetup();
  
  for (i = 0; i < Obj->NumOfV; i++)
    pts[i] = II2_WorldToScreen(Obj->V[i]);

  for (i = 0; i < Obj->NumOfF; i++)
  {
    INT n0 = Obj->F[i][0], n1 = Obj->F[i][1], n2 = Obj->F[i][2];

    /*if (Obj->V[n0].Z < II2_RndProjDist || Obj->V[n1].Z < II2_RndProjDist ||
        Obj->V[n2].Z < II2_RndProjDist)
      continue;*/
    MoveToEx(Ani->hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(Ani->hDC, pts[n1].x, pts[n1].y);
    LineTo(Ani->hDC, pts[n2].x, pts[n2].y);
    LineTo(Ani->hDC, pts[n0].x, pts[n0].y);
  }
  free(pts);
} /* End of 'II2_RndObjDraw' function */

/* END OF 'RENDER.C' FILE */
