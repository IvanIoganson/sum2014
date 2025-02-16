/* FILENAME: ANIM.H
 * PROGRAMMER: II2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"
#include "matr.h"

/* ������������ ���������� ��������� �������� */
#define II2_MAX_UNITS 3000

#define II2_PI 3.1415926535897932384626433832795

/* ���������� ���� �������� �������� "������" */
typedef struct tagii2UNIT ii2UNIT;

/* ��������� �������� ��������� �������� */
typedef struct tagii2ANIM
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ���� */
  HGLRC hRC;

  /* ������ ��������� �������� � �� ���������� */
  ii2UNIT *Units[II2_MAX_UNITS]; 
  INT NumOfUnits;
 
  DBL JX, JY, JZ, JR, JU; /* ������� */
  INT JPOV, JButs[32];    /* ������� */

  DBL
  Ws, Hs, /* ������� ����� � ������ */
  Wp, Hp, /* ������� ������� ������������� */
  ProjDist;      /* ���������� �� ��������� �������� */

  /* ������� */
  MATR
  MatrWorld, /* ������� �������������� ������� �� */
  MatrView,  /* ������� �������������� ������� �� */
  MatrProjection; /* ������� �������� */

  MATR
  MatrWorldViewProj;              /* �������� ������� �������������� */

  DBL MsWheel;            /* ������ ���� */
  INT MsX, MsY, MsDeltaX, MsDeltaY; /* ��������������� ������� � �������� ���������*/

  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */

  /* ���������� ����� */
  BYTE Keys[256];     /* �������� ������ ���������� � ���� */
} ii2ANIM;

extern ii2ANIM II2_Anim;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( ii2UNIT *Unit, ii2ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( ii2UNIT *Unit, ii2ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( ii2UNIT *Unit, ii2ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( ii2UNIT *Unit, ii2ANIM *Ani );
 */
#define II2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ii2UNIT *Unit, ii2ANIM *Ani );       \
  VOID (*Close)( ii2UNIT *Unit, ii2ANIM *Ani );      \
  VOID (*Response)( ii2UNIT *Unit, ii2ANIM *Ani );   \
  VOID (*Render)( ii2UNIT *Unit, ii2ANIM *Ani )

#define II2_JOYST(x) ((DBL)((INT)((2.0 * (ji.dw##x##pos - jc.w##x##min) / (jc.w##x##max - jc.w##x##min - 1) - 1) * 100)) / 100)

/* ������� ��� ������� �������� */
struct tagii2UNIT
{
  II2_UNIT_BASE_FIELDS; /* ������� ���� */
};

/***
 * ������� ���������� ���������
 ***/

/* ������� ������������� �������� */
BOOL II2_AnimInit( HWND hWnd );

/* ������� ��������������� �������� */
VOID II2_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������ */
VOID II2_AnimResize( INT W, INT H );

/* ������� ���������� ����� �������� */
VOID II2_AnimRender( VOID );

/* ������� ������ ����� �������� */
VOID II2_AnimCopyFrame( VOID );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ��������� */
VOID II2_AnimFlipFullScreen( VOID );

/* ������� ��������� ����� �������� */
VOID II2_AnimSetPause( BOOL NewPauseFlag );

/***
 * ������� ��������� �������� ��������
 ***/

/* ������� ���������� � ������� ������� �������� */
VOID II2_AnimAddUnit( ii2UNIT *Unit );

/* ������� �������� ������� �������� */
ii2UNIT *II2_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
