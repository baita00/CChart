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

#include "XY3DPlot.h"

using namespace NsCChart;

CXY3DPlot::CXY3DPlot()
{
	SetDefaults();
}

CXY3DPlot::~CXY3DPlot()
{
	
}

void		CXY3DPlot::SetDefaults( )
{
	m_fLenZAxis		=	150.0;
	m_fAngleZAxis	=	30.0;

	m_bYZMajorHGrids	=	true;
	m_bYZMajorVGrids	=	true;
	m_bYZMinorHGrids	=	false;
	m_bYZMinorVGrids	=	false;

	m_bZXMajorHGrids	=	true;
	m_bZXMajorVGrids	=	true;
	m_bZXMinorHGrids	=	false;
	m_bZXMinorVGrids	=	false;
}
