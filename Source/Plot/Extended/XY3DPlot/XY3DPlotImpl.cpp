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

#pragma warning(disable:4786)
#include "../../Basic/MyGdiPlus.h"
#include "../../Basic/PlotData.h"
#include "XY3DPlotImpl.h"
#include "../../Basic/MyConst.h"

using namespace NsCChart;

CXY3DPlotImpl::CXY3DPlotImpl()
{
	m_pAxisZ	=	new CAxisImpl(this);
	m_pAxisZ->SetTickLabelSEOnly(true);
	m_pAxisZ->SetShowMinorTicks(false);

	SetXRange(0.0, 1.0);
	SetYRange(0.0, 1.0);
}

CXY3DPlotImpl::~CXY3DPlotImpl()
{
	delete	m_pAxisZ;
	DeleteAllData();
}

int			CXY3DPlotImpl::GetZTickCount()
{
	return m_nMajorTicksZ;
}
void		CXY3DPlotImpl::SetZTickCount(int count)
{
	m_nMajorTicksZ = count;
}
int			CXY3DPlotImpl::GetZMinorTickCount()
{
	return m_nMinorTicksZ;
}
void		CXY3DPlotImpl::SetZMinorTickCount(int count)
{
	m_nMinorTicksZ = count;
}


void		CXY3DPlotImpl::DrawPlotDataEach( HDC hDC, double *xRange, double *yRange )
{
	RECT plotRect = GetLastPlotRect();
	RECT plotRectL = GetPlotRectL(plotRect);

	double zRange[2];
	GetPlotZRange(zRange);

	vector<DataPoint2D> vData;

	SetLastPlotRect(plotRectL);

	int i;
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		// modify data
		Get2DData(i, vData, plotRectL, xRange, yRange, zRange);
		// draw data
		DrawSinglePlotData(i, hDC, xRange, yRange);
		// restore data
		Reset2DData(i, vData);
	}
	SetLastPlotRect(plotRect);
}

void		CXY3DPlotImpl::DrawFillArea(  int which, HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	if(!GetAbsPlotData(which)->IsVisible())return;
	if(!GetAbsPlotData(which)->IsDataFill())return;

	if(GetAbsPlotData(which)->GetDataFillMode() == kDataFillClosed)
		GetAbsPlotData(which)->DrawFillArea(hDC, plotRect, xRange, yRange);

	HBRUSH		hBrush, hOldBrush;
	HPEN		hPen, hOldPen;

	int			nDatas;
	bool		bXLog, bYLog;

	CPlotData<DataPoint2D>	*dataSet = GetAbsPlotData( which );
	if( dataSet->GetDataCount()<=0 ) return;

	double xScale, yScale;
	POINT *myPoints;
	int nPts4Fill;
	xScale = GetAbsPlotData(which)->xScale;
	yScale = GetAbsPlotData(which)->yScale;
	myPoints = GetAbsPlotData(which)->pPointsGdi;
	nPts4Fill = GetAbsPlotData(which)->nPts4Fill;

	nDatas = dataSet->GetDataCount();
	
	bXLog = IsXLogarithm();
	bYLog = IsYLogarithm();
	if(bXLog || bYLog)return;

	int plottype = GetAbsPlotData(which)->GetPlotType();

	double z, zx1, zy1, zx2, zy2;
	double x1, y1, x2, y2;
	int i;
	POINT	PtsForSeg[4];
	double scaleZ;
	double zRange[2];
	GetPlotZRange(zRange);
	scaleZ = m_fLenZAxis / (zRange[1] - zRange[0]);

	switch(GetAbsPlotData(which)->GetDataFillBrushType())
	{
	case kBrushSolid:
		hBrush = CreateSolidBrush( GetAbsPlotData(which)->GetDataFillColor() );
		break;
	case kBrushHatch:
		hBrush = CreateHatchBrush(GetAbsPlotData(which)->GetDataFillHatchStyle(),GetAbsPlotData(which)->GetDataFillColor());
		break;
	case kBrushPattern:
		if(GetAbsPlotData(which)->GetDataFillPattern())
			hBrush = CreatePatternBrush(GetAbsPlotData(which)->GetDataFillPattern());
		else
			hBrush = CreateSolidBrush( GetAbsPlotData(which)->GetDataFillColor() );
		break;
	default:
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}
	hOldBrush = (HBRUSH)SelectObject( hDC,hBrush );
	hPen = (HPEN)GetStockObject(NULL_PEN);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
		
	for(i=0; i<nDatas-1; i++)
	{
		PtsForSeg[0] = myPoints[i];
		PtsForSeg[1] = myPoints[i+1];

		if(i+1 >= (int)((XY3DProp*)GetAbsPlotData(which)->GetExtProp())->vfZData.size())break;
		z = (((XY3DProp*)GetAbsPlotData(which)->GetExtProp())->vfZData[i] - zRange[0])*scaleZ;
		zx1 = z * cos(m_fAngleZAxis*myPi/180.0);
		zy1 = z * sin(m_fAngleZAxis*myPi/180.0);
		x1 = myPoints[i].x - zx1;///xScale;
		y1 = myPoints[i].y + zy1;///yScale;
		z = (((XY3DProp*)GetAbsPlotData(which)->GetExtProp())->vfZData[i+1] - zRange[0])*scaleZ;
		zx2 = z * cos(m_fAngleZAxis*myPi/180.0);
		zy2 = z * sin(m_fAngleZAxis*myPi/180.0);
		x2 = myPoints[i+1].x - zx2;///xScale;
		y2 = myPoints[i+1].y + zy2;///yScale;
		switch(GetAbsPlotData(which)->GetDataFillMode())
		{
		case kDataFillFromBottomAxis:
			PtsForSeg[2].x = long(x2 + zx2);
			PtsForSeg[2].y = long(plotRect.bottom - zy2);
			PtsForSeg[3].x = long(x1 + zx1);
			PtsForSeg[3].y = long(plotRect.bottom - zy1);
			break;
		case kDataFillFromTopAxis:
			PtsForSeg[2].x = long(x2 + zx2);
			PtsForSeg[2].y = long(plotRect.top - zy2);
			PtsForSeg[3].x = long(x1 + zx1);
			PtsForSeg[3].y = long(plotRect.top - zy1);
			break;
		case kDataFillFromLeftAxis:
			PtsForSeg[2].x = long(plotRect.left + zx2);
			PtsForSeg[2].y = long(y2 - zy2);
			PtsForSeg[3].x = long(plotRect.left + zx1);
			PtsForSeg[3].y = long(y1 - zy1);
			break;
		case kDataFillFromRightAxis:
			PtsForSeg[2].x = long(plotRect.right + zx2);
			PtsForSeg[2].y = long(y2 - zy2);
			PtsForSeg[3].x = long(plotRect.right + zx1);
			PtsForSeg[3].y = long(y1 - zy1);
			break;
		}
		BeginPath(hDC);
		Polyline(hDC, PtsForSeg, 4);
		EndPath( hDC );

		if(GetAbsPlotData(which)->GetDataFillAlpha() == 255)
		{
			// GDI+ not needed
			StrokeAndFillPath(hDC);
		}
		else
		{
			// use GDI+ to get alpha blend effect
			HRGN hRgn = PathToRegion(hDC);
			Region rgn(hRgn);
			
			byte a, r, g, b;
			a = GetAbsPlotData(which)->GetDataFillAlpha();
			r = GetRValue(GetAbsPlotData(which)->GetDataFillColor());
			g = GetGValue(GetAbsPlotData(which)->GetDataFillColor());
			b = GetBValue(GetAbsPlotData(which)->GetDataFillColor());
			Color fillColor(a, r, g, b);
			SolidBrush brush(fillColor);
			
			Graphics graph(hDC);
			graph.FillRegion(&brush, &rgn);
			
			DeleteObject(hRgn);
		}
	}
		
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void		CXY3DPlotImpl::DrawAxes( HDC hDC )//, RECT clientRect, RECT plotRect )
{
	RECT plotRect = GetLastPlotRect();
	RECT plotRectL = GetPlotRectL(plotRect);
	RECT plotRectU = GetPlotRectU(plotRect);

	CAxisImpl *axis;
	// Draw the axes
	for( int i = 0; i < GetAxesCount(); i++ )
	{
		axis = GetAxis(i);
		if(axis->IsVisible())
		{
			if(axis->GetLocation() == kLocationLeft || axis->GetLocation() == kLocationBottom)
			{
				SetLastPlotRect(plotRectL);
				axis->OnDraw( hDC);
			}
			else if(axis->GetLocation() == kLocationRight || axis->GetLocation() == kLocationTop)
			{
				SetLastPlotRect(plotRectU);
				axis->OnDraw( hDC );
			}
		}
	}
	SetLastPlotRect(plotRect);

	DrawZAxis(hDC, plotRect);
}

void		CXY3DPlotImpl::Get2DData( int which, vector<DataPoint2D> &vData, RECT plotRectL, double *xRange, double *yRange, double *zRange )
{
	if(which <0 || which >= GetPlotDataCount())return;
	CPlotData<DataPoint2D> *pDataSet = GetAbsPlotData(which);

	double scaleX, scaleY, scaleZ;
	
	scaleX = (plotRectL.right - plotRectL.left) / (xRange[1] - xRange[0]);
	scaleY = (plotRectL.bottom - plotRectL.top) / (yRange[1] - yRange[0]);
	scaleZ = m_fLenZAxis / (zRange[1] - zRange[0]);

	double z, zx, zy;

	vData = pDataSet->GetData();
	for( int j = 0; j < (int)pDataSet->GetDataCount(); j++)
	{
		if(j >= (int)((XY3DProp*)GetAbsPlotData(which)->GetExtProp())->vfZData.size())break;
		z = (((XY3DProp*)GetAbsPlotData(which)->GetExtProp())->vfZData[j] - zRange[0])*scaleZ;
		zx = z * cos(m_fAngleZAxis*myPi/180.0);
		zy = z * sin(m_fAngleZAxis*myPi/180.0);
		pDataSet->GetData()[j].val[0] += zx/scaleX;
		pDataSet->GetData()[j].val[1] += zy/scaleY;
	}
}

void		CXY3DPlotImpl::Reset2DData( int which, vector<DataPoint2D> &vData )
{
	if(which <0 || which >= GetPlotDataCount())return;

	CPlotData<DataPoint2D> *pDataSet = GetAbsPlotData(which);

	pDataSet->GetData() = vData;
}

void		CXY3DPlotImpl::DrawZAxis( HDC hDC, RECT plotRect )
{
	DrawZAxisLine(hDC, plotRect);
	DrawZAxisMajorTicks(hDC, plotRect);
	DrawZAxisMinorTicks(hDC, plotRect);
	DrawZAxisTickLabels(hDC, plotRect);
}

void		CXY3DPlotImpl::DrawZAxisLine( HDC hDC, RECT plotRect )
{
	CAxisImpl *axis = m_pAxisZ;
		
	HPEN		hPen,hOldPen;
	
	hPen = CreatePen( axis->GetLineStyle(), axis->GetLineSize(), axis->GetAxisColor() );
	hOldPen = (HPEN)SelectObject(hDC,hPen);

	double dx, dy;
	GetZAxisDist(dx, dy);
	
	MoveToEx(hDC, int(plotRect.right - dx), plotRect.bottom, NULL);
	LineTo(hDC, plotRect.right, int(plotRect.bottom - dy));
	
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}

void		CXY3DPlotImpl::DrawZAxisMajorTicks( HDC hDC, RECT plotRect )
{
	CAxisImpl *axis = m_pAxisZ;

	if(!axis->IsShowMajorTicks())return;

	HPEN		hPen,hOldPen;
	int			tickMarkLength;
	POINT		tickPoint;

	COLORREF	tickcolor;
	if(axis->IsSameColor())
	{
		tickcolor = axis->GetAxisColor();
	}
	else
	{
		tickcolor = axis->GetTickColor();
	}

	tickMarkLength = axis->GetTickLength();

	hPen = CreatePen( PS_SOLID, axis->GetTickSize(), tickcolor );
	hOldPen = (HPEN)SelectObject(hDC,hPen);

	double dx, dy;
	GetZAxisDist(dx, dy);

	double ddx, ddy;
	ddx = dx / (GetZTickCount() + 1);
	ddy = dy / (GetZTickCount() + 1);

	int			i;
	for( i = 0; i <= GetZTickCount()+1; i++ )
	{
		switch(axis->GetTickPosition())
		{
		case kTickPositionOuter:
			tickPoint.x = long(plotRect.right - dx + i*ddx);
			tickPoint.y = long(plotRect.bottom - i* ddy);
			MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL);
			tickPoint.y -= tickMarkLength;
			LineTo(hDC, tickPoint.x, tickPoint.y);
			break;
		case kTickPositionInner:
			tickPoint.x = long(plotRect.right - dx + i*ddx);
			tickPoint.y = long(plotRect.bottom - i* ddy);
			MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL);
			tickPoint.y += tickMarkLength;
			LineTo(hDC, tickPoint.x, tickPoint.y);
			break;
		case kTickPositionBoth:
			tickPoint.x = long(plotRect.right - dx + i*ddx);
			tickPoint.y = long(plotRect.bottom - i* ddy - tickMarkLength/2.0);
			MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL);
			tickPoint.y += tickMarkLength;
			LineTo(hDC, tickPoint.x, tickPoint.y);
			break;
		default:
			break;
		}
	}
		
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}

void		CXY3DPlotImpl::DrawZAxisMinorTicks( HDC hDC, RECT plotRect )
{
	CAxisImpl *axis = m_pAxisZ;
	
	if(!axis->IsShowMinorTicks())return;
	
	HPEN		hPen,hOldPen;
	int			i, j;
	int			tickMarkLength;
	POINT		tickPoint;
	
	COLORREF	tickcolor;
	if(axis->IsSameColor())
	{
		tickcolor = axis->GetAxisColor();
	}
	else
	{
		tickcolor = axis->GetMinorTickColor();
	}
	
	tickMarkLength = axis->GetMinorTickLength();
	
	hPen = CreatePen( PS_SOLID, axis->GetMinorTickSize(), tickcolor );
	hOldPen = (HPEN)SelectObject(hDC,hPen);
	
	double dx, dy;
	GetZAxisDist(dx, dy);
	
	double ddx, ddy;
	ddx = dx / (GetZTickCount() + 1);
	ddy = dy / (GetZTickCount() + 1);

	double dddx, dddy;
	dddx = ddx / GetZMinorTickCount();
	dddy = ddy / GetZMinorTickCount();
	
	for( i = 0; i < GetZTickCount()+1; i++ )
	{
		for( j = 1; j < GetZMinorTickCount(); j++)
		{
			switch(axis->GetTickPosition())
			{
			case kTickPositionOuter:
				tickPoint.x = long(plotRect.right - dx + i*ddx + j*dddx);
				tickPoint.y = long(plotRect.bottom - i* ddy - j*dddy);
				MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL);
				tickPoint.y -= tickMarkLength;
				LineTo(hDC, tickPoint.x, tickPoint.y);
				break;
			case kTickPositionInner:
				tickPoint.x = long(plotRect.right - dx + i*ddx + j*dddx);
				tickPoint.y = long(plotRect.bottom - i* ddy - j*dddy);
				MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL);
				tickPoint.y += tickMarkLength;
				LineTo(hDC, tickPoint.x, tickPoint.y);
				break;
			case kTickPositionBoth:
				tickPoint.x = long(plotRect.right - dx + i*ddx + j*dddx);
				tickPoint.y = long(plotRect.bottom - i* ddy - j*dddy - tickMarkLength/2.0);
				MoveToEx(hDC, tickPoint.x, tickPoint.y, NULL);
				tickPoint.y += tickMarkLength;
				LineTo(hDC, tickPoint.x, tickPoint.y);
				break;
			default:
				break;
			}
		}
		
	}
	
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
}

void		CXY3DPlotImpl::DrawZAxisTickLabels( HDC hDC, RECT plotRect )
{
	CAxisImpl *axis = m_pAxisZ;

	if(!axis->IsTickLabelShow())return;
	
	HFONT		hFont,hOldFont;
	LOGFONT		logFont;
	RECT			textRect;
	SIZE			tmSize={0,0};
	int			i;
	POINT		tickPoint;
	int			tickMarkLength;
	tstring		tickLabel;
	SIZE		labelExtent={0,0};
	
	COLORREF	ticklabelcolor;
	if(axis->IsSameColor())
	{
		ticklabelcolor = axis->GetAxisColor();
	}
	else
	{
		ticklabelcolor = axis->GetTickLabelColor();
	}

	
	// now ready
	logFont = axis->GetLabelFont();
	//logFont.lfHeight = axis->GetLabelFont().lfHeight;

	hFont = CreateFontIndirect( &logFont );
	hOldFont = (HFONT)SelectObject(hDC,hFont);
	
	tickMarkLength = axis->GetTickLength();

	double dx, dy;
	GetZAxisDist(dx, dy);
	
	double ddx, ddy;
	ddx = dx / (GetZTickCount() + 1);
	ddy = dy / (GetZTickCount() + 1);
	
	// Setup of ranges, pens
	SetBkMode(hDC, TRANSPARENT );
	SetTextColor(hDC, ticklabelcolor );

	for( i = 0; i <= GetZTickCount()+1; i++ )
	{
		if(axis->IsTickLabelSEOnly())
		{
			if( (i != 0) && (i != GetZTickCount()+1))continue;
		}

		tickLabel = axis->GetMajorTickLabel( i );
		MyGetTextExtentPoint(hDC, &logFont, tickLabel, DT_LEFT, &tmSize);
		
		tickPoint.x = long(plotRect.right - dx + i*ddx - tmSize.cx/2.0);
		tickPoint.y = long(plotRect.bottom - i*ddy - tmSize.cy);// - axis->GetTickLength();
		if(axis->GetTickPosition() == kTickPositionOuter)
			tickPoint.y -= axis->GetTickLength();
		else if(axis->GetTickPosition() == kTickPositionBoth)
			tickPoint.y -= axis->GetTickLength()/2;
		
		textRect.left = tickPoint.x;
		textRect.top = tickPoint.y;
		textRect.right = textRect.left + tmSize.cx;
		textRect.bottom = textRect.top + tmSize.cy;

		DrawText(hDC, tickLabel.c_str(), (int)tickLabel.length(), &textRect, DT_LEFT);
	}

	SelectObject( hDC, hOldFont);
	DeleteObject( hFont );	
}

void		CXY3DPlotImpl::DrawFrame( HDC hDC )//, RECT plotRect )
{
	if( !CCanvas::m_bBorderShow || m_nBorderSize <= 0 )return;

	RECT plotRect = GetLastPlotRect();
	
	COLORREF bordercolor = m_bLightDataRegion?m_crLightColor:CCanvas::m_crBorderColor;
	int bordersize = m_bLightDataRegion?m_nLightLineSize:m_nBorderSize;
	
	HPEN	hPen,hOldPen;
	HBRUSH hBrush,hOldBrush;
	hPen = CreatePen( m_nBorderStyle, bordersize, bordercolor );
	hBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
	
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC,hBrush);

	double dx, dy;
	GetZAxisDist(dx, dy);

	FillRect(hDC, &plotRect, hBrush);
	
	//
	MoveToEx(hDC, int(plotRect.left + dx), int(plotRect.bottom - dy), NULL);
	LineTo(hDC, plotRect.right, int(plotRect.bottom - dy));
	LineTo(hDC, plotRect.right, plotRect.top);
	LineTo(hDC, int(plotRect.left + dx), plotRect.top);
	LineTo(hDC, int(plotRect.left + dx), int(plotRect.bottom - dy));

	MoveToEx(hDC, int(plotRect.left + dx), plotRect.top, NULL);
	LineTo(hDC, plotRect.left, int(plotRect.top + dy));
	LineTo(hDC, plotRect.left, plotRect.bottom);
	LineTo(hDC, int(plotRect.left + dx), int(plotRect.bottom - dy));

	MoveToEx(hDC, plotRect.left, plotRect.bottom, NULL);
	LineTo(hDC, int(plotRect.right - dx), plotRect.bottom);
	LineTo(hDC, plotRect.right, int(plotRect.bottom - dy));

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hDC, hOldPen );
	DeleteObject(hPen);
}

void		CXY3DPlotImpl::DrawGrids( HDC hDC )//, RECT plotRect )
{
	RECT plotRect = GetLastPlotRect();
	DrawXYGrids(hDC, plotRect);
	DrawYZGrids(hDC, plotRect);
	DrawZXGrids(hDC, plotRect);
}

void		CXY3DPlotImpl::DrawXYGrids( HDC hDC, RECT plotRect )
{
	plotRect = GetPlotRectU(plotRect);
	RECT plotRect0 = GetLastPlotRect();
	SetLastPlotRect(plotRect);
	CXYPlotImpl::DrawGrids(hDC);
	SetLastPlotRect(plotRect0);
}

void		CXY3DPlotImpl::DrawYZGrids( HDC hDC, RECT plotRect )
{
	double dx, dy, ddx, ddy, dddx, dddy;
	GetZAxisDist(dx, dy);

	int i, j;
	CAxisImpl	*axis;
	
	RECT rectL = GetPlotRectL(plotRect);

	HPEN	hPen, hMinorPen, hOldPen;
	hPen = CreatePen( m_nMajorGridLineStyle, m_nMajorGridLineSize, m_crMajorGridColor );
	hMinorPen = CreatePen( m_nMinorGridLineStyle, m_nMinorGridLineSize, m_crMinorGridColor );
	
	double dMajor, dMinor;

	//Draw YZ Major Horizontal Grids
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	if(m_bYZMajorHGrids)
	{
		//axis = GetYMainAxis();
		//if(!axis)goto YZGridsReturn;
		//dMajor = (rectL.bottom - rectL.top)/(axis->GetTickCount() + 1.0);
		dMajor = (rectL.bottom - rectL.top)/(GetYTickCount() + 1.0);
		//for(i = 1; i<= axis->GetTickCount(); i++)
		for(i = 1; i<= GetYTickCount(); i++)
		{
			MoveToEx(hDC, rectL.left, int(rectL.bottom - i*dMajor), NULL);
			LineTo(hDC, int(rectL.left + dx), int(rectL.bottom - i*dMajor - dy));
		}
	}
	if(m_bYZMajorVGrids)
	{
		axis = m_pAxisZ;
		ddx = dx / (GetZTickCount() + 1);
		ddy = dy / (GetZTickCount() + 1);
		for(i = 1; i<= GetZTickCount(); i++)
		{
			MoveToEx(hDC, int(rectL.left + i*ddx), int(rectL.bottom - i*ddy), NULL);
			LineTo(hDC, int(rectL.left + i*ddx), int(rectL.top - i*ddy));
		}
	}

	SelectObject(hDC, hMinorPen);
	if(m_bYZMinorHGrids)
	{
		//axis = GetYMainAxis();
		//if(!axis)goto YZGridsReturn;
		//dMajor = (rectL.bottom - rectL.top)/(axis->GetTickCount() + 1.0);
		//dMinor = dMajor / axis->GetMinorTickCount();
		dMajor = (rectL.bottom - rectL.top)/(GetYTickCount() + 1.0);
		dMinor = dMajor / GetYMinorTickCount();
		for(i = 0; i<= GetZTickCount(); i++)
		{
			for(j = 1; j<GetZMinorTickCount(); j++)
			{
				MoveToEx(hDC, rectL.left, int(rectL.bottom - i*dMajor - j*dMinor), NULL);
				LineTo(hDC, int(rectL.left + dx), int(rectL.bottom - i*dMajor - j*dMinor - dy));
			}
		}
	}
	if(m_bYZMinorVGrids)
	{
		axis = m_pAxisZ;
		ddx = dx / (GetZTickCount() + 1);
		ddy = dy / (GetZTickCount() + 1);
		dddx = ddx / GetZMinorTickCount();
		dddy = ddy / GetZMinorTickCount();
		for(i = 0; i<= GetZTickCount(); i++)
		{
			for(j = 1; j<GetZMinorTickCount(); j++)
			{
				MoveToEx(hDC, int(rectL.left + i*ddx + j*dddx), int(rectL.bottom - i*ddy - j*dddy), NULL);
				LineTo(hDC, int(rectL.left + i*ddx + j*dddx), int(rectL.top - i*ddy - j*dddy));
			}
		}
	}

//YZGridsReturn:
	SelectObject(hDC, hOldPen);
	DeleteObject(hMinorPen);
	DeleteObject(hPen);
}

void		CXY3DPlotImpl::DrawZXGrids( HDC hDC, RECT plotRect )
{
	double dx, dy, ddx, ddy, dddx, dddy;
	GetZAxisDist(dx, dy);
	
	int i, j;
	CAxisImpl	*axis;
	
	RECT rectL = GetPlotRectL(plotRect);
	
	HPEN	hPen, hMinorPen, hOldPen;
	hPen = CreatePen( m_nMajorGridLineStyle, m_nMajorGridLineSize, m_crMajorGridColor );
	hMinorPen = CreatePen( m_nMinorGridLineStyle, m_nMinorGridLineSize, m_crMinorGridColor );
	
	double dMajor, dMinor;
	//Draw YZ Major Horizontal Grids
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	if(m_bZXMajorHGrids)
	{
		axis = m_pAxisZ;
		ddx = dx / (GetZTickCount() + 1);
		ddy = dy / (GetZTickCount() + 1);
		for(i = 1; i<= GetZTickCount(); i++)
		{
			MoveToEx(hDC, int(rectL.left + i*ddx), int(rectL.bottom - i*ddy), NULL);
			LineTo(hDC, int(rectL.right + i*ddx), int(rectL.bottom - i*ddy));
		}
	}
	if(m_bZXMajorVGrids)
	{
		//axis = GetXMainAxis();
		//if(!axis)goto ZXGridsReturn;
		//dMajor = (rectL.right - rectL.left)/(axis->GetTickCount() + 1.0);
		dMajor = (rectL.right - rectL.left)/(GetZTickCount() + 1.0);
		for(i = 1; i<= GetZTickCount(); i++)
		{
			MoveToEx(hDC, int(rectL.left + i*dMajor), rectL.bottom, NULL);
			LineTo(hDC, int(rectL.left + i*dMajor + dx), int(rectL.bottom - dy));
		}
	}
	SelectObject(hDC, hMinorPen);
	if(m_bZXMinorHGrids)
	{
		axis = m_pAxisZ;
		ddx = dx / (GetZTickCount() + 1);
		ddy = dy / (GetZTickCount() + 1);
		dddx = ddx / GetZMinorTickCount();
		dddy = ddy / GetZMinorTickCount();
		for(i = 0; i<= GetZTickCount(); i++)
		{
			for(j = 1; j<GetZMinorTickCount(); j++)
			{
				MoveToEx(hDC, int(rectL.left + i*ddx + j*dddx), int(rectL.bottom - i*ddy - j*dddy), NULL);
				LineTo(hDC, int(rectL.right + i*ddx + j*dddx), int(rectL.bottom - i*ddy - j*dddy));
			}
		}
		
	}
	if(m_bZXMinorVGrids)
	{
		//axis = GetXMainAxis();
		//if(!axis)goto ZXGridsReturn;
		//dMajor = (rectL.right - rectL.left)/(axis->GetTickCount() + 1.0);
		//dMinor = dMajor / axis->GetMinorTickCount();
		dMajor = (rectL.right - rectL.left)/(GetZTickCount() + 1.0);
		dMinor = dMajor / GetZMinorTickCount();
		for(i = 0; i<= GetZTickCount(); i++)
		{
			for(j = 1; j<GetZMinorTickCount(); j++)
			{
				MoveToEx(hDC, int(rectL.left + i*dMajor + j*dMinor), rectL.bottom, NULL);
				LineTo(hDC, int(rectL.left + i*dMajor + j*dMinor + dx), int(rectL.bottom - dy));
			}
		}
	}

//ZXGridsReturn:
	SelectObject(hDC, hOldPen);
	DeleteObject(hMinorPen);
	DeleteObject(hPen);
}

void		CXY3DPlotImpl::GetZAxisDist(double &dx, double &dy)
{
	dx = m_fLenZAxis * cos(m_fAngleZAxis * myPi / 180.0);
	dy = m_fLenZAxis * sin(m_fAngleZAxis * myPi / 180.0);
}

RECT		CXY3DPlotImpl::AdjustPlotSizeForZAxis( HDC hDC, RECT plotRect )
{
	CAxisImpl *axis = m_pAxisZ;

	int		i;
	SIZE	tmSize;
	tstring	tickLabel;

	double	dx, dy;
	GetZAxisDist(dx, dy);
	double	ddx, ddy;
	ddx = dx / (GetZTickCount() + 1);
	ddy = dy / (GetZTickCount() + 1);

	double xpadding = 0.0;
	double rightmost;

	for( i = 0; i <= GetZTickCount()+1; i++ )
	{
		if(axis->IsTickLabelSEOnly())
		{
			if( (i != 0) && (i != GetZTickCount()+1))continue;
		}
		
		tickLabel = axis->GetMajorTickLabel( i );
		MyGetTextExtentPoint(hDC, &axis->GetLabelFont(), tickLabel, DT_LEFT, &tmSize);

		//rightmost = -dx + i*ddx + tmSize.cx/2.0;
		rightmost = -dx + i*ddx + tmSize.cx;
		if( rightmost >= 0.0 )
		{
			if(rightmost > xpadding)xpadding = rightmost;
		}
	}

	plotRect.right -= (int)xpadding;

	return plotRect;
}

RECT		CXY3DPlotImpl::GetPlotRect( HDC hDC, RECT clientRect )
{
	RECT plotRect = CXYPlotImpl::GetPlotRect(hDC, clientRect);
	plotRect = AdjustPlotSizeForZAxis(hDC, plotRect);
	return plotRect;
}

RECT		CXY3DPlotImpl::GetPlotRectL( HDC hDC, RECT clientRect )
{
	RECT plotRect = GetPlotRect(hDC, clientRect);

	return GetPlotRectL(plotRect);
}

RECT		CXY3DPlotImpl::GetPlotRectL( RECT plotRect )
{
	double dx, dy;
	GetZAxisDist(dx, dy);
	
	plotRect.right -= (int)dx;
	plotRect.top += (int)dy;
	
	return plotRect;
}

RECT		CXY3DPlotImpl::GetPlotRectU( HDC hDC, RECT clientRect )
{
	RECT plotRect = GetPlotRect(hDC, clientRect);
	
	return GetPlotRectU(plotRect);
}

RECT		CXY3DPlotImpl::GetPlotRectU( RECT plotRect )
{
	double dx, dy;
	GetZAxisDist(dx, dy);
	
	plotRect.left += (int)dx;
	plotRect.bottom -= (int)dy;
	
	return plotRect;
}

void	CXY3DPlotImpl::GetZDataRange( int dataID )
{
	int			i, j;
	double		minZ, maxZ;
	double		lb = 0.0, ub = 0.0;
	
	i = GetIndex( dataID );
	if( i < 0 ) return;

	CPlotData<DataPoint2D> *plotdata = GetAbsPlotData(i);

	minZ = fMaxVal;
	maxZ = - fMaxVal;

	for(j=0; j<(int)((XY3DProp*)plotdata->GetExtProp())->vfZData.size(); j++)
	{
		if(((XY3DProp*)plotdata->GetExtProp())->vfZData[j] < minZ)minZ = ((XY3DProp*)plotdata->GetExtProp())->vfZData[j];
		if(((XY3DProp*)plotdata->GetExtProp())->vfZData[j] > maxZ)maxZ = ((XY3DProp*)plotdata->GetExtProp())->vfZData[j];
	}

	((XY3DProp*)plotdata->GetExtProp())->pfZDataRange[0] = minZ;
	((XY3DProp*)plotdata->GetExtProp())->pfZDataRange[1] = maxZ;
}

void	CXY3DPlotImpl::GetZDataRanges( void )
{
	for( int i = 0; i < GetPlotDataCount(); i++ )
	{
		GetZDataRange( GetDataID(i) );
	}
}

void	CXY3DPlotImpl::GetZDataRanges(double zRange[2])
{
	GetZDataRanges();
	zRange[0]=fMaxVal;
	zRange[1]=-fMaxVal;

	CPlotData<DataPoint2D> *plotdata;
	for( int i = 0; i < GetPlotDataCount(); i++ )
	{
		plotdata = GetAbsPlotData(i);
		if(((XY3DProp*)plotdata->GetExtProp())->pfZDataRange[0] < zRange[0])zRange[0] = ((XY3DProp*)plotdata->GetExtProp())->pfZDataRange[0];
		if(((XY3DProp*)plotdata->GetExtProp())->pfZDataRange[1] > zRange[1])zRange[1] = ((XY3DProp*)plotdata->GetExtProp())->pfZDataRange[1];
	}
}

void	CXY3DPlotImpl::GetPlotRange( double *xRange, double *yRange )// double *zRange )
{
	CXYPlotImpl::GetPlotRange(xRange, yRange);

	double zRange[2];
	GetPlotZRange(zRange);
}

void	CXY3DPlotImpl::GetPlotZRange( double *zRange )
{
	if(GetPlotDataCount() <= 0)
	{
		zRange[0] = 0.0;
		zRange[1] = 1.0;
		//m_pAxisZ->SetRangeAndCalc(0.0, 1.0);
		return;
	}
	
	GetZDataRanges(zRange);
	
	double optRange[2];
	//int nTicks, nMinorTicks;
	GetOptimalRange(zRange, optRange, &m_nMajorTicksZ, &m_nMinorTicksZ, true, true, true, true, false, false);
	memcpy(zRange, optRange, 2*sizeof(double));
	memcpy(m_pfZRange, optRange, 2*sizeof(double));

	//m_pAxisZ->SetRange(false, zRange[0], zRange[1]);
	//SetZTickCount(m_nMajorTicksZ);
	//SetZMinorTickCount(m_nMinorTicksZ);
}

int		CXY3DPlotImpl::GetNeareastPlotData( HDC hDC, POINT point )
{
	if(GetPlotDataCount() <= 0)return -1;

	vector<vector<DataPoint2D> > vvData;
	vvData.resize(GetPlotDataCount());

	RECT plotRect = GetLastPlotRect();
	RECT plotRectL = GetPlotRectL(hDC, GetLastClientRect());
	double xRange[2], yRange[2], zRange[2];
	GetPlotRange(xRange, yRange );//, zRange);
	GetPlotZRange(zRange);
	SetLastPlotRect(plotRectL);
	int i;
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		Get2DData(i, vvData[i], plotRectL, xRange, yRange, zRange);
	}
	int dataID = CXYPlotImpl::GetNeareastPlotData(hDC, point);
	
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		Reset2DData(i, vvData[i]);
	}
	SetLastPlotRect(plotRect);
	return dataID;
}

int		CXY3DPlotImpl::GetNeareastDataPoint(int dataID, HDC hDC, POINT point, double data[2])
{
	if(GetPlotDataCount() <= 0)return -1;
	
	vector<vector<DataPoint2D> > vvData;
	vvData.resize(GetPlotDataCount());

	RECT rt;
	rt = GetLastPlotRect();
	
	RECT plotRect = GetLastPlotRect();
	RECT plotRectL = GetPlotRectL(hDC, GetLastClientRect());
	double xRange[2], yRange[2], zRange[2];
	GetPlotRange(xRange, yRange);//, zRange);
	GetPlotZRange(zRange);
	SetLastPlotRect(plotRectL);
	int i;
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		Get2DData(i, vvData[i], plotRectL, xRange, yRange, zRange);
	}
	rt = GetLastPlotRect();
	int index = CXYPlotImpl::GetNeareastDataPoint(dataID, hDC, point, data);
	
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		Reset2DData(i, vvData[i]);
	}
	SetLastPlotRect(plotRect);
	rt = GetLastPlotRect();
	return index;
}

int		CXY3DPlotImpl::AddCurve( double *pX, double *pY, double *pZ, int nLen )
{
	if(!pX || !pY || !pZ || nLen<=0)return -1;
	MyVData2D vData;
	DataPoint2D dp;

	int i, dataID;
	for(i=0;i<nLen;i++)
	{
		dp.val[0]=pX[i];
		dp.val[1]=pY[i];
		vData.push_back(dp);
	}
	dataID = AddData(vData);

	if(dataID<0)return -1;

	CPlotData<DataPoint2D> *plotdata = GetPlotData(dataID);
	XY3DProp *pPP = new XY3DProp;
	for(i=0; i<nLen; i++)
	{
		pPP->vfZData.push_back(pZ[i]);
	}
	plotdata->SetExtProp(pPP);
	GetZDataRange(dataID);

	m_bNewDataComming = true;
	
	return dataID;
}

int		CXY3DPlotImpl::AddPoint(double x, double y, double z, int nWhich)
{
	if(nWhich < 0 || nWhich > GetPlotDataCount())return -1;
	
	if( nWhich == GetPlotDataCount() )
	{
		double pX[1], pY[1], pZ[1];
		pX[0] = x;
		pY[0] = y;
		pZ[0] = z;
		return AddCurve(pX, pY, pZ, 1);
	}

	int dataID = GetDataID(nWhich);
	
	CXYPlotImpl::AddPoint(x, y, nWhich);
	if(GetAbsData(nWhich).size()<=0)
	{
		XY3DProp *pPP = new XY3DProp;

		GetAbsPlotData(nWhich)->SetExtProp(pPP);
	}
	((XY3DProp*)GetPlotData(dataID)->GetExtProp())->vfZData.push_back(z);

	GetZDataRange(dataID);
	
	m_bNewDataComming = true;
	
	return dataID;

}

bool	CXY3DPlotImpl::WriteToFile( tstring pathName )
{
	return CXYPlotImpl::WriteToFile(pathName);
}

bool	CXY3DPlotImpl::WriteToFile( int which, tstring pathName )
{
	return CXYPlotImpl::WriteToFile(which, pathName);
}

bool	CXY3DPlotImpl::ReadFromFile( tstring pathName )
{
	return CXYPlotImpl::ReadFromFile(pathName);
}

bool	CXY3DPlotImpl::ReadFromFile( int which, tstring pathName )
{
	return CXYPlotImpl::ReadFromFile(which, pathName);
}
