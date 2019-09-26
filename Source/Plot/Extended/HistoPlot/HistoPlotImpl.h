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
#include "HistoPlot.h"

Declare_Namespace_CChart

class CHistoPlotImpl : public CHistoPlot, public CXYPlotImplI
{
public:
	CHistoPlotImpl();
	virtual	~CHistoPlotImpl();

public:
	bool		Statistic();
};

typedef CHistoPlotImpl CHistoPlotImplI;

Declare_Namespace_End