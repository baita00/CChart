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
#pragma warning(disable:4786)
#include "PlotImpl.h"
#include "XYPlot.h"
#include "DataSetImpl.h"
#pragma warning(disable:4786)
#include "XYHandler.h"
#include "DragStatus.h"
#include "XYReactStatus.h"
#include "More/RangeManageImpl.h"
#include "More/OscilloScopeImpl.h"

Declare_Namespace_CChart

class CXYPlotBasicImpl :	public CXYPlot,
							public CPlotImpl,
							public CDataSetImpl<CXYPlotBasicImpl>,
							public CDragStatus<CXYPlotBasicImpl>
{
public:
	CXYPlotBasicImpl();
	virtual ~CXYPlotBasicImpl();

protected:
	void				ApproachNearestIdx( int curIdx,int &nIdx,double *data,int lo,int up,double dlM,double &dl0 );
	void				ApproachNearestIdxByY( int curIdx,int &nIdx,double *data,int lo,int up,double dlM,double &dl0 );

public:
	int					LightByPoint( HDC hDC, POINT point );
	int					SelectByPoint( HDC hDC, POINT point );

	virtual	int			GetDataIDByPoint( HDC hDC, POINT point );
	
	virtual	int			GetNeareastPlotData( HDC hDC, POINT point );
	virtual	int			GetNeareastDataPoint( int dataID, HDC hDC, POINT point, double data[2]);
	int					GetNeareastDP( int dataID, HDC hDC, POINT point);

public:
	void				GetDataRange1D( int whichDim, double *range );
	void				GetPlotRange1D( int whichDim, double *range );

	bool				CheckLogSafety();
	bool				CheckLogSafety(bool bX);

protected:
	void				DrawDefScreen(HDC hDC, RECT destRect);

public:
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

public:
	virtual	int			RegionIdentifyEx(HDC hDC, POINT point, bool &legendRegion, int &centerAxisRegion);

	inline	bool		IsNoDataPlot(){return false;}

	//virtual tstring		GetAbsDataTitle( int which );
	virtual tstring		GetAbsDataAppendix( int which );

public:
	virtual	void		UniteToColor( COLORREF color );
};

Declare_Namespace_End
