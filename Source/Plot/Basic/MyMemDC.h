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

#ifndef _CMYMEMDC_H
#define _CMYMEMDC_H

#include <windows.h>
#include "CChartNameSpace.h"

Declare_Namespace_CChart

class CMyMemDC
{
public:
	CMyMemDC();
	CMyMemDC(HDC hDC, HBITMAP hBitmap = NULL, RECT *lpRect = NULL);
	virtual ~CMyMemDC();

public:
	void			Update();
	bool			CheckCompatibility(HBITMAP hBitmap);

public:
    // Allow usage as a Handle    
    operator HDC();
	// Allow usage as a pointer    
    CMyMemDC* operator->();  
    // Allow usage as a pointer    
    operator CMyMemDC*();

protected:
	RECT			m_rtRect;
	HDC				m_hOriginDC;
	HBITMAP			m_hBmp;
	HBITMAP			m_hOldBmp;

	HDC				m_hDC;

public:
	void			CreateMemDC(HDC hDC, HBITMAP hBitmap, RECT *lpRect);

public:
	static	HBITMAP	HBitmapFromDC(HDC hDC);
	static	HBITMAP	HBitmapFromWnd(HWND hWnd);
	static	HBITMAP	CopyHBitmap(HBITMAP hSourceHbitmap);
};

Declare_Namespace_End

#endif