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

#include "Legend.h"
#include "Global.h"
#include "../Resources/PlotResources.h"

using namespace NsCChart;

CLegend::CLegend()
{
	m_hBkgndPattern = 0;
	if(CGlobal::m_bLoadDefPatterns)
	{
		HBITMAP hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_LEGEND_PATTERN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if(hb)
		{
			SetLegendBkgndPattern(hb);
			DeleteObject(hb);
		}
	}
	SetDefaults();
}

CLegend::~CLegend()
{
	DeleteUsedHandles();
}

void	CLegend::SetDefaults()
{
	m_bLegendShow = true;
	m_nHorizontalNumOfLegend = 1;
	m_nMaxVerticalNumOfLegend = 8;
	m_nPosition = kLegendArbitrary;
	m_sizeOffset.cx = m_sizeOffset.cy = 0;
#ifndef	DEFAULT_WHITE_BACKGROUND
	// Default is black background
	m_crBorderColor = RGB(255,255,255);
	m_crBkgndColor = RGB(0,0,0);
#else
	m_crBorderColor = RGB(0,0,0);
	m_crBkgndColor = RGB(255,255,255);
#endif
	m_nBorderLineSize = 2;
	m_nBorderLineStyle  = PS_SOLID;
	m_bBorderShow  = true;
	m_bShadow = true;
	m_crShadow = RGB(168, 168, 168);
	m_sizeShadowOffset.cx = 4;
	m_sizeShadowOffset.cy = 4;
	m_bLineShow	= true;
	m_bMarkerShow	= true;
	m_nLineLength = 30;
	m_bTransparent = false;
	m_nMarginSizeXOuter = 2;
	m_nMarginSizeYOuter = 1;
	m_nMarginSizeXInner = 10;
	m_nMarginSizeYInner = 0;
	m_bShowDragMargin = true;
	m_nDragMarginSize = 5;
	
	m_bConfined = true;
	
	m_bColorRect = false;
	
	m_bLighted = false;
	
	m_nBkgndBrushType = kBrushSolid;
	m_nBkgndHatchStyle = HS_CROSS;
	m_crBkgndHatchColor = RGB(128, 128, 128);
	if(m_hBkgndPattern)
	{
		DeleteObject(m_hBkgndPattern);
		m_hBkgndPattern = 0;
	}
	
	HFONT		hFont;
	hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, TRUE, FALSE, 0,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, _TEXT("Arial") );
	GetObject(hFont,sizeof(LOGFONT),&m_lfFont);
	DeleteObject(hFont);
	
	m_bLegendDragMode = false;
	
	m_nGap = 5;
	
	m_bRound = false;
	m_nRoundRadius = 5;
	m_nRightMarginAdded = 5;

	m_bReactLegend = true;
}

void		CLegend::DeleteUsedHandles()
{
	if(m_hBkgndPattern)
	{
		DeleteObject(m_hBkgndPattern);
		m_hBkgndPattern = 0;
	}
}
