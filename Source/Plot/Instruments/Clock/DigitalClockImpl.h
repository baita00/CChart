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

#include "DigitalClock.h"
#include "../InstruBase/InstruBaseImpl.h"
#include "../SevenSegmentCode/SevenSegmentCodeImpl.h"

Declare_Namespace_CChart

class CDigitalClockImpl : public CDigitalClock, public CInstruBaseImpl
{
public:
	CDigitalClockImpl();
	virtual	~CDigitalClockImpl();

public:
	virtual	void	OnDraw(HWND hWnd, RECT destRect);
	virtual	void	OnDraw(HDC hDC, RECT destRect);

protected:
	void			GetYmdAdnHmsRect(RECT destRect, RECT *plotRect, RECT *ymdRect, RECT *hmsRect);

protected:
	void			DrawHMS(HDC	hDC, RECT hmsRect);
	void			DrawYMD(HDC hDC, RECT ymdRect);

public:
	CSevenSegmentCodeImpl	m_SSC;
};

Declare_Namespace_End
