/* FILENAME: MATR.H
 * PROGRAMMER: II2
 * PURPOSE: Matrix handle functions
 * LAST UPDATE: 10.06.2014
 */

#ifndef __MATR_H_
#define __MATR_H_

#include <math.h>

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

typedef struct tagVEC
{
  DBL X, Y, Z;
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

#define II2_UNIT_MATR {{{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1},}}

/* ������� ��������� �������� ��������.
 * ���������:
 *   - �������:
 *       VEC V1, VEC V2;
 * ������������ ��������:
 *   (VEC) B�����.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X,
                V1.Y + V2.Y,
                V1.Z + V2.Z);
}/* End of 'VecAddVec' function */

/* ������� ��������� �������� ������.
 * ���������:
 *   - �������:
 *       MATR M1, MATR M2;
 * ������������ ��������:
 *   (VEC) B�����.
 */
__inline MATR MatrSumMatr( MATR M1, MATR M2 )
{
  MATR m = {{{M1.A[0][0] + M2.A[0][0], M1.A[0][1] + M2.A[0][1], M1.A[0][2] + M2.A[0][2], M1.A[0][3] + M2.A[0][3]}, 
            {M1.A[1][0] + M2.A[1][0], M1.A[1][1] + M2.A[1][1], M1.A[1][2] + M2.A[1][2], M1.A[1][3] + M2.A[1][3]},
            {M1.A[2][0] + M2.A[2][0], M1.A[2][1] + M2.A[2][1], M1.A[2][2] + M2.A[2][2], M1.A[2][3] + M2.A[2][3]},
            {M1.A[3][0] + M2.A[3][0], M1.A[3][1] + M2.A[3][1], M1.A[3][2] + M2.A[3][2], M1.A[3][3] + M2.A[3][3]}}};

  return m;
}/* End of 'MatrSumMatr' function */

/* ������� ��������� ��������� ��������.
 * ���������:
 *   - �������:
 *       VEC V1, VEC V2;
 * ������������ ��������:
 *   (VEC) B�����.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X,
                V1.Y - V2.Y,
                V1.Z - V2.Z);
}/* End of 'VecSubVec' function */

/* ������� ��������� ���������� ������������.
 * ���������:
 *   - �������:
 *       VEC V1, VEC V2;
 * ������������ ��������:
 *   (DBL) ������.
 */
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}/* End of 'VecDotVec' function */

/* ������������ ������� */
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

/* ��������� ������������ �������� */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* ������� ��������� ������������ ������. */
MATR MatrMulMatr( MATR M1, MATR M2 );

/* ������� ��������� ������������ ������� �� ������. */
VEC MatrMulVec( MATR M, VEC V );

MATR MatrRotate( DBL AngleInDegree,
                 DBL X, DBL Y, DBL Z );

#endif /* MATR */

/* END OF 'MATR.H' FILE */
