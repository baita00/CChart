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

#include "GanttPlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class	CGanttPlotImpl : public CGanttPlot, public CXYPlotImplI
{
public:
	CGanttPlotImpl();
	virtual ~CGanttPlotImpl();

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("GanttPlot");}
	int		AddGantt(tstring t1, tstring t2, tstring fmt);
	int		AddGantt(tstring t1, tstring t2, tstring fmt, tstring title);

protected:
	void	DrawSinglePlotData( int which, HDC hDC, double *xRange, double *yRange );
	void	GetPlotRange( double *xRange, double *yRange );
	RECT	GetDataRect( int which, RECT plotRect, double *xRange, double *yRange  );
	void	GetGanttTitleRect( HDC hDC, RECT plotRect, tstring titleL, tstring titleR, RECT dataRect, RECT &leftRect, RECT &rightRect );

public:
	int		GetNeareastPlotData( HDC hDC, POINT point );
	bool	PtInDataArea( int which, HDC hDC, POINT point, RECT plotRect, double *xRange, double *yRange );
};

typedef CGanttPlotImpl CGanttPlotImplI;

Declare_Namespace_End