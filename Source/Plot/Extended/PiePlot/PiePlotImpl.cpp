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
#include "PiePlotImpl.h"
#include "../../Basic/MyConst.h"
#include "../../Basic/MyMemDC.h"

using namespace NsCChart;

CPiePlotImpl::CPiePlotImpl()
{
	ResetApperance();
	SetLegendColorRect(true);
	SetLegendShow(false);
}

CPiePlotImpl::~CPiePlotImpl()
{

}

void	CPiePlotImpl::ResetApperance()
{
	m_vpPlotData.clear();

	for(int i=0; i<GetAxesCount(); i++)
	{
		CAxisImpl *axis=0;
		axis=GetAxis(i);
		if(axis)axis->SetVisible(false);
	}
}

int		CPiePlotImpl::AddPie(double value)
{
	return AddPie(value, RGB(255,0,0), CGlobal::GetStrUntitle(), 1.0);
}

int		CPiePlotImpl::AddPie(double value, COLORREF color, tstring title, double scale)
{
	if(value <= 0.0) return -1;
	if(GetPlotDataCount() < m_nMaxPies)
	{
		if(scale <= 0.0 || scale > 1.0 )
			scale = 1.0;
		
		MyVData2D vData;
		DataPoint2D dp;
		dp.val[0] = 0.0;
		dp.val[1] = value;
		vData.push_back( dp );
		return AddCurve(vData);
	}
	return -1;
}

void	CPiePlotImpl::SetPieTitle(int which, tstring title)
{
	if(which>=0 && which<GetPlotDataCount())
	{
		GetAbsPlotData(which)->SetTitle(title);
	}
}
void	CPiePlotImpl::SetPieTitleFromID(int dataID, tstring title)
{
	int which = GetIndex(dataID);
	if(which<0)
		return;
	GetAbsPlotData(which)->SetTitle(title);
}
tstring	CPiePlotImpl::GetPieTitle(int which)
{
	if(which>=0 && which<GetPlotDataCount())
	{
		return GetAbsPlotData(which)->GetTitle();
	}
	else
	{
		return _TEXT("");
	}
}
tstring	CPiePlotImpl::GetPieTitleFromID(int dataID)
{
	int which = GetIndex(dataID);
	if(which<0)
		return _TEXT("");
	else
		return GetAbsPlotData(which)->GetTitle();
}

void	CPiePlotImpl::GetPlotRange(double *xRange, double *yRange)
{
	SetXRange(0.0,1.0);
	SetXTickCount(3);
	SetYRange(0.0,1.0);
	SetYTickCount(3);

	int i;
	for(i=0; i<(int)m_vpAxis.size(); i++)
	{
		m_vpAxis[i]->SetTickLabelShow(false);
	}
}

void	CPiePlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	//if there is no data, doing nothing
	if(GetPlotDataCount()<=0)
	{
		DrawBkgnd( hDC );
		return;
	}

	//int nOldMapmode = SetMapMode(hDC, MM_TEXT);

	HDC hdc;
	CMyMemDC memdc;
	if(!m_bDoubleBuffer)
	{
		hdc = hDC;
	}
	else
	{
		memdc->CreateMemDC(hDC, NULL, &destRect);
		hdc = memdc;
	}

	double xRange[2], yRange[2];
	GetPlotRange(xRange, yRange);

//	EnterCriticalSection(&m_csPlot);
	// Draw basics
	CPlotImpl::FinalDraw(hdc,destRect);
	// Draw Pies
	DrawPies( hdc );

	DrawLegend(hdc);

//	LeaveCriticalSection(&m_csPlot);

	//SetMapMode(hDC, nOldMapmode);
}

void	CPiePlotImpl::DrawPiesBasic(HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total)
{
	HPEN hPen,hOldPen;
	HBRUSH hBrush,hOldBrush;
	hPen = CreatePen(m_nLineStyle, m_nLineSize, m_crLineColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	int i;
	double ratio;
	double a1,b1,s1;
	double curr;

	
	if(m_bShadowing || m_b3DStyle)
	{
		hBrush = CreateSolidBrush( m_crShadowColor );
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		double ctx1,cty1,ctx2,cty2;
		ctx1 = ctx - 2 * m_n3DHalfLength * cos( m_f3DAngle * myPi /180.0);
		cty1 = cty + 2 * m_n3DHalfLength * sin( m_f3DAngle * myPi /180.0) * b / a;
		Ellipse(hDC, int(ctx1-a), int(cty1-b), int(ctx1+a), int(cty1+b));
		ctx2 = ctx;// + m_nShadowHalfLength * cos( m_fShadowAngle * myPi /180.0);
		cty2 = cty;// - m_nShadowHalfLength * sin( m_fShadowAngle * myPi /180.0) * b / a;
		Ellipse(hDC, int(ctx2-a), int(cty2-b), int(ctx2+a), int(cty2+b));
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);

		if(m_b3DStyle)
		{
			
			// Draw bottom pie
			curr= m_fStartAngle/360.0;
			for(i=0; i<GetPlotDataCount(); i++)
			{
				hBrush = CreateSolidBrush(GetPlotColor(GetDataID(i)));
				hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				s1 = GetPieScale(i);
				if(s1<=0.0 || s1>1.0)s1=1.0;
				a1 = a * s1;
				b1 = b * s1;
				
				ratio = GetAbsData(i)[0].val[1]/total;
				if(m_bClockWise)ratio = -ratio;
				if(ratio > 0.0)
				{
					Pie(hDC, int(ctx1-a1), int(cty1-b1), int(ctx1+a1), int(cty1+b1),
						int(ctx1+a1*cos(curr*2.0*myPi)), int(cty1-b1*sin(curr*2.0*myPi)),
						int(ctx1+a1*cos((curr+ratio)*2.0*myPi)), int(cty1-b1*sin((curr+ratio)*2.0*myPi)));
				}
				else
				{
					Pie(hDC, int(ctx1-a1), int(cty1-b1), int(ctx1+a1), int(cty1+b1),
						int(ctx1+a1*cos((curr+ratio)*2.0*myPi)), int(cty1-b1*sin((curr+ratio)*2.0*myPi)),
						int(ctx1+a1*cos(curr*2.0*myPi)), int(cty1-b1*sin(curr*2.0*myPi)));
				}
				
				SelectObject(hDC, hOldBrush);
				DeleteObject(hBrush);
				
				curr += ratio;
			}
			// repair the edge
			curr= m_fStartAngle/360.0;
			for(i=0; i<GetPlotDataCount(); i++)
			{
				hBrush = CreateSolidBrush(GetPlotColor(GetDataID(i)));
				hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				
				POINT	pt[4];
				
				s1 = GetPieScale(i);
				if(s1<=0.0 || s1>1.0)s1=1.0;
				a1 = a * s1;
				b1 = b * s1;
				
				ratio = GetAbsData(i)[0].val[1]/total;
				if(m_bClockWise)ratio = -ratio;
				if(ratio > 0.0)
				{
					pt[0].x = int(ctx1+a1*cos(curr*2.0*myPi));
					pt[0].y = int(cty1-b1*sin(curr*2.0*myPi));
					pt[1].x = int(ctx1+a1*cos((curr+ratio)*2.0*myPi));
					pt[1].y = int(cty1-b1*sin((curr+ratio)*2.0*myPi));
					pt[2].x = int(ctx2+a1*cos((curr+ratio)*2.0*myPi));
					pt[2].y = int(cty2-b1*sin((curr+ratio)*2.0*myPi));
					pt[3].x = int(ctx2+a1*cos(curr*2.0*myPi));
					pt[3].y = int(cty2-b1*sin(curr*2.0*myPi));
				}
				else
				{
					pt[0].x = int(ctx1+a1*cos((curr+ratio)*2.0*myPi));
					pt[0].y = int(cty1-b1*sin((curr+ratio)*2.0*myPi));
					pt[1].x = int(ctx1+a1*cos(curr*2.0*myPi));
					pt[1].y = int(cty1-b1*sin(curr*2.0*myPi));
					pt[2].x = int(ctx2+a1*cos(curr*2.0*myPi));
					pt[2].y = int(cty2-b1*sin(curr*2.0*myPi));
					pt[3].x = int(ctx2+a1*cos((curr+ratio)*2.0*myPi));
					pt[3].y = int(cty2-b1*sin((curr+ratio)*2.0*myPi));
				}

				BeginPath(hDC);
				MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
				SetArcDirection(hDC, AD_COUNTERCLOCKWISE);
				ArcTo(hDC,int(ctx1-a1), int(cty1-b1), int(ctx1+a1), int(cty1+b1), 
					pt[0].x, pt[0].y, pt[1].x, pt[1].y);
				LineTo(hDC, pt[2].x, pt[2].y);
				SetArcDirection(hDC, AD_CLOCKWISE);
				ArcTo(hDC,int(ctx2-a1), int(cty2-b1), int(ctx2+a1), int(cty2+b1),
					pt[2].x, pt[2].y, pt[3].x, pt[3].y);
				EndPath(hDC);
				StrokeAndFillPath(hDC);
				
				//restore
				SetArcDirection(hDC, AD_COUNTERCLOCKWISE);
				
				SelectObject(hDC, hOldBrush);
				DeleteObject(hBrush);
				
				curr += ratio;
			}
		}
		else if(m_bShadowing)
		{
			BeginPath(hDC);
			double TAngle1,TAngle2;
			TAngle1 = (m_f3DAngle + 90.0) * myPi /180.0;
			TAngle2 = (m_f3DAngle + 270.0) * myPi /180.0;
			
			MoveToEx(hDC, int( ctx1 + a * cos( TAngle2 ) ), int( cty1 - b * sin( TAngle2 ) ), NULL );
			LineTo(hDC, int( ctx2 + a * cos( TAngle2 ) ), int( cty2 - b * sin( TAngle2 ) ) );
			ArcTo(hDC, int (ctx2-a ), int( cty2-b ), int( ctx2+a ), int( cty2+b ), int( ctx2 + a * cos( TAngle2 ) ), int(cty2 - b * sin( TAngle2 ) ), int( ctx2 + a * cos( TAngle1 ) ), int (cty2 - b * sin( TAngle1 ) ) );
			LineTo(hDC, int( ctx1 + a * cos( TAngle1 ) ), int( cty1 - b * sin( TAngle1 ) ) );
			ArcTo(hDC, int( ctx1-a ), int( cty1-b ), int( ctx1+a ), int( cty1+b ), int( ctx1 + a * cos( TAngle1 ) ), int( cty1 - b * sin( TAngle1 ) ), int( ctx1 + a * cos( TAngle2 ) ), int( cty1 - b * sin( TAngle2 ) ) );
			LineTo(hDC, int( ctx1 + a * cos( TAngle2 ) ), int( cty1 - b * sin( TAngle2 ) ) );
			
			EndPath(hDC);
			StrokeAndFillPath(hDC);
		}
	}
	else
	{
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, int(ctx-a), int(cty-b), int(ctx+a), int(cty+b));
		
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
	}

	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CPiePlotImpl::DrawSinglePie(HDC hDC, RECT plotRect, int i, double ctx, double cty, double a, double b, double total, double curr, double &ratio)
{
	double a1,b1,s1;

	HBRUSH hBrush,hOldBrush;

	hBrush = CreateSolidBrush(GetPlotColor(GetDataID(i)));
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	
	s1 = GetPieScale(i);
	if(s1<=0.0 || s1>1.0)s1=1.0;
	a1 = a * s1;
	b1 = b * s1;
	
	ratio = GetAbsData(i)[0].val[1]/total;
	if(m_bClockWise)ratio = -ratio;
	if(ratio > 0.0)
	{
		Pie(hDC, int(ctx-a1), int(cty-b1), int(ctx+a1), int(cty+b1),
			int(ctx+a1*cos(curr*2.0*myPi)), int(cty-b1*sin(curr*2.0*myPi)),
			int(ctx+a1*cos((curr+ratio)*2.0*myPi)), int(cty-b1*sin((curr+ratio)*2.0*myPi)));
	}
	else
	{
		Pie(hDC, int(ctx-a1), int(cty-b1), int(ctx+a1), int(cty+b1),
			int(ctx+a1*cos((curr+ratio)*2.0*myPi)), int(cty-b1*sin((curr+ratio)*2.0*myPi)),
			int(ctx+a1*cos(curr*2.0*myPi)), int(cty-b1*sin(curr*2.0*myPi)));
	}
	
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}

void	CPiePlotImpl::DrawSingleTit(HDC hDC, RECT plotRect, int i, double ctx, double cty, double a, double b, double total, double curr, double &ratio)
{
	int fontheight;

	HPEN hPen,hOldPen;
	hPen = CreatePen(m_nLineStyle, m_nLineSize, m_crLineColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	double a1,b1,s1;

	s1 = GetPieScale(i);
	if(s1<=0.0 || s1>1.0)s1=1.0;
	a1 = a * s1;
	b1 = b * s1;
	
	ratio = GetAbsData(i)[0].val[1]/total;
	if(m_bClockWise)ratio = -ratio;
	
	RECT rect = GetPieTitleRect( hDC, plotRect, i, ctx, cty, a1, b1, curr, ratio, fontheight );
	if( IsRectEmpty(&rect) )return;
	//if Lighted or selected
	if(IsLighted(GetDataID(i)) || IsSelected(GetDataID(i)))
	{
		SelectObject(hDC, hOldPen);
		DeleteObject(hPen);
		hPen = CreatePen(GetPlotLineStyle(GetDataID(i)), GetPlotLineSize(GetDataID(i)), GetPlotColor(GetDataID(i)));
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	}
	//light or select ended
	
	// determine intersect
	LOGFONT logFont = m_lfPieTitleFont;
	logFont.lfHeight = fontheight;
	COLORREF fontColor = GetPlotColor(GetDataID(i));
	
	HRGN hRgnPie = GetPieRegion( i, hDC, plotRect, ctx, cty, a1, b1, curr, ratio );
	HRGN hRgnTitle = CreateRectRgn( rect.left, rect.top, rect.right, rect.bottom );
	HRGN hRgnIntersect = hRgnPie;
		
	int rgn_ret = CombineRgn(hRgnIntersect, hRgnPie, hRgnTitle, RGN_AND);

	DeleteObject(hRgnPie);
	DeleteObject(hRgnTitle);
	//DeleteObject(hRgnIntersect);
	if( rgn_ret == COMPLEXREGION || rgn_ret == SIMPLEREGION)
	{
		fontColor = RGB( 255-GetRValue(fontColor), 255-GetGValue(fontColor), 255-GetBValue(fontColor) );
	}
	// determine intersect ended
	
	DrawTextInRect( hDC, rect, GetRealTitle(i), &logFont, TRANSPARENT, fontColor );

	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

}

void	CPiePlotImpl::DrawPies( HDC hDC )
{
	double ctx, cty, a, b, total;

	RECT plotRect = GetLastPlotRect();

	if(!GetFreqPara(plotRect, ctx, cty, a, b, total))
		return;

	DrawPiesBasic(hDC, plotRect, ctx, cty, a, b, total);

	SetPlotColors();
	DrawPieAreas(hDC, plotRect, ctx, cty, a, b, total);
	DrawPieTitles(hDC, plotRect, ctx, cty, a, b, total);

}

void	CPiePlotImpl::DrawPieAreas(HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total )
{
	double curr;
	double ratio;
	int i;
	// Draw top pie
	curr= m_fStartAngle/360.0;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		DrawSinglePie(hDC, plotRect, i, ctx, cty, a, b, total, curr, ratio);
		curr += ratio;
	}

}

void	CPiePlotImpl::DrawPieTitles(HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total )
{
	double curr= m_fStartAngle/360.0;
	double ratio;
	int i;
	// Draw top pie
	if(m_bShowPieTitle || m_bShowPieTitleRatio)
	{
		curr = m_fStartAngle/360.0;
		
		for(i=0; i<GetPlotDataCount(); i++)
		{
			DrawSingleTit(hDC, plotRect, i, ctx, cty, a, b, total, curr, ratio);
			curr += ratio;
		}
	}
}

bool	CPiePlotImpl::GetFreqPara( RECT plotRect, double &ctx, double &cty, double &a, double &b, double &total )
{
	int i;
	//double ratio,total = 0.0;
	total = 0.0;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(GetAbsData(i)[0].val[1] <= 0)return false;
		total += GetAbsData(i)[0].val[1];
	}
	if(total <= 0.0)return false;
	
	double scale = m_fScale;
	if(scale<=0.0 || scale>1.0)scale=1.0;
	
	double curr = m_fStartAngle/360.0;

	a = scale*abs(plotRect.right - plotRect.left)/2.0;
	b = scale*abs(plotRect.bottom - plotRect.top)/2.0;
	if(m_bCircleMode)
		a = b = min(a,b);
	ctx = (plotRect.right + plotRect.left)/2.0;
	cty = (plotRect.bottom + plotRect.top)/2.0;
	
	if(m_bShadowing || m_b3DStyle)
	{
		ctx += m_n3DHalfLength * cos( m_f3DAngle * myPi /180.0);
		cty -= m_n3DHalfLength * sin( m_f3DAngle * myPi /180.0) * b / a;
	}

	// add offset
	ctx += CPiePlot::m_sizeOffset.cx;
	cty += CPiePlot::m_sizeOffset.cy;
	if(ctx + a > plotRect.right)ctx = plotRect.right - a;
	if(ctx - a < plotRect.left)ctx = plotRect.left + a;
	if(cty + b > plotRect.bottom)cty = plotRect.bottom - b;
	if(cty - b < plotRect.top )cty = plotRect.top + b;
	if(m_bShadowing)
	{
		double ctx1, cty1;
		ctx1 = ctx - 2 * m_n3DHalfLength * cos( m_f3DAngle * myPi /180.0);
		cty1 = cty + 2 * m_n3DHalfLength * sin( m_f3DAngle * myPi /180.0) * b / a;
		if(ctx1 + a > plotRect.right)ctx -= ctx1 + a - plotRect.right;
		if(ctx1 - a < plotRect.left)ctx += plotRect.left -ctx1 + a;
		if(cty1 + b > plotRect.bottom)cty -= cty1 + b - plotRect.bottom;
		if(cty1 - b < plotRect.top )cty += plotRect.top -cty1 + b;
	}

	return true;
}

bool	CPiePlotImpl::GetFreqPara( RECT plotRect, int which, double &ctx, double &cty, double &a, double &b, double &a1, double &b1, double &curr, double &ratio, double &total )
{
	if(which < 0 || which >= GetPlotDataCount())return false;
	if(!GetFreqPara(plotRect, ctx, cty, a, b, total))return false;
	int i;
	curr = m_fStartAngle/360.0;
	for( i=0; i<which; i++)
	{
		ratio = GetAbsData(i)[0].val[1]/total;
		if(m_bClockWise)ratio = -ratio;
		curr += ratio;
	}

	ratio = GetAbsData(i)[0].val[1]/total;
	if(m_bClockWise)ratio = -ratio;

	double s1 = GetPieScale(which);
	if(s1<=0.0 || s1>1.0)s1=1.0;
	a1 = a * s1;
	b1 = b * s1;

	return true;
}

RECT	CPiePlotImpl::GetPieTitleRect( HDC hDC, RECT plotRect, int which, double ctx, double cty, double a1, double b1, double curr, double ratio, int &fontheight )
{
	RECT rect={0,0,0,0};
	if( which < 0 || which >= GetPlotDataCount() )
		return rect;

	POINT ptStart;
	double dx,dy;
	dx = a1*cos((curr+ratio/2.0)*2.0*myPi);;
	dy = b1*sin((curr+ratio/2.0)*2.0*myPi);
	ptStart.x = long(ctx + dx);
	ptStart.y = long(cty - dy);

//	HFONT		hOldFont, hFont;
	LOGFONT		logFont = m_lfPieTitleFont;

	SIZE	tmSize = {0,0};
	bool	keepGoing = true;
	if( GetRealTitle(which).length()>0 )
	{
		// if the rect is too large, change the font size
		while( keepGoing == true )
		{
			MyGetTextExtentPoint( hDC, &logFont, GetRealTitle(which), DT_LEFT, &tmSize );
			if( (tmSize.cx > abs(plotRect.right - plotRect.left ) || tmSize.cy > abs(plotRect.bottom - plotRect.top)) && logFont.lfHeight > 4 )
			{
				logFont.lfHeight--;
			}
			else
			{
				keepGoing = false;
			}
		}

		// the initial position
		if( dx > 0.0 && dy > 0.0 )
		{
			rect.left = ptStart.x;
			rect.right = ptStart.x + tmSize.cx;
			rect.top = ptStart.y - tmSize.cy;
			rect.bottom = ptStart.y;
		}
		else if( dx <= 0.0 && dy > 0.0 )
		{
			rect.left = ptStart.x - tmSize.cx;
			rect.right = ptStart.x;
			rect.top = ptStart.y - tmSize.cy;
			rect.bottom = ptStart.y;
		}
		else if( dx <= 0.0 && dy <= 0.0 )
		{
			rect.left = ptStart.x - tmSize.cx;
			rect.right = ptStart.x;
			rect.top = ptStart.y;
			rect.bottom = ptStart.y + tmSize.cy;
		}
		else
		{
			rect.left = ptStart.x;
			rect.right = ptStart.x + tmSize.cx;
			rect.top = ptStart.y;
			rect.bottom = ptStart.y + tmSize.cy;
		}

		// Adding the offset
		OffsetRect(&rect, GetPieTitleOffset(which).cx, GetPieTitleOffset(which).cy);

		// Adjust the position
		if( rect.left < plotRect.left )
			OffsetRect(&rect, plotRect.left - rect.left, 0);
		if( rect.right > plotRect.right )
			OffsetRect(&rect, plotRect.right - rect.right, 0);
		if( rect.top < plotRect.top )
			OffsetRect(&rect, 0, plotRect.top - rect.top );
		if( rect.bottom > plotRect.bottom )
			OffsetRect(&rect, 0, plotRect.bottom - rect.bottom );
	}

	fontheight = logFont.lfHeight;

	return rect;
}

tstring	CPiePlotImpl::GetRealTitle(int which)
{
	tstring title = _TEXT("");
	if(m_bShowPieTitle)
		title += GetAbsDataTitle(which);
	if(m_bShowPieTitleRatio)
	{
		double ratio = GetRatio(which);
		tostringstream ostr;
		ostr.precision(m_nPieTitleRatioPrecision);
		ostr<<_TEXT(" ")<<fabs(ratio)*100<<_TEXT("%");
		title += ostr.str();
	}
	return title;
}

double	CPiePlotImpl::GetRatio(int which)
{
	double total = 0.0;
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(GetAbsData(i)[0].val[1] <= 0)return -1.0;
		total += GetAbsData(i)[0].val[1];
	}
	if(total <= 0.0)return -1.0;

	//double ratio = ((PieDataInfo*)m_vpXYDataInfo[i])->pDataSet->GetData()[0].val[1]/total;
	return GetAbsData(which)[0].val[1]/total;
}

RECT	CPiePlotImpl::GetPieTitleRect( HDC hDC, RECT plotRect, int which )
{
	RECT rect={0,0,0,0};
	if( which < 0 || which >= GetPlotDataCount() )
		return rect;

	double ctx, cty, a, b, a1, b1, curr, ratio, total;
	if(!GetFreqPara(plotRect, which, ctx, cty, a, b, a1, b1, curr, ratio, total))
		return rect;

	int fontheight;
	return GetPieTitleRect(hDC, plotRect,which, ctx, cty, a1, b1, curr, ratio, fontheight );
}

HRGN	CPiePlotImpl::GetPieRegion( int which, HDC hDC, RECT plotRect, double ctx, double cty, double a1, double b1, double curr, double ratio )
{
	// Pie region
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH),hOldBrush;
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hPen = (HPEN)GetStockObject(NULL_PEN), hOldPen;
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	POINT pt={(long)ctx, (long)cty};
	DPtoLP(hDC, &pt, 1);
	ctx = pt.x;
	cty = pt.y;
		
	BeginPath(hDC);
	if(ratio > 0.0)
	{
		Pie( hDC, int(ctx-a1), int(cty-b1), int(ctx+a1), int(cty+b1),
			int(ctx+a1*cos(curr*2.0*myPi)), int(cty-b1*sin(curr*2.0*myPi)),
			int(ctx+a1*cos((curr+ratio)*2.0*myPi)),int( cty-b1*sin((curr+ratio)*2.0*myPi)));
	}
	else
	{
		Pie( hDC, int(ctx-a1), int(cty-b1), int(ctx+a1), int(cty+b1),
			int(ctx+a1*cos((curr+ratio)*2.0*myPi)), int(cty-b1*sin((curr+ratio)*2.0*myPi)),
			int(ctx+a1*cos(curr*2.0*myPi)), int(cty-b1*sin(curr*2.0*myPi) ));
	}
	EndPath(hDC);
	
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	
	HRGN hRgn = PathToRegion(hDC);

	return hRgn;
}

bool	CPiePlotImpl::PtInPieArea( HDC hDC, POINT point, RECT plotRect )
{
	double scale = m_fScale;
	if(scale<=0.0 || scale>1.0)scale=1.0;

	double a,b,ctx,cty;
	a = scale*abs(plotRect.right - plotRect.left)/2.0;
	b = scale*abs(plotRect.bottom - plotRect.top)/2.0;
	if(m_bCircleMode)
		a = b = min(a,b);
	ctx = (plotRect.right + plotRect.left)/2.0;
	cty = (plotRect.bottom + plotRect.top)/2.0;

	if(m_bShadowing)
	{
		ctx += m_n3DHalfLength * cos( m_f3DAngle * myPi /180.0);
		cty -= m_n3DHalfLength * sin( m_f3DAngle * myPi /180.0) * b / a;
	}

	HRGN hRgn = CreateEllipticRgn(int(ctx-a), int(cty-b), int(ctx+a), int(cty+b));
	if( PtInRegion(hRgn, point.x, point.y) )
	{
		DeleteObject(hRgn);
		return true;
	}

	DeleteObject(hRgn);

	return false;
}

bool	CPiePlotImpl::PtInPieArea( int which, HDC hDC, POINT point, RECT plotRect, bool withTitle )
{
	double ctx, cty, a, b, a1, b1, curr, ratio, total;
	GetFreqPara(plotRect, which, ctx, cty, a, b, a1, b1, curr, ratio, total);

	if(PtInPieArea(which, hDC, point, plotRect, withTitle, ctx, cty, a1, b1, curr, ratio ))
		return true;
	return false;
}

bool	CPiePlotImpl::PtInPieArea( int which, HDC hDC, POINT point, RECT plotRect, bool withTitle, double ctx, double cty, double a1, double b1, double curr, double ratio )
{
	// Pie region
	HRGN hRgn = GetPieRegion( which, hDC, plotRect, ctx, cty, a1, b1, curr, ratio );
	
	if( PtInRegion(hRgn, point.x, point.y) )
	{
		DeleteObject(hRgn);
		return true;
	}

	DeleteObject(hRgn);

	// Title region
	int fontheight;
	RECT PieTitleRect = GetPieTitleRect( hDC, plotRect, which, ctx, cty, a1, b1, curr, ratio, fontheight );
	if( PtInRect( & PieTitleRect, point ) )
	{
		return true;
	}
	
	return false;
}

bool	CPiePlotImpl::PtInPieTitleArea(  int which, HDC hDC, POINT point, RECT plotRect )
{
	RECT PieTitleRect = GetPieTitleRect( hDC, plotRect, which );
	if( PtInRect(&PieTitleRect, point) )return true;
	return false;
}

int		CPiePlotImpl::GetNeareastPlotData( HDC hDC, POINT point )
{
	if(GetPlotDataCount()<=0)return -1;

	RECT plotRect = GetLastPlotRect( );

	int			i, nIdx=-1;

	// prepare begin
	double ratio,total = 0.0;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(GetAbsData(i)[0].val[1] <= 0)return false;
		total += GetAbsData(i)[0].val[1];
	}
	if(total <= 0.0)return false;
	
	double scale = m_fScale;
	if(scale<=0.0 || scale>1.0)scale=1.0;
	
	double curr = m_fStartAngle/360.0;
	double a,b,ctx,cty;
	a = scale*abs(plotRect.right - plotRect.left)/2.0;
	b = scale*abs(plotRect.bottom - plotRect.top)/2.0;
	if(m_bCircleMode)
		a = b = min(a,b);
	ctx = (plotRect.right + plotRect.left)/2.0;
	cty = (plotRect.bottom + plotRect.top)/2.0;
	
	if(m_bShadowing)
	{
		ctx += m_n3DHalfLength * cos( m_f3DAngle * myPi /180.0);
		cty -= m_n3DHalfLength * sin( m_f3DAngle * myPi /180.0) * b / a;
	}
	
	double a1,b1,s1;
	// prepare end

	// determine
	for(i=0; i<GetPlotDataCount(); i++)
	{
		ratio = GetAbsData(i)[0].val[1]/total;
		if(m_bClockWise)ratio = -ratio;

		// prepare begin
		s1 = GetPieScale(i);
		if(s1<=0.0 || s1>1.0)s1=1.0;
		a1 = a * s1;
		b1 = b * s1;
		// prepare end

		if( PtInPieArea( i, hDC, point, plotRect, true, ctx, cty, a1, b1, curr, ratio ) )
		{
			nIdx = i;
			break;
		}

		curr += ratio;
	}

	if(nIdx==-1)
		return -1;
	else
		return GetDataID(nIdx);
}

bool	CPiePlotImpl::WriteToFile( tstring pathName )
{
	return CXYPlotImpl::WriteToFile(pathName);
}

bool	CPiePlotImpl::WriteToFile( int which, tstring pathName )
{
	return WriteToFile(pathName);
}

bool	CPiePlotImpl::WriteToBuff( tstring &buff )
{
	int i;
	tostringstream ostr;
	tstring str;

	buff = _TEXT("");
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(GetAbsData(i).size()>0)
		{
			ostr.str(_TEXT(""));
			ostr<<i<<GetDataSeparator().c_str()<<GetAbsData(i)[0].val[1];
			str = ostr.str();
			buff += str;
		}
	}
	return true;
}

bool	CPiePlotImpl::ReadFromBuff( tstring buff )
{
	return true;
}

int		CPiePlotImpl::AddData( MyVData2D vData )
{
	int dataID = CXYPlotImpl::AddData(vData);
	if(dataID>=0)
	{
		CPlotData<DataPoint2D> *plotdata = GetPlotData(dataID);
		if(!plotdata)return -1;
		CPieProp *pPP = new CPieProp;
		plotdata->SetExtProp(pPP);
	}
	return dataID;
}


CPiePlotImplI::CPiePlotImplI()
{
	
}

CPiePlotImplI::~CPiePlotImplI()
{
	
}
/*
bool	CPiePlotImplI::ShowDlgBySelection(HDC hDC)
{
	bool needUpdate = false;
	if(IsSelectTitle())
	{
		if(DefPlotSettings(this, hDC))
		{
			needUpdate = true;
		}
	}
	else if(IsAxisSelected())
	{
		int index, location;
		index = GetIndexOfSelectedAxis();
		location = GetAxis(index)->GetLocation();
		if(DefAxisSettings(this,hDC,location))
		{
			needUpdate = true;
		}
	}
	else
	{
		if(DefPlotSettings(this,hDC))
		{
			needUpdate = true;
		}
	}
	
	return needUpdate;
}
*/