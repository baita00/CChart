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

#include "PlotDataErrorBar.h"
using namespace NsCChart;

std::vector<double>	CPlotDataErrorBar::m_vfUBound;
std::vector<double>	CPlotDataErrorBar::m_vfLBound;
bool	CPlotDataErrorBar::m_bErrorBarMode = false;

CPlotDataErrorBar::CPlotDataErrorBar()
{
	SetDefaults();
}

CPlotDataErrorBar::~CPlotDataErrorBar()
{

}

void	CPlotDataErrorBar::SetDefaults()
{
	m_bErrorBarMode = false;
	m_nErrorBarLineSize = 1;
	m_nErrorBarLineStyle = PS_DASH;
	m_bErrorBarSameColor = true;
	m_crErrorBarLineColor = RGB(128, 128, 128);
	m_nErrorBarHeadHalfLen = 6;

	m_vfLBound.clear();
	m_vfUBound.clear();
}