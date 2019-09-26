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
#include "TDLinePlot.h"
#include "../../Basic/XYPlot.h"

using namespace NsCChart;

CTDLinePlot::CTDLinePlot()
{
	SetDefaults();
}

CTDLinePlot::~CTDLinePlot()
{
	
}

void	CTDLinePlot::SetDefaults()
{
//	m_bAutoDataColor = true;
//	m_nModeOfSetColors = kSetColorFromTable;
	m_fvLineColor1[0] = 1.0f;
	m_fvLineColor1[1] = 0.0f;
	m_fvLineColor1[2] = 0.0f;
	m_fvLineColor1[3] = 0.0f;
	m_fvLineColor2[0] = 0.0f;
	m_fvLineColor2[1] = 1.0f;
	m_fvLineColor2[2] = 1.0f;
	m_fvLineColor2[3] = 1.0f;
}
