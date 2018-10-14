#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#define PI 3.141593

#define WINDOW_BORDER_SIZE 5
#define WINDOW_HEADER_SIZE 25

int data_width, data_height, strmaxlength;
RECT winRect;

void Draw(HWND hwnd);
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[ ] = _T("Lab1");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
   setlocale(LC_ALL, "");
   HWND hwnd;               /* This is the handle for our window */
   MSG messages;            /* Here messages to the application are saved */
   WNDCLASSEX wincl;        /* Data structure for the windowclass */

   /* The Window structure */
   wincl.hInstance = hThisInstance;
   wincl.lpszClassName = szClassName;
   wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
   wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
   wincl.cbSize = sizeof (WNDCLASSEX);

   /* Use default icon and mouse-pointer */
   wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
   wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
   wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
   wincl.lpszMenuName = NULL;                 /* No menu */
   wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
   wincl.cbWndExtra = 0;                      /* structure or the window instance */
   /* Use Windows's default colour as the background of the window */
   wincl.hbrBackground = (HBRUSH) WHITE_PEN;

   /* Register the window class, and if it fails quit the program */
   if (!RegisterClassEx (&wincl))
      return 0;

   /* The class is registered, let's create the program*/
   hwnd = CreateWindowEx (
             0,                   /* Extended possibilites for variation */
             szClassName,         /* Classname */
             _T("Lab1 for CGA 551006"),       /* Title Text */
             WS_OVERLAPPEDWINDOW,//&(~WS_MAXIMIZEBOX), /* default window */
             CW_USEDEFAULT,       /* Windows decides the position */
             CW_USEDEFAULT,       /* where the window ends up on the screen */
             640,                 /* The programs width */
             480,                 /* and height in pixels */
             HWND_DESKTOP,        /* The window is a child-window to desktop */
             NULL,                /* No menu */
             hThisInstance,       /* Program Instance handler */
             NULL                 /* No Window Creation data */
          );
   ShowWindow (hwnd, nCmdShow);
   /* Run the message loop. It will run until GetMessage() returns 0 */
   while (GetMessage (&messages, NULL, 0, 0))
   {
      /* Translate virtual-key messages into character messages */
      TranslateMessage(&messages);
      /* Send message to WindowProcedure */
      DispatchMessage(&messages);
   }

   return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)                  /* handle the messages */
   {
   case WM_SYSCOMMAND:
      if (wParam == SC_MAXIMIZE)
      {
         DefWindowProc (hwnd, message, wParam, lParam);
         InvalidateRect(hwnd,NULL, true);
         GetWindowRect(hwnd, &winRect);
         Draw(hwnd);

      }
      else
         return DefWindowProc (hwnd, message, wParam, lParam);
      break;
   case WM_ACTIVATE:
   case WM_SIZE:
      InvalidateRect(hwnd,NULL, true);
      GetWindowRect(hwnd, &winRect);
      Draw(hwnd);
      break;
   case WM_DESTROY:
      PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
      break;
   default:                      /* for messages that we don't deal with */
      return DefWindowProc (hwnd, message, wParam, lParam);
   }

   return 0;
}

double GetCoefficient()
{
   return std::min((winRect.bottom - winRect.top - WINDOW_HEADER_SIZE - WINDOW_BORDER_SIZE), (winRect.right - winRect.left - WINDOW_BORDER_SIZE)) / 3;
}

void DrawAxes(HDC hdc)
{
   int height = (winRect.bottom - winRect.top - WINDOW_HEADER_SIZE - WINDOW_BORDER_SIZE);
   int width = (winRect.right - winRect.left - WINDOW_BORDER_SIZE);

   MoveToEx(hdc, 0, height / 2, NULL);
   LineTo(hdc, width, height / 2);

   MoveToEx(hdc, width / 2, 0, NULL);
   LineTo(hdc, width / 2, height);
}

void DrawFigure(HDC hdc)
{
   double a = GetCoefficient();
   int x0 = (winRect.right - winRect.left - WINDOW_BORDER_SIZE) / 2;
   int y0 = (winRect.bottom - winRect.top - WINDOW_HEADER_SIZE - WINDOW_BORDER_SIZE) / 2;
   int n = 100000;
   double h = 2 * PI / n;
   double f = 0;
   for (int i = -n; i < n; i++)
   {
      double t = i * 0.001;
      double x = roundl(a*pow(t, 2)/(1+pow(t, 2)));
      double y = roundl(a*pow(t, 3)/(1+pow(t, 2)));
      SetPixel(hdc, x0 + roundl(x), y0 - roundl(y), 0);
      f += h;
   }
    LOGBRUSH LogBrush;
    LogBrush.lbColor = 0xFF;
    LogBrush.lbStyle = PS_SOLID;
    HPEN hPen = ExtCreatePen( PS_DASH, 1, &LogBrush, 0, NULL );
    SelectObject(hdc, hPen);
   MoveToEx(hdc, x0 + a, 0, NULL);
   LineTo(hdc, x0 + a, 2 * y0);
}

void Draw(HWND hwnd)
{
   PAINTSTRUCT ps;

   HDC hdc = BeginPaint(hwnd, &ps);

   DrawAxes(hdc);
   DrawFigure(hdc);

   EndPaint(hwnd, &ps);
}
