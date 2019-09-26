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
#include "MyGdiPlus.h"
#include "XYPlotBasicImpl.h" 
#include "MyMemDC.h"
#include "PlotUtility.h"
#include "Handlers/Dialogs//ColorControl.h"

#pragma warning(disable:4786)

#include <iomanip>

using namespace NsCChart;

void	MyDefScreen( void *plot, HDC hDC, RECT plotRect, void *pPara )
{
	if(!plot)return;
	CXYPlotBasicImpl *myPlot = (CXYPlotBasicImpl *)plot;

	if(myPlot->GetDefScreenImage())
	{
		HDC	hdc = CreateCompatibleDC(hDC);
		HBITMAP hOldBm = (HBITMAP)SelectObject(hdc, myPlot->GetDefScreenImage());
		BITMAP bm;
		GetObject(myPlot->GetDefScreenImage(), sizeof(BITMAP), &bm);
		SetStretchBltMode(hDC, HALFTONE);
		SetBrushOrgEx(hDC, 0, 0, NULL);
		StretchBlt(hDC, plotRect.left, plotRect.top, Width(plotRect), Height(plotRect),
			hdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		SelectObject(hdc, hOldBm);
		DeleteDC(hdc);
	}
	else
	{
		tstring title = myPlot->GetDefaultMain();
		tstring	subti = myPlot->GetDefaultSub();
		
		LOGFONT	lfTF = myPlot->GetTitleFont();
		LOGFONT lfSF = myPlot->GetTitleFont();
		COLORREF crTF = myPlot->GetTitleColor();
		COLORREF crSF = RGB(25, 225, 125);
		
		lfTF.lfHeight *= 3;
		lfSF.lfItalic = 1;
		
		LOGFONT lfReal;
		RECT rt1, rt2;
		rt1 = plotRect;
		rt1.top	= (long)(rt1.top*0.8 + rt1.bottom*0.2);
		DrawTextInRect(hDC, rt1, title, &lfTF, TRANSPARENT, crTF, DT_CENTER | DT_BOTTOM, &lfReal);

		SIZE sizeM;
		MyGetTextExtentPoint(hDC, &lfReal, title, DT_LEFT, &sizeM);
		
		rt2 = plotRect;
		rt2.top = rt1.top+sizeM.cy*1.4;
		DrawTextInRect(hDC, rt2, subti, &lfSF, TRANSPARENT, crSF);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//

CXYPlotBasicImpl::CXYPlotBasicImpl()
{
	RegisterColorControl();
	InitGrid(CGlobal::MyGetModuleHandle());

	m_fcnDefScreen = MyDefScreen;
}

CXYPlotBasicImpl::~CXYPlotBasicImpl()
{
	
}

int		CXYPlotBasicImpl::GetDataIDByPoint( HDC hDC, POINT point )
{
	if( RegionIdentify(hDC,point) != kXYRegionData )return -1;
		
	return GetNeareastPlotData(hDC,point);
}

int		CXYPlotBasicImpl::LightByPoint( HDC hDC, POINT point )
{
	int dataID = GetDataIDByPoint(hDC, point);
	if(dataID>=0)
	{
		SetLighted(dataID,true);
		return dataID;
	}
	else
	{
		UnlightAllPlotData();
		return -1;
	}
}

int		CXYPlotBasicImpl::SelectByPoint( HDC hDC, POINT point )
{
	int dataID = GetDataIDByPoint(hDC, point);
	if(dataID>=0)
	{
		bool bSel = IsSelected(dataID);
		SelectAllPlotData(false);
		SetSelected(dataID,!bSel);
		if(!bSel)return dataID;
		else return -1;
	}
	else
	{
		SelectAllPlotData(false);
		return -1;
	}
}

int		CXYPlotBasicImpl::GetNeareastPlotData( HDC hDC, POINT point )
{
	if(m_vpPlotData.size()<=0)return -1;

	CPlotDataImpl<DataPoint2D>			*dataSet;

	int					i, j, lo, up;
	double				data[2],range[2];
	
	int					nIdx;
	double				dlM,dl0;
	
	LPToData( &point, data );

	GetYRange(range);
	dlM=(range[1]-range[0])/m_fHitPrecision;

	double xMin, xMax;
	double xm,x1;

	nIdx=-1;
	for(i=0;i<GetPlotDataCount();i++)
	{
		dataSet = GetPlotData( GetDataID( i ) );

		if( dataSet->GetDataCount()<=1 ) continue;

		switch(m_vpPlotData[i]->nSLMode)
		{
		case kSLByFillArea:
			POINT *myPoints;
			int nPts4Fill;
			myPoints = m_vpPlotData[i]->pPointsGdi;
			nPts4Fill = m_vpPlotData[i]->nPts4Fill;

			if(!myPoints)break;
			
			BeginPath(hDC);
			Polyline(hDC, myPoints, nPts4Fill);
			EndPath( hDC );

			{
				HRGN hRgn = PathToRegion(hDC);
				if(!hRgn)break;
				
				Region rgn(hRgn);
				
				if(PtInRegion(hRgn, point.x, point.y))
				{
					nIdx = i;
				}
				DeleteObject(hRgn);
			}

			if(nIdx>=0)
			{
				return GetDataID(nIdx);
			}

			break;
		default:
			lo=-1;
			up=-1;
			
			m_vpPlotData[i]->GetDataRange(  );
			
			if(IsXLogarithm())
			{
				xMin = log10(m_vpPlotData[i]->GetXDataRange()[0]);
				xMax = log10(m_vpPlotData[i]->GetXDataRange()[1]);
			}
			else
			{
				xMin = m_vpPlotData[i]->GetXDataRange()[0];
				xMax = m_vpPlotData[i]->GetXDataRange()[1];
			}
			
			if(data[0]<xMin || data[0]>xMax)
			{
				continue;
			}
			else
			{
				for(j=1;j<dataSet->GetDataCount();j++)
				{
					if(IsXLogarithm())
					{
						xm = log10(dataSet->GetData()[j-1].val[0]);
						x1 = log10(dataSet->GetData()[j].val[0]);
					}
					else
					{
						xm = dataSet->GetData()[j-1].val[0];
						x1 = dataSet->GetData()[j].val[0];
					}
					if( (data[0]-xm)*(x1-data[0])>=0.0 )
					{
						lo=j-1;
						up=j;
						ApproachNearestIdx(i,nIdx,data,lo,up,dlM,dl0);
						if(nIdx>=0)break;
					}
				}
			}
		}
		
	}

	if(nIdx==-1)
		return -1;
	else
		return GetDataID(nIdx);
}

int		CXYPlotBasicImpl::GetNeareastDataPoint( int dataID, HDC hDC, POINT point, double data[2])
{
	CPlotDataImpl<DataPoint2D> *pDataSet=GetPlotData(dataID);
	if(!pDataSet)return -1;
	if(pDataSet->GetDataCount()<=0)return -1;

	int idx = GetIndex(dataID);

	double xMin, xMax;
	if(IsXLogarithm())
	{
		xMin = log10(m_vpPlotData[idx]->GetXDataRange()[0]);
		xMax = log10(m_vpPlotData[idx]->GetXDataRange()[1]);
	}
	else
	{
		xMin = m_vpPlotData[idx]->GetXDataRange()[0];
		xMax = m_vpPlotData[idx]->GetXDataRange()[1];
	}
	double xm, x1;

	LPToData( &point, data );
	if(data[0]<xMin || data[0]>xMax)return -1;

	int lo = -1, up = -1;
	bool set = false;
	double x0=data[0],y0=data[1];
	double y1, dist = 1.0e90;
	double xlo, xup, ylo, yup;

	for(int i=1; i<pDataSet->GetDataCount(); i++)
	{
		if(IsXLogarithm())
		{
			xm = log10(pDataSet->GetData()[i-1].val[0]);
			x1 = log10(pDataSet->GetData()[i].val[0]);
		}
		else
		{
			xm = pDataSet->GetData()[i-1].val[0];
			x1 = pDataSet->GetData()[i].val[0];
		}
		if( (data[0]-xm)*(x1-data[0])>=0.0 )
		{
			lo=i-1;
			up=i;
			set=true;
			
			if(IsXLogarithm())
			{
				xlo = log10(pDataSet->GetData()[lo].val[0]);
				xup = log10(pDataSet->GetData()[up].val[0]);
			}
			else
			{
				xlo = pDataSet->GetData()[lo].val[0];
				xup = pDataSet->GetData()[up].val[0];
			}
			
			if(IsYLogarithm())
			{
				ylo = log10(pDataSet->GetData()[lo].val[1]);
				yup = log10(pDataSet->GetData()[up].val[1]);
			}
			else
			{
				ylo = pDataSet->GetData()[lo].val[1];
				yup = pDataSet->GetData()[up].val[1];
			}

			if( xup - xlo != 0.0 )
				y1=ylo + ( yup - ylo )/(xup - xlo ) * (x0 - xlo);
			else
				y1=y0;
			
			if( fabs(y1-y0)<dist )
			{
				dist=fabs(y1-y0);
				data[1]=y1;
			}
		}
	}

	if(set)return lo;
	else return -1;
}

int		CXYPlotBasicImpl::GetNeareastDP(int dataID, HDC hDC, POINT point)
{
	if(dataID<0)return -1;
	int index = GetIndex(dataID);
	if(index<0)return -1;

	double range[2];
	GetXRange(range);
	double dlMx=(range[1]-range[0])/m_fHitPrecision;
	GetYRange(range);
	double dlMy=(range[1]-range[0])/m_fHitPrecision;

	int i;
	double x1, y1;
	double data[2];
	LPToData(&point, data);
	for(i=0; i<GetAbsPlotData(index)->GetDataCount(); i++)
	{
		x1 = GetAbsData(index)[i].def.x;
		y1 = GetAbsData(index)[i].def.y;
		if(fabs(x1-data[0])<dlMx && fabs(y1-data[1])<dlMy)
			return i;
	}
	return -1;
}

void	CXYPlotBasicImpl::ApproachNearestIdx(int curIdx,int &nIdx,double *data,int lo,int up,double dlM,double &dl0)
{
	ApproachNearestIdxByY(curIdx, nIdx, data, lo, up, dlM, dl0);
}

void	CXYPlotBasicImpl::ApproachNearestIdxByY(int curIdx,int &nIdx,double *data,int lo,int up,double dlM,double &dl0)
{
	double dlt;
	double x0=data[0],y0;
	double xlo,xup,ylo,yup;

	if(IsXLogarithm())
	{
		xlo = log10(GetAbsData( curIdx )[lo].val[0]);
		xup = log10(GetAbsData( curIdx )[up].val[0]);
	}
	else
	{
		xlo = GetAbsData( curIdx )[lo].val[0];
		xup = GetAbsData( curIdx )[up].val[0];
	}
	
	if(IsYLogarithm())
	{
		ylo = log10(GetAbsData( curIdx )[lo].val[1]);
		yup = log10(GetAbsData( curIdx )[up].val[1]);
	}
	else
	{
		ylo = GetAbsData( curIdx )[lo].val[1];
		yup = GetAbsData( curIdx )[up].val[1];
	}
	
	if( xup - xlo != 0.0 )
	{
		int plottype = m_vpPlotData[curIdx]->GetPlotType();
		switch(plottype)
		{
		case kXYPlotConnect:
			y0 = ylo + ( yup - ylo )/(xup - xlo ) * (x0 - xlo);
			break;
		case kXYPlotStepHV:
			y0 = ylo;
			break;
		case kXYPlotStepVH:
			y0 = yup;
			break;
		case kXYPlotStepHVH:
			if(data[0] < (xlo + xup)/2)
				y0 = ylo;
			else
				y0 = yup;
			break;
		case kXYPlotStepVHV:
			y0 = (ylo + yup)/2;
			break;
		default:
			y0 = ylo + ( yup - ylo )/(xup - xlo ) * (x0 - xlo);
		}
		// determine y hit
		dlt = fabs(y0-data[1]);
		if(dlt<dlM)
		{
			goto HitTheCurve;
		}
		// determine x hit
		switch(plottype)
		{
		case kXYPlotStepHV:
			dlt = fabs(xup - data[0]);
			if(dlt<dlM && (data[1]-ylo)*(yup-data[1]>=0))
				goto HitTheCurve;
			break;
		case kXYPlotStepVH:
			dlt = fabs(xlo - data[0]);
			if(dlt<dlM && (data[1]-ylo)*(yup-data[1]>=0))
				goto HitTheCurve;
			break;
		case kXYPlotStepHVH:
			dlt = fabs( (xlo + xup)/2 - data[0]);
			if(dlt<dlM && (data[1]-ylo)*(yup-data[1]>=0))
				goto HitTheCurve;
			break;
		case kXYPlotStepVHV:
			if( data[0] < (xlo+xup)/2 )
			{
				dlt = fabs(xlo - data[0]);
				if(dlt<dlM && (data[1]-ylo)*((ylo+yup)/2-data[1]>=0))
				goto HitTheCurve;
			}
			else
			{
				dlt = fabs(xup - data[0]);
				if(dlt<dlM && (data[1]-(ylo+yup)/2 )*(yup-data[1]>=0))
				goto HitTheCurve;
			}
			break;
		default:
			break;
		}
		
	}
	else
	{
		dlt = fabs(data[0]-xlo);
		if(dlt<dlM && (yup-data[1])*(data[1]-ylo)>=0.0)
		{
			goto HitTheCurve;
		}
	}
	return;

HitTheCurve:
	if(nIdx==-1)
	{
		nIdx=curIdx;
		dl0=dlt;
	}
	else
	{
		if(dlt<dl0)
		{
			nIdx=curIdx;
			dl0=dlt;
		}
	}
}

bool		CXYPlotBasicImpl::CheckLogSafety()
{
	bool bXLog = IsXLogarithm();
	bool bYLog = IsYLogarithm();
	
	if(!bXLog && !bYLog)
		return true;
	else if( (bXLog && !CheckLogSafety(true)) || (bYLog && !CheckLogSafety(false)) )
		return false;
	else
		return true;
}

bool		CXYPlotBasicImpl::CheckLogSafety(bool bX)
{
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if( (bX?GetAbsPlotData(i)->GetXDataRange()[0]:GetAbsPlotData(i)->GetYDataRange()[0]) <=0.0 )return false;
	}
	return true;
}

void	CXYPlotBasicImpl::FinalDraw(HDC hDC, RECT destRect)
{
	//if there is no data, doing nothing
	if(IsEmpty())
	{
		(*m_fcnDefScreen)(this, hDC, destRect, m_pParaDefScreen);
		return;
	}

	if(IsXLogarithm() || IsYLogarithm())
	{
		if(!CheckLogSafety())
		{
			EraseRect(hDC, destRect, GetBkgndColor());
			return;
		}
	}
	
	double xRange[2], yRange[2];
	GetPlotRange(xRange, yRange);

	SetPlotColors();
	SetPlotLineStyles();
	SetPlotLineSizes();

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

//	EnterCriticalSection(&m_csPlot);
	int saveDC = SaveDC(hDC);
	HRGN hRgn=CreateRectRgn( destRect.left, destRect.top, destRect.right, destRect.bottom );
	//HRGN hRgn=CreateRectRgn( 4, 4, destRect.right-destRect.left/2, destRect.bottom-destRect.top/2 );
	//SelectObject(hDC,hRgn);

	CPlotImpl::FinalDraw(hdc, destRect);
	DrawDataSet(hdc, xRange, yRange);
	
	RestoreDC(hDC, saveDC );
	DeleteObject(hRgn);
//	LeaveCriticalSection(&m_csPlot);
}

void	CXYPlotBasicImpl::DrawDefScreen(HDC hDC, RECT destRect)
{
	COLORREF bgcr;
	bgcr=CCanvas::m_crBkgndColor;
	
	EraseRect(hDC, destRect, bgcr);
	if(m_bDrawBasicAnyway)
	{
		CPlotImpl::FinalDraw(hDC, destRect);
		if(m_fcnDefScreen)m_fcnDefScreen(this, hDC, GetPlotRect(hDC, destRect), m_pParaDefScreen);
	}
	else
	{
		if(m_fcnDefScreen)m_fcnDefScreen(this, hDC, destRect, m_pParaDefScreen);
	}
}

void	CXYPlotBasicImpl::GetDataRange1D( int whichDim, double *range )
{
	int i;
	range[0] = fMaxVal;
	range[1] = -fMaxVal;
	for(i = 0; i<GetPlotDataCount(); i++)
	{
		m_vpPlotData[i]->GetDataRange();
	}

	bool bX = (whichDim==0)?true:false;

	int count = 0;
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		if(m_vpPlotData[i]->GetData().size()<=0)continue;
		count++;
		
		if(bX?IsXLogarithm():IsYLogarithm())
		{
			range[0] = min( range[0], log10(bX?m_vpPlotData[i]->GetXDataRange()[0]:m_vpPlotData[i]->GetYDataRange()[0]) );
			range[1] = max( range[1], log10(bX?m_vpPlotData[i]->GetXDataRange()[1]:m_vpPlotData[i]->GetYDataRange()[1]) );
		}
		else
		{
			range[0] = min( range[0], bX?m_vpPlotData[i]->GetXDataRange()[0]:m_vpPlotData[i]->GetYDataRange()[0] );
			range[1] = max( range[1], bX?m_vpPlotData[i]->GetXDataRange()[1]:m_vpPlotData[i]->GetYDataRange()[1] );
		}
		if(range[0]==range[1])
		{
			if(range[0]==0.0)
			{
				range[0]-=0.5;
				range[1]+=0.5;
			}
			else
			{
				range[0]-=pow(10,floor(log10(fabs(range[0]))));;
				range[1]+=pow(10,floor(log10(fabs(range[0]))));;
			}
		}
	}
	if(count <= 0)
	{
		range[0] = 0;
		range[1] = 10;
	}
}

void	CXYPlotBasicImpl::GetPlotRange1D( int whichDim, double *range )
{
	if(whichDim==0)GetXRange(range);
	else GetYRange(range);
}

int		CXYPlotBasicImpl::RegionIdentifyEx(HDC hDC, POINT point, bool &legendRegion, int &centerAxisRegion)
{
	int region = RegionIdentify(hDC, point);

	legendRegion = false;

	if(region == kXYRegionData )
	{
		RECT theRect;
		CAxis *axis;
		for(int i=0; i<GetAxesCount(); i++)
		{
			axis=GetAxis(i);
			theRect=((CAxisImpl*)axis)->GetPlotRect(hDC);//, GetLastClientRect(), GetLastPlotRect( ));
			//if( (theRect.right-point.x)*(point.x-theRect.left)>=0 && (theRect.top-point.y)*(point.y-theRect.bottom)>=0 )
			if( PtInRect(&theRect,point) )
			{
				if(axis->GetLocation()==kLocationCenterHB)centerAxisRegion = kLocationCenterHB;
				else if(axis->GetLocation()==kLocationCenterHT)centerAxisRegion = kLocationCenterHT;
				else if(axis->GetLocation()==kLocationCenterVL)centerAxisRegion = kLocationCenterVL;
				else if(axis->GetLocation()==kLocationCenterVR)centerAxisRegion = kLocationCenterVR;
				else centerAxisRegion = -1;
			}
			else
			{
				centerAxisRegion = -1;
			}
		}
	}
	else
	{
		centerAxisRegion = -1;
	}

	return region;
}

void	CXYPlotBasicImpl::UniteToColor( COLORREF color )
{
	int i;
	SetTitleColor(color);
	SetBorderColor(color);
	
	for(i=0; i<GetAxesCount(); i++)
	{
		GetAxis(i)->SetAxisColor(color);
	}

	if(!m_bExcludeDataWhenUniteColor)
	{
		SetAutoDataColorGeneral(false);
		for(i=0; i<GetPlotDataCount(); i++)
		{
			SetAutoColor(GetDataID(i), false);
			SetDataColor(GetDataID(i), color);
		}
	}
	
	SetFootnoteColor(color);
	SetHeadernoteColor(color);
	for(i=0; i<GetCommentsCount(); i++)
	{
		GetAbsComment(i)->SetBorderColor(color);
		GetAbsComment(i)->SetCommentColor(color);
	}
}
/*
tstring		CXYPlotBasicImpl::GetAbsDataTitle( int which )
{
	CPlotData<DataPoint2D>		*plotData = GetAbsPlotData( which );
	
	if(!plotData)
		return _TEXT("");
	else
		return plotData->GetTitle();
}*/

tstring		CXYPlotBasicImpl::GetAbsDataAppendix( int which )
{
	CPlotData<DataPoint2D>		*plotData = GetAbsPlotData( which );
	
	if(!plotData)
		return _TEXT("");
	else
		return plotData->GetAppendix();
}


