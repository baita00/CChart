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

#include "PolarPlot.h"

using namespace NsCChart;

CPolarPlot::CPolarPlot()
{
	SetDefaults();
}

CPolarPlot::~CPolarPlot()
{

}

void		CPolarPlot::SetDefaults()
{
	m_bUseMajorRGrids = true;
	m_bUseMajorPGrids = true;
	m_bUseMinorRGrids = false;
	m_bUseMinorPGrids = false;
}
