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

#include "ScrollBar.h"
#include "../InstruBase/InstruBaseImpl.h"

Declare_Namespace_CChart

class CScrollBarImpl : public CScrollBar, public CInstruBaseImpl
{
public:
	CScrollBarImpl();
	virtual	~CScrollBarImpl();

public:
	void	OnDraw(HWND hWnd, RECT destRect);
	void	OnDraw(HDC hDC, RECT destRect);
	RECT	GetBarRect(HDC hDC, RECT plotRect);
	RECT	GetSliderRect(HDC hDC, RECT barRect);
	RECT	GetLowerRect(HDC hDC, RECT barRect);
	RECT	GetUpperRect(HDC hDC, RECT barRect);
	RECT	GetLowerPRect(HDC hDC, RECT barRect);
	RECT	GetUpperPRect(HDC hDC, RECT barRect);

protected:
	void	DrawSlider(HDC hDC, RECT barRect);
	long	GetPosInBar(double val, RECT barRect, bool bHoriz);
	double	GetValInBar(long pos, RECT barRect, bool bHoriz);

public:
	double	GetRatio(RECT barRect);

	bool	HitLower(POINT point, RECT barRect);
	bool	HitUpper(POINT point, RECT barRect);
};

Declare_Namespace_End