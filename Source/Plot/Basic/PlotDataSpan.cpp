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

#include "PlotDataSpan.h"

Declare_Namespace_CChart


CPlotDataSpan::CPlotDataSpan()
{
	SetDefaults();
}

CPlotDataSpan::~CPlotDataSpan()
{

}

void	CPlotDataSpan::SetDefaults()
{
	m_bSpanMode = false;
	m_bSpanHorizontal = false;
	m_fSpan[0] = m_fSpan[1] = 0.0;
	m_crSpan[0] = m_crSpan[1] = RGB(128, 128, 128);
	m_bSpanBkgnd = true;
}

Declare_Namespace_End