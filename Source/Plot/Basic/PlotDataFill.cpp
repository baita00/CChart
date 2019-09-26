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

#include "PlotDataFill.h"
#include "Plot.h"

using namespace NsCChart;

CPlotDataFill::CPlotDataFill()
{
	m_hDataFillPattern = NULL;
	SetDefaults();
}

CPlotDataFill::~CPlotDataFill()
{
	DeleteUsedHandles();
}

void	CPlotDataFill::SetDefaults()
{
	m_nDataFillMode = kDataFillFromBottomAxis;
	m_bDataFill = false;
	m_nDataFillBrushType = kBrushSolid;
	m_crDataFillColor = RGB(240,240,168);
	m_nDataFillHatchStyle = HS_CROSS;
	if(!m_hDataFillPattern)
	{
		DeleteObject(m_hDataFillPattern);
	}
	m_hDataFillPattern = NULL;
	m_nDataFillAlpha = 255;
}

void	CPlotDataFill::DeleteUsedHandles()
{
	if(m_hDataFillPattern)
	{
		DeleteObject(m_hDataFillPattern);
	}
	m_hDataFillPattern = 0;
}