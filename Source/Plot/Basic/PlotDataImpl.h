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
//#include "MyGdiPlus.h"
#include "PlotBasicImpl.h"
#include "PlotData.h"
#include "PlotDataFillImpl.h"
#include "PlotDataMarkerImpl.h"
#include "PlotDataBarImpl.h"
#include "PlotDataErrorBarImpl.h"
#include "PlotDataComplexColorImpl.h"
#include "PlotDataHistoImpl.h"
#include "PlotDataPointInfoImpl.h"
#include "PlotDataSpanImpl.h"

//#include "DataSet.h"

Declare_Namespace_CChart

template<typename DataT>
class CDataSet;

typedef void (AdjustDataRange)(int whichDim, vector<double> &vValue, double *range);

//class CPlotImpl;

template<typename DataT>
class	CPlotDataImpl : public CPlotData<DataT>,
						public CPlotDataFillImpl<CPlotDataImpl<DataT> >,
						public CPlotDataMarkerImpl<CPlotDataImpl<DataT> >,
						public CPlotDataBarImpl<CPlotDataImpl<DataT> >,
						public CPlotDataErrorBarImpl<CPlotDataImpl<DataT> >,
						public CPlotDataComplexColorImpl<CPlotDataImpl<DataT> >,
						public CPlotDataHistoImpl<CPlotDataImpl<DataT> >,
						public CPlotDataPointInfoImpl<CPlotDataImpl<DataT> >,
						public CPlotDataSpanImpl<CPlotDataImpl<DataT> >
{
public:
	CPlotDataImpl(CPlotBasicImpl *pT);
	virtual ~CPlotDataImpl();
protected:
	CPlotBasicImpl	*m_pPlot;
	CDataSet<DataT>	*m_pDataSet;

	vector<AdjustDataRange*>	m_vFcnAdjustDataRange;
public:
	inline	CPlotBasicImpl	*GetPlot(){return m_pPlot;}
	inline	CDataSet<DataT>	*GetDataSet(){return m_pDataSet;}
	inline	void		SetDataSet(CDataSet<DataT> *pDataSet){m_pDataSet = pDataSet;}
	vector<AdjustDataRange*>	&GetFcnAdjustDataRange(){return m_vFcnAdjustDataRange;}

	void	GetDataRange();

protected:
	void	GetDataRange(int whichDim);
	bool	CreateClipData(int whichDim, double *range);

//the following functions are for 2D only
protected:
	int		CreateCompactData(RECT plotRect, double *xRange, double *yRange);
	bool	IsDataScaleNeeded(RECT plotRect, double *xRange, double *yRange);
	bool	CreateScreenPoints( HDC hDC, RECT plotRect, double *xRange, double *yRange, POINT **myPoints, int &nPts, int &nPts4Fill );

	void	DrawCurveOnly( HDC hDC, RECT plotRect, double *xRange, double *yRange );
	void	DrawSegHided( HDC hDC, RECT plotRect, double *xRange, double *yRange );
	void	DrawPlotDataInfo(HDC hDC, RECT plotRect, double *xRange, double *yRange );

public:
	void	DrawPlotData(HDC hDC, RECT plotRect, double *xRange, double *yRange);
};

template<typename DataT>
CPlotDataImpl<DataT>::CPlotDataImpl(CPlotBasicImpl *pT)
{
	m_pPlot = pT;
	m_pDataSet = 0;
}

template<typename DataT>
CPlotDataImpl<DataT>::~CPlotDataImpl()
{
	
}

template<typename DataT>
void	CPlotDataImpl<DataT>::GetDataRange(  )
{
	int nDataDim = sizeof(DataT)/sizeof(double);

	if(m_bDataRangeSet == false)
	{
		m_bDataRangeSet = true;
		
		int i;
		for(i=0; i<nDataDim; i++)
		{
			GetDataRange(i);
		}
	}
}

template<typename DataT>
void	CPlotDataImpl<DataT>::GetDataRange(int whichDim)
{
	int nDataDim = sizeof(DataT)/sizeof(double);
	if(whichDim<0 || whichDim>=nDataDim)return;

	int			j;
	double		minV, maxV;
	double		lb = 0.0, ub = 0.0;
	vector<double> vVal;

	minV = fMaxVal;
	maxV = -fMaxVal;

	for( j = 0; j < GetDataCount(); j++)
	{
		if( (*m_pvData)[j].val[whichDim] < minV ) minV = (*m_pvData)[j].val[whichDim];
		if( (*m_pvData)[j].val[whichDim] > maxV ) maxV = (*m_pvData)[j].val[whichDim];

		vVal.push_back((*m_pvData)[j].val[whichDim]);
	}
	m_ppfDataRange[whichDim][0] = minV;
	m_ppfDataRange[whichDim][1] = maxV;
	
	for(j=0; j<(int)m_vFcnAdjustDataRange.size(); j++)
	{
		if(m_vFcnAdjustDataRange[j])m_vFcnAdjustDataRange[j](whichDim, vVal, m_ppfDataRange[whichDim]);
	}
	MyAdjustDataErrorBar(whichDim, vVal, m_ppfDataRange[whichDim]);//暂时这样，把函数作为指针push进去就出问题
}

template<typename DataT>
bool	CPlotDataImpl<DataT>::CreateClipData(int whichDim, double *range)
{
//	int nDataDim = sizeof(DataT)/sizeof(double);
	if(whichDim<0 || whichDim>=sizeof(DataT)/sizeof(double))return false;

	if(!m_bSorted)
	{
		switch(whichDim)
		{
		case 0:
			m_nSortType = kSortXInc;
			break;
		case 1:
			m_nSortType = kSortYInc;
			break;
		case 2:
			m_nSortType = kSortZInc;
			break;
		default:
			m_nSortType = kSortXInc;
		}
		SortData();
		m_bSorted = true;
	}
	int i, i1, i2;
	i1 = (int)m_pvData->size();
	i2 = -1;
	for(i=0; i<(int)m_pvData->size(); i++)
	{
		if((*m_pvData)[i].val[whichDim]>=range[0])
		{
			i1 = i;
			break;
		}
	}
	for(i=(int)m_pvData->size()-1; i>=0; i--)
	{
		if((*m_pvData)[i].val[whichDim]<=range[1])
		{
			i2 = i;
			break;
		}
	}
	if(i1 == m_pvData->size() || i2 == -1 )
	{
		i1 = i2 = -1;
	}
	else
	{
		if(i1>0)i1--;
		if(i2<(int)m_pvData->size()-1)i2++;
	}

	if(i1==i2)return false;

	pnClipRange[0] = i1;
	pnClipRange[1] = i2;

	bCompactDraw = true;
	vDrawData.clear();
	for(i=i1; i<=i2; i++)
	{
		vDrawData.push_back((*m_pvData)[i]);
	}
	return true;
}

//template<typename DataT>
//int	CPlotDataImpl<DataT>::CreateCompactData(RECT plotRect, double *xRange, double *yRange);
int	CPlotDataImpl<DataPoint2D>::CreateCompactData(RECT plotRect, double *xRange, double *yRange);

//template<typename DataT>
//bool	CPlotDataImpl<DataT>::IsDataScaleNeeded(RECT plotRect, double *xRange, double *yRange);
bool	CPlotDataImpl<DataPoint2D>::IsDataScaleNeeded(RECT plotRect, double *xRange, double *yRange);

//template<typename DataT>
//bool	CPlotDataImpl<DataT>::CreateScreenPoints( HDC hDC, RECT plotRect, double *xRange, double *yRange, POINT **myPoints, int &nPts, int &nPts4Fill );
bool	CPlotDataImpl<DataPoint2D>::CreateScreenPoints( HDC hDC, RECT plotRect, double *xRange, double *yRange, POINT **myPoints, int &nPts, int &nPts4Fill );

//void	MyDrawLineCrPtByPt(HDC hDC, int plottype, std::vector<COLORREF> vColor, int lineSize, int lineStyle, POINT *mypoints, int nPts);
//void	MyDrawLineCrMulti(HDC hDC, CPlotImpl *pPlot, RECT plotRect, int xScale, int yScale, double *xRange, double *yRange, std::vector<SegmentColor> vSegColor, int nMultiColorInputType, int lineSize, int lineStyle, POINT *mypoints, int nPts);
//void	MyDrawLineCrBi(HDC hDC, CPlotImpl *pPlot, RECT plotRect, int xScale, int yScale, double *xRange, double *yRange, double *xDataRange, double *yDataRange, COLORREF crDataColor1, COLORREF crDataColor2, int nMultiColorInputType, int lineSize, int lineStyle, POINT *mypoints, int nPts);
//void	MyDrawPolyline(HDC hDC, COLORREF color, int lineSize, int lineStyle, POINT *mypoints, int nPts, bool autoSmooth);
//void	MyDrawBeziers(HDC hDC, COLORREF color, int lineSize, int lineStyle, POINT *mypoints, int nPts);

//template<typename DataT>
//void	CPlotDataImpl<DataT>::DrawCurveOnly( HDC hDC, RECT plotRect, double *xRange, double *yRange );
void	CPlotDataImpl<DataPoint2D>::DrawCurveOnly( HDC hDC, RECT plotRect, double *xRange, double *yRange );

//template<typename DataT>
//void	CPlotDataImpl<DataT>::DrawSegHided( HDC hDC, RECT plotRect, double *xRange, double *yRange );
void	CPlotDataImpl<DataPoint2D>::DrawSegHided( HDC hDC, RECT plotRect, double *xRange, double *yRange );

//template<typename DataT>
//void	CPlotDataImpl<DataT>::DrawPlotDataInfo(HDC hDC, RECT plotRect, double *xRange, double *yRange );
void	CPlotDataImpl<DataPoint2D>::DrawPlotDataInfo(HDC hDC, RECT plotRect, double *xRange, double *yRange );

//template<typename DataT>
//void	CPlotDataImpl<DataT>::DrawPlotData(HDC hDC, RECT plotRect, double *xRange, double *yRange);
void	CPlotDataImpl<DataPoint2D>::DrawPlotData(HDC hDC, RECT plotRect, double *xRange, double *yRange);

Declare_Namespace_End
