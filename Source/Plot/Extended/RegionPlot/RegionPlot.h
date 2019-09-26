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
#include "../../Basic/CChartNameSpace.h"
#include "../../Instruments/ScrollBar/ScrollBarImpl.h"
#include <memory.h>

Declare_Namespace_CChart

class CRegionPlot
{
public:
	CRegionPlot();
	virtual	~CRegionPlot();

	void	SetDefaults();

protected:
	bool	m_bRegionHorizontal,m_bRegionVerticle;
	int		m_nHScrollWidth, m_nVScrollWidth;
	double	m_fXRegion[2], m_fYRegion[2];
	CScrollBarImpl	*m_pHScroll, *m_pVScroll;

public:
	inline	bool	IsRegionHorizontal(){return m_bRegionHorizontal;}
	inline	void	SetRegionHorizontal(bool bHoriz){m_bRegionHorizontal = bHoriz;}
	inline	bool	IsRegionVerticle(){return m_bRegionVerticle;}
	inline	void	SetRegionVerticle(bool bHoriz){m_bRegionVerticle = bHoriz;}
	inline	int		GetHScrollWidth(){return m_nHScrollWidth;}
	inline	void	SetHScrollWidth(int width){m_nHScrollWidth = width;}
	inline	int		GetVScrollWidth(){return m_nVScrollWidth;}
	inline	void	SetVScrollWidth(int width){m_nVScrollWidth = width;}
	inline	double	*GetXRegion(){return m_fXRegion;}
	inline	void	SetXRegion(double *range){memcpy(m_fXRegion, range, 2*sizeof(double));}
	inline	void	SetXRegion(double low, double high){m_fXRegion[0]=low; m_fXRegion[1]=high;}
	inline	double	*GetYRegion(){return m_fYRegion;}
	inline	void	SetYRegion(double *range){memcpy(m_fYRegion, range, 2*sizeof(double));}
	inline	void	SetYRegion(double low, double high){m_fYRegion[0]=low; m_fYRegion[1]=high;}
};

Declare_Namespace_End