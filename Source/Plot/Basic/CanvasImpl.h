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
#include "Canvas.h"

Declare_Namespace_CChart

template <class PlotImplT>
class CCanvasImpl : public CCanvas
{
public:
	CCanvasImpl();
	virtual ~CCanvasImpl();

public:
	void	DrawCanvas( HDC hDC );

protected:
	void	DrawBkgnd( HDC hDC );
	void	DrawPlotEdge( HDC hDC );
	virtual	void	DrawFrame( HDC hDC );
};

template <class PlotImplT>
CCanvasImpl<PlotImplT>::CCanvasImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
CCanvasImpl<PlotImplT>::~CCanvasImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(CGlobal::m_bLoadDefPatterns)
	{
		HBITMAP hb = 0;
		hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_BACKGROUND_IMAGE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if(hb)
		{
			SetBkgndImage(hb);
			SetBkgndPattern(hb);
			SetDataRegionPattern(hb);
			DeleteObject(hb);
		}
		//hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_DEFSCREEN_IMAGE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		//if(hb)
		//{
		//	SetDefScreenImage(hb);
		//	DeleteObject(hb);
		//}
	}
}

template <class PlotImplT>
void	CCanvasImpl<PlotImplT>::DrawCanvas(HDC hDC)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!pT->IsNothingElse())DrawBkgnd(hDC);
	DrawPlotEdge(hDC);
	DrawFrame(hDC);
}

template <class PlotImplT>
void	CCanvasImpl<PlotImplT>::DrawBkgnd( HDC hDC )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	// Erase the Bkgnd
	if( !m_bEraseBkgnd ) return;
	
	RECT destRect = pT->GetLastClientRect();
	RECT plotRect = pT->GetLastPlotRect();

	HDC hTmpDC;
	HBITMAP hbmPlotRect, hbmOld;
	if(m_bEraseReservePlotRect)
	{
		hTmpDC = CreateCompatibleDC(hDC);
		hbmPlotRect = CreateCompatibleBitmap(hDC, Width(plotRect), Height(plotRect));
		hbmOld = (HBITMAP)SelectObject(hTmpDC, hbmPlotRect);
		BitBlt(hTmpDC, 0, 0, Width(plotRect), Height(plotRect), hDC, plotRect.left, plotRect.top, SRCCOPY);
	}
	
	// Erase backgound
	HBRUSH			hBrush;//, hBrush1;
	
	// Gradient Bkgnd
	if(m_bBkgndImage)
	{
		if(m_hBkgndImage)
		{
			HDC	hdc = CreateCompatibleDC(hDC);
			HBITMAP hOldBm = (HBITMAP)SelectObject(hdc, m_hBkgndImage);
			BITMAP bm;
			GetObject(m_hBkgndImage, sizeof(BITMAP), &bm);
			SetStretchBltMode(hDC, HALFTONE);
			SetBrushOrgEx(hDC, 0, 0, NULL);
			StretchBlt(hDC, destRect.left, destRect.top, Width(destRect), Height(destRect),
				hdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			SelectObject(hdc, hOldBm);
			DeleteDC(hdc);
		}
		else
		{
			if(m_bGradientBkgnd)
				goto EraseGradient;
			else
				goto EraseSolidBrush;
		}
	}
	else if(m_bGradientBkgnd)
	{
EraseGradient:
	GradientRect(hDC, destRect, m_nGradientType, m_crGradientColor1, m_crGradientColor2);
	}
	else
	{
		switch(m_nBkgndBrushType)
		{
		case kBrushSolid:
EraseSolidBrush:
			hBrush = CreateSolidBrush( m_crBkgndColor );
			break;
		case kBrushHatch:
			hBrush = CreateHatchBrush( m_nBkgndHatchStyle, m_crBkgndHatchColor);
			break;
		case kBrushPattern:
			if(m_hBkgndPattern)
				hBrush = CreatePatternBrush( m_hBkgndPattern );
			else
				hBrush = CreateSolidBrush( m_crBkgndColor );
			break;
		default:
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			break;
		}
		FillRect(hDC, &destRect, hBrush);
		DeleteObject(hBrush);
	}
	
	// Set data region color
	if(  m_bUseDataRegionBrush )
	{
		if(m_bGradientDataRegion)
		{
			GradientRect(hDC, plotRect, m_nDataRegionGradientType, m_crDataRegionGradientColor1, m_crDataRegionGradientColor2);
/*
			POINT pt[4];
			pt[0].x = plotRect.left;
			pt[0].y = plotRect.top;
			
			pt[3].x = plotRect.left;
			pt[3].y = plotRect.bottom;
			
			pt[2].x = plotRect.right;
			pt[2].y = plotRect.bottom;

			pt[1].x = plotRect.right;
			pt[1].y = plotRect.top;
			GradientSides(hDC, pt[0], pt[1], pt[2], pt[3], m_crDataRegionGradientColor1, m_crDataRegionGradientColor2, m_crDataRegionGradientColor2);
*/
		}
		else
		{
			switch(m_nDataRegionBrushType)
			{
			case kBrushSolid:
				hBrush = CreateSolidBrush( m_crDataRegionColor );
				break;
			case kBrushHatch:
				hBrush = CreateHatchBrush( m_nDataRegionHatchStyle, m_crDataRegionHatchColor);
				break;
			case kBrushPattern:
				if(m_hDataRegionPattern)
					hBrush = CreatePatternBrush( m_hDataRegionPattern );
				else
					hBrush = CreateSolidBrush( m_crDataRegionColor );
				break;
			default:
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				break;
			}
			FillRect( hDC, &plotRect, hBrush );
			DeleteObject(hBrush);
		}
	}
	if(m_bEraseReservePlotRect)
	{
		BitBlt(hDC, plotRect.left, plotRect.top, Width(plotRect), Height(plotRect), hTmpDC, 0, 0, SRCCOPY);
		SelectObject(hTmpDC, hbmOld);
		DeleteObject(hbmPlotRect);
		DeleteDC(hTmpDC);
	}
}

void	DrawRoundRect(HDC hDC, RECT destRect, COLORREF edgeColor, int edgeSize, int edgeStyle, int conorRadius);

template <class PlotImplT>
void	CCanvasImpl<PlotImplT>::DrawPlotEdge( HDC hDC )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if( !m_bEdgeShow )return;

	RECT destRect = pT->GetLastClientRect();
	
	if(m_bEdgeBelowTitle)
	{
		destRect.top = pT->GetTitleRect(hDC).bottom;
	}
	
	if(m_bEdgeButton)
	{		
		DrawEdge(hDC, &destRect, m_nEdgeType, m_nEdgeFlag);
		return;
	}
	
	if( m_nEdgeSize <= 0 )return;
	
	COLORREF edgecolor = pT->ChooseLineColor(m_crEdgeColor, pT->IsPlotLighted(), pT->IsPlotSelected());
	int edgesize = pT->ChooseLineWidth(m_nEdgeSize, pT->IsPlotLighted(), pT->IsPlotSelected());
	
	HPEN	hPen,hOldPen;
	HBRUSH hBrush,hOldBrush;
	hPen = CreatePen( m_nEdgeStyle, edgesize, edgecolor );
	hBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
	
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC,hBrush);
	
	if(!m_bRoundEdgeConor)
	{
		Rectangle(hDC,destRect.left,destRect.top,destRect.right,destRect.bottom);
	}
	else
	{
		DrawRoundRect(hDC, destRect, edgecolor, edgesize, pT->GetEdgeStyle(), pT->GetEdgeConorRadius());
	}
	
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen );
	DeleteObject(hPen);
}

template <class PlotImplT>
void	CCanvasImpl<PlotImplT>::DrawFrame( HDC hDC )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	RECT plotRect = pT->GetLastPlotRect();

	if( !m_bBorderShow || m_nBorderSize <= 0 )return;
	if(m_bLightDataRegion && pT->GetLightLineSize() <= 0)return;
	
	COLORREF bordercolor = m_bLightDataRegion?pT->GetLightColor():m_crBorderColor;
	int bordersize = m_bLightDataRegion?pT->GetLightLineSize():m_nBorderSize;
	
	HPEN	hPen,hOldPen;
	HBRUSH hBrush,hOldBrush;
	hPen = CreatePen( m_nBorderStyle, bordersize, bordercolor );
	hBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
	
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC,hBrush);
	
	Rectangle(hDC,plotRect.left,plotRect.top,plotRect.right,plotRect.bottom);
	
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen );
	DeleteObject(hPen);
}

Declare_Namespace_End