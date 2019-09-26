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

#include <windows.h>
#include "TrafficLight.h"
#include "../InstruBase/InstruBaseImpl.h"

Declare_Namespace_CChart

class CTrafficLightImpl : public CTrafficLight, public CInstruBaseImpl
{
public:
	CTrafficLightImpl();
	virtual	~CTrafficLightImpl();

public:
	virtual	void	OnDraw(HWND hWnd, RECT destRect);
	virtual	void	OnDraw(HDC hDC, RECT destRect);

protected:
	virtual	RECT	GetPlotRect(HDC hDC, RECT destRect);
	virtual	RECT	GetBarRect(HDC hDC, RECT plotRect);

	void			DrawLight(HDC hDC, RECT barRect);

};

Declare_Namespace_End
