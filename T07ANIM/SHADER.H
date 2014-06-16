/* FILENAME: SHADER.H
 * PROGRAMMER: II2
 * PURPOSE: Shader handle functions
 * LAST UPDATE: 10.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

extern UINT II2_ShaderProg;

/* Функция инициализации шейдерной программы */
UINT II2_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Функция удаления шейдерной программы */
VOID II2_ShadProgClose( UINT ProgId );

#endif /* SHADER */

/* END OF 'SHADER.H' FILE */
