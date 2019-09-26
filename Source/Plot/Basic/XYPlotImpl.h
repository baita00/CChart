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

#include "PlotImpl.h"
#include "XYPlot.h"
#include "DataSetImpl.h"
#include "LegendImpl.h"
#include "XYPlotBasicImpl.h"
#include "XYHandler.h"
#include "XYReactStatus.h"
#include "More/RangeManageImpl.h"
#include "More/OscilloScopeImpl.h"
#include "More/TooltipImpl.h"

Declare_Namespace_CChart

class CXYPlotImpl : public CXYPlotBasicImpl,
					public CLegendImpl<CXYPlotImpl>					
{
public:
	CXYPlotImpl();
	virtual ~CXYPlotImpl();
	
public:
	TCHAR*				GetTypeName(){return _TEXT("XYPlot");}
//protected:
	virtual	RECT		AddtionalPlotRectAdjust(HDC hDC, RECT plotRect);
	virtual	tstring		GetDescriptionByMousePos(HDC hDC, POINT point){return _TEXT("");}
		
public:	
	int					GetDataIDByPoint( HDC hDC, POINT point );
	
public:
	virtual	void		FinalDraw( HDC hDC, RECT destRect );
	
public:
	int					RegionIdentifyEx(HDC hDC, POINT point, bool &legendRegion, int &centerAxisRegion);
	
public:
	void				SetApperanceMode(int mode);
	void				UniteToColor( COLORREF color );
};

class	CXYPlotImplI :	public CXYPlotImpl,
						public CTooltipImpl<CXYPlotImplI>,
						public CXYHandler<CXYPlotImplI>,
						public CXYReactStatus<CXYPlotImplI>,						
						public CRangeManageImpl<CXYPlotImplI>,
						public COscilloScopeImpl<CXYPlotImplI>
{
public:
	CXYPlotImplI();
	virtual ~CXYPlotImplI();
public:
	tstring				GetDescriptionByMousePos(HDC hDC, POINT point);
	//bool				ShowDlgBySelection(HDC hDC);
public:
	void				FinalDraw( HDC hDC, RECT destRect );
};

Declare_Namespace_End
