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

#include "../../Basic/MyGdiPlus.h"
#include "CompassImpl.h"

using namespace NsCChart;

CCompassImpl::CCompassImpl()
{
	m_nInstruShape = kInstruShapeCircle;

	Set8Directions();

	m_nTickLength = 15;
	m_nMinorTickLength = 10;

	m_bShowPointer = false;
	m_bShowStaticPointer = true;

	m_bShowBar = false;
	m_bShowBorder = false;

	m_fAngleStart = myPi/2.0;
	m_bPointerCircle3D = false;
}

CCompassImpl::~CCompassImpl()
{

}

void	CCompassImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CCompassImpl::OnDraw(HDC hDC, RECT destRect)
{
	RECT plotRect = GetPlotRect(hDC, destRect);
	RECT barRect = GetBarRect(hDC, plotRect);

	DrawBorder(hDC, plotRect); 
	DrawWholeCircleFilling(hDC, barRect);
	DrawTitle(hDC, barRect, plotRect);
	DrawAxis(hDC, barRect);
	DrawStaticPointers(hDC, barRect);
	DrawPointer(hDC, barRect);
}

void	CCompassImpl::Set4Directions()
{
	SetRange(0, 4);
	m_nTickCount = 3;
	m_nMinorTickCount = 3;

	m_fAngleStart = myPi/2.0;
	m_bClockWise = true;

	m_bReplaceLabel = true;
	m_vstrAlternateLabels.resize(5);
	m_vstrAlternateLabels[0] = _TEXT("N");
	m_vstrAlternateLabels[1] = _TEXT("E");
	m_vstrAlternateLabels[2] = _TEXT("S");
	m_vstrAlternateLabels[3] = _TEXT("W");
	m_vstrAlternateLabels[4] = _TEXT("N");
}

void	CCompassImpl::Set8Directions()
{
	SetRange(0, 4);
	m_nTickCount = 7;
	m_nMinorTickCount = 2;

	m_fAngleStart = myPi/2.0;
	m_bClockWise = true;
	
	m_bReplaceLabel = true;
	m_vstrAlternateLabels.resize(9);
	m_vstrAlternateLabels[0] = _TEXT("N");
	m_vstrAlternateLabels[1] = _TEXT("NE");
	m_vstrAlternateLabels[2] = _TEXT("E");
	m_vstrAlternateLabels[3] = _TEXT("SE");
	m_vstrAlternateLabels[4] = _TEXT("S");
	m_vstrAlternateLabels[5] = _TEXT("SW");
	m_vstrAlternateLabels[6] = _TEXT("W");
	m_vstrAlternateLabels[7] = _TEXT("NW");
	m_vstrAlternateLabels[8] = _TEXT("N");
}



