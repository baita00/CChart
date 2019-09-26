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

#include "RingPlot.h"

using namespace NsCChart;

CRingPlot::CRingPlot()
{
	SetDefaults();
}

CRingPlot::~CRingPlot()
{

}

void		CRingPlot::SetDefaults()
{
	m_fInnerRatio = 0.6;
}

void		CRingPlot::CopySettings(CRingPlot *plot)
{
	m_fInnerRatio = plot->m_fInnerRatio;
}

