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

//#include "StdAfx.h"
#include <windows.h>
#include "TDPlot.h"
//#include "../../Basics/XYPlot.h"
//#include "../../Basics/PlotUtility.h"

using namespace NsCChart;

CTDPlot::CTDPlot()
{
	SetDefaults();
	SetRectEmpty( &m_rctLastClientRect );
	m_pAxes[0] = m_pAxes[1] = m_pAxes[2] = NULL;
}

CTDPlot::~CTDPlot()
{
//	DeleteAxes();
}

void	CTDPlot::SetDefaults()
{
	m_fvBkgndColor[0] = 0.9f;
	m_fvBkgndColor[1] = 0.9f;
	m_fvBkgndColor[2] = 0.9f;
	m_fvBkgndColor[3] = 0.0f;
	m_fvFrameColor[0] = 0.2f;
	m_fvFrameColor[1] = 0.2f;
	m_fvFrameColor[2] = 0.2f;
	m_fvFrameColor[3] = 0.0f;
	m_fvAxisPlaneColor[0] = 0.8f;
	m_fvAxisPlaneColor[1] = 0.8f;
	m_fvAxisPlaneColor[2] = 0.8f;
	m_fvAxisPlaneColor[3] = 0.0f;
	m_fvAxisColor[0] = 0.0f;
	m_fvAxisColor[1] = 0.0f;
	m_fvAxisColor[2] = 1.0f;
	m_fvAxisColor[3] = 0.0f;
	m_fvTickLabelColor[0] = 0.6f;
	m_fvTickLabelColor[1] = 0.1f;
	m_fvTickLabelColor[2] = 0.1f;
	m_fvTickLabelColor[3] = 0.0f;

	m_bShowXYFrame = true;
	m_bShowYZFrame = true;
	m_bShowZXFrame = true;

	m_bShowXYMajorGrid = true;
	m_bShowXYMinorGrid = false;
	m_bShowYZMajorGrid = true;
	m_bShowYZMinorGrid = false;
	m_bShowZXMajorGrid = true;
	m_bShowZXMinorGrid = false;
	m_fvMajorGridColor[0] = 0.6f;
	m_fvMajorGridColor[1] = 0.5f;
	m_fvMajorGridColor[2] = 0.0f;
	m_fvMajorGridColor[3] = 0.0f;
	m_fvMinorGridColor[0] = 0.0f;
	m_fvMinorGridColor[1] = 0.8f;
	m_fvMinorGridColor[2] = 0.0f;
	m_fvMinorGridColor[3] = 0.0f;

	m_pfOffset[0] = 0.0f;
	m_pfOffset[1] = 0.0f;
	m_pfOffset[2] = 0.0f;
	m_pfLen[0] = 10.0f;
	m_pfLen[1] = 10.0f;
	m_pfLen[2] = 10.0f;
	m_pfAngle[0] = -90.0f;
	m_pfAngle[1] = 5.0f;
	m_pfAngle[2] = -100.0f;

	m_fProjectionRange = 40.0f;
	m_fZoomFactor = 1.3f;

	m_fSF4AxisLabel = 1.2f;
	m_fSF4Title = 1.6f;
	m_fSF4AxisTickLabel = 0.8f;

	m_fvTitleColor[0] = 0.5f;
	m_fvTitleColor[1] = 0.5f;
	m_fvTitleColor[2] = 0.0f;
	m_fvTitleColor[3] = 0.0f;
	m_strTitle = _TEXT("3D Plot");
	m_bTitleShow = true;

	m_nNurbsPoints = 40;
}

void	CTDPlot::InitializeOpenGL(HWND hWnd)
{
	PIXELFORMATDESCRIPTOR	pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0, 
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	HDC hDC = GetDC(hWnd);
	int pf = ChoosePixelFormat(hDC, &pfd);
	BOOL rt = SetPixelFormat(hDC, pf, &pfd);
	m_hGLRC = wglCreateContext(hDC);
	ReleaseDC(hWnd, hDC);
}

void	CTDPlot::FinallizeOpenGL()
{
	wglDeleteContext(m_hGLRC);
}

void	CTDPlot::ResetTransform()
{
	m_pfAngle[0] = -105.0f;
	m_pfAngle[1] = 0.0f;
	m_pfAngle[2] = 45.0f;
}
/*
void	CTDPlot::DeleteAxes()
{
	int count = 3;//GetAxisCount();
	for(int i=0; i<count; i++)
	{
		if(m_pAxes[i])
		{
			delete m_pAxes[i];
			m_pAxes[0] = 0;
		}
	}
}

CAxis	*CTDPlot::GetAxis(int which)
{
	int count = 3;//GetAxisCount();
	if(which<0 || which>=count)
		return 0;
	else
		return m_pAxes[which];
}

void	CTDPlot::GetRange( double *xRange, double *yRange, double *zRange)
{
	double *pRange;
	for(int i=0; i<3; i++)
	{
		if(i==0)pRange = xRange;
		else if(i==1)pRange = yRange;
		else pRange = zRange;
	
		m_pAxes[i]->GetRange(pRange);
	}
}

// Shortcut to get xRange of plot
void	CTDPlot::GetXRange( double *range )
{
	double			yRange[2], zRange[2];
	GetRange( range, yRange, zRange );
}

// Shortcut to get yRange of plot
void	CTDPlot::GetYRange( double *range )
{
	double			xRange[2], zRange[2];
	GetRange( xRange, range, zRange );
}

// Shortcut to get zRange of plot
void	CTDPlot::GetZRange( double *range )
{
	double			xRange[2], yRange[2];
	GetRange( xRange, yRange, range );
}
*/