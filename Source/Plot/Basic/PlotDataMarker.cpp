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

#include "PlotDataMarker.h"

using namespace NsCChart;

CPlotDataMarker::CPlotDataMarker()
{
	m_hMarkerImage = 0;
	SetDefaults();
}

CPlotDataMarker::~CPlotDataMarker()
{
	DeleteUsedHandles();
}

void	CPlotDataMarker::SetDefaults()
{
	m_nMarkerType = kXYMarkerSquareUpright;
	m_bMarkerShow = false;
	m_nMarkerSize = 4;
	m_bMarkerFill = true;
	if(!m_hMarkerImage)
	{
		DeleteObject(m_hMarkerImage);
	}
	m_hMarkerImage = 0;
	m_bMarkerImageRound = true;
	m_nMarkerFrequency = -5;
	m_nMarkerStartIndex = 0;
	m_nMarkerFillAlpha = 255;
	m_bMarkerBarVertical = true;
	m_nMarkerBarHWidth = 2;
	m_nMarkerBarHHeight = 4;
	m_nMarkerBarExtLen = 4;
	m_bMarkerFillOtherColor = false;
	m_crMarkerFillColor = RGB(128, 128, 128);
}

void	CPlotDataMarker::DeleteUsedHandles()
{
	if(!m_hMarkerImage)
	{
		DeleteObject(m_hMarkerImage);
	}
	m_hMarkerImage = 0;
}

