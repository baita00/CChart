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

#include "CommentImpl.h"
#include "../Global.h"
#include "../../Resources/PlotResources.h"
#include "../AxisImpl.h"
#include "../PlotImpl.h"

using namespace NsCChart;

CCommentImpl::CCommentImpl(CPlotImpl *plot)
{
	m_pPlot = plot;

	if(CGlobal::m_bLoadDefPatterns)
	{
		HBITMAP hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_LEDBULB_ON), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if(hb)
		{
			SetCommentBitmap(hb);
			DeleteObject(hb);
		}
	}
}

CCommentImpl::~CCommentImpl()
{

}

void	CCommentImpl::OnDraw( HDC hDC )
{
	if(!m_bCommentShow)return;
	
	if(m_bTextComment)DrawTextComments(hDC);
	else DrawPictureComments(hDC);
}

void	CCommentImpl::DrawTextComments( HDC hDC )
{	
	RECT commentRect;
	commentRect = GetTextCommentRect(hDC);

	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;

	if(m_bTransparent)hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	else hBrush = CreateSolidBrush(m_crCommentBkColor);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	
	hFont = CreateFontIndirect(&m_lfCommentFont);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	hPen = CreatePen(m_nBorderStyle, m_bDragMode?m_nBorderSize+1:m_nBorderSize, m_bDragMode?GetPlot()->GetSelectColor():m_crBorderColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	COLORREF oldColor = SetTextColor(hDC, m_crCommentColor);
	
	if(m_bBorderShow || m_bDragMode)
	{
		Rectangle(hDC, commentRect.left, commentRect.top, commentRect.right, commentRect.bottom);
	}
	else
	{
		FillRect(hDC, &commentRect, hBrush);
	}
	DrawText(hDC, m_strComment.c_str(), (int)m_strComment.length(), &commentRect, m_nFormat);
	
	SetTextColor(hDC, oldColor);
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	if(!m_bTransparent)DeleteObject(hBrush);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void	CCommentImpl::DrawPictureComments( HDC hDC )
{
	RECT commentRect;
	
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap;

	commentRect = GetPictureCommentRect(hDC);
	Rectangle(hDC, commentRect.left, commentRect.top, commentRect.right, commentRect.bottom);
	
	hBitmap = m_hBitmap;
	if(!hBitmap)
	{
		MessageBox(WindowFromDC(hDC), _TEXT("HBitmap is invalid"), _TEXT("Warning"), MB_OK);
		return;
	}
	SelectObject(hMemDC, hBitmap);
	BitBlt(hDC, commentRect.left, commentRect.top, commentRect.right - commentRect.left, commentRect.bottom - commentRect.top, hMemDC, 0, 0, m_nRop);

	DeleteDC(hMemDC);
}

SIZE CCommentImpl::GetCommentSize(HDC hDC)
{
	if(m_bTextComment)return GetTextCommentSize(hDC);
	else return GetPictureCommentSize();
}

RECT	CCommentImpl::GetCommentRect(HDC hDC)
{
	if(m_bTextComment)return GetTextCommentRect(hDC);
	else return GetPictureCommentRect(hDC);
}

SIZE	CCommentImpl::GetTextCommentSize(HDC hDC)
{
	SIZE commentSize = {0, 0};
	RECT commentRect = {0, 0, 0, 0};
		
	HFONT hFont, hOldFont;
	hFont = CreateFontIndirect(&m_lfCommentFont);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	DrawText(hDC, m_strComment.c_str(), (int)m_strComment.length(), &commentRect, m_nFormat|DT_CALCRECT);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
	
	commentSize.cx = commentRect.right - commentRect.left;
	commentSize.cy = commentRect.bottom - commentRect.top;
	
	return commentSize;
}

SIZE	CCommentImpl::GetPictureCommentSize()
{
	SIZE commentSize = {0, 0};
		
	HBITMAP hBm = m_hBitmap;
	if(!hBm)return commentSize;
	
	return GetBitmapSize(hBm);
}

RECT	CCommentImpl::GetTextCommentRect(HDC hDC)
{
	RECT commentRect = {0, 0, 0, 0};
	
	POINT pos;
	pos = GetInitCommentPoint(hDC, m_nPosition, m_nPosX, m_nPosY, m_bExcludeTitle);
	
	commentRect.left = commentRect.right = pos.x;
	commentRect.bottom = commentRect.top = pos.y;
	SIZE commentSize = GetTextCommentSize(hDC);
	commentRect.right = commentRect.left + commentSize.cx;
	commentRect.bottom = commentRect.top + commentSize.cy;
	
	RecalcCommentRect(m_nPosition, m_sizeOffset, commentRect);
	
	return commentRect;
}

RECT	CCommentImpl::GetPictureCommentRect(HDC hDC)
{
	RECT commentRect = {0, 0, 0, 0};
		
	POINT pos;
	pos = GetInitCommentPoint(hDC, m_nPosition, m_nPosX, m_nPosY, m_bExcludeTitle);
	
	commentRect.left = commentRect.right = pos.x;
	commentRect.bottom = commentRect.top = pos.y;
	SIZE commentSize = GetPictureCommentSize();
	commentRect.right = commentRect.left + commentSize.cx;
	commentRect.bottom = commentRect.top + commentSize.cy;
	
	RecalcCommentRect(m_nPosition, m_sizeOffset, commentRect);
	
	return commentRect;
}

void	CCommentImpl::RecalcCommentRect(int nPosition, SIZE sizeOffset, RECT &commentRect)
{
	int cx, cy;
	cx = commentRect.right - commentRect.left;
	cy = commentRect.bottom - commentRect.top;

	switch(nPosition)
	{
	case kCommentLeftOut:
		commentRect.left -= cx;
		commentRect.right -= cx;
		commentRect.bottom -= cy/2;
		commentRect.top -= cy/2;
		break;
	case kCommentRightOut:
		commentRect.bottom -= cy/2;
		commentRect.top -= cy/2;
		break;
	case kCommentBottomOut:
		commentRect.left -= cx/2;
		commentRect.right -= cx/2;
		break;
	case kCommentTopOut:
		commentRect.left -= cx/2;
		commentRect.right -= cx/2;
		commentRect.top -= cy;
		commentRect.bottom -= cy;
		break;
	case kCommentTopLeftOut:
		commentRect.top -= cy;
		commentRect.bottom -= cy;
		break;
	case kCommentLeftTopOut:
		commentRect.left -= cx;
		commentRect.right -= cx;
		break;
	case kCommentBottomLeftOut:
		break;
	case kCommentLeftBottomOut:
		commentRect.left -= cx;
		commentRect.right -= cx;
		commentRect.top -= cy;
		commentRect.bottom -= cy;
		break;
	case kCommentTopRightOut:
		commentRect.left -= cx;
		commentRect.right -= cx;
		commentRect.top -= cy;
		commentRect.bottom -= cy;
		break;
	case kCommentRightTopOut:
		break;
	case kCommentBottomRightOut:
		commentRect.left -= cx;
		commentRect.right -= cx;
		break;
	case kCommentRightBottomOut:
		commentRect.bottom -= cy;
		commentRect.top -= cy;
		break;
	case kCommentLeftIn:
	case kCommentLeftHighIn:
	case kCommentLeftHigherIn:
	case kCommentLeftLowIn:
	case kCommentLeftLowerIn:
		commentRect.top -= cy/2;
		commentRect.bottom -= cy/2;
		break;
	case kCommentRightIn:
	case kCommentRightHighIn:
	case kCommentRightHigherIn:
	case kCommentRightLowIn:
	case kCommentRightLowerIn:
		commentRect.left -= cx;
		commentRect.right -= cx;
		commentRect.top -= cy/2;
		commentRect.bottom -= cy/2;
		break;
	case kCommentBottomIn:
		commentRect.left -= cx/2;
		commentRect.right -= cx/2;
		commentRect.top -= cy;
		commentRect.bottom -= cy;
		break;
	case kCommentTopIn:
		commentRect.left -= cx/2;
		commentRect.right -= cx/2;
		break;
	case kCommentLeftTopIn:
	case kCommentTopLeftIn:
		break;
	case kCommentLeftBottomIn:
	case kCommentBottomLeftIn:
		commentRect.top -= cy;
		commentRect.bottom -= cy;
		break;
	case kCommentRightTopIn:
	case kCommentTopRightIn:
		commentRect.left -= cx;
		commentRect.right -= cx;
		break;
	case kCommentRightBottomIn:
	case kCommentBottomRightIn:
		commentRect.left -= cx;
		commentRect.right -= cx;
		commentRect.top -= cy;
		commentRect.bottom -= cy;
		break;
	case kCommentCenter:
	case kCommentCenterHigh:
	case kCommentCenterHigher:
	case kCommentCenterLow:
	case kCommentCenterLower:
		commentRect.left -= cx/2;
		commentRect.right -= cx/2;
		commentRect.top -= cy/2;
		commentRect.bottom -= cy/2;
		break;
	}
	
	commentRect.left += sizeOffset.cx;
	commentRect.right += sizeOffset.cx;
	commentRect.top += sizeOffset.cy;
	commentRect.bottom += sizeOffset.cy;

	if(IsConfined())
	{
		// restrict in the rectangle
		int width = Width(commentRect);
		int height = Height(commentRect);
		RECT plotRect = GetPlot()->GetLastPlotRect();
		if(commentRect.right > plotRect.right)
		{
			commentRect.right = plotRect.right;
			commentRect.left = commentRect.right - width;
		}
		if(commentRect.left < plotRect.left)
		{
			commentRect.left = plotRect.left;
			commentRect.right = commentRect.left + width;
		}
		if(commentRect.top < plotRect.top)
		{
			commentRect.top = plotRect.top;
			commentRect.bottom = commentRect.top + height;
		}
		if(commentRect.bottom > plotRect.bottom)
		{
			commentRect.bottom = plotRect.bottom;
			commentRect.top = commentRect.bottom - height;
		}
	}
}

POINT	CCommentImpl::GetInitCommentPoint(HDC hDC, int nPosition, int nPosX, int nPosY, bool bExcludeTitle)
{
	RECT destRect = GetPlot()->GetLastClientRect();
	RECT plotRect = GetPlot()->GetLastPlotRect();
	
	POINT pos;

	CAxisImpl *axis;
	SIZE axisSize;
	SIZE titleSize;
	switch(nPosition)
	{
	case kCommentDataRegion:
		pos.x = nPosX + plotRect.left;
		pos.y = nPosY + plotRect.top;
		break;
	case kCommentClientRect:
		pos.x = nPosX + destRect.left;
		pos.y = nPosY + destRect.top;
		break;
	case kCommentLeftOut:
		pos.x = plotRect.left;
		pos.y = (plotRect.bottom + plotRect.top)/2;
		axis = GetPlot()->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.x -= axisSize.cx;
		}
		
		break;
	case kCommentRightOut:
		pos.x = plotRect.right;
		pos.y = (plotRect.bottom + plotRect.top)/2;
		axis = GetPlot()->GetAxisByLocation(kLocationRight);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.x += axisSize.cx;
		}
		
		break;
	case kCommentBottomOut:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = plotRect.bottom;
		axis = GetPlot()->GetAxisByLocation(kLocationBottom);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.y += axisSize.cy;
		}
		
		break;
	case kCommentTopOut:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = plotRect.top;
		axis = GetPlot()->GetAxisByLocation(kLocationTop);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.y -= axisSize.cy;
		}
		if(!bExcludeTitle)
		{
			titleSize = GetPlot()->GetTitleSize(hDC);
			pos.y -= titleSize.cy;
		}
		
		break;
	case kCommentTopLeftOut:
	case kCommentLeftTopOut:
		pos.x = plotRect.left;
		pos.y = plotRect.top;
		axis = GetPlot()->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.x -= axisSize.cx;
		}
		
		axis = GetPlot()->GetAxisByLocation(kLocationTop);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.y -= axisSize.cy;
		}
		if(!bExcludeTitle)
		{
			titleSize = GetPlot()->GetTitleSize(hDC);
			pos.y -= titleSize.cy;
		}
		
		break;
	case kCommentBottomLeftOut:
	case kCommentLeftBottomOut:
		pos.x = plotRect.left;
		pos.y = plotRect.bottom;

		axis = GetPlot()->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.x -= axisSize.cx;
		}
		
		axis = GetPlot()->GetAxisByLocation(kLocationBottom);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.y += axisSize.cy;
		}
		
		break;
	case kCommentTopRightOut:
	case kCommentRightTopOut:
		pos.x = plotRect.right;
		pos.y = plotRect.top;

		axis = GetPlot()->GetAxisByLocation(kLocationRight);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.x += axisSize.cx;
		}
		
		axis = GetPlot()->GetAxisByLocation(kLocationTop);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.y -= axisSize.cy;
		}

		if(!bExcludeTitle)
		{
			titleSize = GetPlot()->GetTitleSize(hDC);
			pos.y -= titleSize.cy;
		}
		
		break;
	case kCommentBottomRightOut:
	case kCommentRightBottomOut:
		pos.x = plotRect.right;
		pos.y = plotRect.bottom;

		axis = GetPlot()->GetAxisByLocation(kLocationRight);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.x += axisSize.cx;
		}
		
		axis = GetPlot()->GetAxisByLocation(kLocationBottom);
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetSize(hDC);
			pos.y += axisSize.cy;
		}
		
		break;
	case kCommentLeftIn:
		pos.x = plotRect.left;
		pos.y = (plotRect.top + plotRect.bottom)/2;
		break;
	case kCommentRightIn:
		pos.x = plotRect.right;
		pos.y = (plotRect.top + plotRect.bottom)/2;
		break;
	case kCommentBottomIn:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = plotRect.bottom;
		break;
	case kCommentTopIn:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = plotRect.top;
		break;
	case kCommentLeftTopIn:
	case kCommentTopLeftIn:
		pos.x = plotRect.left;
		pos.y = plotRect.top;
		break;
	case kCommentLeftBottomIn:
	case kCommentBottomLeftIn:
		pos.x = plotRect.left;
		pos.y = plotRect.bottom;
		break;
	case kCommentRightTopIn:
	case kCommentTopRightIn:
		pos.x = plotRect.right;
		pos.y = plotRect.top;
		break;
	case kCommentRightBottomIn:
	case kCommentBottomRightIn:
		pos.x = plotRect.right;
		pos.y = plotRect.bottom;
		break;
	case kCommentLeftHighIn:
		pos.x = plotRect.left;
		pos.y = (plotRect.bottom + 3*plotRect.top)/4;
		break;
	case kCommentLeftHigherIn:
		pos.x = plotRect.left;
		pos.y = (plotRect.bottom + 7*plotRect.top)/8;
		break;
	case kCommentLeftLowIn:
		pos.x = plotRect.left;
		pos.y = (3*plotRect.bottom + plotRect.top)/4;
		break;
	case kCommentLeftLowerIn:
		pos.x = plotRect.right;
		pos.y = (7*plotRect.bottom + plotRect.top)/8;
		break;
	case kCommentRightHighIn:
		pos.x = plotRect.right;
		pos.y = (plotRect.bottom + 3*plotRect.top)/4;
		break;
	case kCommentRightHigherIn:
		pos.x = plotRect.right;
		pos.y = (plotRect.bottom + 7*plotRect.top)/8;
		break;
	case kCommentRightLowIn:
		pos.x = plotRect.right;
		pos.y = (3*plotRect.bottom + plotRect.top)/4;
		break;
	case kCommentRightLowerIn:
		pos.x = plotRect.right;
		pos.y = (7*plotRect.bottom + plotRect.top)/8;
		break;
	case kCommentCenter:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = (plotRect.bottom + plotRect.top)/2;
		break;
	case kCommentCenterHigh:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = (plotRect.bottom + 3*plotRect.top)/4;
		break;
	case kCommentCenterHigher:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = (plotRect.bottom + 5*plotRect.top)/6;
		break;
	case kCommentCenterLow:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = (3*plotRect.bottom + plotRect.top)/4;
		break;
	case kCommentCenterLower:
		pos.x = (plotRect.left + plotRect.right)/2;
		pos.y = (5*plotRect.bottom + plotRect.top)/6;
		break;
	default:
		pos.x = 0;
		pos.y = 0;
	}
	return pos;
}