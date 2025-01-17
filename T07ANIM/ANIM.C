/* FILENAME: ANIM.C
 * PROGRAMMER: II2
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include "render.h"
#include "shader.h"
#include "geom.h"

/* ��������� �������� �������� */
ii2ANIM II2_Anim;

ii2PRIM II2_Map;

#define II2_HEIGHT_MAP 30

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
 * ������������ ��������: 
 *   (BOOL) TRUE for secsess.
 */
BOOL II2_AnimInit( HWND hWnd )
{
  LARGE_INTEGER li;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /*** ������������� OpenGL ***/

  /* ��������� ������ ����� */
  II2_Anim.hDC = GetDC(hWnd);
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(II2_Anim.hDC, &pfd);
  DescribePixelFormat(II2_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(II2_Anim.hDC, i, &pfd);

  /* ������� �������� ���������� */
  II2_Anim.hRC = wglCreateContext(II2_Anim.hDC);
  /* ������ �������� ��������� */
  wglMakeCurrent(II2_Anim.hDC, II2_Anim.hRC);

  II2_Anim.hDC = GetDC(hWnd);
  II2_Anim.hWnd = hWnd;
  II2_Anim.W = 30;
  II2_Anim.H = 30;
  II2_Anim.NumOfUnits = 0;

  II2_Anim.Ws = II2_Anim.W, II2_Anim.Hs = II2_Anim.H,
  II2_Anim.Wp = 4, II2_Anim.Hp = 3,
  II2_Anim.ProjDist = 1;

  II2_Anim.MatrWorld = MatrIdentify();
  II2_Anim.MatrView = II2_VieverCamera(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  II2_Anim.MatrProjection = MatrProjection( -II2_Anim.Wp / 2, II2_Anim.Wp / 2, -II2_Anim.Hp / 2, II2_Anim.Hp / 2, II2_Anim.ProjDist, 1000 );

  II2_Anim.MatrWorldViewProj = MatrMulMatr(MatrMulMatr(II2_Anim.MatrWorld, II2_Anim.MatrView), II2_Anim.MatrProjection);

  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;

  /* �������������� ���������� */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(II2_Anim.hRC);
    ReleaseDC(II2_Anim.hWnd, II2_Anim.hDC);
    memset(&II2_Anim, 0, sizeof(ii2ANIM));
    return FALSE;
  }

  if(!II2_PrimCreateHeightField( &II2_Map, "hm6.bmp", II2_HEIGHT_MAP, 1 ))
    return FALSE;

  return TRUE;
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
  /* ��������� ��������� */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(II2_Anim.hRC);
  ReleaseDC(II2_Anim.hWnd, II2_Anim.hDC);
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
  DBL ratio_x = 1, ratio_y = 1;

  /* ���������� ������� */
  II2_Anim.W = W;
  II2_Anim.H = H;
  /* ���� ��������� */
  glViewport(0, 0, W, H);

  if (W > H)
    ratio_x = (DBL)W / H;
  else
    ratio_y = (DBL)H / W;
  II2_Anim.Wp = ratio_x;
  II2_Anim.Hp = ratio_y;

  II2_Anim.MatrProjection = MatrProjection( -II2_Anim.Wp / 2, II2_Anim.Wp / 2, -II2_Anim.Hp / 2, II2_Anim.Hp / 2, II2_Anim.ProjDist, 1000 );
  /* ���� ��������� */
  glViewport(0, 0, W, H);

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
  static VEC cam = {0, 0, 5}, dir = {0, 0, 1}, to = {0, 0, 4};
  static DBL alf = 0, time, theta, phi = 0, r;
  static JButsClick[32], JButsOld[32], loc;
  static MATR M;
  MATR WVP;

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

  II2_Anim.MatrWorldViewProj = MatrMulMatr(MatrMulMatr(II2_Anim.MatrWorld, II2_Anim.MatrView), II2_Anim.MatrProjection);

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

  phi -= II2_Anim.JR / 30;

  dir = VecAddVec(VecMulNum(VecSet(0, 0, 1), cos(phi)), VecMulNum(VecSet(1, 0, 0), sin(phi)));
  cam.X += VecMulNum(dir, II2_Anim.JX / 2).X;
  cam.Y -= II2_Anim.JY / 2;
  cam.Z += VecMulNum(dir, II2_Anim.JZ / 2).Z;
  /*theta += II2_Anim.JZ * II2_PI / 5; 
  phi += II2_Anim.JR * II2_PI / 5;
  r += II2_Anim.MsWheel / II2_PI;*/
  to = VecAddVec(cam, dir);
  //to = to1;
  //M = /*MatrSumMatr(*/MatrMulMatr(MatrTranslate(-to.X, 0, -to.Z), MatrRotateY(II2_Anim.JR / 5))
    /*MatrTranslate(to.X, 0, to.Z))*/

  //to = MatrMulVec(M, to);
  /*to = to1;
  to.Z += sin(phi) * 2 * II2_PI;
  to.X += cos(phi) * 2 * II2_PI;*/

  /*cam.X += r * sin(theta) * cos(phi);
  cam.Y += r * sin(theta) * sin(phi);
  cam.Z += r * cos(phi);*/
  II2_Anim.MatrView = II2_VieverCamera(VecSet(cam.X, cam.Y, cam.Z), VecSet(to.X, to.Y, to.Z), VecSet(0, 1, 0));

  /* ������� ���� */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < II2_Anim.NumOfUnits; i++)
    II2_Anim.Units[i]->Response(II2_Anim.Units[i], &II2_Anim);

  alf = II2_Anim.Time * II2_PI;
  /*II2_Anim.MatrWorld = MatrRotateY(alf);*/
  //II2_Anim.MatrWorld = MatrMulMatr(MatrRotateX(II2_Anim.JZ * II2_PI), MatrRotateY(II2_Anim.JR * II2_PI));
  //II2_Anim.MatrView = II2_VieverCamera(VecSet(cam.X, cam.Y, cam.Z), VecSet(cam.X, cam.Y, 0), VecSet(0, 1, 0));

  II2_Anim.MatrWorldViewProj = MatrMulMatr(MatrMulMatr(II2_Anim.MatrWorld, II2_Anim.MatrView), II2_Anim.MatrProjection);

  glLoadMatrixf(&II2_Anim.MatrWorldViewProj.A[0][0]);
  glEnable(GL_DEPTH_TEST);

  time += II2_Anim.GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    II2_ShadProgClose(II2_ShaderProg);
    II2_ShaderProg = II2_ShadProgInit("a.vert", "a.frag");
  }
 
  /* ����� ��������� �������� ������ ���������� */
  glUseProgram(II2_ShaderProg);
  loc = glGetUniformLocation(II2_ShaderProg, "Matr");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, II2_Anim.MatrWorldViewProj.A[0]);
  loc = glGetUniformLocation(II2_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, II2_Anim.Time);

  glClearColor(0, 0, 0, 0);

  II2_PrimDraw(&II2_Map);
  
  /* ��������� �������� */
  for (i = 0; i < II2_Anim.NumOfUnits; i++)
    II2_Anim.Units[i]->Render(II2_Anim.Units[i], &II2_Anim);

    /* ��� � ������������������ */
  /*II2_Anim.MatrWorld = MatrIdentify();
  II2_Anim.MatrView =
    II2_VieverCamera(
      MatrMulVec(MatrRotateZ(II2_Anim.JY * 180), MatrMulVec(MatrRotateY(II2_Anim.JR * 180), VecSet(25, 25, 25))),
      VecSet(0, 0, 0), VecSet(0, 1, 0));*/
  WVP = MatrMulMatr(II2_Anim.MatrWorld, MatrMulMatr(II2_Anim.MatrView, II2_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);

  /* ������ ��������� */

  glLineWidth(1);
  if (II2_Anim.Keys['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /*glBegin(GL_LINE);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);

    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);

    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();*/

  FrameCounter++;
} /* End of 'II2_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID II2_AnimCopyFrame( VOID )
{
  /* ����� ����� */
  glFinish();
  SwapBuffers(II2_Anim.hDC);
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
