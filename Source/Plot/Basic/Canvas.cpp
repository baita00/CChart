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

#include "Canvas.h"

#include "Plot.h"
using namespace NsCChart;

CCanvas::CCanvas()
{
	m_hBkgndPattern = NULL;
	m_hBkgndImage = NULL;
	m_hDefScreenImage = NULL;
	m_hDataRegionPattern = NULL;
	SetDefaults();
}

CCanvas::~CCanvas()
{
	DeleteUsedHandles();
}

void	CCanvas::SetDefaults()
{
#ifndef	DEFAULT_WHITE_BACKGROUND
	// Default is black background
	m_crBkgndColor = RGB( 0, 0, 0 );
	m_crBorderColor = RGB( 255, 255, 255 );
#else
	m_crBkgndColor = RGB( 255, 255, 255 );
	m_crBorderColor = RGB( 0, 0, 0 );
#endif
	m_crBkgndHatchColor = RGB(128, 128, 128);

	m_nBorderSize = 1;
	m_nBorderStyle = PS_SOLID;
	m_crBorderColor = RGB(128,0,0);
	m_bBorderShow = true;
	m_bRoundEdgeConor = false;
	m_nEdgeConorRadius = 20;
	
	m_nEdgeSize = 3;
	m_nEdgeStyle = PS_SOLID;
	m_crEdgeColor = RGB(128,0,0);
	m_bEdgeShow = false;
	m_bEdgeButton = true;
	m_nEdgeType = EDGE_SUNKEN;
	m_nEdgeFlag = BF_RECT;
	m_bEdgeBelowTitle = false;
	
	m_bEraseBkgnd = true;
	m_nBkgndBrushType = kBrushSolid;
	m_nBkgndHatchStyle = HS_CROSS;
	if(m_hBkgndPattern)
	{
		DeleteObject(m_hBkgndPattern);
		m_hBkgndPattern = 0;
	}
	if(m_hBkgndImage)
	{
		DeleteObject(m_hBkgndImage);
		m_hBkgndImage = 0;
	}
	m_bBkgndImage = false;

	if(m_hDefScreenImage)
	{
		DeleteObject(m_hDefScreenImage);
		m_hDefScreenImage = 0;
	}

	m_bGradientBkgnd = false;
	m_nGradientType = kGradientT2B;
	m_crGradientColor1 = RGB(168,168,224);
	m_crGradientColor2 = RGB(255,255,255);

	m_nDataRegionBrushType = kBrushSolid;
	m_nDataRegionHatchStyle = HS_CROSS;
	m_crDataRegionHatchColor = RGB(128, 128, 0);
	if(m_hDataRegionPattern)
	{
		DeleteObject(m_hDataRegionPattern);
		m_hDataRegionPattern = 0;
	}
	m_bUseDataRegionBrush = false;
	m_crDataRegionColor = RGB( 244, 244, 244 );//m_crBkgndColor;
	
	m_bGradientDataRegion = false;
	m_nDataRegionGradientType = kGradientLB2RT;
	m_crDataRegionGradientColor1 = RGB(64,64,224);
	m_crDataRegionGradientColor2 = RGB(255,255,255);

	m_bLightDataRegion = false;

	m_bEraseReservePlotRect = false;
}

void	CCanvas::DeleteUsedHandles()
{
	if(m_hBkgndPattern)
	{
		DeleteObject(m_hBkgndPattern);
		m_hBkgndPattern = 0;
	}
	if(m_hBkgndImage)
	{
		DeleteObject(m_hBkgndImage);
		m_hBkgndImage = 0;
	}
	if(m_hDataRegionPattern)
	{
		DeleteObject(m_hDataRegionPattern);
		m_hDataRegionPattern = 0;
	}
	if(m_hDefScreenImage)
	{
		DeleteObject(m_hDefScreenImage);
		m_hDefScreenImage = 0;
	}
}