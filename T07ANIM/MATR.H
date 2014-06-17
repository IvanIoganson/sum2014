/* FILENAME: MATR.H
 * PROGRAMMER: II2
 * PURPOSE: Matrix handle functions
 * LAST UPDATE: 10.06.2014
 */

#ifndef __MATR_H_
#define __MATR_H_

#include <math.h>
#include "def.h"

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

__inline MATR MatrProjection( DBL Left, DBL Right,
                              DBL Bottom, DBL Top,
                              DBL Near, DBL Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return m;
} /* End of 'MatrProjection' function */

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
}

extern INT Hs, Ws;

#define II2_UNIT_MATR {{{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}}}

__inline MATR MatrIdentify( VOID )
{
  MATR m = II2_UNIT_MATR;

  return m;
}/* End of 'MatrIdentify' function */

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 -
         A11 * A23 * A32 -
         A12 * A21 * A33 +
         A12 * A23 * A31 +
         A13 * A21 * A32 - 
         A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) - 
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR m = II2_UNIT_MATR;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
} /* End of 'MatrTranslate' function */

__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
} /* End of 'MatrTranspose' function */

/*__inline MATR VieverRotateY( VEC Loc, Vec At )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
} /* End of 'MatrTranspose' function */

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
  {
    static MATR m = II2_UNIT_MATR;

    return m;
  }

  /* строим присоединенную матрицу (adjoint matrix) */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                 M.A[1][1], M.A[1][2], M.A[1][3],
                 M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]);
 
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]);
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]);
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]);

  /* делим на определитель */
  r.A[0][0] /= det;
  r.A[1][0] /= det;
  r.A[2][0] /= det;
  r.A[3][0] /= det;
  r.A[0][1] /= det;
  r.A[1][1] /= det;
  r.A[2][1] /= det;
  r.A[3][1] /= det;
  r.A[0][2] /= det;
  r.A[1][2] /= det;
  r.A[2][2] /= det;
  r.A[3][2] /= det;
  r.A[0][3] /= det;
  r.A[1][3] /= det;
  r.A[2][3] /= det;
  r.A[3][3] /= det;

  return r;
} /* End of 'MatrInverse' function */

/* Функция получения сложения векторов.
 * АРГУМЕНТЫ:
 *   - вектора:
 *       VEC V1, VEC V2;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (VEC) Bектор.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X,
                V1.Y + V2.Y,
                V1.Z + V2.Z);
}/* End of 'VecAddVec' function */

/* Функция получения сложения матриц.
 * АРГУМЕНТЫ:
 *   - матрицы:
 *       MATR M1, MATR M2;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (VEC) Bектор.
 */
__inline MATR MatrSumMatr( MATR M1, MATR M2 )
{
  MATR m = {{{M1.A[0][0] + M2.A[0][0], M1.A[0][1] + M2.A[0][1], M1.A[0][2] + M2.A[0][2], M1.A[0][3] + M2.A[0][3]}, 
            {M1.A[1][0] + M2.A[1][0], M1.A[1][1] + M2.A[1][1], M1.A[1][2] + M2.A[1][2], M1.A[1][3] + M2.A[1][3]},
            {M1.A[2][0] + M2.A[2][0], M1.A[2][1] + M2.A[2][1], M1.A[2][2] + M2.A[2][2], M1.A[2][3] + M2.A[2][3]},
            {M1.A[3][0] + M2.A[3][0], M1.A[3][1] + M2.A[3][1], M1.A[3][2] + M2.A[3][2], M1.A[3][3] + M2.A[3][3]}}};

  return m;
}/* End of 'MatrSumMatr' function */

__inline VEC VecMulNum( VEC V1, DBL Num )
{
  return VecSet(V1.X * Num,
                V1.Y * Num,
                V1.Z * Num);
}/* End of 'VecMulNum' function */

/* Функция получения вычитания векторов.
 * АРГУМЕНТЫ:
 *   - вектора:
 *       VEC V1, VEC V2;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (VEC) Bектор.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X,
                V1.Y - V2.Y,
                V1.Z - V2.Z);
}/* End of 'VecSubVec' function */

/* Функция получения скалярного произведения.
 * АРГУМЕНТЫ:
 *   - вектора:
 *       VEC V1, VEC V2;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (DBL) Скаляр.
 */
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}/* End of 'VecDotVec' function */

/* нормирование вектора */
__inline VEC VecNormalize( VEC V )
{
  DBL len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;
  if (len != 0 && len != 1)
  {
    len = sqrt(len);
    return VecSet(V.X / len, V.Y / len,  V.Z / len);
  }
  return V;
} /* End of 'VecNormalize' function */

/* векторное произведение векторов */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Функция получения произведения матриц. */
MATR MatrMulMatr( MATR M1, MATR M2 );

/* Функция получения произведения матрицы на вектор. */
VEC MatrMulVec( MATR M, VEC V );

MATR MatrRotate( DBL AngleInDegree,
                 DBL X, DBL Y, DBL Z );

MATR MatrRotateZ( DBL Angle );

MATR MatrRotateY( DBL Angle );

MATR MatrRotateX( DBL Angle );

#endif /* MATR */

/* END OF 'MATR.H' FILE */
