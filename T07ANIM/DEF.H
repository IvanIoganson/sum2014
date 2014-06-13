/* FILENAME: DEF.H
 * PROGRAMMER: II2
 * PURPOSE: Default definitions.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __DEF_H_
#define __DEF_H_

#pragma warning(disable: 4244 4305)

#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <mmsystem.h>

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32")
#pragma comment(lib, "winmm")

/* Large integer data types */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;

/* Common float point types */
typedef float FLT;
typedef double DBL;

#endif /* __DEF_H_ */

/* END OF 'DEF.H' FILE */
