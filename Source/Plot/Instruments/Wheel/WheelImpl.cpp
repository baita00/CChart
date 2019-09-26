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

#include "WheelImpl.h"

using namespace NsCChart;

CWheelImpl::CWheelImpl()
{
	m_nInstruShape = kInstruShapeWheel;

	//m_bBorder3D = true;

	m_crBkColor = RGB(192, 192, 192);

	m_fCurrSizeAngle = 2.0*myPi/60.0;

	SetRange(0, 10);

	m_fAngleStart = -m_fAngleRange/2.0;

	m_bFillCurr = false;
//	m_bShowBar = false;
	m_bFillWholeCircle = false;
}

CWheelImpl::~CWheelImpl()
{

}

void	CWheelImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CWheelImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);

	DrawBorder(hDC, plotRect);
	DrawBar(hDC, barRect);
}

