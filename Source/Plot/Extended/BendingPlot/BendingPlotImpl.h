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

#include "BendingPlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class CBendingPlotImpl : public CBendingPlot, public CXYPlotImplI
{
public:
	CBendingPlotImpl();
	virtual	~CBendingPlotImpl();

	virtual	TCHAR* GetTypeName(){return _TEXT("BendingPlot");}

public:
	void	InitBendingAxis();
	
public:
	int		AddCurve( double *pX, double *pY, int nLen );
};

typedef CBendingPlotImpl CBendingPlotImplI;

Declare_Namespace_End