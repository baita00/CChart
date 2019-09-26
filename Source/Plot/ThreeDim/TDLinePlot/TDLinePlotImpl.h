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

#include "../TDPlot/TDPlotImpl.h"
#include "../TDPlot/TDHandler.h"
#include "TDLinePlot.h"
#include "DataSet3DImpl.h"

Declare_Namespace_CChart

class CTDLinePlotImpl : public CTDLinePlot,
						public CTDPlotImpl,
						public CDataSet3DImpl<CTDLinePlotImpl>
{
public:
	CTDLinePlotImpl(HWND hWnd);
	~CTDLinePlotImpl();

	virtual	TCHAR*		GetTypeName(){return _TEXT("TDLinePlot");}
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

protected:
	void		GetDataRange1D( int whichDim, double *range );//virtual funciton from CRanges
	void		GetPlotRange( double *xRange, double *yRange, double *zRange );

	void		DrawDataSet( double *xRange, double *yRange, double *zRange );
	void		DrawSinglePlotData( int which, double *xRange, double *yRange, double *zRange );
	void		DrawMarkers( int which, double *xRange, double *yRange, double *zRange );

};

class CTDLinePlotImplI : public CTDLinePlotImpl, public CTDHandler<CTDLinePlotImplI>
{
public:
	CTDLinePlotImplI( HWND hWnd );
	virtual ~CTDLinePlotImplI();
};

Declare_Namespace_End