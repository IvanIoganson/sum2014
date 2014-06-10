/* FILENAME: MATR.C
 * PROGRAMMER: II2
 * PURPOSE: Matrix handle functions
 * LAST UPDATE: 10.06.2014
 */

#include <math.h>

#include "anim.h"
#include "matr.h"

/* Функция получения произведения матриц.
 * АРГУМЕНТЫ:
 *   - матрицы:
 *       MATR M1, MATR M2;
 * ВОЗВРАЩАЕЗНАЧЕНИЕ:
 *   (MATR) Матрица.
 */
MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  INT i, j, k;
  
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
    {
      r.A[i][j] = 0;
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
    }
  return r;
}/* End of 'MatrMulMatr' function */

MATR MatrRotate( DBL Angle,
                 DBL X, DBL Y, DBL Z )
{
  DBL a, si, co, len;
  MATR m;

  a = Angle;
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 * co * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * co * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * co * Y;
  m.A[2][1] = 2 * co * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
}

/* Функция получения произведения матрицы на вектор.
 * АРГУМЕНТЫ:
 *   - матрица:
 *       MATR M;
 *   - вектор:
 *       VEC V;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (VEC) Вектор.
 */
VEC MatrMulVec( MATR M, VEC V )
{
  VEC r;
 
  r.X = V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0];
  r.Y = V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1];
  r.Z = V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2];

  return r;
}/* End of 'MatrMulVec' function */

/* END OF 'MATR.C' FILE */
