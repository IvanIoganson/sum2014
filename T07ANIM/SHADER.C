/* FILENAME: SHADER.C
 * PROGRAMMER: VG4
 * PURPOSE: Shader handle functions.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

/* Функция загрузки текстового файла в память.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (CHAR *) - указатель на выделенную и
 *              загруженную память (или NULL).
 */
static CHAR *LoadFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT len;

  if ((F = fopen(FileName, "r")) == NULL)
    return NULL;

  /* определяем длину файла */
  fseek(F, 0, SEEK_END);
  len = ftell(F);

  /* выделяем память */
  if ((txt = malloc(len + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  /* загружаем файл */
  rewind(F);
  len = fread(txt, 1, len, F);
  txt[len] = 0;
  fclose(F);
  return txt;
} /* End of 'LoadFile' function */

/* Функция инициализации шейдерной программы.
 * АРГУМЕНТЫ:
 *   - имена файлов вершинного и фрагментного шейдеров:
 *       CHAR *VSFileName, *FSFileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) - идентификатор загруженной и скомпилированной
 *            программы или 0 при ошибке.
 */
UINT II2_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName )
{
  CHAR *txt;
  UINT shv, shf, prg;
  INT res, len;
  static CHAR Buf[1000];

  /* Инициализируем вершинный шейдер */
  if ((txt = LoadFile(VSFileName)) == NULL)
    return 0;
  if ((shv = glCreateShader(GL_VERTEX_SHADER)) == 0)
  {
    free(txt);
    return 0;
  }
  /* закрепляем текст */
  glShaderSource(shv, 1, &txt, NULL);
  free(txt);
  /* компилируем шейдер */
  glCompileShader(shv);
  glGetShaderiv(shv, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shv, sizeof(Buf), &len, Buf);
    glDeleteShader(shv);
    return 0;
  }

  /* Инициализируем фрагментный шейдер */
  if ((txt = LoadFile(FSFileName)) == NULL)
  {
    glDeleteShader(shv);
    return 0;
  }
  if ((shf = glCreateShader(GL_FRAGMENT_SHADER)) == 0)
  {
    glDeleteShader(shv);
    free(txt);
    return 0;
  }
  /* закрепляем текст */
  glShaderSource(shf, 1, &txt, NULL);
  free(txt);
  /* компилируем шейдер */
  glCompileShader(shf);
  glGetShaderiv(shf, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shf, sizeof(Buf), &len, Buf);
    glDeleteShader(shf);
    glDeleteShader(shv);
    return 0;
  }

  /* Инициализируем программу - набор шейдеров */
  if ((prg = glCreateProgram()) == 0)
  {
    glDeleteShader(shf);
    glDeleteShader(shv);
    return 0;
  }
  /* присоединяем к программе шейдера */
  glAttachShader(prg, shv);
  glAttachShader(prg, shf);

  /* компонуем программу */
  glLinkProgram(prg);
  glGetProgramiv(prg, GL_LINK_STATUS, &res);
  if (res != 1)
  {
    glGetProgramInfoLog(prg, sizeof(Buf), &len, Buf);
    glDetachShader(prg, shv);
    glDetachShader(prg, shf);
    glDeleteShader(shv);
    glDeleteShader(shf);
    glDeleteProgram(prg);
    return 0;
  }
  return prg;
} /* End of 'II2_ShadProgInit' function */

/* Функция удаления шейдерной программы.
 * АРГУМЕНТЫ:
 *   - идентификатор загруженной программы:
 *       UINT ProgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_ShadProgClose( UINT ProgId )
{
  UINT i, n, shdrs[4];

  if (ProgId == 0)
    return;
  /* получаем присоединенные шейдера */
  glGetAttachedShaders(ProgId, 4, &n, shdrs);

  /* удаляем */
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'II2_ShadProgClose' function */

/* END OF 'SHADER.C' FILE */
