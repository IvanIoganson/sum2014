/* FILENAME: EDGE.C
 * PROGRAMMER: II2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include "anim.h"
#include "matr.h"

/* Структура описания объекта анимации */
typedef struct tagii2UNIT_EDGE
{
  II2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL X, Y, Z;          /* Смещение */
  VEC V;                /* Вектор */
} ii2UNIT_EDGE;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_EDGE *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID EdgeUnitInit( ii2UNIT_EDGE *Unit, ii2ANIM *Ani )
{
} /* End of 'EdgeUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_EDGE *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID EdgeUnitClose( ii2UNIT_EDGE *Unit, ii2ANIM *Ani )
{
} /* End of 'EdgeUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_EDGE *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID EdgeUnitResponse( ii2UNIT_EDGE *Unit, ii2ANIM *Ani )
{
} /* End of 'EdgeUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ii2UNIT_EDGE *Unit;
 *   - указатель на контекст анимации:
 *       ii2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID EdgeUnitRender( ii2UNIT_EDGE *Unit, ii2ANIM *Ani )
{
  DBL ProjDist = 1; 
  DBL x, y, z, x1, y1;
  VEC V = {Unit->X, Unit->Y, Unit->Z};

  //p = WorldtoScreen(VecSet(Unit->X, Unit->Y, Unit->Z));

  V = MatrMulVec(MatrRotate(Ani->JX * II2_PI / 10, 0, 0, 1), V);
  Unit->X = V.X, Unit->Y = V.Y, Unit->Z = V.Z;
  Unit->Z += Ani->MsWheel / 10;
  
  //Unit->V = MatrMulVec(MatrRotate(Ani->JX * II2_PI / 20, Unit->X, Unit->Y, Unit->Z), Unit->V);  
  if (Unit->Z < ProjDist)
    return;

  x1 = x = (Unit->X) * ProjDist / Unit->Z + 0.5;
  y1 = y = (Unit->Y) * ProjDist / Unit->Z + 0.5;
  MoveToEx(Ani->hDC, x * Ani->W, y * Ani->H, NULL);
  //Ellipse(Ani->hDC, p.x - 10 / Unit->Z, p.y - 10 / Unit->Z, p.x + 10 / Unit->Z, p.y + 10 / Unit->Z);
  Ellipse(Ani->hDC, x * Ani->W - 10 / Unit->Z, y * Ani->H - 10 / Unit->Z, x * Ani->W + 10 / Unit->Z, y * Ani->H + 10 / Unit->Z);

  z = Unit->Z;
  x = (Unit->X + Unit->V.X) * ProjDist / z + 0.5;
  y = (Unit->Y) * ProjDist / z + 0.5;
  Ellipse(Ani->hDC, x * Ani->W - 10 / z, y * Ani->H - 10 / z, x * Ani->W + 10 / z, y * Ani->H + 10 / z);
  LineTo(Ani->hDC, x * Ani->W, y * Ani->H);
  MoveToEx(Ani->hDC, x1 * Ani->W, y1 * Ani->H, NULL);

  z = Unit->Z;
  x = (Unit->X) * ProjDist / z + 0.5;
  y = (Unit->Y + Unit->V.Y) * ProjDist / z + 0.5;
  Ellipse(Ani->hDC, x * Ani->W - 10 / z, y * Ani->H - 10 / z, x * Ani->W + 10 / z, y * Ani->H + 10 / z);
  LineTo(Ani->hDC, x * Ani->W, y * Ani->H);
  MoveToEx(Ani->hDC, x1 * Ani->W, y1 * Ani->H, NULL);

  z = Unit->Z + Unit->V.Z;
  x = (Unit->X) * ProjDist / z + 0.5;
  y = (Unit->Y) * ProjDist / z + 0.5;
  Ellipse(Ani->hDC, x * Ani->W - 10 / z, y * Ani->H - 10 / z, x * Ani->W + 10 / z, y * Ani->H + 10 / z);
  LineTo(Ani->hDC, x * Ani->W, y * Ani->H);

  z = Unit->Z + Unit->V.Z;
  x1 = x = (Unit->X + Unit->V.X) * ProjDist / z + 0.5;
  y1 = y = (Unit->Y + Unit->V.Y) * ProjDist / z + 0.5;
  Ellipse(Ani->hDC, x * Ani->W - 10 / z, y * Ani->H - 10 / z, x * Ani->W + 10 / z, y * Ani->H + 10 / z);
  MoveToEx(Ani->hDC, x1 * Ani->W, y1 * Ani->H, NULL);

  z = Unit->Z;
  x = (Unit->X + Unit->V.X) * ProjDist / z + 0.5;
  y = (Unit->Y + Unit->V.Y) * ProjDist / z + 0.5;
  Ellipse(Ani->hDC, x * Ani->W - 10 / z, y * Ani->H - 10 / z, x * Ani->W + 10 / z, y * Ani->H + 10 / z);
  LineTo(Ani->hDC, x * Ani->W, y * Ani->H);
  MoveToEx(Ani->hDC, x1 * Ani->W, y1 * Ani->H, NULL);

  z = Unit->Z + Unit->V.Z;
  x = (Unit->X + Unit->V.X) * ProjDist / z + 0.5;
  y = (Unit->Y) * ProjDist / z + 0.5;
  Ellipse(Ani->hDC, x * Ani->W - 10 / z, y * Ani->H - 10 / z, x * Ani->W + 10 / z, y * Ani->H + 10 / z);
  LineTo(Ani->hDC, x * Ani->W, y * Ani->H);
  MoveToEx(Ani->hDC, x1 * Ani->W, y1 * Ani->H, NULL);

  z = Unit->Z + Unit->V.Z;
  x = (Unit->X) * ProjDist / z + 0.5;
  y = (Unit->Y + Unit->V.Y) * ProjDist / z + 0.5;
  Ellipse(Ani->hDC, x * Ani->W - 10 / z, y * Ani->H - 10 / z, x * Ani->W + 10 / z, y * Ani->H + 10 / z);
  LineTo(Ani->hDC, x * Ani->W, y * Ani->H);

} /* End of 'II2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ii2UNIT *) указатель на созданный объект анимации.
 */
ii2UNIT *II2_EdgeUnitCreate( DBL X, DBL Y, DBL Z )
{
  ii2UNIT_EDGE *Unit;

  if ((Unit = (ii2UNIT_EDGE *)II2_AnimUnitCreate(sizeof(ii2UNIT_EDGE))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)EdgeUnitInit;
  Unit->Close = (VOID *)EdgeUnitClose;
  Unit->Response = (VOID *)EdgeUnitResponse;
  Unit->Render = (VOID *)EdgeUnitRender;
  Unit->V.X = Unit->V.Y = Unit->V.Z = 0.4;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  return (ii2UNIT *)Unit;
} /* End of 'II2_EDGEUnitCreate' function */

/* END OF 'EDGE.C' FILE */
