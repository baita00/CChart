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

#include "FixAxisPlot.h"
#include "../../Basic/XYPlotImpl.h"
#include "../../Basic/More/OscilloScopeImpl.h"
#include "../../Basic/More/TooltipImpl.h"

Declare_Namespace_CChart

typedef class CFixAxisProp
{
public:
	CFixAxisProp(){SetDefaults();}
	virtual ~CFixAxisProp(){}
	
protected:
	void	SetDefaults()
	{
		bFixAxisXRangeSet = false;
		bFixAxisYRangeSet = false;
		fRangeXL = -1.0;
		fRangeXH = 1.0;
		fRangeYL = -1.0;
		fRangeYH = 1.0;
	}
public:
	bool				bFixAxisXRangeSet;
	bool				bFixAxisYRangeSet;
	double				fRangeXL;
	double				fRangeXH;
	double				fRangeYL;
	double				fRangeYH;
}FixAxisProp, *pFixAxisProp;


class	CFixAxisPlotImpl :	public CFixAxisPlot,
							public CXYPlotImpl,
							public COscilloScopeImpl<CFixAxisPlotImpl>
{
public:
	CFixAxisPlotImpl();
	virtual ~CFixAxisPlotImpl();

public:
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

public:
	virtual	void		GetPlotRange( double *xRange, double *yRange );
	void				GetPlotRange( double *xRange, double *yRange, int dataID);

	virtual	int			GetNeareastPlotData( HDC hDC, POINT point );
	virtual	int			GetNeareastDataPoint( int dataID, HDC hDC, POINT point, double data[2]);

protected:
	virtual	void		DrawPlotDataEach( HDC hDC, double *xRange, double *yRange );

public:
	virtual tstring		GetAbsDataTitle( int which );
	virtual tstring		GetAbsDataAppendix( int which );

	void				DataToLP( double *data, LPPOINT point, int nWhich );
	void				LPToData( LPPOINT point, double *data, int nWhich );

	virtual	void		DataToLP( double *data, LPPOINT point );
	virtual	void		LPToData( LPPOINT point, double *data );

private:
	void				InitAxes();

	void				GetMyCeil(double val, double &ceilVal);
	void				GetMyFloor(double val, double &floorVal);

	int		AddData( MyVData2D vData );

	// for oscilloscope
public:
	void				SetCurrentCurve(int nIndex);

public:
	//virtual	tstring		GetDescriptionByMousePos(HDC hDC, POINT point);

	virtual	void				GetLastPlotRange(double xRange[2], double yRange[2]);
	virtual	void				GetLastXRange(double xRange[2]);
	virtual	void				GetLastYRange(double yRange[2]);
};

class	CFixAxisPlotImplI :	public CFixAxisPlotImpl,
							public CTooltipImpl<CFixAxisPlotImplI>,
							public CXYHandler<CFixAxisPlotImplI>,
							public CXYReactStatus<CFixAxisPlotImplI>,
							public CRangeManageImpl<CFixAxisPlotImplI>
{
public:
	CFixAxisPlotImplI();
	virtual ~CFixAxisPlotImplI();
public:
	tstring				GetDescriptionByMousePos(HDC hDC, POINT point);
	//bool				ShowDlgBySelection(HDC hDC);
};

Declare_Namespace_End
