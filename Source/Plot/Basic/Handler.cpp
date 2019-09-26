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

#include "Handler.h"

#include <windowsx.h>
#include <commctrl.h>

using namespace NsCChart;

CHandler::CHandler()
{
	SetDefaults();
}

CHandler::~CHandler()
{

}

void	CHandler::SetDefaults()
{
	m_bMsgHandled = false;

	m_bReact = true;

	m_bMouseTrack = true;
	
	// User functions
	m_fcnPreMouseMove = 0;
	m_pParaPreMouseMove = 0;
	
	m_fcnPreLButtonDown = 0;
	m_pParaPreLButtonDown = 0;
	
	m_fcnPreLButtonUp = 0;
	m_pParaPreLButtonUp = 0;
	
	m_fcnPreLButtonDblClk = 0;
	m_pParaPreLButtonDblClk = 0;
	
	m_fcnPreKeyDown = 0;
	m_pParaPreKeyDown = 0;
	
	m_fcnPostMouseMove = 0;
	m_pParaPostMouseMove = 0;
	
	m_fcnPostLButtonDown = 0;
	m_pParaPostLButtonDown = 0;
	
	m_fcnPostLButtonUp = 0;
	m_pParaPostLButtonUp = 0;
	
	m_fcnPostLButtonDblClk = 0;
	m_pParaPostLButtonDblClk = 0;
	
	m_fcnPostKeyDown = 0;
	m_pParaPostKeyDown = 0;

	m_pFcnAddPluginMenu = 0;
	m_pFcnInterpretPluginMenu = 0;
	m_pParaAddPlugInMenu = 0;
	m_pParaInterpretPlugInMenu = 0;

	m_bContextMenu = true;

}

bool	CHandler::OnLButtonDown( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_bReact)return false;

	HDC hDC = GetDC(hWnd); 
	bool ret = OnLButtonDown(hDC, point, ctrlKey); 
	ReleaseDC(hWnd, hDC); 
	return ret;
}
bool	CHandler::OnLButtonUp( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_bReact)return false;

	HDC hDC = GetDC(hWnd);
	bool ret = OnLButtonUp(hDC, point, ctrlKey);
	ReleaseDC(hWnd, hDC);
	return ret;
}
bool	CHandler::OnLButtonDblClk( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_bReact)return false;

	HDC hDC = GetDC(hWnd);
	bool ret = OnLButtonDblClk(hDC, point, ctrlKey);
	ReleaseDC(hWnd, hDC);
	return ret;
}
bool	CHandler::OnMouseMove( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_bReact)return false;

	HDC hDC = GetDC(hWnd);
	bool ret = OnMouseMove(hDC, point, ctrlKey);
	ReleaseDC(hWnd, hDC);
	return ret;
}
bool	CHandler::OnMouseLeave( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_bReact)return false;

	HDC hDC = GetDC(hWnd);
	bool ret = OnMouseLeave(hDC, point, ctrlKey);
	ReleaseDC(hWnd, hDC);
	return ret;
}
bool	CHandler::OnContextMenu( HMENU hMenu, HWND hWnd, POINT point )
{
	if(!m_bReact)return false;

	HDC hDC = GetDC(hWnd);
	bool ret = OnContextMenu(hMenu, hDC, point);
	ReleaseDC(hWnd, hDC);
	return ret;
}
bool	CHandler::OnKeyDown( HWND hWnd, UINT key )
{
	if(!m_bReact)return false;

	HDC hDC = GetDC(hWnd);
	bool ret = OnKeyDown(hDC, key);
	ReleaseDC(hWnd, hDC);
	return ret;
}

bool	CHandler::OnEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(!m_bReact)return false;

	POINT	point;
	//point.x = LOWORD(lParam);
	//point.y = HIWORD(lParam);
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);
	UINT	ctrlKey = (UINT)wParam;
	UINT	key = (UINT)wParam;
	
	switch(message)
	{
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd, point, ctrlKey);
	case WM_LBUTTONUP:
		return OnLButtonUp(hWnd, point, ctrlKey);
	case WM_LBUTTONDBLCLK:
		return OnLButtonDblClk(hWnd, point, ctrlKey);
	case WM_MOUSEMOVE:
		return OnMouseMove(hWnd, point, ctrlKey);
	case WM_MOUSELEAVE:
		return OnMouseLeave(hWnd, point, ctrlKey);
	case WM_CONTEXTMENU:
		return OnContextMenu(NULL, hWnd, point);
	case WM_KEYDOWN:
		return OnKeyDown(hWnd, key);
	case WM_SIZE:
		return true;
	default:
		return false;
	}
}