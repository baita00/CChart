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

#include "TDPlot.h"
#include "Ranges3DImpl.h"
#include "../../Basic/PlotBasicImpl.h"

Declare_Namespace_CChart

class CTDPlotImpl : public CTDPlot,
					public CPlotBasicImpl,
					public CRanges3DImpl<CTDPlotImpl>
{
public:
	CTDPlotImpl(HWND hWnd);
	~CTDPlotImpl();

	virtual	void		OnDraw(  );
	virtual	void		OnDraw( HWND hWnd );
	virtual	void		OnDraw( HWND hWnd, RECT destRect );
	virtual	void		OnDraw( HDC hDC );
	virtual	void		OnDraw(HDC hDC, RECT destRect);
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

	virtual	void		Update(HDC hDC, RECT destRect);

protected:
	void		SetProjection(int cx, int cy);
	void		EraseBkgnd( HDC hDC, RECT destRect );
	void		InitialTransform();

protected:
	void		DrawFrame();
	void		DrawGrids( double *xRange, double *yRange, double *zRange );
	void		DrawMajorGrids( double *xRange, double *yRange, double *zRange );
	void		DrawMinorGrids( double *xRange, double *yRange, double *zRange );
	void		DrawTitle();
	void		DrawAxisLabel();
	void		DrawAxisTick( double *xRange, double *yRange, double *zRange );
public:

	CAxis			*GetAxis(int which);
	int				GetAxisCount();// { return 3; }

	void			AddAxes();
	void			DeleteAxes();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

Declare_Namespace_End