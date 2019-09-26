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

#ifndef __RINGPLOTIMPL_H_122333444455555__
#define __RINGPLOTIMPL_H_122333444455555__

#include "RingPlot.h"
#include "PiePlotImpl.h"

Declare_Namespace_CChart

class	CRingPlotImpl : public CRingPlot, public CPiePlotImpl
{
public:
	CRingPlotImpl();
	virtual ~CRingPlotImpl();

public:
	void	DrawPieAreas( HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total );
};

class CRingPlotImplI :	public CRingPlotImpl,
						public CPieHandler<CRingPlotImplI>,
						public CXYReactStatus<CRingPlotImplI>
{
public:
//	bool	ShowDlgBySelection(HDC hDC);	
};

void	InitPageParaSpecial(CRingPlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CRingPlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CRingPlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CRingPlotImplI *plot, HMENU subMenu);
void	CreateTooltipPopup(CRingPlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CRingPlotImplI *plot, HWND hWnd, int result );
int		InterpretTooltipPopupItem( CRingPlotImplI *plot, HWND hWnd, int result );

void	CreateDataPopup(CRingPlotImplI *plot, HMENU menu);
int		InterpretDataPopupItem( CRingPlotImplI *plot, HWND hWnd, int result );
void	CreateStatusPopup(CRingPlotImplI *plot, HMENU menu);
int		InterpretStatusPopupItem( CRingPlotImplI *plot, HWND hWnd, int result );

//void	CreateTitlePopup(CRingPlotImplI *plot, HMENU menu);
//int		InterpretTitlePopupItem( CRingPlotImplI *plot, HWND hWnd, int result );
void	AddGridPopup(CRingPlotImplI *plot, HMENU addMenu, tstring titleString, int startVal);

void	InitPageParaAutoSetting(CRingPlotImplI *plot, vPagePara &vPP);
void	InitPageParaRanges(CRingPlotImplI *plot, vPagePara &vPP);
void	InitPageParaMajorGrids(CRingPlotImplI *plot, vPagePara &vPP);
void	InitPageParaMinorGrids(CRingPlotImplI *plot, vPagePara &vPP);

Declare_Namespace_End

#endif