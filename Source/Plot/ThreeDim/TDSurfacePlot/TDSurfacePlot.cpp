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
#include <windows.h>
#include "TDSurfacePlot.h"
#include "../TDPlot/TDPlot.h"

using namespace NsCChart;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

namespace NsCChart{
extern double TestFunction(double x, double y);

CTDSurfacePlot *CTDSurfacePlot::m_pThis = 0;
}

void CTDSurfacePlot::f4min(int *n, double x[2], double *y)
{
	*y = m_pThis->m_pFieldFcn(x[0], x[1]);
}

void CTDSurfacePlot::f4max(int *n, double x[2], double *y)
{
	*y = -m_pThis->m_pFieldFcn(x[0], x[1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTDSurfacePlot::CTDSurfacePlot()
{
	m_pThis = this;
	SetDefaults();
}

CTDSurfacePlot::~CTDSurfacePlot()
{
}

void	CTDSurfacePlot::SetDefaults()
{
	m_fvColor1[0] = 1.0f;
	m_fvColor1[1] = 0.0f;
	m_fvColor1[2] = 0.0f;
	m_fvColor2[0] = 0.0f;
	m_fvColor2[1] = 1.0f;
	m_fvColor2[2] = 1.0f;

	m_pFieldFcn = TestFunction;

	m_nXSteps = m_nYSteps = 32;
	m_nXMapSteps = m_nYMapSteps = 20;

	m_bExponential = false;
	m_bLighting = true;
}




