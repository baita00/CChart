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

//#include "StdAfx.h"
#include "PiePlot.h"
#include <tchar.h>

using namespace NsCChart;

CPiePlot::CPiePlot()
{
	SetDefaults();
}

CPiePlot::~CPiePlot()
{

}

void	CPiePlot::SetDefaults()
{
	m_nMaxPies = 10;
	m_fScale = 0.8;
	m_nLineSize = 1;
	m_nLineStyle = PS_SOLID;
	m_crLineColor = RGB(128,168,128);
	m_bCircleMode = false;

	m_bShowPieTitle = true;
	m_bShowPieTitleRatio = true;
	m_nPieTitleRatioPrecision = 3;
	m_fStartAngle = 90.0;
	m_bClockWise = false;

	m_bShadowing = false;
	m_n3DHalfLength = 2;
	m_f3DAngle = 90.0;
	m_crShadowColor = RGB(128,128,128);
	m_b3DStyle = false;
	m_bSplit = false;

	SIZE offset={0,0};
	m_sizeOffset = offset;
	m_bDragMode = false;

	HFONT	hFont=CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _TEXT("Arial") );
	GetObject(hFont,sizeof(LOGFONT),&m_lfPieTitleFont);
	DeleteObject(hFont);
}


