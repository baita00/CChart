/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2019 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//                                  版权申明                                  //
//                         版权所有(C)2006-2019，杨国君                       //
//                                保留全部权利                                //
////////////////////////////////////////////////////////////////////////////////
/* ############################################################################################################################## */

//#include "StdAfx.h"
#include <tchar.h>
#include "ColorControl.h"
#include "../../Global.h"

Declare_Namespace_CChart

WNDPROC fcnOldWndProc_ColorCtrl = 0;
BOOL MyChooseColor(COLORREF &cr, HWND hWnd)
{
	static CHOOSECOLOR cc;
	static COLORREF crCustColors[16];
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=hWnd;
	cc.hInstance=NULL;
	cc.rgbResult=cr;
	cc.lpCustColors =crCustColors;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN|CC_ANYCOLOR;
	cc.lCustData=0;
	cc.lpfnHook=NULL;
	cc.lpTemplateName=NULL;
	BOOL ret=ChooseColor(&cc);
	if(ret)cr=cc.rgbResult;
	return ret;
}

LRESULT CALLBACK ColorControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	COLORREF color,*pcolor;
	HBRUSH hb;
	
	switch (message) 
	{
	case WM_CREATE:
#ifndef _WIN64
		SetWindowLong(hWnd,GWL_USERDATA,RGB(128,128,128));
#else
		SetWindowLongPtr(hWnd,GWLP_USERDATA,RGB(128,128,128));
#endif
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rt);
#ifndef _WIN64
		color=GetWindowLong(hWnd,GWL_USERDATA);
#else
		color=(COLORREF)GetWindowLongPtr(hWnd,GWLP_USERDATA);
#endif
		hb=CreateSolidBrush(color);
		FillRect(hdc, &rt, hb);
		DeleteObject(hb);
		DrawEdge(hdc, &rt, EDGE_SUNKEN, BF_RECT);

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
	//case WM_LBUTTONDBLCLK:
#ifndef _WIN64
		color=GetWindowLong(hWnd, GWL_USERDATA);
#else
		color=(COLORREF)GetWindowLongPtr(hWnd, GWLP_USERDATA);
#endif
		if( MyChooseColor(color, hWnd) )
		{
#ifndef _WIN64
			SetWindowLong(hWnd,GWL_USERDATA,color);
#else
			SetWindowLongPtr(hWnd,GWLP_USERDATA,color);
#endif
			GetClientRect(hWnd, &rt);
			InvalidateRect( hWnd, &rt, TRUE);
		}
		break;
	case WM_GETCOLOR:
		pcolor=(COLORREF *)lParam;
#ifndef _WIN64
		*pcolor=GetWindowLong(hWnd,GWL_USERDATA);
#else
		*pcolor=(COLORREF)GetWindowLongPtr(hWnd,GWLP_USERDATA);
#endif
		break;
	case WM_SETCOLOR:
		color=(COLORREF)lParam;
#ifndef _WIN64
		SetWindowLong(hWnd, GWL_USERDATA, color);
#else
		SetWindowLongPtr(hWnd, GWLP_USERDATA, color);
#endif
		GetClientRect(hWnd, &rt);
		InvalidateRect( hWnd, &rt, TRUE);
		break;
	default:
		//return DefWindowProc(hWnd, message, wParam, lParam);
		return CallWindowProc(fcnOldWndProc_ColorCtrl, hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM RegisterColorControl()
{	
	WNDCLASS wcls;
	// check to see if class already registered
	static const TCHAR szClass[] = _T("colorcontrol");
	if (GetClassInfo(CGlobal::MyGetModuleHandle(), szClass, &wcls))
	{
		return 1;// name already registered - ok if it was us
	}
	// Use standard "button" control as a template.
	GetClassInfo(NULL, _T("button"), &wcls);
	fcnOldWndProc_ColorCtrl = wcls.lpfnWndProc;
	// set new values
	wcls.style |= CS_DBLCLKS; // Make it to receive double clicks
	wcls.lpfnWndProc = (WNDPROC)ColorControlProc;
	wcls.hInstance = CGlobal::MyGetModuleHandle();
	wcls.lpszClassName = szClass;
	
	return RegisterClass(&wcls);
}

Declare_Namespace_End