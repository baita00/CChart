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

#ifndef __SHAREXPLOTIMPL_H_122333444455555__
#define __SHAREXPLOTIMPL_H_122333444455555__

#include "../../Basic/XYPlotImpl.h"
#include "LayeredPlotBasicImpl.h"
#include "ShareXPlot.h"
#include "ShareXHandler.h"
#include "ShareXReactStatus.h"
#include "LayeredPlot.h"
#include "ShareXLayerImpl.h"
#include "../../Basic/PlotUtility.h"

Declare_Namespace_CChart

class CShareXPlotImpl :	public CShareXPlot,
						public CLayeredPlotBasicImpl<CShareXLayerImplI>
{
public:
	CShareXPlotImpl();
	virtual ~CShareXPlotImpl();

public:
	virtual	TCHAR*	GetTypeName(){return _TEXT("ShareXPlot");}
	
	int			AddLayer();
	int			SetLayerCount( int num );

protected:
	void		DrawBasic( HDC hDC, RECT destRect );

	SIZE		GetAxisSize( HDC hDC, int location );
	SIZE		GetAxisPaddingSize( HDC hDC , int location );

	void		GetPlotRange1D( int whichDim, double *range );

	bool		IsToCalcXRange();

public:
	void		GetPlotRange( double xRange[2], double yRange[2] );
	RECT		GetPlotRect( HDC hDC, RECT clientRect);

	RECT		GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect );
	int			GetIdxOfLayerByPointInAxisRegion(HDC hDC, RECT plotRect, POINT point);

	int			RegionIdentify(HDC hDC,POINT point);

	void		SetShareXRange(double x1, double x2);
};

class CShareXPlotImplI :	public CShareXPlotImpl,
							public CShareXHandler<CShareXPlotImplI>,
							public CShareXReactStatus<CShareXPlotImplI>,
							public CDragStatus<CShareXPlotImplI>
{
public:
	CShareXPlotImplI();
	virtual ~CShareXPlotImplI();

	virtual	tstring	GetDescriptionByMousePos(HDC hDC, POINT point);
};

class CLRPlotImpl : public CShareXPlotImplI
{
public:
	CLRPlotImpl();
	virtual	~CLRPlotImpl();
	
protected:
	bool	m_bExactXRange;
	
public:
	bool	IsExactXRange(){return m_bExactXRange;}
	void	SetExactXrange(bool set){m_bExactXRange = set;}
	
public:
	virtual	void	GetPlotRange( double xRange[2], double yRange[2] );
	
public:
	int		AddLeftCurve( double *pX, double *pY, int nLen);
	int		AddRightCurve( double *pX, double *pY, int nLen);
	int		AddLeftCurve( double *pX, double *pY, int nLen, tstring datatitle );
	int		AddRightCurve( double *pX, double *pY, int nLen, tstring datatitle );
	int		AddLeftPoint( double x, double y, int nWhich );
	int		AddLeftPoint( double x, double y) ;
	int		AddRightPoint( double x, double y, int nWhich );
	int		AddRightPoint( double x, double y) ;
	
public:
	void	SetBottomTitle(tstring title);
	void	SetLeftTitle(tstring title);
	void	SetRightTitle(tstring title);
	
	void	SetLeftRange(double low, double high);
	void	SetRightRange(double low, double high);
	void	SetBottomRange(double low, double high);
	
};

typedef CLRPlotImpl CLRPlotImplI;

void	InitPageParaSpecial(CShareXPlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CShareXPlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CShareXPlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result );
void	CreateTooltipPopup(CShareXPlotImplI *plot, HMENU subMenu);
int		InterpretTooltipPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result );
void	CreateDataPopup(CShareXPlotImplI *plot, HMENU menu);
int		InterpretDataPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result );
void	CreateStatusPopup(CShareXPlotImplI *plot, HMENU menu);
int		InterpretStatusPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result );

void	InitPageParaAutoSetting(CShareXPlotImplI *pPlot, vPagePara &vPP);
void	SetupDataForAutoSettings(HWND hWnd, CShareXPlotImplI *plot);
void	SaveDataForAutoSettings(HWND hWnd, CShareXPlotImplI *plot);

void	InitPageParaLegend(CShareXPlotImplI *pPlot, vPagePara &vPP);
void	CreateLegendPopup(CShareXPlotImplI *plot, HMENU menu);
int		InterpretLegendPopupItem( CShareXPlotImplI *plot, HWND hWnd, int result );


Declare_Namespace_End

#endif