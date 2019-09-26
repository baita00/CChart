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

#include "PartialDial.h"
#include "DialImpl.h"

Declare_Namespace_CChart

class CPartialDialImpl : public CPartialDial, public CDialImpl
{
public:
	CPartialDialImpl();
	virtual	~CPartialDialImpl();

public:
	virtual	void	OnDraw(HWND hWnd, RECT destRect);
	virtual	void	OnDraw(HDC hDC, RECT destRect);

protected:
	virtual	RECT	GetPlotRect(HDC hDC, RECT destRect);
	RECT			GetPartialBarRect(HDC hDC, RECT plotRect);
	RECT			GetPartialAxisRect(HDC hDC, RECT barRect);
	virtual	RECT	GetSubTitleRect(HDC hDC, RECT barRect);

protected:
	RECT			GetPsedoAxisRect(HDC hDC, RECT axisRect);
	RECT			GetPsedoBarRectFromAxis(HDC hDC, RECT psedoAxisRect);

protected:
	void			DrawGlass(HDC hDC, RECT barRect, RECT psedoBarRect);
};

Declare_Namespace_End
