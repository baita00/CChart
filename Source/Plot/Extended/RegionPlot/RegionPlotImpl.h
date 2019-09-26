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
#include "RegionPlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class CRegionPlotImpl : public CRegionPlot, public CXYPlotImplI
{
public:
	CRegionPlotImpl();
	virtual	~CRegionPlotImpl();

protected:
	double		m_fXRangeSave[2], m_fYRangeSave[2];

protected:
	RECT		AddtionalPlotRectAdjust(HDC hDC, RECT plotRect);

	RECT		GetHScrollRect(HDC hDC, RECT destRect, RECT plotRect);
	RECT		GetVScrollRect(HDC hDC, RECT destRect, RECT plotRect);

	void		GetPlotRange( double *xRange, double *yRange );

public:
	void		FinalDraw( HDC hDC, RECT destRect );
};

class CRegionPlotImplI :	public CRegionPlotImpl
{
public:
	CRegionPlotImplI();
	virtual ~CRegionPlotImplI();
protected:
	int			m_nDragWhich;
	SIZE		m_sizeOffset;
	double		m_fRegionOld[2];

public:
	bool		OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnMouseMove( HDC hDC, POINT point, UINT ctrlKey );
};

Declare_Namespace_End