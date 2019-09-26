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
#include "Axis.h"

#include "Global.h"
#include "../Resources/PlotResources.h"
#include <time.h>

Declare_Namespace_CChart

class CPlotImpl;

class	CAxisImpl : public CAxis
{
public:
	CAxisImpl(CPlotImpl	*plot);
	~CAxisImpl();

protected:
	CPlotImpl	*m_pPlot;
	int			m_nRealTitleHeight, m_nRealTickLabelHeight;

	bool		m_bIndependent;
	CPlotImpl	*m_pPlotDep, *m_pPlotIndep;

public:
	bool		IsIndependent();
	void		SetIndependent(bool indep);
	
public:
	void		SetPlot(CPlotImpl *pPlot){m_pPlot = pPlot;}
	CPlotImpl	*GetPlot(){return m_pPlot;}

	CPlotImpl	*GetPlotIndep(){return m_pPlotIndep;}
	
	void		OnDraw( HDC hDC );
		
	tstring		GetMajorTickLabel( int whichTick );
	
	RECT		GetPlotRect( HDC hDC );
	RECT		GetPlotRectWithoutBar( HDC hDC );
	RECT		GetBarRect( HDC hDC );
	
	SIZE		GetMinDisplaySize( HDC hDC );
	SIZE		GetPaddingSize( HDC hDC, RECT chartRect );
	SIZE		GetPaddingSize( HDC hDC );
	SIZE		GetPaddingSizeForFloatTicks( HDC hDC, RECT chartRect );
	SIZE		GetSize( HDC hDC);
	
	bool		CheckLabelHiden(double val);

	void		ApplyMapAlternated();
	
protected:
	int			GetMaxTickLength();
	SIZE		GetRealTickLabelDimension(HDC hDC);
	int			GetRealTitleHeight(HDC hDC);
	RECT		GetTitleRect(HDC hDC);
	RECT		GetPseudoChartRect( );
	
	void		DrawFrame(HDC hDC);
	void		DrawTitle(HDC hDC);
	void		DrawAxisLine(HDC hDC);
	void		DrawAxisArrows(HDC hDC);
	void		DrawMajorTicks(HDC hDC);
	void		DrawMinorTicks(HDC hDC);
	void		DrawTickLabels(HDC hDC);
	void		DrawColorBar(HDC hDC);
	void		DrawArrow(HDC hDC, POINT point, bool bHor, HPEN hPen, HBRUSH hBrush, bool bEnd);
	void		DrawSNPower(HDC hDC);
	
	//tstring		GetTimeString(time_t theTime);
	SIZE		GetSizeAntilog(HDC hDC);
	SIZE		GetSizePower(HDC hDC);
	SIZE		GetSizeSN(HDC hDC);
	int			GetPowerMag();
	RECT		GetSNRect(HDC hDC);
};

Declare_Namespace_End

