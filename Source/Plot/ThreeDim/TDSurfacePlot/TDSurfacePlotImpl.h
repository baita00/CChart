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

/* ############################################################################################################################## */
#pragma once

#include "../TDPlot/TDPlotImpl.h"
#include "../TDPlot/TDHandler.h"
#include "TDSurfacePlot.h"


Declare_Namespace_CChart

const int nTextureWidth = 64;
class CTDSurfacePlotImpl : public CTDSurfacePlot, public CTDPlotImpl
{
public:
	CTDSurfacePlotImpl(HWND hWnd);
	~CTDSurfacePlotImpl();

	virtual	TCHAR*		GetTypeName(){return _T("TDSurfacePlot");}
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

protected:
	void		GetPlotRange( double *xRange, double *yRange, double *zRange );
	void		DrawSurface( double *xRange, double *yRange, double *zRange );

	double		GetStepValue( int index, double v1, double v2, int nSegs, bool bExponential);

	void		InitEnv();
	void		ResetEnv();

public:
	void		SetContourRange(double xRange[2], double yRange[2]);
	void		SetContourRange(double xl, double xu, double yl, double yu);

protected:
	void		GetDataRanges(double xRange[2], double yRange[2], double zRange[2]);
	void		GetZRange();

	void		GetDataRange1D( int whichDim, double *range ){};//virtual funciton from CRanges

protected:
	GLubyte		m_mImage[3*nTextureWidth];
	GLfloat		m_fTextureParams[4];
	void		BuildImage();
};

class CTDSurfacePlotImplI : public CTDSurfacePlotImpl, public CTDHandler<CTDSurfacePlotImplI>
{
public:
	CTDSurfacePlotImplI( HWND hWnd );
	virtual ~CTDSurfacePlotImplI();
};

Declare_Namespace_End