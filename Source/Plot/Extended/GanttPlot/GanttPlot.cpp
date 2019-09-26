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

#include "GanttPlot.h"

using namespace NsCChart;

CGanttPlot::CGanttPlot()
{
	SetDefaults();
}

CGanttPlot::~CGanttPlot()
{

}

void	CGanttPlot::SetDefaults( )
{
	m_fRatioOccupy = 0.2;
	m_bReverseOrder = true;

	m_bDrawGanttTitle = true;
	m_strGanttTitleFormat = _TEXT("%Y年%m月%d日");
	m_bGanttTitleUpper = true;

	HFONT	hFont=CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _TEXT("Arial") );
	GetObject(hFont,sizeof(LOGFONT),&m_lfGanttTitleFont);
	DeleteObject(hFont);
}

