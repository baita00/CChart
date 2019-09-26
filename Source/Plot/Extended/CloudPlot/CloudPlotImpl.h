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

#ifndef __CLOUDPLOTIMPL_H_122333444455555__
#define __CLOUDPLOTIMPL_H_122333444455555__

#include "../../Basic/XYPlotImpl.h"
#include "../../Basic/DragStatus.h"
#include "../ContourLinePlot/ListContour.h"
#include "CloudPlot.h"
#include "CloudLegendImpl.h"
#include "CloudReactStatus.h"
#include "CloudHandler.h"

Declare_Namespace_CChart

//class CCloudLegendImpl;
//#include "CloudLegendImpl.h"

// usage step by step
// SetFieldFcn(double f(double, double));
// SetRanges(double x[2], double y[2]);
// CreateMemDC(HWND);
// CreateCloud();

class CCloudPlotImpl:	public CCloudPlot,
						public CPlotImpl,
						public CCloudLegendImpl<CCloudPlotImpl>,
						public CDragStatus<CCloudPlotImpl>
{
public:
	CCloudPlotImpl();
	virtual ~CCloudPlotImpl();

public:
	inline	bool		IsNoDataPlot(){return true;}

	RECT				AddtionalPlotRectAdjust(HDC hDC, RECT plotRect);

	void				GetDataRange1D( int whichDim, double *range );

	int					RegionIdentifyEx(HDC hDC, POINT point, bool &legendRegion, int &centerAxisRegion);

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("CloudPlot");}

	virtual	void		FinalDraw( HDC hDC, RECT destRect );

protected:
	void		DrawBasic( HDC hDC, RECT destRect );
	//void		DrawCloud(HDC hDC, RECT plotRect);

	void		SetAxisLegend(int location, bool set);
	void		SetAllAxesLegend();

public:
	virtual	int			GetPlotDataCount( void );// { return ((CCloudLegendImpl *)m_pLegend)->GetLegends(); }
	virtual tstring		GetAbsDataTitle( int which );
	virtual tstring		GetAbsDataAppendix( int which );
	inline	void		SetXRange( double low, double high ) {SetContourXRange(low, high); CPlotImpl::SetXRange(low, high);}
	inline	void		SetYRange( double low, double high ) {SetContourYRange(low, high); CPlotImpl::SetYRange(low, high);}
	inline	bool		IsEmpty(){return GetFieldFcn()==0;}
	inline	bool		CheckLogSafety(bool bX){return true;}
	
	void		GetPlotRange( double *xRange, double *yRange );

public:
	void		MyLock(){}
	void		MyUnlock(){}
};

class CCloudPlotImplI : public CCloudPlotImpl,
						public CCloudHandler<CCloudPlotImplI>,
						public CCloudReactStatus<CCloudPlotImplI>
{
public:
	CCloudPlotImplI();
	virtual	~CCloudPlotImplI();

public:
//	bool	ShowDlgBySelection(HDC hDC);
};

void	InitPageParaSpecial(CCloudPlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CCloudPlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CCloudPlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CCloudPlotImplI *plot, HMENU subMenu);
void	CreateTooltipPopup(CCloudPlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CCloudPlotImplI *plot, HWND hWnd, int result );
int		InterpretTooltipPopupItem( CCloudPlotImplI *plot, HWND hWnd, int result );

void	CreateDataPopup(CCloudPlotImplI *plot, HMENU menu);
int		InterpretDataPopupItem( CCloudPlotImplI *plot, HWND hWnd, int result );
void	CreateStatusPopup(CCloudPlotImplI *plot, HMENU menu);
int		InterpretStatusPopupItem( CCloudPlotImplI *plot, HWND hWnd, int result );

void	InitPageParaAutoSetting(CCloudPlotImplI *plot, vPagePara &vPP);

Declare_Namespace_End

#endif