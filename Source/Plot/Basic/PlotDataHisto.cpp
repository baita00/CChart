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

#include "PlotDataHisto.h"

using namespace NsCChart;

CPlotDataHisto::CPlotDataHisto()
{
	SetDefaults();
}

CPlotDataHisto::~CPlotDataHisto()
{

}

void	CPlotDataHisto::SetDefaults()
{
	m_bHisto = false;
	m_nHistoMode = kHistoFromBottom;
}