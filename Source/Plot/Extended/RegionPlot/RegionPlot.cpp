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

#include "RegionPlot.h"
using namespace NsCChart;

CRegionPlot::CRegionPlot()
{
	SetDefaults();
	m_pHScroll = new CScrollBarImpl;
	m_pVScroll = new CScrollBarImpl;

	m_pHScroll->SetInstruShape(kInstruShapeHorizontal);
	m_pVScroll->SetInstruShape(kInstruShapeVertical);
}

CRegionPlot::~CRegionPlot()
{
	if(m_pHScroll)delete m_pHScroll;
	if(m_pVScroll)delete m_pVScroll;
}

void	CRegionPlot::SetDefaults()
{
	m_bRegionHorizontal = false;
	m_bRegionVerticle = false;
	m_nHScrollWidth = 24;
	m_nVScrollWidth = 24;
	m_fXRegion[0] = m_fYRegion[0] = 0.0;
	m_fXRegion[1] = m_fYRegion[1] = 1.0;
}