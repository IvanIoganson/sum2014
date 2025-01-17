/* FILENAME: PLANE.C
 * PROGRAMMER: ii2
 * PURPOSE: Plane object load functions.
 * LAST UPDATE: 14.06.2014
 */

#include "geom.h"
#include "units.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID II2_PlaneUnitInit( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_PlaneUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID II2_PlaneUnitClose( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_PlaneUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ii2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ii2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID II2_PlaneUnitResponse( ii2UNIT *Unit, ii2ANIM *Ani )
{
} /* End of 'II2_PlaneUnitResponse' function */

/* ������� �������� ���������.
 * ���������:
 *   - ���������:
 *       ii2PLANE *PL;
 *   - ������:
 *       DBL Z;
 *   - ��� ����� ����������:
 *       CHAR *FN;
 * ������������ ��������: ���.
*/
VOID II2_LoadPlane(ii2PLANE *PL, DBL Y, CHAR *FN )
{
  IMAGE Img;
                 
  ImageLoad(&Img, FN/*"grass.bmp"*/);

  /* �������� ��������� ����� �������� */
  glGenTextures(1, &PL->TexNo);
  /* ������ �� �������� */
  glBindTexture(GL_TEXTURE_2D, PL->TexNo);
  /*for (i = 0; i < Img.W * Img.H; i++)
    Img.Bits[i] |= 0xFF000000;*/
  /* ���������� �������� � ����������� */
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
    GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

  /* ��������� ������ */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);
  PL->Y = Y;
} /* End of 'II2_LoadPlane' function */

/* ������� ��������� ���������.
 * ���������:
 *   - ���������:
 *       ii2PLANE *PL;
 * ������������ ��������: ���.
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

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ii2UNIT *) ��������� �� ��������� ������ ��������.
 */
ii2UNIT *II2_PlaneUnitCreate( DBL Y, CHAR *FN )
{
  ii2PLANE *Unit;

  if ((Unit = malloc(sizeof(ii2PLANE))) == NULL)
    return NULL;

  II2_LoadPlane(Unit, Y, FN);
  /* ��������� ���� ��-��������� */
  Unit->Size = sizeof(ii2PLANE);
  Unit->Init = (VOID *)II2_PlaneUnitInit;
  Unit->Close = (VOID *)II2_PlaneUnitClose;
  Unit->Response = (VOID *)II2_PlaneUnitResponse;
  Unit->Render = (VOID *)II2_RenderPlane;
  return (ii2UNIT *)Unit;
} /* End of 'II2_CowUnitCreate' function */

/* END OF 'PLANE.C' FILE */
