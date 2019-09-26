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
#include "LayeredHandler.h"
#include "ShareXReactStatus.h"
#include "LayeredPlot.h"
#include "LayeredPlotBasicImpl.h"
#include "PlotLayerImpl.h"
#include "../../Basic/PlotUtility.h"

Declare_Namespace_CChart

class CLayeredPlotImpl : public CLayeredPlotBasicImpl<CPlotLayerImplI>
{
public:
	CLayeredPlotImpl();
	virtual ~CLayeredPlotImpl();
	
public:
	virtual	TCHAR*	GetTypeName(){return _TEXT("LayeredPlot");}
	
	int			AddLayer();
	int			SetLayerCount( int num );

protected:
	void		DrawBasic( HDC hDC, RECT destRect );

	SIZE		GetAxisSize( HDC hDC, int location );
	SIZE		GetAxisPaddingSize( HDC hDC , int location );

public:
	void		GetPlotRange( double xRange[2], double yRange[2] );
	RECT		GetPlotRect( HDC hDC, RECT clientRect);

	RECT		GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect );
	int			GetIdxOfLayerByPointInAxisRegion(HDC hDC, RECT plotRect, POINT point);

	int			RegionIdentify(HDC hDC,POINT point);
};



class CLayeredPlotImplI :	public CLayeredPlotImpl,
							public CLayeredHandler<CLayeredPlotImplI>,
							public CShareXReactStatus<CLayeredPlotImplI>,
							public CDragStatus<CLayeredPlotImplI>
{
public:
	CLayeredPlotImplI();
	virtual ~CLayeredPlotImplI();

	virtual	tstring	GetDescriptionByMousePos(HDC hDC, POINT point);
};

void	InitPageParaSpecial(CLayeredPlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CLayeredPlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CLayeredPlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result );
void	CreateTooltipPopup(CLayeredPlotImplI *plot, HMENU subMenu);
int		InterpretTooltipPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result );
void	CreateDataPopup(CLayeredPlotImplI *plot, HMENU menu);
int		InterpretDataPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result );
void	CreateStatusPopup(CLayeredPlotImplI *plot, HMENU menu);
int		InterpretStatusPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result );

void	InitPageParaAutoSetting(CLayeredPlotImplI *pPlot, vPagePara &vPP);
void	SetupDataForAutoSettings(HWND hWnd, CLayeredPlotImplI *plot);
void	SaveDataForAutoSettings(HWND hWnd, CLayeredPlotImplI *plot);

void	InitPageParaLegend(CLayeredPlotImplI *pPlot, vPagePara &vPP);
void	CreateLegendPopup(CLayeredPlotImplI *plot, HMENU menu);
int		InterpretLegendPopupItem( CLayeredPlotImplI *plot, HWND hWnd, int result );


Declare_Namespace_End
