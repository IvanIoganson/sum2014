/* FILENAME: ANIM.C
 * PROGRAMMER: II2
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include "render.h"

#pragma comment(lib, "winmm")

#include <mmsystem.h>

/* ��������� �������� �������� */
static ii2ANIM II2_Anim;

ii2CAMERA II2_RndCam;

/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID II2_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  II2_Anim.hWnd = hWnd;
  II2_Anim.hDC = CreateCompatibleDC(hDC);
  II2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  II2_Anim.W = 30;
  II2_Anim.H = 30;
  II2_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* �������������� ������ */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'II2_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID II2_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < II2_Anim.NumOfUnits; i++)
  {
    II2_Anim.Units[i]->Close(II2_Anim.Units[i], &II2_Anim);
    free(II2_Anim.Units[i]);
  }
  DeleteObject(II2_Anim.hBmFrame);
  DeleteDC(II2_Anim.hDC);
  memset(&II2_Anim, 0, sizeof(ii2ANIM));
} /* End of 'II2_AnimInit' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID II2_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(II2_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(II2_Anim.hBmFrame);
  II2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(II2_Anim.hDC, II2_Anim.hBmFrame);

  /* ���������� ������� */
  II2_Anim.W = W;
  II2_Anim.H = H;

  ReleaseDC(II2_Anim.hWnd, hDC);
} /* End of 'II2_AnimResize' function */

/* ������ ��� ��������� ���� */
static INT
  II2_MouseGlobalX, II2_MouseGlobalY, /* ������� ���������� */
  II2_MouseXOld, II2_MouseYOld,       /* ����������� �� �������� ����� ���������� */
  II2_MouseGlobalWheel;               /* ��������� ������ ���� */

/* ������� ��������� ����������� �������� ���������.
 * ���������:
 *   - ��� �������:
 *       INT Code;
 *   - �������� ��������� ('word parameter') -
 *     ��� ��������� �� ����:
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter') -
 *     (MSLLHOOKSTRUCT *) ��� �������� ���������;
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
static LRESULT CALLBACK II2_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    II2_MouseGlobalX = hs->pt.x;
    II2_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    II2_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'II2_MouseHook' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID II2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  static JButsClick[32], JButsOld[32];

  /* ���������� ����� */
  GetKeyboardState(II2_Anim.Keys);
  for (i = 0; i < 256; i++)
    II2_Anim.Keys[i] >>= 7;

      /* �������� */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* ��������� ����� ���������� � �������� */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ��������� �������� ��������� */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* ������ */
        memcpy(JButsOld, II2_Anim.JButs, sizeof(II2_Anim.JButs));
        for (i = 0; i < 32; i++)
          II2_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          JButsClick[i] = II2_Anim.JButs[i] && !JButsOld[i];

        /* ��� */
        II2_Anim.JX = II2_JOYST(X);
        II2_Anim.JY = II2_JOYST(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          II2_Anim.JZ = II2_JOYST(Z);
        if (jc.wCaps & JOYCAPS_HASR)
          II2_Anim.JR = II2_JOYST(R);
        if (jc.wCaps & JOYCAPS_HASU)
          II2_Anim.JU = II2_JOYST(U);

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            II2_Anim.JPOV = 0;
          else
            II2_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* �������������� ������ ��������� �� ���� */
  SetWindowsHookEx(WH_MOUSE_LL, II2_MouseHook, GetModuleHandle(NULL), 0);

  /* ���� */
  /*  ������ */
  II2_Anim.MsWheel = II2_MouseGlobalWheel / 120;
  II2_MouseGlobalWheel = 0;
  /* ���������� ������� */
  pt.x = II2_MouseGlobalX;
  pt.y = II2_MouseGlobalY;
  ScreenToClient(II2_Anim.hWnd, &pt);
  II2_Anim.MsX = pt.x;
  II2_Anim.MsY = pt.y;
  /* ������������� ����������� */
  II2_Anim.MsDeltaX = II2_MouseGlobalX - II2_MouseXOld;
  II2_Anim.MsDeltaY = II2_MouseGlobalY - II2_MouseYOld;
  II2_MouseXOld = II2_MouseGlobalX;
  II2_MouseYOld = II2_MouseGlobalY;
  
  /* ���������� ������� */
  II2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* ���������� ����� */
  QueryPerformanceCounter(&li);

  /* ���������� ����� */
  II2_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  II2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* ��������� ����� */
  if (II2_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    II2_Anim.DeltaTime = 0;
  }
  else
    II2_Anim.DeltaTime = II2_Anim.GlobalDeltaTime;

  II2_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    II2_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  /* ������� ���� */
  SelectObject(II2_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(II2_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(II2_Anim.hDC, RGB(0, 110, 0));
  Rectangle(II2_Anim.hDC, 0, 0, II2_Anim.W, II2_Anim.H);

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < II2_Anim.NumOfUnits; i++)
    II2_Anim.Units[i]->Response(II2_Anim.Units[i], &II2_Anim);
  
  CameraUnitRender(&II2_RndCam, &II2_Anim);
  /* ��������� �������� */
  for (i = 0; i < II2_Anim.NumOfUnits; i++)
  {
    SelectObject(II2_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(II2_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(II2_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(II2_Anim.hDC, RGB(55, 155, 255));
    II2_Anim.Units[i]->Render(II2_Anim.Units[i], &II2_Anim);
  }

  FrameCounter++;
} /* End of 'II2_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID II2_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(II2_Anim.hWnd);
  BitBlt(hDC, 0, 0, II2_Anim.W, II2_Anim.H, II2_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(II2_Anim.hWnd, hDC);
} /* End of 'II2_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ii2UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID II2_AnimAddUnit( ii2UNIT *Unit )
{
  if (II2_Anim.NumOfUnits < II2_MAX_UNITS)
  {
    II2_Anim.Units[II2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &II2_Anim);
  }
} /* End of 'II2_AnimAddUnit' function */

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID II2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(II2_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(II2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    /* ��� ������ ��������:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(II2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(II2_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* ��������������� ������ ���� */
    SetWindowPos(II2_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'II2_AnimFlipFullScreen' function */

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID II2_AnimSetPause( BOOL NewPauseFlag )
{
  II2_Anim.IsPause = NewPauseFlag;
} /* End of 'II2_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
