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

#pragma once

#include "SevenSegmentCode.h"
#include "../InstruBase/InstruBaseImpl.h"
#include <windows.h>

Declare_Namespace_CChart

class CSevenSegmentCodeImpl : public CSevenSegmentCode , public CInstruBaseImpl
{
public:
	CSevenSegmentCodeImpl();
	virtual	~CSevenSegmentCodeImpl();

public:
	virtual	void	OnDraw(HWND hWnd, RECT destRect);
	virtual	void	OnDraw(HDC hDC, RECT destRect);

protected:
	RECT			GetPlotRect(HDC hDC, RECT destRect);
	RECT			GetBarRect(HDC hDC, RECT plotRect);

	RECT			GetSingleSegRect(HDC hDC, RECT barRect, int nIndex);

protected:
	void			DrawSingleSeg(HDC hDC, RECT barRect, int nIndex, bool bLight);

public:
	SIZE			GetSize(int val);
	SIZE			GetSize(int val, RECT destRect);
};

Declare_Namespace_End