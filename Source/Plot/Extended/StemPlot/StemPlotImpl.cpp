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
#include "StemPlotImpl.h"
#include "../../Basic/PlotUtility.h"
#include "../../Basic/MyConst.h"

using namespace NsCChart;

CStemPlotImpl::CStemPlotImpl()
{
	m_nMaxSize = 1;
	
	SetLegendShow(false);
	SetLegendColorRect(true);
}

CStemPlotImpl::~CStemPlotImpl()
{
}

int	CStemPlotImpl::AddData(vector<double> vData, int nStemBase)
{
	if((int)vData.size() <= 0)
		return -1;
	if(GetPlotDataCount() >= m_nMaxDataSets)
		return -1;

	if(!m_bKeepFormer)DeleteAllData();

	if((int)vData.size() > m_nMaxDataSetLen)
		vData.resize(m_nMaxDataSetLen);

	MyVData2D data;
	DataPoint2D dp;
	for(int i=0; i<(int)vData.size(); i++)
	{
		if(nStemBase == kStemBaseBottom || nStemBase == kStemBaseTop)
		{
			dp.val[0] = i+1;
			dp.val[1] = vData[i];
		}
		else
		{
			dp.val[0] = vData[i];
			dp.val[1] = i+1;
		}
		data.push_back(dp);
	}
	m_nStemBase = nStemBase;
	return CXYPlotImpl::AddCurve(data);
}

int		CStemPlotImpl::AddData(double *pData, int nLen, int nStemBase)
{
	if(nLen<0 || !pData)
		return -1;
	if(GetPlotDataCount() >= m_nMaxDataSets)
		return -1;

	vector<double> vData;
	for(int i=0; i<min(nLen, m_nMaxDataSetLen); i++)
	{
		vData.push_back(pData[i]);
	}

	return AddData(vData, nStemBase);
}

void	CStemPlotImpl::SetXLabel(tstring label, int which)
{
	if(which < 0 || which > (int)m_vstrLabels.size())return;

	m_vstrLabels[which] = label;
}

void	CStemPlotImpl::SetDrawCurve(bool bDraw, int whichData)
{
	if(whichData<0 || whichData>=GetPlotDataCount())return;
	GetAbsPlotData(whichData)->SetFlag(bDraw);
}

void	CStemPlotImpl::GetPlotRange( double *xRange, double *yRange )
{
	CAxisImpl *axis;
	CPlotData<DataPoint2D> *dataSet;
	int i;

	SetOptimalXlExtend(true); SetOptimalYlExtend(true);
	SetOptimalXuExtend(true); SetOptimalYuExtend(true);
	CXYPlotImpl::GetPlotRange(xRange, yRange);
	
	m_nMaxSize = 0;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(GetAbsPlotData(i)->GetDataCount() > m_nMaxSize )
			m_nMaxSize = GetAbsPlotData(i)->GetDataCount();
	}
	if(m_nStemBase == kStemBaseBottom || m_nStemBase == kStemBaseTop)
	{
		xRange[0] = 1.0;
		xRange[1] = m_nMaxSize+1;

		SetXRange(xRange[0], xRange[1]);

		axis = AddAxis(kLocationBottom);

		SetXTickCount(m_nMaxSize-1);
		SetXMinorTickCount(2);
		axis->GetAlternateLabels().resize(m_nMaxSize);

		if(m_bForceYZeroStart)
		{
			axis = AddAxis(kLocationLeft);
			yRange[0] = 0.0;
			SetYRange(yRange[0], yRange[1]);
			//int ntick;
			//ResetTicks(yRange, ntick);
			//SetYTickCount(ntick);
		}

		if(m_bAddUp)
		{
			axis = AddAxis(kLocationLeft);
			yRange[0] = 0.0;
			yRange[1] = 0.0;
			for( i = 0; i < GetPlotDataCount(); i++ )
			{
				dataSet = GetAbsPlotData( i );
				
				if(m_nStemBase == kStemBaseBottom || m_nStemBase == kStemBaseTop)
					yRange[1] += GetAbsPlotData(i)->GetYDataRangeHigh();//m_vpXYDataInfo[i]->fYDataMax;
				else
					yRange[1] += GetAbsPlotData(i)->GetXDataRangeHigh();//m_vpXYDataInfo[i]->fXDataMax;
			}
			yRange[0] = 0.0;
			//axis->SetRange( false, yRange[0], yRange[1] );
			SetYRange( yRange[0], yRange[1] );
		}
	}
	else
	{
		yRange[0] = 1.0;
		yRange[1] = m_nMaxSize+1;
		
		SetYRange(yRange[0], yRange[1]);
		
		axis = AddAxis(kLocationLeft);
		
		SetYTickCount(m_nMaxSize-1);
		SetYMinorTickCount(2);
		axis->GetAlternateLabels().resize(m_nMaxSize);

		if(m_bForceYZeroStart)
		{
			axis = AddAxis(kLocationBottom);
			xRange[0] = 0.0;
			SetXRange(xRange[0], xRange[1]);
			//int ntick;
			//ResetTicks(xRange, ntick);
			//SetXTickCount(ntick);
		}

		if(m_bAddUp)
		{
			axis = AddAxis(kLocationBottom);
			xRange[0] = 0.0;
			xRange[1] = 0.0;
			for( i = 0; i < GetPlotDataCount(); i++ )
			{
				dataSet = GetAbsPlotData( i );
				
				xRange[1] += GetAbsPlotData(i)->GetXDataRangeHigh();//m_vpXYDataInfo[i]->fXDataMax;
			}
			xRange[0] = 0.0;
			//axis->SetRange( false, xRange[0], xRange[1] );
			SetXRange( xRange[0], xRange[1] );
			return;
		}
	}
	//memcpy(m_pfXPlotRange, xRange, 2*sizeof(double));
	//memcpy(m_pfYPlotRange, yRange, 2*sizeof(double));
	SetXRange(xRange);
	SetYRange(yRange);
}

void	CStemPlotImpl::SetMaxDataSetLen(int len)
{
	CAxis *axis;
	if( len>0 )
	{
		m_nMaxDataSetLen = len;
		if(m_nStemBase == kStemBaseBottom || m_nStemBase == kStemBaseTop)
			axis = AddAxis(kLocationBottom);
		else
			axis = AddAxis(kLocationLeft);

		if(axis)
			axis->GetAlternateLabels().resize(m_nMaxDataSetLen);
	}
}

void	CStemPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	int nOldMapmode = SetMapMode(hDC, MM_TEXT);

	RECT	plotRect = GetPlotRect(hDC, destRect);

	if(IsEmpty())
	{
		DrawDefScreen(hDC, destRect);
		return;
	}
	
	double xRange[2],yRange[2];
	if(IsRangeUpdateNeeded())
	{
		GetPlotRange( xRange, yRange );
		SetLastPlotRange(xRange, yRange);
	}
	ChangeAxisData(plotRect);

	if(m_nStemBase == kStemBaseRight)
	{
		m_bXOpposite = true;
	}
	else if(m_nStemBase == kStemBaseTop)
	{
		m_bYOpposite = true;
	}
	else
	{
		m_bXOpposite = m_bYOpposite = false;
	}

	CXYPlotImpl::FinalDraw( hDC, destRect );

	SetMapMode(hDC, nOldMapmode);
}

void	CStemPlotImpl::ChangeAxisData(RECT plotRect)
{
	CAxis *axis;
	if(m_nStemBase == kStemBaseLeft || m_nStemBase == kStemBaseRight)
	{
		SetYAutoRange(false);
		SetYTickCount((int)GetAbsData(0).size()-1);

		axis = AddAxis(kLocationLeft);
		//axis->SetShowTickLabel(false);
		axis->SetShowMajorTicks(false);
		axis->SetShowMinorTicks(false);
		axis->SetReplaceLabel(true);
		axis->GetAlternateLabels().resize(m_nMaxDataSetLen);
		axis->SetLabelSuffix(_TEXT(""));
		axis->GetAlternateLabels() = m_vstrLabels;
		SIZE offset = { 0, -long((plotRect.bottom - plotRect.top)/(m_nMaxSize<=1?2:m_nMaxSize)*m_fRatioOccupy/2.0)};
		axis->SetLabelOffset(offset);
		
		axis = AddAxis(kLocationBottom);
		//axis->SetShowTickLabel(false);
		axis->SetShowMajorTicks(true);
		axis->SetShowMinorTicks(true);
		axis->SetReplaceLabel(false);
		axis->GetAlternateLabels().resize(0);
		axis->SetLabelSuffix(_TEXT(""));
	}
	else
	{
		axis = AddAxis(kLocationLeft);
		//axis->SetShowTickLabel(false);
		axis->SetShowMajorTicks(true);
		axis->SetShowMinorTicks(true);
		axis->SetReplaceLabel(false);
		axis->GetAlternateLabels().resize(0);
		axis->SetLabelSuffix(_TEXT(""));
		
		SetXAutoRange(false);
		SetXTickCount((int)GetAbsData(0).size()-1);
		axis = AddAxis(kLocationBottom);
		//axis->SetShowTickLabel(false);
		axis->SetShowMajorTicks(false);
		axis->SetShowMinorTicks(false);
		axis->SetReplaceLabel(true);
		axis->GetAlternateLabels().resize(m_nMaxDataSetLen);
		axis->SetLabelSuffix(_TEXT(""));
		axis->GetAlternateLabels() = m_vstrLabels;
		SIZE offset = { long((plotRect.right - plotRect.left)/(m_nMaxSize<=1?2:m_nMaxSize)*m_fRatioOccupy/2.0), 0};
		axis->SetLabelOffset(offset);
	}
}

void	CStemPlotImpl::DrawSinglePlotData( int which, HDC hDC, double *xRange, double *yRange )
{
	if(!GetAbsPlotData(which)->IsVisible())return;

	if(GetAbsPlotData(which)->GetFlag())
	{
		CXYPlotImpl::DrawSinglePlotData(which, hDC, xRange, yRange);
	}

	RECT plotRect = GetLastPlotRect();
	
	HPEN		hPen, hOldPen;
	HBRUSH		hBrush, hOldBrush;
	
	CPlotData<DataPoint2D>	*dataSet = GetAbsPlotData( which );
	if( dataSet->GetDataCount()<=0 ) return;

	//COLORREF linecolor = m_vpXYDataInfo[which]->crPlotColor;
	COLORREF linecolor = m_crStemEdgeColor;
	if( GetAbsPlotData(which)->IsLighted() || GetAbsPlotData(which)->IsSelected() )
	{
		linecolor = RGB(255 - GetRValue(linecolor), 255 - GetGValue(linecolor), 255 - GetBValue(linecolor));
	}
	COLORREF stemColor = GetAbsPlotData(which)->GetPlotColor();
	
	// We now have a rect and an xRange and a yRange, just some
	// simple transformations should get us our plot

	hPen = CreatePen( GetAbsPlotData(which)->GetPlotLineStyle(), GetAbsPlotData(which)->GetPlotLineSize(), linecolor );
	hOldPen = (HPEN)SelectObject(hDC, hPen );
	hBrush	= CreateSolidBrush( GetAbsPlotData(which)->GetPlotColor() );
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	
	int i;
	POINT pt[4];
	RECT frontFace;
	if(m_b3DStem)
	{
		HRGN hRgn = 0;
		
		COLORREF shadowColor;
		int r, g, b;
		r = GetRValue(stemColor);
		g = GetGValue(stemColor);
		b = GetBValue(stemColor);
		
		shadowColor = RGB(r+50>255?255:r+50, g+50>255?255:g+50, b+50>255?255:b+50);
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
		hBrush = CreateSolidBrush(shadowColor);
		for(i=0; i<GetAbsPlotData(which)->GetDataCount(); i++)
		{
			GetFrontFace(which, i, plotRect, xRange, yRange, frontFace);
			Get3DYFaceAdded(frontFace, pt);

			if(hRgn)
				DeleteObject(hRgn);
			hRgn = CreatePolygonRgn(pt,4, ALTERNATE);
			FillRgn(hDC, hRgn, hBrush);
		}
		shadowColor = RGB(r/2, g/2, b/2);
		DeleteObject(hBrush);
		hBrush = CreateSolidBrush(shadowColor);
		for(i=0; i<GetAbsPlotData(which)->GetDataCount(); i++)
		{
			GetFrontFace(which, i, plotRect, xRange, yRange, frontFace);
			Get3DXFaceAdded(frontFace, pt);
			if(hRgn)
				DeleteObject(hRgn);
			hRgn = CreatePolygonRgn(pt,4, ALTERNATE);
			FillRgn(hDC, hRgn, hBrush);
		}
		if(hRgn)
			DeleteObject(hRgn);
		DeleteObject(hBrush);
	}
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	hBrush	= CreateSolidBrush( GetAbsPlotData(which)->GetPlotColor() );
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	for(i=0; i<GetAbsPlotData(which)->GetDataCount(); i++)
	{	
		GetFrontFace(which, i, plotRect, xRange, yRange, frontFace);
		if(m_bStemGradientEffect)
		{
			COLORREF cr2 = RGB(GetRValue(stemColor)*0.6, GetGValue(stemColor)*0.6, GetBValue(stemColor)*0.6);
			int gtype = m_nStemGradientType;
			if(gtype<0)
			{
				if(m_nStemBase == kStemBaseLeft || m_nStemBase == kStemBaseRight)
					gtype = kGradientHCt2Bd;
				else
					gtype = kGradientVCt2Bd;
			}
			GradientRect(hDC, frontFace, gtype, stemColor, cr2);
		}
		else
		{
			Rectangle(hDC, frontFace.left, frontFace.top, frontFace.right, frontFace.bottom);
		}
		if(m_bShowValOnTop)
		{
			double val;
			if(m_nStemBase == kStemBaseBottom || m_nStemBase == kStemBaseTop)
				val = GetAbsPlotData(which)->GetData()[i].def.y;
			else
				val = GetAbsPlotData(which)->GetData()[i].def.x;

			tstring str;
			tostringstream ostr;
			ostr<<val;
			str = ostr.str();

			RECT valRect = GetValueRect(hDC, str, frontFace);
			DrawTextInRect(hDC, valRect, str, &m_lfValOnTop, TRANSPARENT, stemColor);
		}
	}

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

RECT	CStemPlotImpl::GetValueRect(HDC hDC, tstring text, RECT faceRect)
{
	SIZE textSize;
	MyGetTextExtentPoint(hDC, &m_lfValOnTop, text, DT_LEFT, &textSize);

	RECT valRect;
	valRect = faceRect;
	switch(m_nStemBase)
	{
	case kStemBaseBottom:
		valRect.bottom = faceRect.top;
		valRect.top = valRect.bottom - textSize.cy;
		break;
	case kStemBaseTop:
		valRect.top = faceRect.bottom;
		valRect.bottom = valRect.top + textSize.cy;
		break;
	case kStemBaseLeft:
		valRect.left = faceRect.right;
		valRect.right = valRect.left + textSize.cx;
		break;
	case kStemBaseRight:
		valRect.right = faceRect.left;
		valRect.left = valRect.right - textSize.cx;
		break;
	}
	return valRect;
}

int		CStemPlotImpl::GetNeareastPlotData( HDC hDC, POINT point )
{
	if(GetPlotDataCount()<=0)return -1;

	int			i, nIdx;

	double		xRange[] = {fMaxVal, -fMaxVal}, yRange[]={fMaxVal,-fMaxVal};
	GetLastPlotRange( xRange, yRange );
	RECT plotRect = GetLastPlotRect( );

	nIdx = -1;
	for(i=0;i<GetPlotDataCount();i++)
	{
		if( PtInDataArea( i, hDC, point, plotRect, xRange, yRange) )
		{
			nIdx = i;
			break;
		}
	}
	if(nIdx==-1)
		return -1;
	else
		return GetDataID(nIdx);
}

bool	CStemPlotImpl::PtInDataArea( int which, HDC hDC, POINT point, RECT plotRect, double *xRange, double *yRange )
{
	if(which<0 || which>=GetPlotDataCount())return false;
	if(!GetAbsPlotData( which )->IsVisible())return false;
		
	CPlotData<DataPoint2D>	*dataSet = GetAbsPlotData( which );
	if( dataSet->GetDataCount()<=0 ) return false;
		
	RECT rt;
	for(int i=0; i<GetAbsPlotData( which )->GetDataCount(); i++)
	{
		GetFrontFace(which, i, plotRect, xRange, yRange, rt);
		if( PtInRect(&rt, point) )return true;
	}
	
	return false;
}

void	CStemPlotImpl::GetFrontFace(int which, int iPoint, RECT plotRect, double *xRange, double *yRange, RECT &faceRect)
{
	if(which<0 || which>=GetPlotDataCount())return;
	if(!GetAbsPlotData( which )->IsVisible())return;
	
	CPlotData<DataPoint2D>	*dataSet = GetAbsPlotData( which );
	if( dataSet->GetDataCount()<=0 ) return;
	
	int			y;
	double		yScale;

	double width;
	double SegLen;
	double step;
	int	y0;
	
	int	bl, br;
	
	double lowerdata = 0.0;
	
	switch(m_nStemBase)
	{
	case kStemBaseRight:
		yScale = ((double)(plotRect.right - plotRect.left))/fabs((xRange[1] - xRange[0]));
		
		width = plotRect.bottom - plotRect.top;
		SegLen = width / m_nMaxSize;
		step = SegLen*m_fRatioOccupy/GetPlotDataCount();
		y0 = plotRect.right;
		
		bl = int(plotRect.bottom - SegLen*iPoint - which*step);
		br = int(plotRect.bottom - SegLen*iPoint - (which+1)*step);
		if(m_bAddUp)
		{
			lowerdata = 0.0;
			int j;
			for(j=0; j<which; j++)
			{
				lowerdata += GetAbsPlotData( j )->GetData()[iPoint].val[0];
			}
			y0 = (long)(plotRect.right - yScale * (lowerdata-xRange[0]));
			
			bl = int(plotRect.bottom - SegLen*iPoint);
			br = int(plotRect.bottom - SegLen*iPoint - SegLen*m_fRatioOccupy);
		}
		
		y = (long)(plotRect.right - yScale * (lowerdata + dataSet->GetData()[iPoint].val[0]-xRange[0]));
		faceRect.left = y;
		faceRect.top = br;
		faceRect.right = y0;
		faceRect.bottom = bl;
		break;
	case kStemBaseLeft:
		yScale = ((double)(plotRect.right - plotRect.left))/fabs((xRange[1] - xRange[0]));
		
		width = plotRect.bottom - plotRect.top;
		SegLen = width / m_nMaxSize;
		step = SegLen*m_fRatioOccupy/GetPlotDataCount();
		y0 = plotRect.left;
		
		bl = int(plotRect.bottom - SegLen*iPoint - which*step);
		br = int(plotRect.bottom - SegLen*iPoint - (which+1)*step);
		if(m_bAddUp)
		{
			lowerdata = 0.0;
			int j;
			for(j=0; j<which; j++)
			{
				lowerdata += GetAbsPlotData( j )->GetData()[iPoint].val[0];
			}
			y0 = (long)(plotRect.left + yScale * (lowerdata-xRange[0]));
			
			bl = int(plotRect.bottom - SegLen*iPoint);
			br = int(plotRect.bottom - SegLen*iPoint - SegLen*m_fRatioOccupy);
		}
		
		y = (long)(plotRect.left + yScale * (lowerdata + dataSet->GetData()[iPoint].val[0]-xRange[0]));
		faceRect.left = y0;
		faceRect.top = br;
		faceRect.right = y;
		faceRect.bottom = bl;
		break;
	case kStemBaseTop:
		yScale = ((double)(plotRect.bottom-plotRect.top))/fabs((yRange[1] - yRange[0]));
		
		width = plotRect.right - plotRect.left;
		SegLen = width / m_nMaxSize;
		step = SegLen*m_fRatioOccupy/GetPlotDataCount();
		y0 = plotRect.top;
		
		bl = int(plotRect.left + SegLen*iPoint + which*step);
		br = int(plotRect.left + SegLen*iPoint + (which+1)*step);
		if(m_bAddUp)
		{
			lowerdata = 0.0;
			int j;
			for(j=0; j<which; j++)
			{
				lowerdata += GetAbsPlotData( j )->GetData()[iPoint].val[1];
			}
			y0 = (long)(plotRect.top + yScale * (lowerdata-yRange[0]));
			
			bl = int(plotRect.left+SegLen*iPoint);
			br = int(plotRect.left+SegLen*iPoint+SegLen*m_fRatioOccupy);
		}
		
		y = (long)(plotRect.top + yScale * (lowerdata + dataSet->GetData()[iPoint].val[1]-yRange[0]));
		faceRect.left = bl;
		faceRect.top = y0;
		faceRect.right = br;
		faceRect.bottom = y;
		break;
	case kStemBaseBottom:
	default:
		yScale = ((double)(plotRect.bottom-plotRect.top))/fabs((yRange[1] - yRange[0]));
		
		width = plotRect.right - plotRect.left;
		SegLen = width / m_nMaxSize;
		step = SegLen*m_fRatioOccupy/GetPlotDataCount();
		y0 = plotRect.bottom;
		
		bl = int(plotRect.left + SegLen*iPoint + which*step);
		br = int(plotRect.left + SegLen*iPoint + (which+1)*step);
		if(m_bAddUp)
		{
			lowerdata = 0.0;
			int j;
			for(j=0; j<which; j++)
			{
				lowerdata += GetAbsPlotData( j )->GetData()[iPoint].val[1];
			}
			y0 = (long)(plotRect.bottom - yScale * (lowerdata-yRange[0]));
			
			bl = int(plotRect.left+SegLen*iPoint);
			br = int(plotRect.left+SegLen*iPoint+SegLen*m_fRatioOccupy);
		}
		
		y = (long)(plotRect.bottom - yScale * (lowerdata + dataSet->GetData()[iPoint].val[1]-yRange[0]));
		faceRect.left = bl;
		faceRect.top = y;
		faceRect.right = br;
		faceRect.bottom = y0;
	}
}

void	CStemPlotImpl::Get3DXFaceAdded(RECT faceRect, POINT pt[4])
{
	double dx, dy;
	dx = m_f3DStemThick * cos(m_f3DStemAngle * myPi/180.0);
	dy = m_f3DStemThick * sin(m_f3DStemAngle * myPi/180.0);

	switch(m_nStemBase)
	{
	case kStemBaseRight:
		pt[0].x = faceRect.left;
		pt[0].y = faceRect.bottom;
		pt[1].x = faceRect.left;
		pt[1].y = faceRect.top;
		pt[2].x = int(faceRect.left - dx);
		pt[2].y = int(faceRect.top - dy);
		pt[3].x = int(faceRect.left - dx);
		pt[3].y = int(faceRect.bottom - dy);
		break;
	case kStemBaseLeft:
		pt[0].x = faceRect.right;
		pt[0].y = faceRect.bottom;
		pt[1].x = faceRect.right;
		pt[1].y = faceRect.top;
		pt[2].x = int(faceRect.right + dx);
		pt[2].y = int(faceRect.top - dy);
		pt[3].x = int(faceRect.right + dx);
		pt[3].y = int(faceRect.bottom - dy);
		break;
	case kStemBaseTop:
		pt[0].x = faceRect.right;
		pt[0].y = faceRect.bottom;
		pt[1].x = faceRect.right;
		pt[1].y = faceRect.top;
		pt[2].x = int(faceRect.right + dx);
		pt[2].y = int(faceRect.top + dy);
		pt[3].x = int(faceRect.right + dx);
		pt[3].y = int(faceRect.bottom + dy);
		break;
	case kStemBaseBottom:
	default:
		pt[0].x = faceRect.right;
		pt[0].y = faceRect.bottom;
		pt[1].x = faceRect.right;
		pt[1].y = faceRect.top;
		pt[2].x = int(faceRect.right + dx);
		pt[2].y = int(faceRect.top - dy);
		pt[3].x = int(faceRect.right + dx);
		pt[3].y = int(faceRect.bottom - dy);
	}
}

void	CStemPlotImpl::Get3DYFaceAdded(RECT faceRect, POINT pt[4])
{	
	double dx, dy;
	dx = m_f3DStemThick * cos(m_f3DStemAngle * myPi/180.0);
	dy = m_f3DStemThick * sin(m_f3DStemAngle * myPi/180.0);

	switch(m_nStemBase)
	{
	case kStemBaseRight:
		pt[0].x = faceRect.right;
		pt[0].y = faceRect.top;
		pt[1].x = faceRect.left;
		pt[1].y = faceRect.top;
		pt[2].x = int(faceRect.left - dx);
		pt[2].y = int(faceRect.top - dy);
		pt[3].x = int(faceRect.right - dx);
		pt[3].y = int(faceRect.top - dy);
		break;
	case kStemBaseLeft:
		pt[0].x = faceRect.right;
		pt[0].y = faceRect.top;
		pt[1].x = faceRect.left;
		pt[1].y = faceRect.top;
		pt[2].x = int(faceRect.left + dx);
		pt[2].y = int(faceRect.top - dy);
		pt[3].x = int(faceRect.right + dx);
		pt[3].y = int(faceRect.top - dy);
		break;
	case kStemBaseTop:
		pt[0].x = faceRect.right;
		pt[0].y = faceRect.bottom;
		pt[1].x = faceRect.left;
		pt[1].y = faceRect.bottom;
		pt[2].x = int(faceRect.left + dx);
		pt[2].y = int(faceRect.bottom + dy);
		pt[3].x = int(faceRect.right + dx);
		pt[3].y = int(faceRect.bottom + dy);
		break;
	case kStemBaseBottom:
	default:
		pt[0].x = faceRect.right;
		pt[0].y = faceRect.top;
		pt[1].x = faceRect.left;
		pt[1].y = faceRect.top;
		pt[2].x = int(faceRect.left + dx);
		pt[2].y = int(faceRect.top - dy);
		pt[3].x = int(faceRect.right + dx);
		pt[3].y = int(faceRect.top - dy);
	}
}

CStemPlotImplI::CStemPlotImplI()
{

}

CStemPlotImplI::~CStemPlotImplI()
{

}
/*
bool	CStemPlotImplI::ShowDlgBySelection(HDC hDC)
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