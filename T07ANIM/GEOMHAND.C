/* FILENAME: GEOMHAND.C
 * PROGRAMMER: ii2
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 14.06.2014
 */

#include <stdlib.h>

#include "geom.h"

ii2ANIM II2_Anim;
UINT II2_ShaderProg;

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ii2GEOM *G;
 *   - добавляемый материал:
 *       ii2MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT II2_GeomAddMaterial( ii2GEOM *G, ii2MATERIAL *Mtl )
{
  INT i;
  ii2MATERIAL *M;

  /* ищем материал в библиотеке */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(ii2MATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(ii2MATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'II2_GeomAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ii2GEOM *G;
 *   - добавляемый примитив:
 *       ii2PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT II2_GeomAddPrim( ii2GEOM *G, ii2PRIM *Prim )
{
  ii2PRIM *P;

  if ((P = malloc(sizeof(ii2PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(ii2PRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'II2_GeomAddPrim' function */

/* Функция освобождения */
VOID II2_GeomFree( ii2GEOM *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    II2_PrimFree(G->Prims + i);
  free(G->Prims);
  for (i = 0; i < G->NumOfMtls; i++)
    glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(ii2GEOM));
} /* End of 'II2_GeomFree' function */

/* Функция рисования */
VOID II2_GeomDraw( ii2GEOM *G )
{
  INT i, loc;
  MATR WVP, MatrWorldInvTrans;
  VEC V;

  /* вычислили матрицы преобразования */
  WVP = MatrMulMatr(II2_Anim.MatrWorld,
    MatrMulMatr(II2_Anim.MatrView, II2_Anim.MatrProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(II2_Anim.MatrWorld));

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(II2_ShaderProg);
  loc = glGetUniformLocation(II2_ShaderProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);

  loc = glGetUniformLocation(II2_ShaderProg, "MatrWorldInverseTranspose");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MatrWorldInvTrans.A[0]);

  loc = glGetUniformLocation(II2_ShaderProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, II2_Anim.MatrWorld.A[0]);

  loc = glGetUniformLocation(II2_ShaderProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, II2_Anim.MatrView.A[0]);

  loc = glGetUniformLocation(II2_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, II2_Anim.Time);

  V = VecSet(-II2_Anim.MatrView.A[0][2],
             -II2_Anim.MatrView.A[1][2],
             -II2_Anim.MatrView.A[2][2]);
  loc = glGetUniformLocation(II2_ShaderProg, "ViewDir");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);

  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    /* подготавливаем материал */
    if (G->Mtls != NULL)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        IMAGE Img;
                       
        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* получаем свободный номер текстуры */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* делаем ее активной */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
          Img.Bits[j] |= 0xFF000000;
        /* отправляем картинку в видеопамять */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

        /* Параметры вывода */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* передаем параметры */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(II2_ShaderProg, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
        /*glEnable(GL_TEXTURE_2D);*/
        /*glActiveTexture(GL_TEXTURE0);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        /*glActiveTexture(GL_TEXTURE1);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
      }
      loc = glGetUniformLocation(II2_ShaderProg, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.X);
      loc = glGetUniformLocation(II2_ShaderProg, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.X);
      loc = glGetUniformLocation(II2_ShaderProg, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.X);
      loc = glGetUniformLocation(II2_ShaderProg, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(II2_ShaderProg, "Trans");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Trans);
    }
    II2_PrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'II2_GeomDraw' function */

/* END OF 'GEOMHAND.C' FILE */
