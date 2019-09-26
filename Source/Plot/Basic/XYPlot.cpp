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
#pragma warning(disable:4786)
#include "XYPlot.h"

using namespace NsCChart;

CXYPlot::CXYPlot()
{
	SetDefaults();
}

CXYPlot::~CXYPlot()
{
	
}

void	CXYPlot::SetDefaults()
{	
	m_bAutoDataColorGeneral = true;
	m_nModeOfSetColors = kSetColorFromTable;
	m_crLineColor1		= RGB(255,0,0);
	m_crLineColor2		= RGB(255,255,0);
	m_bAutoLineStyleGeneral = true;
	m_nAutoColorStartIndex = 0;

	m_bKeepFormer = true;

	m_fHitPrecision = 128;

	m_bClipEllipse = false;
}

