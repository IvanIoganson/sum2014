/* FILENAME: ANIM.C
 * PROGRAMMER: II2
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

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

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID II2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* Обновление ввода */
  GetKeyboardState(II2_Anim.Keys);
  for (i = 0; i < 256; i++)
    II2_Anim.Keys[i] >>= 7;

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
