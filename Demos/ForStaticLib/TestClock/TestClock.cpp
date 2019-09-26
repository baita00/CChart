// TestClock.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include "../../../Source/Plot/PlotInterface.h"
#ifdef _DEBUG
#	if defined(_UNICODE) || defined(UNICODE)
#		pragma comment(lib,"..\\..\\..\\Source\\Bin\\Plotdu.lib")
#	else
#		pragma comment(lib,"..\\..\\..\\Source\\Bin\\Plotd.lib")
#	endif
#else
#	if defined(_UNICODE) || defined(UNICODE)
#		pragma comment(lib,"..\\..\\..\\Source\\Bin\\Plotu.lib")
#	else
#		pragma comment(lib,"..\\..\\..\\Source\\Bin\\Plot.lib")
#	endif
#endif
using namespace NsCChart;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTCLOCK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TESTCLOCK);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TESTCLOCK);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_TESTCLOCK;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	static CAnalogClockImpl acPlot;
	static CDigitalClockImpl dcPlot;
	static CDialImpl dialPlot;
	static CGradienterImpl gradPlot;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			SetTimer(hWnd,1,1000,NULL); 

			dcPlot.SetHour(4);
			dcPlot.SetMinute(30);
			dcPlot.SetSecond(0);
			dcPlot.SetIncludeYMD(true);
			//dcPlot.SetSingleLine(true);
			//dcPlot.SyncTime();

			dialPlot.SetRange(0, 240);
			dialPlot.SetTickCount(11);
			dialPlot.SetMinorTickCount(2);
			dialPlot.SetAngleStart(myPi*5.0/4.0);
			dialPlot.SetAngleRange(myPi*3.0/2.0);
			dialPlot.SetShowBar(false);
			dialPlot.SetTickBar(true);
			dialPlot.SetTickBarHalfWidthBase(2);
			dialPlot.SetTickBarHalfWidthEnd(2);
			dialPlot.SetMinorTickBarHalfWidthBase(1);
			dialPlot.SetMinorTickBarHalfWidthEnd(1);
			dialPlot.SetMinorTickBar(true);
			dialPlot.SetPointerCircle3D(false);
			dialPlot.SetHideIdleArc(true);
			dialPlot.SetSubTitle(_T("km/h"));
			dialPlot.SetShowSubTitle(true);
			dialPlot.SetLabelEdgeType(kShowLabelEdgeBoth);
			dialPlot.SetCurrValue(85);

			gradPlot.SetCurrValue(0.1*myPi);
			SIZE size;
			size.cx = 0;
			size.cy = 50;
			dialPlot.SetSubTitleOffset(size);
			break;
		case WM_TIMER:
			dcPlot.SyncTime();
			acPlot.SyncTime();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case WM_ERASEBKGND:
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			{
				CMyMemDC hDC(hdc);
				RECT rt;
				rt.left = 20;
				rt.right = 300;
				rt.top = 20;
				rt.bottom = 300;
				acPlot.OnDraw(hDC, rt);
				
				OffsetRect(&rt, 280, 0);
				dialPlot.OnDraw(hDC, rt);
				
				OffsetRect(&rt, 280, 0);
				gradPlot.OnDraw(hDC, rt);
				
				rt.left = 20;
				rt.right = 300;
				rt.top = 20;
				rt.bottom = 300;
				OffsetRect(&rt, 0, 280);
				dcPlot.OnDraw(hDC, rt);
			}

			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
