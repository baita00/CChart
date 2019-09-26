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

#include "Slider.h"
#include "../InstruBase/InstruBaseImpl.h"

Declare_Namespace_CChart

class CSliderImpl : public CSlider, public CInstruBaseImpl
{
public:
	CSliderImpl();
	virtual	~CSliderImpl();

public:
	virtual	void	OnDraw(HWND hWnd, RECT destRect);
	virtual	void	OnDraw(HDC hDC, RECT destRect);

protected:
	virtual	RECT	GetBarRect(HDC hDC, RECT plotRect);
	virtual	RECT	GetAxisRect(HDC hDC, RECT barRect);

protected:
	virtual	void	DrawBar(HDC hDC, RECT barRect);
	virtual	void	DrawCurrentFilling(HDC hDC, RECT barRect);
	void			DrawBarCenter(HDC hDC, RECT barRect);
	virtual	void	DrawSlider(HDC hDC, RECT barRect);
};

Declare_Namespace_End