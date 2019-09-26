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

#pragma warning(disable:4786)

#include "Title.h"
#include "Plot.h"
#include "PlotUtility.h"
#include "../Resources/PlotResources.h"

Declare_Namespace_CChart

template <class PlotImplT>
class CTitleImpl : public CTitle
{
public:
	CTitleImpl();
	virtual ~CTitleImpl();

protected:
	RECT	AdjustByTitle( HDC hDC, RECT plotRect );

public:
	SIZE	GetTitleTextSize( HDC hDC );
	RECT	GetTitleRect( HDC hDC );
	SIZE	GetTitleSize( HDC hDC );

	void	DrawTitle( HDC hDC );
};

template <class PlotImplT>
CTitleImpl<PlotImplT>::CTitleImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(CGlobal::m_bLoadDefPatterns)
	{
		HBITMAP hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_BACKGROUND_IMAGE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if(hb)
		{
			SetTitlePattern(hb);
			DeleteObject(hb);
		}
	}
}

template <class PlotImplT>
CTitleImpl<PlotImplT>::~CTitleImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
RECT	CTitleImpl<PlotImplT>::AdjustByTitle( HDC hDC, RECT plotRect )
{
	if(!IsTitleRealyShow())return plotRect;

	SIZE size = GetTitleTextSize(hDC);
	
	switch(m_nTitlePosition)
	{
	case kPositionL:
		plotRect.left += ( size.cx + m_nTitleGap );
		break;
	case kPositionR:
		plotRect.right -= ( size.cx + m_nTitleGap );
		break;
	case kPositionT:
		plotRect.top += ( size.cy + m_nTitleGap );
		break;
	case kPositionB:
		plotRect.bottom -= ( size.cy + m_nTitleGap );
		break;
	}
	
	return plotRect;
}

template <class PlotImplT>
SIZE	CTitleImpl<PlotImplT>::GetTitleTextSize( HDC hDC )
{
	SIZE size={0,0};
	
	tstring strTitle=GetTitle();
	if( strTitle != _TEXT("") )
	{
		MyGetTextExtentPoint( hDC, &GetTitleFont(), strTitle, DT_LEFT, &size);
	}
	if(m_nTitleOrientation == kOrientationTB || m_nTitleOrientation == kOrientationBT)
	{
		std::swap(size.cx, size.cy);
	}
	
	return size;
}

template <class PlotImplT>
RECT	CTitleImpl<PlotImplT>::GetTitleRect(HDC hDC)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	RECT titleRect, clientRect, plotRect;
	clientRect = pT->GetLastClientRect();
	plotRect = pT->GetLastPlotRect();

	SIZE size=GetTitleTextSize(hDC);
	titleRect = clientRect;
	switch(m_nTitlePosition)
	{
	case kPositionL:
	case kPositionR:
		if(m_nTitlePosition == kPositionL)
		{
			titleRect.left += pT->GetMarginLeft();
			titleRect.right = titleRect.left + size.cx;
		}
		if(m_nTitlePosition == kPositionR)
		{
			titleRect.right -= pT->GetMarginRight();
			titleRect.left = titleRect.right - size.cx;
		}		
		titleRect.top = plotRect.top;
		titleRect.bottom = plotRect.bottom;
		break;

	case kPositionT:
	case kPositionB:
		if(m_nTitlePosition == kPositionT)
		{
			titleRect.top += pT->GetMarginTop();
			titleRect.bottom = titleRect.top + size.cy;
		}
		if(m_nTitlePosition == kPositionB)
		{
			titleRect.bottom -= pT->GetMarginBottom();
			titleRect.top = titleRect.bottom - size.cy;
		}
		titleRect.left = plotRect.left;
		titleRect.right = plotRect.right;
		break;
	}
	titleRect = pT->AdjustTitleRect(hDC, titleRect);//for headernote or footnote
	return titleRect;
}

template <class PlotImplT>
SIZE	CTitleImpl<PlotImplT>::GetTitleSize( HDC hDC )
{
	SIZE size={0,0};
	
	RECT titleRect = GetTitleRect(hDC);
	size.cx = Width(titleRect);
	size.cy = Height(titleRect);
	
	return size;
}

template <class PlotImplT>
void	CTitleImpl<PlotImplT>::DrawTitle( HDC hDC )
{
	if(m_strTitle == _TEXT("") || !m_bTitleShow)return;

	PlotImplT* pT = static_cast<PlotImplT*>(this);

	RECT titleRect = GetTitleRect(hDC);

	//Brush
	HBRUSH hBrush, hOldBrush;
	switch(m_nTitleBrushType)
	{
	case kBrushSolid:
		hBrush = CreateSolidBrush(m_crTitleBgColor);
		break;
	case kBrushHatch:
		hBrush = CreateHatchBrush(m_nTitleHatchStyle, m_crTitleHatchColor);
		break;
	case kBrushPattern:
		hBrush = CreatePatternBrush(m_hTitlePattern);
		break;
	default:
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hPen, hOldPen;
	if(m_bTitleBorderShow)
	{
		hPen = CreatePen(PS_SOLID, m_nTitleBorderSize, m_crTitleBorderColor);
	}
	else
	{
		hPen = (HPEN)GetStockObject(NULL_PEN);
	}
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, titleRect.left, titleRect.top, titleRect.right, titleRect.bottom );
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	// Border
	if( (m_bLightTitle || m_bSelectTitle) && m_nTitleSLShow==kSLShowEdge )
	{
		if(m_bSelectTitle)
		{
			hPen = CreatePen( PS_SOLID, pT->GetLightLineSize(), pT->GetLightColor() );
		}
		else
		{
			hPen = CreatePen( PS_SOLID, pT->GetSelectLineSize(), pT->GetSelectColor() );
		}
		
		hOldPen = (HPEN)SelectObject( hDC,hPen );
		
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		hOldBrush = (HBRUSH)SelectObject(hDC,hBrush);
		Rectangle(hDC,titleRect.left,titleRect.top,titleRect.right,titleRect.bottom);
		SelectObject(hDC,hBrush);
		SelectObject(hDC,hOldPen);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}
	
	//Draw Text
	UINT format;
	if(m_nTitleAlignment<0) format = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	else if(m_nTitleAlignment>0) format = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	else format = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	COLORREF cr = m_crTitleColor;
	if( (m_bLightTitle || m_bSelectTitle) && m_nTitleSLShow==kSLShowText )
	{
		if(m_bLightTitle)cr = pT->GetLightColor();
		if(m_bSelectTitle)cr = pT->GetSelectColor();
	}
	
	if(m_nTitleOrientation == kOrientationLR)
	{
		DrawTextInRect(hDC, titleRect, m_strTitle, &m_lfTitleFont, TRANSPARENT, cr, format);
	}
	else
	{
		SIZE size = {Width(titleRect), Height(titleRect)};
		if(m_nTitleOrientation==kOrientationTB || m_nTitleOrientation==kOrientationBT)
		{
			std::swap(size.cx, size.cy);
		}
		RECT rect = {0, 0, size.cx, size.cy};
		
		HDC hMemDC;
		HBITMAP hbm1, hbm0;
		hMemDC = CreateCompatibleDC(hDC);
		hbm1 = CreateCompatibleBitmap(hDC, size.cx, size.cy);//the first parameter shouldn't be hMemDC, or the image will be black
		hbm0 = (HBITMAP)SelectObject(hMemDC, hbm1);
		
		POINT points[3];
		if(m_nTitleOrientation == kOrientationRL)
		{
			points[0].x = rect.right; points[0].y = rect.bottom; //left top
			points[1].x = rect.left; points[1].y = rect.bottom;  //right top
			points[2].x = rect.right; points[2].y = rect.top;    //left bottom
		}
		else if(m_nTitleOrientation == kOrientationBT)
		{
			points[0].x = rect.right; points[0].y = rect.top;
			points[1].x = rect.right; points[1].y = rect.bottom;
			points[2].x = rect.left; points[2].y = rect.top;
		}
		else if(m_nTitleOrientation == kOrientationTB)
		{
			points[0].x = rect.left; points[0].y = rect.bottom;
			points[1].x = rect.left; points[1].y = rect.top;
			points[2].x = rect.right; points[2].y = rect.bottom;
		}
		PlgBlt(hMemDC, points, hDC, titleRect.left, titleRect.top, Width(titleRect), Height(titleRect), (HBITMAP)NULL, 0, 0);
		
		//Draw in memory dc
		DrawTextInRect(hMemDC, rect, m_strTitle, &m_lfTitleFont, TRANSPARENT, cr, format);
		
		if(m_nTitleOrientation == kOrientationRL)
		{
			points[0].x = titleRect.right; points[0].y = titleRect.bottom; //left top
			points[1].x = titleRect.left; points[1].y = titleRect.bottom;  //right top
			points[2].x = titleRect.right; points[2].y = titleRect.top;    //left bottom
		}
		else if(m_nTitleOrientation == kOrientationBT)
		{
			points[0].x = titleRect.left; points[0].y = titleRect.bottom;
			points[1].x = titleRect.left; points[1].y = titleRect.top;
			points[2].x = titleRect.right; points[2].y = titleRect.bottom;
		}
		else if(m_nTitleOrientation == kOrientationTB)
		{
			points[0].x = titleRect.right; points[0].y = titleRect.top;
			points[1].x = titleRect.right; points[1].y = titleRect.bottom;
			points[2].x = titleRect.left; points[2].y = titleRect.top;
		}	
		PlgBlt(hDC, points, hMemDC, rect.left, rect.top, Width(rect), Height(rect), (HBITMAP)NULL, 0, 0);
		
		hbm1 = (HBITMAP)SelectObject(hMemDC, hbm0);	
		DeleteObject(hbm1);	
		DeleteDC(hMemDC);
	}	
}

Declare_Namespace_End