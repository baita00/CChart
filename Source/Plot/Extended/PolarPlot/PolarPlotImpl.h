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

#include "PolarPlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class	CPolarPlotImpl : public CPolarPlot, public CXYPlotImplI
{
public:
	CPolarPlotImpl();
	virtual ~CPolarPlotImpl();

protected:
	CAxisImpl			*m_pPhiAxis;
	CAxisImpl			*m_pRAxis;

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("PolarPlot");}

	virtual	void		FinalDraw( HDC hDC, RECT destRect );

public:
	virtual	void		GetPlotRange( double *xRange, double *yRange );

public:
	int					AddPolarCurve( double *pRadius, double *pTheta, int nLen );
	int					AddPolarCurve( double x0, double y0, double *pRadius, double *pTheta, int nLen);
	int					AddCircle( double x0, double y0, double radius, int segs);

protected:
	void				DrawPolarGrids( HDC hDC, RECT plotRect );
	void				DrawPhiAxis( HDC hDC, RECT plotRect );

public:
	CAxisImpl			*GetPhiAxis(){return m_pPhiAxis;}
	CAxisImpl			*GetRAxis(){return m_pRAxis;}
	void				SetCircleMode(bool circle){SetEqualXYAxis(circle); SetEqualXYRange(circle);}

	double				GetPolarXRange();
	double				GetPolarYRange();
	void				SetPolarXRange(double rng);
	void				SetPolarYRange(double rng);
	void				SetPolarRange(double xrng, double yrng);
};

typedef CPolarPlotImpl CPolarPlotImplI;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	kSmithCenterLeft,
	kSmithCenterRight,
	kSmithCenterTop,
	kSmithCenterBottom,

	kSmithCenterCount
};

class CSmithChart : public CPolarPlotImpl
{
public:
	CSmithChart();
	virtual ~CSmithChart();

protected:
	int		m_nCircleSegments;
	int		m_nCenterPos;

public:
	void	InitSmith();

	inline	int		GetCenterPos(){return m_nCenterPos;}
	inline	void	SetCenterPos(int pos){m_nCenterPos = pos;}
};

typedef CSmithChart CSmithChartI;

Declare_Namespace_End