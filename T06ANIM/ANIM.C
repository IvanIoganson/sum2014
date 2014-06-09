/* FILENAME: ANIM.C
 * PROGRAMMER: II2
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

#pragma comment(lib, "winmm")

#include <mmsystem.h>

/* Системный контекст анимации */
static ii2ANIM II2_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'II2_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < II2_Anim.NumOfUnits; i++)
  {
    II2_Anim.Units[i]->Close(II2_Anim.Units[i], &II2_Anim);
    free(II2_Anim.Units[i]);
  }
  DeleteObject(II2_Anim.hBmFrame);
  DeleteDC(II2_Anim.hDC);
  memset(&II2_Anim, 0, sizeof(ii2ANIM));
} /* End of 'II2_AnimInit' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(II2_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(II2_Anim.hBmFrame);
  II2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(II2_Anim.hDC, II2_Anim.hBmFrame);

  /* Сохранение размера */
  II2_Anim.W = W;
  II2_Anim.H = H;

  ReleaseDC(II2_Anim.hWnd, hDC);
} /* End of 'II2_AnimResize' function */

/* Данные для обработки мыши */
static INT
  II2_MouseGlobalX, II2_MouseGlobalY, /* Текущие координаты */
  II2_MouseXOld, II2_MouseYOld,       /* Сохраненные от прошлого кадра координаты */
  II2_MouseGlobalWheel;               /* Состояние колеса мыши */

/* Функция обработки захваченных мышинных сообщений.
 * АРГУМЕНТЫ:
 *   - код захвата:
 *       INT Code;
 *   - параметр сообшения ('word parameter') -
 *     код сообщения от мыши:
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter') -
 *     (MSLLHOOKSTRUCT *) для мышинных сообщений;
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
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

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  static JButs[32], JButsClick[32], JButsOld[32];

  /* Обновление ввода */
  GetKeyboardState(II2_Anim.Keys);
  for (i = 0; i < 256; i++)
    II2_Anim.Keys[i] >>= 7;

      /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(JButsOld, JButs, sizeof(JButs));
        for (i = 0; i < 32; i++)
          JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          JButsClick[i] = JButs[i] && !JButsOld[i];

        /* Оси */
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

  /* инициализируем захват сообщений от мыши */
  SetWindowsHookEx(WH_MOUSE_LL, II2_MouseHook, GetModuleHandle(NULL), 0);

  /* Мышь */
  /*  колесо */
  II2_Anim.MsWheel = II2_MouseGlobalWheel / 120;
  II2_MouseGlobalWheel = 0;
  /* абсолютная позиция */
  pt.x = II2_MouseGlobalX;
  pt.y = II2_MouseGlobalY;
  ScreenToClient(II2_Anim.hWnd, &pt);
  II2_Anim.MsX = pt.x;
  II2_Anim.MsY = pt.y;
  /* относительное перемещение */
  II2_Anim.MsDeltaX = II2_MouseGlobalX - II2_MouseXOld;
  II2_Anim.MsDeltaY = II2_MouseGlobalY - II2_MouseYOld;
  II2_MouseXOld = II2_MouseGlobalX;
  II2_MouseYOld = II2_MouseGlobalY;
  
  /* Обновление таймера */
  II2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Обновление кадра */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  II2_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  II2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (II2_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    II2_Anim.DeltaTime = 0;
  }
  else
    II2_Anim.DeltaTime = II2_Anim.GlobalDeltaTime;

  II2_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    II2_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* очистка фона */
  SelectObject(II2_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(II2_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(II2_Anim.hDC, RGB(0, 110, 0));
  Rectangle(II2_Anim.hDC, 0, 0, II2_Anim.W, II2_Anim.H);

  /* опрос на изменение состояний объектов */
  for (i = 0; i < II2_Anim.NumOfUnits; i++)
    II2_Anim.Units[i]->Response(II2_Anim.Units[i], &II2_Anim);

  /* рисование объектов */
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

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(II2_Anim.hWnd);
  BitBlt(hDC, 0, 0, II2_Anim.W, II2_Anim.H, II2_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(II2_Anim.hWnd, hDC);
} /* End of 'II2_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ii2UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimAddUnit( ii2UNIT *Unit )
{
  if (II2_Anim.NumOfUnits < II2_MAX_UNITS)
  {
    II2_Anim.Units[II2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &II2_Anim);
  }
} /* End of 'II2_AnimAddUnit' function */

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(II2_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(II2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
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
    /* восстанавливаем размер окна */
    SetWindowPos(II2_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'II2_AnimFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimSetPause( BOOL NewPauseFlag )
{
  II2_Anim.IsPause = NewPauseFlag;
} /* End of 'II2_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
