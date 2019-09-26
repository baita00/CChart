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

#include "MyMemDC.h"

using namespace NsCChart;

CMyMemDC::CMyMemDC()
{
	m_hOriginDC = 0;
	m_hBmp = 0;
	m_hOldBmp = 0;
	m_hDC = 0;
}

CMyMemDC::CMyMemDC(HDC hDC, HBITMAP hBitmap, RECT *lpRect)
{
	m_hOriginDC = 0;
	m_hBmp = 0;
	m_hOldBmp = 0;
	m_hDC = 0;
	CreateMemDC(hDC, hBitmap, lpRect);
}

CMyMemDC::~CMyMemDC()
{
	Update();

	SelectObject(m_hDC, m_hOldBmp);
	DeleteObject(m_hBmp);
	DeleteDC(m_hDC);
}

CMyMemDC::operator HDC()
{
	return this->m_hDC;
}

CMyMemDC* CMyMemDC::operator->()
{
	return this;
}

CMyMemDC::operator CMyMemDC*()
{
	return this;
}

void		CMyMemDC::Update()
{
	BitBlt(m_hOriginDC, m_rtRect.left, m_rtRect.top, m_rtRect.right - m_rtRect.left, m_rtRect.bottom - m_rtRect.top,
		m_hDC, m_rtRect.left, m_rtRect.top, SRCCOPY);
		//m_hDC, 0, 0, SRCCOPY); 
}

bool		CMyMemDC::CheckCompatibility(HBITMAP hBitmap)
{
	BITMAP bm, bmMem;
	GetObject(hBitmap, sizeof(BITMAP), &bm);
	GetObject(m_hBmp, sizeof(BITMAP), &bmMem);

	return bm.bmWidth == bmMem.bmWidth && bm.bmHeight == bmMem.bmHeight;
}

void		CMyMemDC::CreateMemDC(HDC hDC, HBITMAP hBitmap, RECT *lpRect)
{
	//Get rect
	if(!lpRect)
	{
		::GetClipBox(hDC, &m_rtRect);
	}
	else
	{
		m_rtRect = *lpRect;
	}
	
	// Create memory dc
	m_hDC = CreateCompatibleDC(hDC);
	//LPtoDP(hDC, (LPPOINT)&m_rtRect, 2);
	
	if(hBitmap)
	{
		m_hBmp = CopyHBitmap(hBitmap);
		m_hOldBmp = (HBITMAP)SelectObject(m_hDC, m_hBmp);
	}
	else
	{
		m_hBmp = CreateCompatibleBitmap(hDC, m_rtRect.right - m_rtRect.left, m_rtRect.bottom - m_rtRect.top);
		m_hOldBmp = (HBITMAP)SelectObject(m_hDC, m_hBmp);
		
		// Fill the memory dc with the contents of the original dc
		//BitBlt(m_hDC, m_rtRect.left, m_rtRect.top, m_rtRect.right - m_rtRect.left, m_rtRect.bottom - m_rtRect.top, 
		BitBlt(m_hDC, 0, 0, m_rtRect.right - m_rtRect.left, m_rtRect.bottom - m_rtRect.top, 
			hDC, m_rtRect.left, m_rtRect.top, SRCCOPY);
	}
	
	// set window property
	SetMapMode(m_hDC, GetMapMode(hDC));
	
	SIZE someSize;
	GetWindowExtEx(hDC, &someSize);
	SetWindowExtEx(m_hDC, someSize.cx, someSize.cy, NULL);
	GetViewportExtEx(hDC, &someSize);
	SetViewportExtEx(m_hDC, someSize.cx, someSize.cy, NULL);
	
	//DPtoLP(hDC, (LPPOINT)&m_rtRect, 2);
	SetWindowOrgEx(m_hDC, m_rtRect.left, m_rtRect.top, NULL);
	//SetWindowOrgEx(m_hDC, 0, 0, NULL);

	SetBkMode(m_hDC, TRANSPARENT);
	
	// destination DC
	m_hOriginDC = hDC;
}

HBITMAP	CMyMemDC::HBitmapFromDC(HDC hDC)
{
	HDC hMemDC;
	hMemDC = CreateCompatibleDC(hDC);
	
	HBITMAP hBm, hOldBm;
	RECT rt;
	GetClientRect(WindowFromDC(hDC), &rt);
	hBm = CreateCompatibleBitmap(hDC, rt.right-rt.left, rt.bottom-rt.top);
	hOldBm = (HBITMAP)SelectObject(hMemDC, hBm);
	BitBlt(hMemDC, rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top, hDC, rt.left, rt.top, SRCCOPY);
	hBm = (HBITMAP)SelectObject(hMemDC, hOldBm);
	
	DeleteDC(hMemDC);
	
	return hBm;
}

HBITMAP	CMyMemDC::HBitmapFromWnd(HWND hWnd)
{
	if(!IsWindow(hWnd))
		return 0;
	HDC hDC = GetDC(hWnd);
	HBITMAP hBm = HBitmapFromDC(hDC);
	ReleaseDC(hWnd, hDC);
	return hBm;
}

HBITMAP	CMyMemDC::CopyHBitmap(HBITMAP hSourceHbitmap)
{
	HDC sourceDC; 
	HDC destDC; 
	sourceDC = CreateCompatibleDC(NULL); 
	destDC = CreateCompatibleDC(NULL); 
	//The bitmap information. 
	BITMAP bm = {0}; 
	//Get the bitmap information. 
	::GetObject(hSourceHbitmap, sizeof(bm), &bm); 
	// Create a bitmap to hold the result 
	HBITMAP hbmResult = ::CreateCompatibleBitmap(destDC, bm.bmWidth, bm.bmHeight); 
	
	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC, hSourceHbitmap); 
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult); 
	BitBlt(destDC, 0, 0, bm.bmWidth, bm.bmHeight, sourceDC, 0, 0, SRCCOPY ); 
	
	// Restore DCs 
	SelectObject(sourceDC, hbmOldSource); 
	SelectObject(destDC, hbmOldDest); 
	DeleteObject(sourceDC); 
	DeleteObject(destDC); 
	
	return hbmResult; 
}
