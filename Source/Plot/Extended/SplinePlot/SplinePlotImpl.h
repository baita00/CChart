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

#pragma once

#include "../../Basic/XYPlotImpl.h"
#include "../../Basic/XYHandler.h"
#include "SplinePlot.h"
#include "Spline.h"

Declare_Namespace_CChart

class CSplinePlotImpl : public CSplinePlot, public CXYPlotImplI
{
public:
	CSplinePlotImpl();
	virtual ~CSplinePlotImpl();

	int AddSplineByNum(MyVData2D vData, int nSplineMode = kSplModeParabolic,
		COLORREF color = RGB(255, 0, 0), int nSegments = 20, int nOrder = 2,
		int nEdgeMode = kSplEdgeModeFree); 
	int AddSplineByStep(MyVData2D vData, int nSplineMode = kSplModeParabolic,
		COLORREF color = RGB(255, 0, 0), double fMaxStep = 1.0,
		int nOrder = 2, int nEdgeMode = kSplEdgeModeFree); 

	int AddSpline(MyVData2D vData)
	{
		return AddSplineByNum(vData);
	}
};

typedef CSplinePlotImpl CSplinePlotImplI;

Declare_Namespace_End