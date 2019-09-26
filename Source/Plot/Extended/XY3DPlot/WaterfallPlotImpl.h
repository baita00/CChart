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

#include "WaterfallPlot.h"
#include "XY3DPlotImpl.h"

Declare_Namespace_CChart

class	CWaterfallPlotImpl : public CWaterfallPlot, public CXY3DPlotImplI
{
public:
	CWaterfallPlotImpl();
	virtual ~CWaterfallPlotImpl();

//	void	SetDefaults( int index );

public:
	int		AddCurve( double *pX, double *pY, double z, int nLen );
	virtual	int		AddPoint( double x, double y, double z, int nWhich);

};

typedef CWaterfallPlotImpl CWaterfallPlotImplI;

Declare_Namespace_End