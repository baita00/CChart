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
#include "XY3DPlot.h"
#include "../../Basic/XYPlotImpl.h"
#include "../../Basic/More/TooltipImpl.h"

Declare_Namespace_CChart

typedef class CXY3DProp
{
public:
	CXY3DProp(){SetDefaults();}
	virtual ~CXY3DProp(){}
	
protected:
	void	SetDefaults(){vfZData.clear();}
public:
	//double		fZDataMin;
	//double		fZDataMax;
	double			pfZDataRange[2];

	vector<double>	vfZData;
}XY3DProp, *pXY3DProp;

class	CXY3DPlotImpl : public CXY3DPlot, public CXYPlotImpl
{
public:
	CXY3DPlotImpl();
	virtual ~CXY3DPlotImpl();

	virtual	TCHAR*		GetTypeName(){return _TEXT("XY3DPlot");}

public:
	int				AddCurve( double *pX, double *pY, double *pZ, int nLen );
	virtual	int		AddPoint( double x, double y, double z, int nWhich);

public:
	CAxisImpl			*m_pAxisZ;

	double			m_pfZRange[2];
	bool			m_bZRangeSet;
	int				m_nMajorTicksZ, m_nMinorTicksZ;

public:
	int					GetZTickCount();
	void				SetZTickCount(int count);
	int					GetZMinorTickCount();
	void				SetZMinorTickCount(int count);

protected:
	virtual	void	DrawFrame( HDC hDC );//, RECT plotRect );
	virtual	void	DrawGrids( HDC hDC );//, RECT plotRect );
	void			DrawZAxis( HDC hDC, RECT plotRect );

	void			DrawPlotDataEach( HDC hDC, double *xRange, double *yRange );
	void			DrawFillArea(  int which, HDC hDC, RECT plotRect, double *xRange, double *yRange );
	void			DrawAxes( HDC hDC );//, RECT clientRect, RECT plotRect );

	void			DrawZAxisLine( HDC hDC, RECT plotRect );
	void			DrawZAxisMajorTicks( HDC hDC, RECT plotRect );
	void			DrawZAxisMinorTicks( HDC hDC, RECT plotRect );
	void			DrawZAxisTickLabels( HDC hDC, RECT plotRect );

	void			DrawXYGrids( HDC hDC, RECT plotRect );
	void			DrawYZGrids( HDC hDC, RECT plotRect );
	void			DrawZXGrids( HDC hDC, RECT plotRect );

protected:
	void			GetZAxisDist(double &dx, double &dy);
	RECT			AdjustPlotSizeForZAxis( HDC hDC, RECT plotRect );
	virtual	RECT	GetPlotRect( HDC hDC, RECT clientRect );
	RECT			GetPlotRectL( HDC hDC, RECT clientRect );
	RECT			GetPlotRectL( RECT plotRect );
	RECT			GetPlotRectU( HDC hDC, RECT clientRect );
	RECT			GetPlotRectU( RECT plotRect );

protected:
	void			GetZDataRange( int dataID );
	void			GetZDataRanges();
	void			GetZDataRanges(double zRange[2]);

	void			GetPlotRange( double *xRange, double *yRange );//, double *zRange );
	void			GetPlotZRange( double *zRange );

	void			Get2DData( int which, vector<DataPoint2D> &vData, RECT plotRectL, double *xRange, double *yRange, double *zRange );
	void			Reset2DData( int which, vector<DataPoint2D> &vData );

public:
	//virtual	void	DataToLP( double *data, LPPOINT point );
	//virtual	void	LPToData( LPPOINT point, double *data );

public:
	virtual	int		GetNeareastPlotData( HDC hDC, POINT point );
	virtual	int		GetNeareastDataPoint( int dataID, HDC hDC, POINT point, double data[2]);

public:
	virtual	bool	WriteToFile( tstring pathName );
	virtual	bool	WriteToFile( int which, tstring pathName );
	virtual	bool	ReadFromFile( tstring pathName );
	virtual	bool	ReadFromFile( int which, tstring pathName );
};

//typedef CXY3DPlotImpl CXY3DPlotImplI;
/*
class	CXY3DPlotImplI : public CXY3DPlotImpl, public CXYHandler<CXY3DPlotImplI>
{

};
*/

class	CXY3DPlotImplI :	public CXY3DPlotImpl,
							public CTooltipImpl<CXY3DPlotImplI>,
							public CXYHandler<CXY3DPlotImplI>,
							public CXYReactStatus<CXY3DPlotImplI>,						
							public CRangeManageImpl<CXY3DPlotImplI>,
							public COscilloScopeImpl<CXY3DPlotImplI>
{
public:
	//tstring				GetDescriptionByMousePos(HDC hDC, POINT point);
	//bool				ShowDlgBySelection(HDC hDC);
};
Declare_Namespace_End