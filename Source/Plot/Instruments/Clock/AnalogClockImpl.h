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

#include "AnalogClock.h"
//#include "ToyBaseImpl.h"
#include "../Dial/DialImpl.h"

Declare_Namespace_CChart

class CAnalogClockImpl : public CAnalogClock, public CDialImpl
{
public:
	CAnalogClockImpl();
	virtual	~CAnalogClockImpl();

public:
	virtual	void	OnDraw(HWND hWnd, RECT destRect);
	virtual	void	OnDraw(HDC hDC, RECT destRect);

protected:
	void			DrawPointer(HDC hDC, RECT barRect);

public:
	void			SetShowTime(bool show);
};

Declare_Namespace_End
