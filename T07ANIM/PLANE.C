/* FILENAME: PLANE.C
 * PROGRAMMER: ii2
 * PURPOSE: Plane object load functions.
 * LAST UPDATE: 14.06.2014
 */

#include "geom.h"
#include "units.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID II2_PlaneUnitInit( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_PlaneUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID II2_PlaneUnitClose( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_PlaneUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID II2_PlaneUnitResponse( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_PlaneUnitResponse' function */

/* Функция загрузки плоскости.
 * АРГУМЕНТЫ:
 *   - плоскость:
 *       ii2PLANE *PL;
 *   - высота:
 *       DBL Z;
 *   - имя файла материалов:
 *       CHAR *FN;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID II2_LoadPlane(ii2PLANE *PL, DBL Y, CHAR *FN )
{
  IMAGE Img;
                 
  ImageLoad(&Img, FN/*"grass.bmp"*/);

  /* получаем свободный номер текстуры */
  glGenTextures(1, &PL->TexNo);
  /* делаем ее активной */
  glBindTexture(GL_TEXTURE_2D, PL->TexNo);
  /*for (i = 0; i < Img.W * Img.H; i++)
    Img.Bits[i] |= 0xFF000000;*/
  /* отправляем картинку в видеопамять */
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
    GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

  /* Параметры вывода */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);
  PL->Y = Y;
} /* End of 'II2_LoadPlane' function */

/* Функция рисования плоскости.
 * АРГУМЕНТЫ:
 *   - плоскость:
 *       ii2PLANE *PL;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID II2_RenderPlane( ii2PLANE *PL )
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, PL->TexNo);
  glColor3d(1, 1, 1);
  glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2d(-1000,-1000);
    glVertex3d(-1000, PL->Y, -1000);
    glTexCoord2d(-1000, 1000);
    glVertex3d(-1000, PL->Y, 1000);
    glTexCoord2d(1000, -1000);
    glVertex3d(1000, PL->Y, -1000);
    glTexCoord2d(1000, 1000);
    glVertex3d(1000, PL->Y, 1000);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
} /* End of 'II2_RenderPlane' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ii2UNIT *) указатель на созданный объект анимации.
 */
ii2UNIT *II2_PlaneUnitCreate( DBL Y, CHAR *FN )
{
  ii2PLANE *Unit;

  if ((Unit = malloc(sizeof(ii2PLANE))) == NULL)
    return NULL;

  II2_LoadPlane(Unit, Y, FN);
  /* заполняем поля по-умолчанию */
  Unit->Size = sizeof(ii2PLANE);
  Unit->Init = (VOID *)II2_PlaneUnitInit;
  Unit->Close = (VOID *)II2_PlaneUnitClose;
  Unit->Response = (VOID *)II2_PlaneUnitResponse;
  Unit->Render = (VOID *)II2_RenderPlane;
  return (ii2UNIT *)Unit;
} /* End of 'II2_CowUnitCreate' function */

/* END OF 'PLANE.C' FILE */
