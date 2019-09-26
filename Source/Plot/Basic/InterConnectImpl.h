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
#include "InterConnect.h"

Declare_Namespace_CChart

template<class DataSetT>
class CInterConnectImpl : public CInterConnect
{
public:
	CInterConnectImpl();
	virtual ~CInterConnectImpl();

public:
	void	DrawInterConnection( HDC hDC, RECT plotRect, double *xRange, double *yRange );
};

template<class DataSetT>
CInterConnectImpl<DataSetT>::CInterConnectImpl()
{

}

template<class DataSetT>
CInterConnectImpl<DataSetT>::~CInterConnectImpl()
{
	
}

void	MyDrawInterConnect(HDC hDC, int alpha, COLORREF lineColor, int lineSize, int lineStyle, POINT *pPoints1, POINT *pPoints2, int nDatas);

template<class DataSetT>
void	CInterConnectImpl<DataSetT>::DrawInterConnection( HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	DataSetT* pT = static_cast<DataSetT*>(this);

	if(pT->GetPlotDataCount() <= 1)return;
	int count=0;

	CPlotData<DataPoint2D>	*plotData1;
	CPlotData<DataPoint2D>	*plotData2;
	
	POINT *pPoints1, *pPoints2;
	
	int i, nDatas;

	while(true)
	{
		if(count+1 >= pT->GetPlotDataCount())break;

		plotData1 = pT->GetAbsPlotData(count);
		plotData2 = pT->GetAbsPlotData(count+1);

		nDatas = min(plotData1->GetDataCount(), plotData2->GetDataCount());

		if(nDatas<1)
		{
			count += 2;
			continue;
		}

		pPoints1 = plotData1->pPointsGdi;
		pPoints2 = plotData2->pPointsGdi;
		
		if( m_nInterConnectAlpha == 255 || m_nInterConnectLineSize == 1 || (m_nInterConnectLineSize != 1 && m_nInterConnectLineStyle == PS_SOLID) )
		{
			HPEN hPen, hOldPen;
			hPen = CreatePen(m_nInterConnectLineStyle, m_nInterConnectLineSize, m_crInterConnectLineColor);
			hOldPen = (HPEN)SelectObject(hDC, hPen);
			
			for(i=0; i<nDatas; i++)
			{
				MoveToEx(hDC, pPoints1[i].x, pPoints1[i].y, NULL);
				LineTo(hDC, pPoints2[i].x, pPoints2[i].y);
			}
			
			SelectObject(hDC, hOldPen);
			DeleteObject(hPen);
		}
		else
		{
			MyDrawInterConnect(hDC, m_nInterConnectAlpha, m_crInterConnectLineColor, m_nInterConnectLineSize, m_nInterConnectLineStyle, pPoints1, pPoints2, nDatas);
/*			Graphics graph(hDC);
			graph.SetSmoothingMode(SmoothingModeHighQuality);
			
			Color cr(m_nInterConnectAlpha, GetRValue(m_crInterConnectLineColor), GetGValue(m_crInterConnectLineColor), GetBValue(m_crInterConnectLineColor));
			Pen pen(cr, (Gdiplus::REAL)m_nInterConnectLineSize);
			pen.SetDashStyle((enum Gdiplus::DashStyle)m_nInterConnectLineStyle);
			
			Point pt1, pt2;
			
			for(i=0; i<nDatas; i++)
			{
				pt1.X = pPoints1[i].x;
				pt1.Y = pPoints1[i].y;
				pt2.X = pPoints2[i].x;
				pt2.Y = pPoints2[i].y;
				graph.DrawLine(&pen, pt1, pt2);
			}*/
		}

		count += 2;
	}
}

Declare_Namespace_End
