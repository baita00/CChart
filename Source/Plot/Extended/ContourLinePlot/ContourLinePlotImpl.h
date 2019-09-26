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

#ifndef __COUNTOURLINEPLOTIMPL_H_122333444455555__
#define __COUNTOURLINEPLOTIMPL_H_122333444455555__

#include "../../Basic/XYPlotBasicImpl.h"
#include "ContourLinePlot.h"
#include "ContourLineHandler.h"

Declare_Namespace_CChart

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CContourLinePlotImpl :	public CContourLinePlot,
								public CXYPlotBasicImpl
{
public:
	CContourLinePlotImpl();
	virtual ~CContourLinePlotImpl();

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("ContourLinePlot");}
public:
	virtual	void		FinalDraw( HDC hDC, RECT destRect );
public:
	void	SetDataInfoShow(bool bDraw);
	bool	IsDataInfoShow();
	
	bool	CreateContour();
	bool	CreateContour(int nPlanes);
	bool	CreateContour(int nPlanes,bool bExponential);

	int		GetPlotDataLevels();//{return m_listContour.GetNPlanes();}

	bool	IsContourCreated();

	bool	IsEmpty(){return false;}//otherwise the dblplot will terminate drawing
protected:
	bool	m_bDataInfoShow;

};

class CContourLinePlotImplI :	public CContourLinePlotImpl,
								public CContourLineHandler<CContourLinePlotImplI>,
								public CXYReactStatus<CContourLinePlotImplI>
{
public:
	CContourLinePlotImplI::CContourLinePlotImplI();
	virtual	CContourLinePlotImplI::~CContourLinePlotImplI();
//	bool				ShowDlgBySelection(HDC hDC);
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void	InitPageParaSpecial(CContourLinePlotImplI *plot, vPagePara &vPP);
void	CreateOscilloPopup(CContourLinePlotImplI *plot, HMENU menu);
int		InterpretOscilloPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result );
void	CreateExtraFunctionPopup(CContourLinePlotImplI *plot, HMENU subMenu);
int		InterpretExtraFunctionPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result );
void	CreateTooltipPopup(CContourLinePlotImplI *plot, HMENU subMenu);
int		InterpretTooltipPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result );

void	CreateLegendPopup(CContourLinePlotImplI *plot, HMENU menu);
int		InterpretLegendPopupItem( CContourLinePlotImplI *plot, HWND hWnd, int result );

void	InitPageParaLegend(CContourLinePlotImplI *pPlot, vPagePara &vPP);
void	SetupDataForLegend(HWND hWnd, CContourLinePlotImplI *plot, LOGFONT	&lfLegendFont);
void	SaveDataForLegend(HWND hWnd, CContourLinePlotImplI *plot, LOGFONT	lfLegendFont);

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

Declare_Namespace_End

#endif