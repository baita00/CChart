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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Three Dimension plot use the OpenGL API to draw on the screen

#pragma once

#include "gl/gl.h"
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class CTDSurfacePlot
{
public:
	CTDSurfacePlot();
	~CTDSurfacePlot();

	void		SetDefaults();

public:
	static	void f4min(int *n, double x[2], double *y);
	static	void f4max(int *n, double x[2], double *y);
	//static	void f4min(double x[2], double &y);
	//static	void f4max(double x[2], double &y);

	void		SetFieldFcn(double (*_pFieldFcn) (double, double)){m_pFieldFcn = _pFieldFcn;}
	double		(*GetFieldFcn( ))( double, double ) { return m_pFieldFcn; }
//	void		SetPlotRange(double xRange[2], double yRange[2]);
	
protected:
	double		(*m_pFieldFcn)(double x, double y);
//	double		m_fXRange[2], m_fYRange[2], m_fZRange[2];

	GLfloat		m_fvColor1[3];
	GLfloat		m_fvColor2[3];

	int			m_nXSteps, m_nYSteps;
	int			m_nXMapSteps, m_nYMapSteps;

	bool		m_bExponential;
	bool		m_bLighting;
protected:
//	void		GetDataRanges(double xRange[2], double yRange[2], double zRange[2]);
//	void		GetZRange();

private:
	static	CTDSurfacePlot	*m_pThis;
};

Declare_Namespace_End