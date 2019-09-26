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

#include "Gradienter.h"
#include "../InstruBase/InstruBaseImpl.h"

using namespace NsCChart;

class CGradienterImpl : public CGradienter, public CInstruBaseImpl
{
public:
	CGradienterImpl();
	virtual	~CGradienterImpl();

public:
	virtual	void	OnDraw(HWND hWnd, RECT destRect);
	virtual	void	OnDraw(HDC hDC, RECT destRect);

protected:
	virtual	void	DrawBar(HDC hDC, RECT barRect);
	virtual	void	DrawPointer(HDC hDC, RECT barRect);

};

