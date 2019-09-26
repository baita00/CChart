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
#include "ShareXPlot.h"

using namespace NsCChart;

CShareXPlot::CShareXPlot()
{
	SetDefaults();
}

CShareXPlot::~CShareXPlot()
{

}

void	CShareXPlot::SetDefaults()
{
	m_bShareExact = false;
	m_bOriginRange = true;
}

