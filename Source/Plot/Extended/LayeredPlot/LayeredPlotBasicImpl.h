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

#include "../../Basic/XYPlotImpl.h"
#include "ShareXHandler.h"
#include "ShareXReactStatus.h"
#include "LayeredPlot.h"
#include "PlotLayerImpl.h"
#include "../../Basic/PlotUtility.h"
#include "../../Basic/MyMemDC.h"

Declare_Namespace_CChart

template<class PlotLayerImplIT>
class CLayeredPlotBasicImpl :	public CLayeredPlot<PlotLayerImplIT>,
								public CPlotImpl,
								public CLegendImpl<CLayeredPlotBasicImpl<PlotLayerImplIT> >
{
public:
	CLayeredPlotBasicImpl();
	virtual ~CLayeredPlotBasicImpl();

	bool		IsEmpty();
	bool		NeedDrawing();

	int			AddCurve(double *pX, double *pY, int nLen, int nLayerIdx);
	int			AddPoint( double x, double y, int nWhich, int nLayerIdx );

	virtual	void	GetPlotRange( double xRange[2], double yRange[2] )=0;
	virtual	bool	IsRangeUpdateNeeded();
	virtual	void	GetDataRange1D( int whichDim, double *range );
	virtual	void	GetPlotRange1D( int whichDim, double *range );
	void		GetSubPlotRanges();

	int			GetPlotDataCount( void ) const;
	CPlotDataImpl<DataPoint2D>	*GetAbsPlotData( int which );
	int			GetVisiblePlotDataCount();

	CAxisImpl	*AddAxis( int nLayerIdx, int location );
	CAxisImpl	*GetAxis( int nLayerIdx, int location );

public:
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

protected:
	virtual	void		DrawBasic( HDC hDC, RECT destRect )=0;

public:
	virtual	CPlotData<DataPoint2D>	*GetPlotDataByTitle( tstring title );
	void		DeleteAllData();
	double		GetValueFromX(double x, int which, int nIndex);

	virtual tstring		GetAbsDataTitle( int which );
	virtual tstring		GetAbsDataAppendix( int which );

	virtual	bool		IsNoDataPlot(){return false;};

public:	
	virtual	void		SetLRAxis();
	virtual	SIZE		GetAxisSize( HDC hDC, int location )=0;
	virtual	SIZE		GetAxisPaddingSize( HDC hDC , int location )=0;
	virtual	RECT		GetPlotRect( HDC hDC, RECT clientRect)=0;

	RECT		AddtionalPlotRectAdjust(HDC hDC, RECT plotRect);

public:
	int			GetLocalIdx(int which, int &idxOfLayer, int &idxInLayer);

	virtual	int			RegionIdentify(HDC hDC,POINT point)=0;

public:
	bool		IsPlotDataLighted();
	bool		IsPlotDataSelected();
	void		UnlightAllPlotData();
	void		SelectAllPlotData(bool bSel);
	
	bool		IsAxisLighted();
	void		UnlightAllAxis();
	bool		IsAxisSelected();
	void		UnselectAllAxis();

	int			GetIndexOfLightedAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfSelectedAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfLightedPlotData(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfSelectedPlotData(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfFirstVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfLastVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfFirstVisiblePlotData(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfLastVisiblePlotData(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfNextVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfPrevVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfNextVisiblePlotData(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfPrevVisiblePlotData(int &idxOfLayer, int &idxInLayer);

	bool		SelectAxis(int &idxOfLayer, int &idxInLayer, bool bSel);
	bool		SelectPlotData(int &idxOfLayer, int &idxInLayer, bool bSel);
	
	int			LightByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer );
	int			SelectByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer );
	
	int			GetNeareastPlotData( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer );
		
	virtual	RECT		GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect )=0;
	virtual	int			GetIdxOfLayerByPointInAxisRegion( HDC hDC, RECT plotRect, POINT point )=0;

public:
	void		SetColor(tstring name, COLORREF color);
	void		SetLineStyle(tstring name, int style);
	void		SetLineSize(tstring name, int size);

public:
	bool		CheckLogSafety();
	bool		CheckLogSafety(bool bX);

	int			GetAxesCount( );
	int			GetVisibleAxisCount();
protected:
	int			m_nTopLayer;
public:
	int			GetTop(){return m_nTopLayer;}
	void		SetTop(int nToyLayer){m_nTopLayer = nToyLayer;}

public:
	void		MyLock();
	void		MyUnlock();
};

template<class PlotLayerImplIT>
CLayeredPlotBasicImpl<PlotLayerImplIT>::CLayeredPlotBasicImpl()
{
	CAxisImpl *axis;
	axis = CPlotImpl::AddAxis(kLocationBottom);

	m_nTopLayer = -1;
}

template<class PlotLayerImplIT>
CLayeredPlotBasicImpl<PlotLayerImplIT>::~CLayeredPlotBasicImpl()
{
	//ClearLayers();
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::IsEmpty()
{
	if(m_vpLayers.size()<=0)return true;
	for(int i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->IsEmpty())return false;
	}
	return true;
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::NeedDrawing()
{
	if(IsEmpty())return false;
	for(int i=0;i<(int)m_vpLayers.size();i++)
	{
		if(m_vpLayers[i]->NeedDrawing())return true;
	}
	return false;
}

template<class PlotLayerImplIT>
CAxisImpl*	CLayeredPlotBasicImpl<PlotLayerImplIT>::AddAxis( int nLayerIdx, int location )
{
	if( nLayerIdx<0 || nLayerIdx>=this->GetLayerCount() )return NULL;
	return ((CPlotLayerImplI *)m_vpLayers[nLayerIdx])->AddAxis( location );
}

template<class PlotLayerImplIT>
CAxisImpl*	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetAxis( int nLayerIdx, int location )
{
	if( nLayerIdx<0 || nLayerIdx>=this->GetLayerCount() )return NULL;
	return ((CPlotLayerImplI *)m_vpLayers[nLayerIdx])->GetAxis( location );
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetLocalIdx(int which, int &idxOfLayer, int &idxInLayer)
{
	if(which<0 || which>=GetPlotDataCount())
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		return -1;
	}
	int count=0,cntOfLayer,dataID;
	for(int i=0;i<GetLayerCount();i++)
	{
		cntOfLayer=m_vpLayers[i]->GetPlotDataCount();
		if(count<=which && count+cntOfLayer>which)
		{
			idxOfLayer=i;
			idxInLayer=which-count;
			dataID=((CPlotLayerImplI *)m_vpLayers[idxOfLayer])->GetDataID(idxInLayer);
			return dataID;
		}
		count+=cntOfLayer;
	}
	return -1;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetPlotDataCount( void ) const
{
	int count = 0;
	for(int i=0; i<(int)m_vpLayers.size(); i++)
	{
		count+=m_vpLayers[i]->GetPlotDataCount();
	}
	return count;
}

template<class PlotLayerImplIT>
CPlotDataImpl<DataPoint2D>	*CLayeredPlotBasicImpl<PlotLayerImplIT>::GetAbsPlotData( int which )
{
	if(which<0 || which>GetPlotDataCount())return NULL;
	int count = 0, cnt0=0;
	for(int i=0; i<(int)m_vpLayers.size(); i++)
	{
		count+=m_vpLayers[i]->GetPlotDataCount();
		if(which>=cnt0 && which<count)
		{
			return m_vpLayers[i]->GetAbsPlotData(which-cnt0);
		}
		cnt0+=m_vpLayers[i]->GetPlotDataCount();
	}
	return NULL;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetVisiblePlotDataCount(  )
{
	int count = 0;
	for(int i=0; i<(int)m_vpLayers.size(); i++)
	{
		count+=m_vpLayers[i]->GetVisiblePlotDataCount();
	}
	return count;
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::SetColor(tstring name, COLORREF color)
{
	int i, j;
	for(j=0; j<GetLayerCount(); j++)
	{
		for(i=0; i<GetLayer(j)->GetPlotDataCount(); i++)
		{
			if(GetLayer(j)->GetAbsDataTitle(i)==name)
			{
				GetLayer(j)->SetDataColor(GetLayer(j)->GetDataID(i), color);
				return;
			}
		}
	}
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::SetLineStyle(tstring name, int style)
{
	int i, j;
	for(j=0; j<GetLayerCount(); j++)
	{
		for(i=0; i<GetLayer(j)->GetPlotDataCount(); i++)
		{
			if(GetLayer(j)->GetAbsDataTitle(i)==name)
			{
				GetLayer(j)->SetDataLineStyle(GetLayer(j)->GetDataID(i), style);
				return;
			}
		}
	}
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::SetLineSize(tstring name, int size)
{
	int i, j;
	for(j=0; j<GetLayerCount(); j++)
	{
		for(i=0; i<GetLayer(j)->GetPlotDataCount(); i++)
		{
			if(GetLayer(j)->GetAbsDataTitle(i)==name)
			{
				GetLayer(j)->SetDataLineSize(GetLayer(j)->GetDataID(i), size);
				return;
			}
		}
	}
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetSubPlotRanges()
{
	double xrng[2],yrng[2];
	
	int i;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		
		if(m_vpLayers[i]->IsRangeUpdateNeeded())
		{
			
			m_vpLayers[i]->GetPlotRange( xrng, yrng );
			m_vpLayers[i]->SetLastPlotRange( xrng, yrng );
		}
	}
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetDataRange1D( int whichDim, double *range )
{
	double rng[2];
	range[0] = fMaxVal;
	range[1] = -fMaxVal;

	bool bX=(whichDim==0)?true:false;
	
	int i;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;

		m_vpLayers[i]->GetDataRange1D( whichDim, rng );

		if(bX?IsXLogarithm():IsYLogarithm())
		{
			range[0] = min( range[0], log10(rng[0]) );
			range[1] = max( range[1], log10(rng[1]) );
		}
		else
		{
			range[0] = min( range[0], rng[0] );
			range[1] = max( range[1], rng[1] );
		}
	}
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetPlotRange1D( int whichDim, double *range )
{
	double rng[2];
	range[0] = fMaxVal;
	range[1] = -fMaxVal;
	
	bool bX=(whichDim==0)?true:false;
	
	int i;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		
		m_vpLayers[i]->GetPlotRange1D( whichDim, rng );
		
		if(bX?IsXLogarithm():IsYLogarithm())
		{
			range[0] = min( range[0], log10(rng[0]) );
			range[1] = max( range[1], log10(rng[1]) );
		}
		else
		{
			range[0] = min( range[0], rng[0] );
			range[1] = max( range[1], rng[1] );
		}
	}
}
/*
template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetPlotRange( double xRange[2], double yRange[2] )
{
	xRange[0]=-fMaxVal;
	xRange[1]=fMaxVal;
	yRange[0]=-fMaxVal;
	yRange[1]=fMaxVal;

	if(!NeedDrawing())
	{
		xRange[0]=yRange[0]=0.0;
		xRange[1]=yRange[1]=50.0;
		return;
	}
	
	double xrng[2],yrng[2];

	GetSubPlotRanges();
	GetDataRange(true, xRange);
	GetDataRange(false, yRange);

	int nTicks,nMinorTicks;
	if(IsRangeUpdateNeeded())
	{
		GetOptimalRange(xRange, xrng, &nTicks, &nMinorTicks, true, true, false, false, false, false);
		memcpy(xRange, xrng, 2*sizeof(double));
	}
	else
	{
		GetXRange(xRange);
		nTicks = GetXTickCount();
		nMinorTicks = GetXMinorTickCount();
	}

	int i;
	for(i=0; i<(int)m_vpLayers.size(); i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		m_vpLayers[i]->SetXRange(xRange[0],xRange[1]);
		m_vpLayers[i]->SetLastXPlotRange(xRange);
	}

	if(!m_bFloatXTicks)
	{
		SetXRange( xRange[0],xRange[1] );
		SetLastXPlotRange(xRange);

		SetXTickCount(nTicks);
		SetXMinorTickCount(nMinorTicks);
	}
	else
	{
		int nTicks;
		double tickMin, tickMax;
		RegulateTicks(xRange[0], xRange[1], nTicks, tickMin, tickMax);
		SetXRange( xRange[0],xRange[1] );
		SetLastXPlotRange(xRange);
		SetXTickCount(nTicks-2);
		SetXTickMin(tickMin);
		SetXTickMax(tickMax);

	}

	int nGridLayer = m_nGridBindLayer;
	if(nGridLayer<0)nGridLayer=0;
	if(nGridLayer>=(int)m_vpLayers.size())nGridLayer=(int)m_vpLayers.size()-1;
	m_vpLayers[nGridLayer]->GetLastPlotRange( xrng, yrng );
	SetYRange(yrng[0], yrng[1]);
	SetLastYPlotRange(yrng);
	SetYTickCount(m_vpLayers[nGridLayer]->GetYTickCount());
	SetYMinorTickCount(m_vpLayers[nGridLayer]->GetYMinorTickCount());
	memcpy(yRange, yrng, 2*sizeof(double));

	memcpy(m_pfXPlotRange, xRange, 2*sizeof(double));
	memcpy(m_pfYPlotRange, yRange, 2*sizeof(double));
	SetLastPlotRange(xRange, yRange);
}
*/
template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::IsRangeUpdateNeeded()
{
	if(CPlotImpl::IsRangeUpdateNeeded())return true;

	int i;
	for(i=0; i<(int)m_vpLayers.size(); i++)
	{
		if(m_vpLayers[i]->IsRangeUpdateNeeded())return true;
	}
	return false;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::AddCurve(double *pX, double *pY, int nLen, int nLayerIdx)
{
	if(nLayerIdx < 0 || nLayerIdx >= GetLayerCount() )return -1;
	int dataID = ((CPlotLayerImplI*)m_vpLayers[nLayerIdx])->AddCurve(pX,pY,nLen);

	double xRange[2],yRange[2];
	GetPlotRange(xRange,yRange);
	SetLastPlotRange(xRange, yRange);

	return dataID;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::AddPoint( double x, double y, int nWhich, int nLayerIdx )
{
	if(nLayerIdx<0 || nLayerIdx>=GetLayerCount())return -1;

	int dataID = GetLayer(nLayerIdx)->AddPoint(x, y, nWhich);

/*	CAxis *axis = CXYPlotImpl::GetXMainAxis();
	if(axis)
	{
		axis->SetRangeSet(false);
	}
*/
	SetXRangeSet(false);

	double xRange[2],yRange[2];
	GetPlotRange(xRange,yRange);

	return dataID;
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::SetLRAxis()
{
	ResizePlots(2);
	
	CAxisImpl *axis;
	GetLayer(0)->DeleteAxisByLocation(kLocationRight);
	axis = GetLayer(0)->AddAxis(kLocationLeft);
	
	GetLayer(1)->DeleteAxisByLocation(kLocationLeft);
	axis = GetLayer(1)->AddAxis(kLocationRight);
}
/*
template<class PlotLayerImplIT>
SIZE	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetAxisSize( HDC hDC, int location )
{
	SIZE axisSize={0,0};
	SIZE dispSize,paddingSize,maxPaddingSize;
	int i;
	CAxisImpl *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
		axis=GetAxisByLocation(location);
		if(axis)
		{
			dispSize = axis->GetMinDisplaySize( hDC );
			paddingSize = axis->GetPaddingSize( hDC );
			axisSize = dispSize;
			axisSize.cx += (paddingSize.cx+paddingSize.cy);
			axisSize.cy += axis->GetAxisGap();
		}
		break;
	case kLocationLeft:
	case kLocationRight:
		for(i=0;i<GetLayerCount();i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			SIZE tmp={0,0};
			maxPaddingSize=tmp;
			axis=m_vpLayers[i]->GetAxisByLocation(location);
			if(axis)
			{
				dispSize = axis->GetMinDisplaySize( hDC );
				paddingSize = axis->GetPaddingSize( hDC );
				axisSize.cx += dispSize.cx;
				if( dispSize.cy > axisSize.cy ) axisSize.cy = dispSize.cy;
				if( paddingSize.cx > maxPaddingSize.cx ) maxPaddingSize.cx = paddingSize.cx;
				if( paddingSize.cy > maxPaddingSize.cy ) maxPaddingSize.cy = paddingSize.cy;

				axisSize.cx += axis->GetAxisGap();
			}
		}
		axisSize.cy+=(maxPaddingSize.cx+maxPaddingSize.cy);
		break;
	default:
		break;
	}
	return axisSize;
}

template<class PlotLayerImplIT>
SIZE	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetAxisPaddingSize( HDC hDC, int location )
{
	SIZE paddingSize,maxPaddingSize={0,0};
	int i;
	CAxisImpl *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
		axis=GetAxisByLocation(location);
		if(axis)
		{
			maxPaddingSize = axis->GetPaddingSize( hDC );
		}
		break;
	case kLocationLeft:
	case kLocationRight:
		for(i=0;i<GetLayerCount();i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			axis=m_vpLayers[i]->GetAxisByLocation(location);
			if(axis)
			{
				paddingSize = axis->GetPaddingSize( hDC );

				if( paddingSize.cx > maxPaddingSize.cx ) maxPaddingSize.cx = paddingSize.cx;
				if( paddingSize.cy > maxPaddingSize.cy ) maxPaddingSize.cy = paddingSize.cy;
			}
		}
		break;
	default:
		break;
	}
	return maxPaddingSize;
}
*/
/*
template<class PlotLayerImplIT>
RECT	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetPlotRect( HDC hDC, RECT clientRect)
{
	//////////////////////////////////////////////////////////////
	// Get Plot Rect
	int i;

	CAxisImpl *axis;
	
	RECT plotRect=clientRect;
	SIZE sizeRect={0,0},sizeTmp={0,0};
	
	// First title
	plotRect = AdjustByTitle(hDC, plotRect);
	
	// then top axis
	sizeRect = GetAxisSize( hDC, kLocationTop);
	plotRect.top+=sizeRect.cy;
	
	// then bottom axis
	sizeRect = GetAxisSize( hDC, kLocationBottom);
	plotRect.bottom-=sizeRect.cy;
	
	// then left axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		axis = m_vpLayers[i]->GetAxisByLocation(kLocationLeft);
		if(axis)
		{
			if(axis->IsVisible())
			{
				sizeTmp=m_vpLayers[i]->GetAxisSize( hDC, kLocationLeft );
				sizeRect.cx+=sizeTmp.cx;
			}
		}
		
	}
	plotRect.left+=sizeRect.cx;
	
	// then right axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		axis = m_vpLayers[i]->GetAxisByLocation(kLocationRight);
		
		if(axis)
		{
			if(axis->IsVisible())
			{
				sizeTmp=m_vpLayers[i]->GetAxisSize( hDC, kLocationRight );
				sizeRect.cx+=sizeTmp.cx;
			}
		}
	}
	plotRect.right-=sizeRect.cx;

	// the right padding size
	SIZE paddingSize;
	bool exist;
	exist = false;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		if( m_vpLayers[i]->GetAxisByLocation(kLocationRight) )
		{
			exist = true;
			break;
		}
	}
	if(!exist)
	{
		paddingSize = GetAxisPaddingSize( hDC, kLocationBottom);
		plotRect.right -= paddingSize.cy;
	}
	// the left padding size
	exist = false;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		if( m_vpLayers[i]->GetAxisByLocation(kLocationLeft) )
		{
			exist = true;
			break;
		}
	}
	if(!exist)
	{
		paddingSize = GetAxisPaddingSize( hDC, kLocationBottom);
		plotRect.left += paddingSize.cx;
	}
	
	// header and footnote
	plotRect = AdjustPlotSizeForHeadernote(hDC, plotRect);
	plotRect = AdjustPlotSizeForFootnote(hDC, plotRect);
	// legend
	plotRect = AddtionalPlotRectAdjust(hDC, plotRect);
	// Add spacing
	//InflateRect( &plotRect, -10, -10 );
	
	return plotRect;
}
*/
template<class PlotLayerImplIT>
RECT	CLayeredPlotBasicImpl<PlotLayerImplIT>::AddtionalPlotRectAdjust(HDC hDC, RECT plotRect)
{
	return CLegendImpl<CLayeredPlotBasicImpl>::AdjustByLegend(hDC, plotRect);
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::FinalDraw( HDC hDC, RECT destRect)
{
	if(GetLayerCount()==0)
	{
		DrawBkgnd( hDC);
		return;
	}
	int i;
	if(m_bUniteLayerColor)
	{
		for(i=0; i<GetLayerCount(); i++)
		{
			m_vpLayers[i]->UniteToColor(m_pColorTable[i%m_nColorsInTable]);
		}
	}

	// Get number of left axis and right axis
	if(IsRangeUpdateNeeded())
	{
		double xRange[2], yRange[2];
		GetPlotRange(xRange,yRange);
		SetLastPlotRange(xRange, yRange);
	}

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

	DrawBasic(hdc, destRect);
	//if there is no data, doing nothing
	if(!NeedDrawing())
	{
//		LeaveCriticalSection(&m_csPlot);
		return;
	}

	// get plot rect
	RECT plotRect=GetPlotRect(hdc,destRect);

	
	// Draw each layer,only draw curve
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		m_vpLayers[i]->FinalDraw( hdc, plotRect );
		m_vpLayers[i]->SetLastClientRect(destRect);
		m_vpLayers[i]->SetLastPlotRect(plotRect);
		//m_vpLayers[i]->GetPlotRange(xRange, yRange);
		//m_vpLayers[i]->DrawData( hDC, plotRect, xRange, yRange );
	}

	// Legend should be drawn after the datasets have drawn for the setting of line color etc
	// Draw Legend
	if(IsLegendShow())DrawLegend(hdc);

	// Header and footnote
	if(m_bShowHeadernote)
	{
		DrawHeadernote(hdc);
	}
	if(m_bShowFootnote)
	{
		DrawFootnote(hdc);
	}
	
	// User drawing actions
	if(m_fcnUserDrawing)
	{
		(*m_fcnUserDrawing)(this, hdc, destRect, plotRect, m_pParaUserDrawing);
	}

	// User update actions
	if(m_fcnUserUpdate)
	{
		(*m_fcnUserUpdate)(this, m_pParaUserUpdate);
	}
	for(i=0; i<(int)m_vFcnUserUpdate.size(); i++)
	{
		if(m_vFcnUserUpdate[i].fcn)
		{
			(*m_vFcnUserUpdate[i].fcn)(this, m_vFcnUserUpdate[i].pPara);
		}
	}

//	LeaveCriticalSection(&m_csPlot);

	SetLastClientRect(destRect);
	SetLastPlotRect(plotRect);
}
/*
template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::DrawBasic( HDC hDC, RECT destRect )
{
	int i;
	
	// Get number of left axis and right axis
	CAxisImpl *axis=NULL;

	// get plot rect
	RECT plotRect=GetPlotRect(hDC,destRect);
	SetLastClientRect(destRect);
	SetLastPlotRect(plotRect);

	// Erase the background using parent function
	EraseBkgnd( hDC );

	// First Draw Frame
	DrawFrame(hDC);

	// Draw the grids using the parent function
	DrawGrids( hDC );

	// The draw title
	//RECT titleRect = GetTitleRect( hDC, destRect, plotRect);
	if(GetTop()<0 || m_bShowParentTitle)
	{
		DrawTitle(hDC);
	}
	else if(GetTop()<GetLayerCount())
	{
		tstring title = GetTitle();
		SetTitle(GetLayer(GetTop())->GetTitle());
		DrawTitle(hDC);
		SetTitle(title);
	}

	// then bottom and top axis;
	axis=GetAxisByLocation(kLocationBottom);
	if(axis)
	{
		axis->OnDraw(hDC);
	}
	axis=GetAxisByLocation(kLocationTop);
	if(axis)
	{
		axis->OnDraw(hDC);
	}
	// then left and right axis;
	SIZE axisSize;
	RECT newRect=plotRect;
	
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
//		if(!m_vpLayers[i]->NeedDrawing())continue;

		axis=m_vpLayers[i]->GetAxisByLocation(kLocationLeft);
		{
			if(axis)
			{
				if(axis->IsVisible())
				{
					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(newRect);
					axis->OnDraw(hDC);
					
					axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationLeft );
					newRect.left-=axisSize.cx;
				}
				
			}
		}
		
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationRight);
		{
			if(axis)
			{
				if(axis->IsVisible())
				{
					m_vpLayers[i]->SetLastClientRect(destRect);
					m_vpLayers[i]->SetLastPlotRect(newRect);
					axis->OnDraw(hDC);
					
					axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationRight );
					newRect.right+=axisSize.cx;
				}
				
			}
		}
		m_vpLayers[i]->SetLastPlotRect(plotRect);
	}
}
*/
template<class PlotLayerImplIT>
CPlotData<DataPoint2D>	*CLayeredPlotBasicImpl<PlotLayerImplIT>::GetPlotDataByTitle( tstring title )
{
	CPlotData<DataPoint2D>	*pDataSet = 0;
	for(int i=0; i<GetLayerCount(); i++)
	{
		pDataSet = GetLayer(i)->GetPlotDataByTitle(title);
		if(pDataSet)return pDataSet;
	}
	return 0;
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::DeleteAllData()
{
	for(int i=0; i<GetLayerCount(); i++)
	{
		GetLayer(i)->DeleteAllData();
	}
}

template<class PlotLayerImplIT>
double	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetValueFromX(double x, int which, int nIndex)
{
	CPlotLayerImplI *pLayer=GetLayer(nIndex);
	if(!pLayer)return 0.0;
	return pLayer->GetValueFromX(x, which);
}

template<class PlotLayerImplIT>
tstring		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetAbsDataTitle( int which )
{
	int dataID, idxOfLayer, idxInlayer;
	dataID = GetLocalIdx(which, idxOfLayer, idxInlayer);
	if(dataID<0)
	{
		return _TEXT("");
	}
	else
	{
		return GetLayer(idxOfLayer)->GetAbsDataTitle(idxInlayer);
	}
}

template<class PlotLayerImplIT>
tstring		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetAbsDataAppendix( int which )
{
	int dataID, idxOfLayer, idxInlayer;
	dataID = GetLocalIdx(which, idxOfLayer, idxInlayer);
	if(dataID<0)
	{
		return _TEXT("");
	}
	else
	{
		return GetLayer(idxOfLayer)->GetAbsDataAppendix(idxInlayer);
	}
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::IsAxisLighted()
{
	if(CPlotImpl::IsAxisLighted())return true;
	
	for(int i=0;i<GetLayerCount();i++)
	{
		if(m_vpLayers[i]->IsAxisLighted())return true;
	}
	
	return false;
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::UnlightAllAxis()
{
	CPlotImpl::UnlightAllAxis();
	
	for(int i=0;i<GetLayerCount();i++)
	{
		m_vpLayers[i]->UnlightAllAxis();
	}
	
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::IsAxisSelected()
{
	if(CPlotImpl::IsAxisSelected())return true;
	
	for(int i=0;i<GetLayerCount();i++)
	{
		if(m_vpLayers[i]->IsAxisSelected())return true;
	}
	
	return false;
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::UnselectAllAxis()
{
	CPlotImpl::UnselectAllAxis();
	
	for(int i=0;i<GetLayerCount();i++)
	{
		m_vpLayers[i]->UnselectAllAxis();
	}
	
}
/*
template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::RegionIdentify(HDC hDC,POINT point)
{
	//CAxis *pAxis=0;
	RECT theRect;
	
	//data region
	theRect=GetLastPlotRect( );
	if( PtInRect(&theRect, point) )
		return kXYRegionData;
	//title region
	theRect=GetTitleRect(hDC);
	if( PtInRect(&theRect, point) )
		return kXYRegionTitle;
	//top and bottom axis region
	CAxisImpl *axis;
	for(int i=0;i<CPlotImpl::GetAxesCount();i++)
	{
		axis=CPlotImpl::GetAxis(i);
		theRect=axis->GetPlotRect(hDC);
		if( PtInRect(&theRect, point) )
		{
			if(axis->GetLocation()==kLocationBottom)return kXYRegionBottom;
			if(axis->GetLocation()==kLocationTop)return kXYRegionTop;
		}
	}
	
	//left and right axis region
	theRect=GetLastPlotRect( );
	SIZE axisSize,padddingSize;
	RECT axisRect;
	
	axisSize = GetAxisSize(hDC,kLocationLeft);
	padddingSize = GetAxisPaddingSize(hDC, kLocationLeft);
	axisRect.right = theRect.left;
	axisRect.left = axisRect.right - axisSize.cx;
	axisRect.top = theRect.top - padddingSize.cx;
	axisRect.bottom = theRect.bottom + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionLeft;
	
	axisSize = GetAxisSize(hDC,kLocationRight);
	padddingSize = GetAxisPaddingSize(hDC, kLocationRight);
	axisRect.left = theRect.right;
	axisRect.right = axisRect.left + axisSize.cx;
	axisRect.top = theRect.top - padddingSize.cx;
	axisRect.bottom = theRect.bottom + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionRight;
	
	return kXYRegionOut;
}
*/
template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::LightByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer )
{
	if( RegionIdentify(hDC,point) != kXYRegionData )
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		return -1;
	}
	
	int n=LegendRegionIdentify(hDC,point);
	int dataID;
	if(n>=0)
	{
		dataID=GetLocalIdx(n,idxOfLayer,idxInLayer);
		if(dataID<0)
		{
			return -1;
		}
		goto LightCurveByPoint;
	}
	
	dataID= GetNeareastPlotData( hDC, point, idxOfLayer, idxInLayer );
LightCurveByPoint:
	if(dataID>=0)
	{
		((CPlotLayerImplI *)m_vpLayers[idxOfLayer])->SetLighted(dataID,true);
		return dataID;
	}
	else
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		UnlightAllPlotData();
		return -1;
	}
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::SelectByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer )
{
	if( RegionIdentify(hDC,point) != kXYRegionData )
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		return -1;
	}
	
	int n=LegendRegionIdentify(hDC,point);
	int dataID;
	if(n>=0)
	{
		dataID=GetLocalIdx(n,idxOfLayer,idxInLayer);
		if(dataID<0)
		{
			return -1;
		}
		goto SelectCurveByPoint;
	}
	
	dataID = GetNeareastPlotData( hDC, point, idxOfLayer, idxInLayer );
	
SelectCurveByPoint:
	if(dataID>=0)
	{
		((CPlotLayerImplI *)m_vpLayers[idxOfLayer])->SetSelected(dataID,!((CPlotLayerImplI *)m_vpLayers[idxOfLayer])->IsSelected(dataID));
		return dataID;
	}
	else
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		SelectAllPlotData(false);
		return -1;
	}
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetNeareastPlotData( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer )
{
	int dataID;
	for(int i=0;i<GetLayerCount();i++)
	{
		dataID=m_vpLayers[i]->GetNeareastPlotData(hDC,point);
		if(dataID>=0)
		{
			idxOfLayer=i;
			idxInLayer=((CPlotLayerImplI *)m_vpLayers[idxOfLayer])->GetIndex(dataID);
			if(idxInLayer>=0)return dataID;
		}
	}
	idxOfLayer=-1;
	idxInLayer=-1;
	return -1;
}
/*
template<class PlotLayerImplIT>
RECT	CLayeredPlotBasicImpl<PlotLayerImplIT>::GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect )
{
	CAxisImpl *axis;
	SIZE axisSize,paddingSize;
	RECT axisRect={0,0,0,0};
	RECT newRect=plotRect;
	
	if(!m_vpLayers[idxOfLayer]->NeedDrawing())return axisRect;
	if(location==kLocationBottom || location==kLocationTop)return axisRect;
	
	for(int i=0;i<idxOfLayer;i++)
	{
		if(!m_vpLayers[i]->NeedDrawing())continue;
		
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationLeft);
		{
			if(axis)
			{
				axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationLeft );
				newRect.left-=axisSize.cx;
			}
		}
		axis=m_vpLayers[i]->GetAxisByLocation(kLocationRight);
		{
			if(axis)
			{
				axisSize=m_vpLayers[i]->GetAxisSize( hDC, kLocationRight );
				newRect.right+=axisSize.cx;
			}
		}
	}
	
	axis=m_vpLayers[idxOfLayer]->GetAxisByLocation(location);
	{
		if(axis)
		{
			axisSize = axis->GetMinDisplaySize( hDC );
			paddingSize = axis->GetPaddingSize( hDC );
			
			axisRect=newRect;
			if(location==kLocationLeft)
			{
				axisRect.right=newRect.left;
				axisRect.left=axisRect.right-axisSize.cx;
			}
			else
			{
				axisRect.left=newRect.right;
				axisRect.right=axisRect.left+axisSize.cx;
			}
			axisRect.bottom+=paddingSize.cy;
			axisRect.top-=paddingSize.cx;
		}
	}
	return axisRect;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIdxOfLayerByPointInAxisRegion(HDC hDC, RECT plotRect, POINT point)
{
	int i,region=RegionIdentify(hDC,point);
	RECT axisRect;
	if( region==kXYRegionLeft )
	{
		for(i=0;i<GetLayerCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationLeft,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	else if( region==kXYRegionRight )
	{
		for(i=0;i<GetLayerCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationRight,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	return -1;
}
*/
template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::IsPlotDataLighted()
{
	for(int i=0;i<GetLayerCount();i++)
		if(m_vpLayers[i]->IsPlotDataLighted())return true;
	return false;
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::IsPlotDataSelected()
{
	for(int i=0;i<GetLayerCount();i++)
		if(m_vpLayers[i]->IsPlotDataSelected())return true;
	return false;
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::UnlightAllPlotData()
{
	for(int i=0;i<GetLayerCount();i++)
		m_vpLayers[i]->UnlightAllPlotData();
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::SelectAllPlotData(bool bSel)
{
	for(int i=0;i<GetLayerCount();i++)
		m_vpLayers[i]->SelectAllPlotData(bSel);
}

/*
bool	CLayeredPlotBasicImpl::IsLayerSelected()
{
	bool bSel = false;
	for(int i=0; i<GetLayersCount(); i++)
	{
		if(GetLayer(i)->IsPlotSelected())
		{
			bSel = true;
			break;
		}
	}
	return bSel;
}

bool	CLayeredPlotBasicImpl::IsPlotSelected()
{
	return IsSelectTitle() || IsLayerSelected();
}
*/

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::CheckLogSafety()
{
	if(!CheckLogSafety(true))return false;
	if(!CheckLogSafety(false))return false;
	return true;
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::CheckLogSafety(bool bX)
{
	int i;
	if(bX)
	{
		for(i=0; i<GetLayerCount(); i++)
		{
			if(GetLayer(i)->IsXLogarithm() != CPlotImpl::IsXLogarithm())return false;
			if(GetLayer(i)->IsXLogarithm() && !GetLayer(i)->CheckLogSafety(bX))return false;
		}
	}
	else
	{
		for(i=0; i<GetLayerCount(); i++)
		{
			if(GetLayer(i)->IsYLogarithm() && !GetLayer(i)->CheckLogSafety(bX))return false;
		}
	}
	return true;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetAxesCount( )
{
	CPlotImpl *pPlot;
	int location;
	int i, j, count = 0;
	for(i=0; i<=GetLayerCount(); i++)
	{
		if(i == GetLayerCount())
		{
			pPlot = this;
			for(j=0; j<CPlotImpl::GetAxesCount(); j++)
			{
				location = pPlot->GetAxis(j)->GetLocation();
				if(location == kLocationBottom || location == kLocationTop
					|| location == kLocationCenterHB || location == kLocationCenterHT)
				{
					count++;
				}
			}
		}
		else
		{
			pPlot = GetLayer(i);
			for(j=0; j<pPlot->GetAxesCount(); j++)
			{
				location = pPlot->GetAxis(j)->GetLocation();
				if(location == kLocationLeft || location == kLocationRight
					|| location == kLocationCenterVL || location == kLocationCenterVR)
				{
					count++;
				}
			}
		}
	}
	
	return count;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetVisibleAxisCount( )
{
	CPlotImpl *pPlot;
	int location;
	int i, j, count = 0;
	for(i=0; i<=GetLayerCount(); i++)
	{
		if(i == GetLayerCount())
		{
			pPlot = this;
			for(j=0; j<CPlotImpl::GetAxesCount(); j++)
			{
				if(!pPlot->GetAxis(j)->IsVisible())continue;
				
				location = pPlot->GetAxis(j)->GetLocation();
				if(location == kLocationBottom || location == kLocationTop
					|| location == kLocationCenterHB || location == kLocationCenterHT)
				{
					count++;
				}
			}
		}
		else
		{
			pPlot = GetLayer(i);
			for(j=0; j<pPlot->GetAxesCount(); j++)
			{
				if(!pPlot->GetAxis(j)->IsVisible())continue;
				
				location = pPlot->GetAxis(j)->GetLocation();
				if(location == kLocationLeft || location == kLocationRight
					|| location == kLocationCenterVL || location == kLocationCenterVR)
				{
					count++;
				}
			}
		}
	}
	
	return count;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfLightedAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;

	CPlotImpl *pPlot;
	int i, j;
	for(i=0; i<=GetLayerCount(); i++)
	{
		if(i==GetLayerCount())
		{
			pPlot = this;
		}
		else
		{
			pPlot = GetLayer(i);
		}
		for(j=0; j<pPlot->GetAxesCount(); j++)
		{
			if(pPlot->GetAxis(j)->IsLighted())
			{
				idxOfLayer = i;
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	return idxInLayer;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfSelectedAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	CPlotImpl *pPlot;
	int i, j;
	for(i=0; i<=GetLayerCount(); i++)
	{
		if(i==GetLayerCount())
		{
			pPlot = this;
		}
		else
		{
			pPlot = GetLayer(i);
		}
		for(j=0; j<pPlot->GetAxesCount(); j++)
		{
			if(pPlot->GetAxis(j)->IsSelected())
			{
				idxOfLayer = i;
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	return idxInLayer;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfLightedPlotData(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=0; i<GetLayerCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetPlotDataCount(); j++)
		{
			if(GetLayer(i)->IsLighted(j))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfSelectedPlotData(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=0; i<GetLayerCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetPlotDataCount(); j++)
		{
			if(GetLayer(i)->IsSelected(j))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfFirstVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	int location;
	for(j=0; j<CPlotImpl::GetAxesCount(); j++)
	{
		location = CPlotImpl::GetAxis(j)->GetLocation();
		if(CPlotImpl::GetAxis(j)->IsVisible())
		{
			if(location == kLocationBottom
				|| location == kLocationTop
				|| location == kLocationCenterHT
				|| location == kLocationCenterHB)
			{
				idxOfLayer = GetLayerCount();
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	for(i=0; i<GetLayerCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetAxesCount(); j++)
		{
			location = GetLayer(i)->GetAxis(j)->GetLocation();
			if(GetLayer(i)->GetAxis(j)->IsVisible())
			{
				if(location == kLocationBottom
					|| location == kLocationTop
					|| location == kLocationCenterHT
					|| location == kLocationCenterHB)
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
			}
		}
	}
	return idxInLayer;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfLastVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	int location;
	for(i=GetLayerCount()-1; i>=0; i--)
	{
		for(j=GetLayer(i)->GetAxesCount()-1; j>=0; j--)
		{
			location = GetLayer(i)->GetAxis(j)->GetLocation();
			if(GetLayer(i)->GetAxis(j)->IsVisible())
			{
				if(location == kLocationBottom
					|| location == kLocationTop
					|| location == kLocationCenterHT
					|| location == kLocationCenterHB)
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
			}
		}
	}
	for(j=CPlotImpl::GetAxesCount()-1; j>=0; j--)
	{
		location = CPlotImpl::GetAxis(j)->GetLocation();
		if(CPlotImpl::GetAxis(j)->IsVisible())
		{
			if(location == kLocationBottom
				|| location == kLocationTop
				|| location == kLocationCenterHT
				|| location == kLocationCenterHB)
			{
				idxOfLayer = GetLayerCount();
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	return idxInLayer;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfFirstVisiblePlotData(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=0; i<GetLayerCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetPlotDataCount(); j++)
		{
			if(GetLayer(i)->IsVisible(GetLayer(i)->GetDataID(j)))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfLastVisiblePlotData(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=GetLayerCount()-1; i>=0; i--)
	{
		for(j=GetLayer(i)->GetPlotDataCount()-1; j>=0; j--)
		{
			if(GetLayer(i)->IsVisible(GetLayer(i)->GetDataID(j)))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfNextVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer > GetLayerCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == GetLayerCount()-1 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfLastVisibleAxis())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int location;
		int i, j;
		if(idxOfLayer == GetLayerCount())
		{
			j = idxInLayer<0?0:idxInLayer+1;
			while(j<CPlotImpl::GetAxesCount())
			{
				location = CPlotImpl::GetAxis(j)->GetLocation();
				if(location == kLocationBottom
					|| location == kLocationTop
					|| location == kLocationCenterHB
					|| location == kLocationCenterHT)
				{
					if(CPlotImpl::GetAxis(j)->IsVisible())
					{
						//idxOfLayer = i;
						idxInLayer = j;
						return idxInLayer;
					}
					
				}
				j++;
			}
		}
		i = ( idxOfLayer<0 || idxOfLayer == GetLayerCount() )?0:idxOfLayer;
		j = (idxOfLayer == GetLayerCount())?0:(idxInLayer<0?0:idxInLayer+1);
		while(i<GetLayerCount())
		{
			while(j < GetLayer(i)->GetAxesCount())
			{
				location = GetLayer(i)->GetAxis(j)->GetLocation();
				if(location == kLocationLeft
					|| location == kLocationRight
					|| location == kLocationCenterVL
					|| location == kLocationCenterVR)
				{
					if(GetLayer(i)->GetAxis(j)->IsVisible())
					{
						idxOfLayer = i;
						idxInLayer = j;
						return idxInLayer;
					}
				}
				j++;
			}
			i++;
			j=0;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfPrevVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer >= GetLayerCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == 0 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfFirstVisibleAxis())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int location;
		int i, j;
		
		if(idxOfLayer != GetLayerCount())
		{
			i = idxOfLayer<0 ?0:idxOfLayer;
			j = idxInLayer<0?GetLayer(idxOfLayer)->GetAxesCount()-1:idxInLayer-1;
			while(i>=0)
			{
				while(j>=0)
				{
					location = GetLayer(i)->GetAxis(j)->GetLocation();
					if(location == kLocationLeft
						|| location == kLocationRight
						|| location == kLocationCenterVL
						|| location == kLocationCenterVR)
					{
						if(GetLayer(i)->GetAxis(j)->IsVisible())
						{
							idxOfLayer = i;
							idxInLayer = j;
							return idxInLayer;
						}
					}
					j--;
				}
				i--;
				if(i>=0)
					j = GetLayer(i)->GetAxesCount()-1;
			}
		}
		
		j = idxInLayer<0?CPlotImpl::GetAxesCount()-1:idxInLayer-1;
		while(j>=0)
		{
			location = CPlotImpl::GetAxis(j)->GetLocation();
			if(location == kLocationBottom
				|| location == kLocationTop
				|| location == kLocationCenterHB
				|| location == kLocationCenterHT)
			{
				if(CPlotImpl::GetAxis(j)->IsVisible())
				{
					idxOfLayer = GetLayerCount();
					idxInLayer = j;
					return idxInLayer;
				}
			}
			j--;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfNextVisiblePlotData(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer >= GetPlotDataCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == GetPlotDataCount()-1 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfLastVisiblePlotData())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int i, j;
		i = idxOfLayer<0?0:idxOfLayer;
		j = idxInLayer<0?0:idxInLayer+1;
		while(i<GetLayerCount())
		{
			while(j<GetLayerCount())
			{
				if(GetLayer(i)->IsVisible(j))
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
				j++;
			}
			i++;
			j=0;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}

template<class PlotLayerImplIT>
int		CLayeredPlotBasicImpl<PlotLayerImplIT>::GetIndexOfPrevVisiblePlotData(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer >= GetPlotDataCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == 0 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfFirstVisiblePlotData())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int i, j;
		i = idxOfLayer<0?GetLayerCount()-1:idxOfLayer;
		j = idxInLayer<0?GetLayer(i)->GetPlotDataCount()-1:idxInLayer-1;
		while(i<GetLayerCount())
		{
			while(j<GetLayerCount())
			{
				if(GetLayer(i)->IsVisible(j))
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
				j--;
			}
			i--;
			if(i>=0)
				j = GetLayer(i)->GetPlotDataCount()-1;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::SelectAxis(int &idxOfLayer, int &idxInLayer, bool bSel)
{
	if(idxOfLayer == GetLayerCount())
	{
		if(idxInLayer>=0 && idxInLayer<CPlotImpl::GetAxesCount())
		{
			CPlotImpl::GetAxis(idxInLayer)->SetSelected(bSel);
			return true;
		}
		return false;
	}
	else if(idxOfLayer<0 || idxOfLayer>GetLayerCount())
	{
		return false;
	}
	else
	{
		if(idxInLayer>=0 && idxInLayer<GetLayer(idxOfLayer)->GetAxesCount())
		{
			GetLayer(idxOfLayer)->GetAxis(idxInLayer)->SetSelected(bSel);
			return true;
		}
		return false;
	}
}

template<class PlotLayerImplIT>
bool	CLayeredPlotBasicImpl<PlotLayerImplIT>::SelectPlotData(int &idxOfLayer, int &idxInLayer, bool bSel)
{
	if(idxOfLayer<0 || idxOfLayer>GetLayerCount())
	{
		return false;
	}
	else
	{
		if(idxInLayer>=0 && idxInLayer<GetLayer(idxOfLayer)->GetPlotDataCount())
		{
			GetLayer(idxOfLayer)->SetSelected(GetLayer(idxOfLayer)->GetDataID(idxInLayer),bSel);
			return true;
		}
		return false;
	}
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::MyLock()
{
	int i;
	for(i=0; i<GetLayerCount(); i++)
	{
		GetLayer(i)->MyLock();
	}
}

template<class PlotLayerImplIT>
void	CLayeredPlotBasicImpl<PlotLayerImplIT>::MyUnlock()
{
	int i;
	for(i=0; i<GetLayerCount(); i++)
	{
		GetLayer(i)->MyUnlock();
	}
}

Declare_Namespace_End
