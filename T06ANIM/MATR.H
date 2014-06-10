/* FILENAME: MATR.H
 * PROGRAMMER: II2
 * PURPOSE: Matrix handle functions
 * LAST UPDATE: 10.06.2014
 */

#ifndef __MATR_H_
#define __MATR_H_

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

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

/* Функция получения произведения матриц. */
MATR MatrMulMatr( MATR M1, MATR M2 );

/* Функция получения произведения матрицы на вектор. */
VEC MatrMulVec( MATR M, VEC V );

MATR MatrRotate( DBL AngleInDegree,
                 DBL X, DBL Y, DBL Z );

#endif /* MATR */

/* END OF 'MATR.H' FILE */
