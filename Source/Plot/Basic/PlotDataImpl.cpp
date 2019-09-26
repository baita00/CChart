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

#include "MyGdiPlus.h"
#include "PlotDataImpl.h"
#include "PlotImpl.h"
#include "DataSetImpl.h"

Declare_Namespace_CChart

void	MyDrawLineCrPtByPt(HDC hDC, int plottype, std::vector<COLORREF> vColor, int lineSize, int lineStyle, POINT *mypoints, int nPts)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	int i;
	for(i=0; i<nPts-1; i++)
	{
		if(mypoints[i].x == mypoints[i+1].x && mypoints[i].y == mypoints[i+1].y)continue;
		
		Point *points = new Point[2];
		points[0].X = mypoints[i].x;
		points[0].Y = mypoints[i].y;
		points[1].X = mypoints[i+1].x;
		points[1].Y = mypoints[i+1].y;
		
		Color *colors=new Color[2];
		switch(plottype)
		{
		case kXYPlotConnect:
			colors[0].SetFromCOLORREF(vColor[i]);
			colors[1].SetFromCOLORREF(vColor[i+1]);
			break;
		case kXYPlotStepHV:
		case kXYPlotStepVH:
			colors[0].SetFromCOLORREF(vColor[i/2]);
			colors[1].SetFromCOLORREF(vColor[(i+1)/2]);
			break;
		case kXYPlotStepHVH:
		case kXYPlotStepVHV:
			colors[0].SetFromCOLORREF(vColor[(i+1)/3]);
			colors[1].SetFromCOLORREF(vColor[(i+2)/3]);
			break;
		}
		
		LinearGradientBrush lgBrush(points[0], points[1], colors[0], colors[1]);
		
		Pen Mypen(&lgBrush,lineSize);
		Mypen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
		Mypen.SetStartCap(LineCapRound);
		Mypen.SetEndCap(LineCapRound);
		graph.DrawLine(&Mypen, points[0], points[1]);
		
		delete []colors;
		delete []points;
	}
}

void	MyDrawLineCrMulti(HDC hDC, CPlotImpl *pPlot, RECT plotRect, int xScale, int yScale, double *xRange, double *yRange, std::vector<SegmentColor> vSegColor, int nMultiColorInputType, int lineSize, int lineStyle, POINT *mypoints, int nPts)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	COLORREF cr1, cr2;
	double val, rt1, rt2;
	byte r1, g1, b1, r2, g2, b2;
	int k;
	Point *points = new Point[2];
	Color *colors = new Color[2];
	
	int i, j;
	for(i=0; i<nPts-1; i++)
	{
		if(mypoints[i].x == mypoints[i+1].x && mypoints[i].y == mypoints[i+1].y)continue;
		
		for(k=0; k<2; k++)
		{
			points[k].X = mypoints[i+k].x;
			points[k].Y = mypoints[i+k].y;
			switch(nMultiColorInputType)
			{
			case kInputColorByPointRatio:
				val = (i+k)/(nPts-1.0);
				break;
			case kInputColorByXVal:
				if(!pPlot->IsXOpposite())
					val = (mypoints[(i+k)].x - plotRect.left)/xScale + xRange[0];
				else
					val = (plotRect.right - mypoints[(i+k)].x)/xScale + xRange[0];
				break;
			case kInputColorByYVal:
				if(!pPlot->IsYOpposite())
					val = (plotRect.bottom - mypoints[(i+k)].y)/yScale + yRange[0] ;
				else
					val = (mypoints[(i+k)].y - plotRect.top)/yScale + yRange[0] ;
				break;
			}
			if(val<=vSegColor[0].val)
			{
				cr1 = cr2 = vSegColor[0].color;
				colors[i].SetFromCOLORREF(cr1);
			}
			else if(val>=vSegColor[vSegColor.size()-1].val)
			{
				cr1 = cr2 = vSegColor[vSegColor.size()-1].color;
				colors[i].SetFromCOLORREF(cr1);
			}
			else
			{
				for(j=0; j<(int)vSegColor.size(); j++)
				{
					if(val>=vSegColor[j].val && val<vSegColor[j+1].val)
					{
						rt1 = vSegColor[j].val;
						rt2 = vSegColor[j+1].val;
						cr1 = vSegColor[j].color;
						cr2 = vSegColor[j+1].color;
						
						r1 = GetRValue(cr1);
						r2 = GetRValue(cr2);
						g1 = GetGValue(cr1);
						g2 = GetGValue(cr2);
						b1 = GetBValue(cr1);
						b2 = GetBValue(cr2);
						colors[k].SetFromCOLORREF( RGB(
							((rt2-val)*r1+(val-rt1)*r2)/(rt2-rt1),
							((rt2-val)*g1+(val-rt1)*g2)/(rt2-rt1),
							((rt2-val)*b1+(val-rt1)*b2)/(rt2-rt1) ) );
						break;
					}
				}
			}
		}
		
		LinearGradientBrush lgBrush(points[0], points[1], colors[0], colors[1]);
		
		Pen Mypen(&lgBrush,lineSize);
		Mypen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
		Mypen.SetStartCap(LineCapRound);
		Mypen.SetEndCap(LineCapRound);
		graph.DrawLine(&Mypen, points[0], points[1]);
	}
	delete []colors;
	delete []points;
}

void	MyDrawLineCrBi(HDC hDC, CPlotImpl *pPlot, RECT plotRect, int xScale, int yScale, double *xRange, double *yRange, double *xDataRange, double *yDataRange, COLORREF crDataColor1, COLORREF crDataColor2, int nMultiColorInputType, int lineSize, int lineStyle, POINT *mypoints, int nPts)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	COLORREF cr1, cr2;
	double val, rt1, rt2;
	int k;
	byte r1, g1, b1, r2, g2, b2;
	
	cr1 = crDataColor1;
	cr2 = crDataColor2;
	
	Point *points = new Point[2];
	Color *colors = new Color[2];
	
	int i;//, j;
	for(i=0; i<nPts-1; i++)
	{
		if(mypoints[i].x == mypoints[i+1].x && mypoints[i].y == mypoints[i+1].y)continue;
		
		for(k=0; k<2; k++)
		{
			points[k].X = mypoints[i+k].x;
			points[k].Y = mypoints[i+k].y;
			switch(nMultiColorInputType)
			{
			case kInputColorByPointRatio:
				rt1 = 0.0;
				rt2 = 1.0;
				val = (i+k)/(nPts-1.0);
				break;
			case kInputColorByXVal:
				rt1 = xDataRange[0];
				rt2 = xDataRange[1];
				if(!pPlot->IsXOpposite())
					val = (mypoints[(i+k)].x - plotRect.left)/xScale + xRange[0];
				else
					val = (plotRect.right - mypoints[(i+k)].x)/xScale + xRange[0];
				break;
			case kInputColorByYVal:
				rt1 = yDataRange[0];
				rt2 = yDataRange[1];
				if(!pPlot->IsYOpposite())
					val = (plotRect.bottom - mypoints[(i+k)].y)/yScale + yRange[0] ;
				else
					val = (mypoints[(i+k)].y - plotRect.top)/yScale + yRange[0] ;
				break;
			}
			r1 = GetRValue(cr1);
			r2 = GetRValue(cr2);
			g1 = GetGValue(cr1);
			g2 = GetGValue(cr2);
			b1 = GetBValue(cr1);
			b2 = GetBValue(cr2);
			colors[k].SetFromCOLORREF( RGB(
				((rt2-val)*r1+(val-rt1)*r2)/(rt2-rt1),
				((rt2-val)*g1+(val-rt1)*g2)/(rt2-rt1),
				((rt2-val)*b1+(val-rt1)*b2)/(rt2-rt1) ) );
			
			//					colors[k].SetFromCOLORREF( RGB(
			//						((rt2-val)*GetRValue(cr1)+(val-rt1)*GetRValue(cr2))/(rt2-rt1),
			//						((rt2-val)*GetGValue(cr1)+(val-rt1)*GetGValue(cr2))/(rt2-rt1),
			//						((rt2-val)*GetBValue(cr1)+(val-rt2)*GetBValue(cr2))/(rt2-rt1) ) );
		}
		
		LinearGradientBrush lgBrush(points[0], points[1], colors[0], colors[1]);
		
		Pen Mypen(&lgBrush,lineSize);
		Mypen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
		Mypen.SetStartCap(LineCapRound);
		Mypen.SetEndCap(LineCapRound);
		graph.DrawLine(&Mypen, points[0], points[1]);
	}
	delete []colors;
	delete []points;
}

void	MyDrawPolyline(HDC hDC, COLORREF color, int lineSize, int lineStyle, Point *mypoints, int nPts, bool autoSmooth)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color cr(GetRValue(color), GetGValue(color), GetBValue(color));
	Pen pen(cr, (Gdiplus::REAL)lineSize);
	pen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
	
/*	int i;
	Point *points = new Point[nPts];
	for(i=0; i<nPts; i++)
	{
		points[i].X = mypoints[i].x;
		points[i].Y = mypoints[i].y;
	}*/
	if(autoSmooth)
	{
		graph.DrawCurve(&pen, mypoints, nPts);
	}
	else
	{
		graph.DrawLines(&pen, mypoints, nPts);
	}
	
//	delete []points;
}

void	MyDrawBeziers(HDC hDC, COLORREF color, int lineSize, int lineStyle, Point *mypoints, int nPts)
{
	Graphics graph(hDC);
	graph.SetSmoothingMode(SmoothingModeHighQuality);
	
	Color cr(GetRValue(color), GetGValue(color), GetBValue(color));
	Pen pen(cr, (Gdiplus::REAL)lineSize);
	pen.SetDashStyle((enum Gdiplus::DashStyle)lineStyle);
	
/*	Point *points = new Point[nPts];
	for(int i=0; i<nPts; i++)
	{
		points[i].X = mypoints[i].x;
		points[i].Y = mypoints[i].y;
	}*/
	graph.DrawBeziers(&pen, mypoints, nPts);
//	delete []points;
}

//template<typename DataT>
//int	CPlotDataImpl<DataT>::CreateCompactData(RECT plotRect, double *xRange, double *yRange)
int	CPlotDataImpl<DataPoint2D>::CreateCompactData(RECT plotRect, double *xRange, double *yRange)
{
	if(GetDataCount()<=0)return 0;
	if(!m_bVisible)return 0;
	
	double		myX, myY;
	
	if(bCompactDraw)
	{
		DataPoint2D dp;
		
		long X0, X1;
		
		vDrawData.clear();
		
		int step, count = 0;
		while(count<GetDataCount())
		{
			dp.val[0] = dp.val[1] = 0;
			
			step = 0;
			while(true)
			{
				if(count+step > GetDataCount())break;
				
				myX = GetData()[count+step].val[0];
				myY = GetData()[count+step].val[1];
				X1 = (long)(xScale * (myX-xRange[0]) + plotRect.left);
				if(step == 0)X0 = X1;
				
				step++;
				if(X1 != X0)break;
				
				dp.val[0] += myX;
				dp.val[1] += myY;
			}
			if(step <= 1)break;// unexpected!!
			
			dp.val[0] /= (double)(step-1);
			dp.val[1] /= (double)(step-1);
			vDrawData.push_back(dp);
			
			count += step;
		}
	}
	
	return (int)vDrawData.size();
}

//template<typename DataT>
//bool	CPlotDataImpl<DataT>::IsDataScaleNeeded(RECT plotRect, double *xRange, double *yRange)
bool	CPlotDataImpl<DataPoint2D>::IsDataScaleNeeded(RECT plotRect, double *xRange, double *yRange)
{
	if(!EqualRect(&plotRect, &rctOldPlot))return true;
	if(m_nPlotType != nOldType)return true;
	if(!IsClipData())
	{
		if(nDatas != GetDataCount())return true;
	}
	else
	{
		if(nDatas != vDrawData.size())return true;
	}

	if(memcmp(xRange, pfOldRange[0], 2*sizeof(double)) != 0)return true;
	if(memcmp(yRange, pfOldRange[1], 2*sizeof(double)) != 0)return true;

	if(!pPointsGdi)return true;
	if(bDragMoved)return true;

	if(bForceUpdate)
	{
		bForceUpdate = false;
		return true;
	}
	return false;
}

//template<typename DataT>
//bool	CPlotDataImpl<DataT>::CreateScreenPoints( HDC hDC, RECT plotRect, double *xRange, double *yRange, POINT **myPoints, int &nPts, int &nPts4Fill )
bool	CPlotDataImpl<DataPoint2D>::CreateScreenPoints( HDC hDC, RECT plotRect, double *xRange, double *yRange, POINT **myPoints, int &nPts, int &nPts4Fill )
{
	if(GetDataCount()<=0)return false;
	if(!m_bVisible)return false;

	int			j, nDatas;
	POINT		point;
	double		x1,x2, y1, y2;
	double		myX, myY;
	bool		bXLog, bYLog;

	if(m_pbClipData[0])
	{
		if(!CreateClipData(0, xRange))return false;
	}
	else if(m_pbClipData[1])
	{
		if(!CreateClipData(1, yRange))return false;
	}

	// We now have a rect and an xRange and a yRange, just some
	// simple transformations should get us our plot

	nOldType = m_nPlotType;
	memcpy(pfOldRange[0], xRange, 2*sizeof(double));
	memcpy(pfOldRange[1], yRange, 2*sizeof(double));
	rctOldPlot = plotRect;

	bool		bCompact = bCompactDraw;
	
/*	bCompact = false;*/

	// Draw the dataset
	if(!bCompact)
	{
		nPts = nDatas = GetDataCount();
	}
	else
	{
		nPts = nDatas = (int)vDrawData.size();
	}
	int plottype = m_nPlotType;
	if(plottype == kXYPlotBezier && GetDataCount() < 4)
	{
		plottype = kXYPlotConnect;
	}
	switch( plottype )
	{
	case kXYPlotBezier:
		if(nDatas % 3 == 0)nPts += 1;
		else if( nDatas % 3 == 2 )	nPts += 2;
		break;
	case kXYPlotStepHV:
	case kXYPlotStepVH:
		nPts = 2 * nDatas - 1;
		break;
	case kXYPlotStepHVH:
	case kXYPlotStepVHV:
		nPts = 3 * nDatas - 2;
	}
	
	//Data fill
	nPts4Fill = nPts;
	
	int fillmode = m_nDataFillMode;
	if( fillmode == kDataFillFromBottomAxis || fillmode == kDataFillFromTopAxis
		|| fillmode == kDataFillFromLeftAxis || fillmode == kDataFillFromRightAxis)
	{
		nPts4Fill += 2;
	}

	if(*myPoints)
	{
		delete [](*myPoints);
		*myPoints = 0;
	}
	*myPoints = new POINT[nPts4Fill];

	bXLog = ((CPlotImpl*)m_pPlot)->IsXLogarithm();
	bYLog = ((CPlotImpl*)m_pPlot)->IsYLogarithm();

	switch( plottype )
	{
	case kXYPlotScatter:
	case kXYPlotConnect:
	case kXYPlotBezier:
	case kXYPlotBar:
		for( j = 0; j < nDatas; j++ )
		{
			if(!bCompact)
			{
				myX = GetData()[j].val[0];
				myY = GetData()[j].val[1];
			}
			else
			{
				myX = vDrawData[j].val[0];
				myY = vDrawData[j].val[1];
			}
			
			if(bXLog)myX = log10(myX);
			if(bYLog)myY = log10(myY);

			if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
				point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
			else
				point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);

			if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
				point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
			else
				point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
			(*myPoints)[j] = point;
		}
		if( plottype == kXYPlotBezier )
		{
			for( j = nDatas; j < nPts; j++ )
			{
				(*myPoints)[j] = point;
			}
		}
		break;
	case kXYPlotStepHV:
	case kXYPlotStepVH:
		for( j = 0; j < nDatas-1; j++ )
		{
			if(!bCompact)
			{
				myX = GetData()[j].val[0];
				myY = GetData()[j].val[1];
			}
			else
			{
				myX = vDrawData[j].val[0];
				myY = vDrawData[j].val[1];
			}
			if(bXLog)myX = log10(myX);
			if(bYLog)myY = log10(myY);

			if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
				point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
			else
				point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
			if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
				point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
			else
				point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
			(*myPoints)[2*j] = point;

			if(!bCompact)
			{
				myX = GetData()[j+1].val[0];
				myY = GetData()[j+1].val[1];
			}
			else
			{
				myX = vDrawData[j+1].val[0];
				myY = vDrawData[j+1].val[1];
			}
			if(bXLog)myX = log10(myX);
			if(bYLog)myY = log10(myY);
			if( plottype == kXYPlotStepHV )
			{
				if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
					point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
				else
					point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
			}
			else if( plottype == kXYPlotStepVH )
			{
				if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
					point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
				else
					point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
			}
			(*myPoints)[2*j+1] = point;
		}
		if(!bCompact)
		{
			myX = GetData()[nDatas-1].val[0];
			myY = GetData()[nDatas-1].val[1];
		}
		else
		{
			myX = vDrawData[nDatas-1].val[0];
			myY = vDrawData[nDatas-1].val[1];
		}
		if(bXLog)myX = log10(myX);
		if(bYLog)myY = log10(myY);
		if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
			point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
		else
			point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
		if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
			point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
		else
			point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
		(*myPoints)[2*nDatas-2] = point;
		break;
	case kXYPlotStepHVH:
	case kXYPlotStepVHV:
		for( j = 0; j < nDatas-1; j++ )
		{
			if(!bCompact)
			{
				myX = GetData()[j].val[0];
				myY = GetData()[j].val[1];
			}
			else
			{
				myX = vDrawData[j].val[0];
				myY = vDrawData[j].val[1];
			}
			if(bXLog)myX = log10(myX);
			if(bYLog)myY = log10(myY);

			if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
				point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
			else
				point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
			if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
				point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
			else
				point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
			(*myPoints)[3*j] = point;

			if(!bCompact)
			{
				myX = GetData()[j+1].val[0];
				myY = GetData()[j+1].val[1];
			}
			else
			{
				myX = vDrawData[j+1].val[0];
				myY = vDrawData[j+1].val[1];
			}
			if(bXLog)myX = log10(myX);
			if(bYLog)myY = log10(myY);
			if( plottype == kXYPlotStepHVH )
			{			
				x1 = point.x;
				if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
					x2 = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
				else
					x2 = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
				point.x = long( (x1 + x2) / 2);
				(*myPoints)[3*j+1] = point;

				if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
					y2 = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
				else
					y2 = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
				point.y = long( y2 );
				(*myPoints)[3*j+2] = point;
			}
			else if( plottype == kXYPlotStepVHV )
			{
				y1 = point.y;
				if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
					y2 = (long)(plotRect.bottom - yScale * (myY-yRange[0])+ 0.5);
				else
					y2 = (long)(plotRect.top + yScale * (myY-yRange[0])+ 0.5);
				point.y = long( (y1 + y2) / 2  );
				(*myPoints)[3*j+1] = point;

				if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
					x2 = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
				else
					x2 = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
				point.x = long( x2  );
				(*myPoints)[3*j+2] = point;
			}
		}
		if(!bCompact)
		{
			myX = GetData()[nDatas-1].val[0];
			myY = GetData()[nDatas-1].val[1];
		}
		else
		{
			myX = vDrawData[nDatas-1].val[0];
			myY = vDrawData[nDatas-1].val[1];
		}
		if(bXLog)myX = log10(myX);
		if(bYLog)myY = log10(myY);
		if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
			point.x = (long)(plotRect.left + xScale * (myX-xRange[0]) + 0.5);
		else
			point.x = (long)(plotRect.right - xScale * (myX-xRange[0]) + 0.5);
		if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
			point.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]) + 0.5);
		else
			point.y = (long)(plotRect.top + yScale * (myY-yRange[0]) + 0.5);
		(*myPoints)[3*nDatas-3] = point;
		break;
	default:
		return false;
	}

	bDragMoved = false;

	return true;
}
//void	MyDrawLineCrPtByPt(HDC hDC, int plottype, std::vector<COLORREF> vColor, int lineSize, int lineStyle, POINT *mypoints, int nPts);
//void	MyDrawLineCrMulti(HDC hDC, CPlotImpl *pPlot, RECT plotRect, int xScale, int yScale, double *xRange, double *yRange, std::vector<SegmentColor> vSegColor, int nMultiColorInputType, int lineSize, int lineStyle, POINT *mypoints, int nPts);
//void	MyDrawLineCrBi(HDC hDC, CPlotImpl *pPlot, RECT plotRect, int xScale, int yScale, double *xRange, double *yRange, double *xDataRange, double *yDataRange, COLORREF crDataColor1, COLORREF crDataColor2, int nMultiColorInputType, int lineSize, int lineStyle, POINT *mypoints, int nPts);
//void	MyDrawPolyline(HDC hDC, COLORREF color, int lineSize, int lineStyle, POINT *mypoints, int nPts, bool autoSmooth);
//void	MyDrawBeziers(HDC hDC, COLORREF color, int lineSize, int lineStyle, POINT *mypoints, int nPts);

//template<typename DataT>
//void	CPlotDataImpl<DataT>::DrawCurveOnly( HDC hDC, RECT plotRect, double *xRange, double *yRange )
void	CPlotDataImpl<DataPoint2D>::DrawCurveOnly( HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	if(GetDataCount()<=0)return;
	if(!m_bVisible)return;

	int	i;//, j;

	HPEN		hPen, hOldPen;

	hPen = CreatePen( m_nPlotLineStyle, m_nPlotLineSize, m_crPlotColor );
	hOldPen = (HPEN)SelectObject(hDC, hPen );

	// Draw the dataset
	int plottype = m_nPlotType;
	if(plottype == kXYPlotBezier && GetDataCount() < 4)plottype = kXYPlotConnect;
	switch( plottype )
	{
	case kXYPlotScatter:
		//Only need draw markers, have already done
		SelectObject(hDC,(HBRUSH)GetStockObject(NULL_BRUSH));
		break;
	case kXYPlotConnect:
	case kXYPlotStepHV:
	case kXYPlotStepVH:
	case kXYPlotStepHVH:
	case kXYPlotStepVHV:
		if(!bCompactDraw && m_bColorPtByPt && m_vPointColors.size() == GetData().size())
		{
			MyDrawLineCrPtByPt(hDC, plottype, m_vPointColors, m_nPlotLineSize, m_nPlotLineStyle, pPointsGdi, nPts);
/*			Graphics graph(hDC);
			graph.SetSmoothingMode(SmoothingModeHighQuality);
			
			for(i=0; i<nPts-1; i++)
			{
				if(myPoints[i].x == myPoints[i+1].x && myPoints[i].y == myPoints[i+1].y)continue;
				
				Point *points = new Point[2];
				points[0].X = myPoints[i].x;
				points[0].Y = myPoints[i].y;
				points[1].X = myPoints[i+1].x;
				points[1].Y = myPoints[i+1].y;
				
				Color *colors=new Color[2];
				switch(plottype)
				{
				case kXYPlotConnect:
					colors[0].SetFromCOLORREF(m_vPointColors[i]);
					colors[1].SetFromCOLORREF(m_vPointColors[i+1]);
					break;
				case kXYPlotStepHV:
				case kXYPlotStepVH:
					colors[0].SetFromCOLORREF(m_vPointColors[i/2]);
					colors[1].SetFromCOLORREF(m_vPointColors[(i+1)/2]);
					break;
				case kXYPlotStepHVH:
				case kXYPlotStepVHV:
					colors[0].SetFromCOLORREF(m_vPointColors[(i+1)/3]);
					colors[1].SetFromCOLORREF(m_vPointColors[(i+2)/3]);
					break;
				}

				LinearGradientBrush lgBrush(points[0], points[1], colors[0], colors[1]);
				
				Pen Mypen(&lgBrush,m_nPlotLineSize);
				Mypen.SetDashStyle((enum Gdiplus::DashStyle)m_nPlotLineStyle);
				Mypen.SetStartCap(LineCapRound);
				Mypen.SetEndCap(LineCapRound);
				graph.DrawLine(&Mypen, points[0], points[1]);
				
				delete []colors;
				delete []points;
			}*/
		}
		else if(!bCompactDraw && (m_bMultiColorMode && CheckSegColors()) )
		{
			MyDrawLineCrMulti(hDC, ((CPlotImpl*)m_pPlot), plotRect, xScale, yScale, xRange, yRange, m_vSegColors, m_nMultiColorInputType, m_nPlotLineSize, m_nPlotLineStyle, pPointsGdi, nPts);
/*			Graphics graph(hDC);
			graph.SetSmoothingMode(SmoothingModeHighQuality);
			
			COLORREF cr1, cr2;
			double val, rt1, rt2;
			byte r1, g1, b1, r2, g2, b2;
			int k;
			Point *points = new Point[2];
			Color *colors = new Color[2];
			
			for(i=0; i<nPts-1; i++)
			{
				if(myPoints[i].x == myPoints[i+1].x && myPoints[i].y == myPoints[i+1].y)continue;
				
				for(k=0; k<2; k++)
				{
					points[k].X = myPoints[i+k].x;
					points[k].Y = myPoints[i+k].y;
					switch(m_nMultiColorInputType)
					{
					case kInputColorByPointRatio:
						val = (i+k)/(nPts-1.0);
						break;
					case kInputColorByXVal:
						if(!((CPlotImpl*)m_pPlot)->IsXLogarithm())
							val = (myPoints[(i+k)].x - plotRect.left)/xScale + xRange[0];
						else
							val = (plotRect.right - myPoints[(i+k)].x)/xScale + xRange[0];
						break;
					case kInputColorByYVal:
						if(!((CPlotImpl*)m_pPlot)->IsYLogarithm())
							val = (plotRect.bottom - myPoints[(i+k)].y)/yScale + yRange[0] ;
						else
							val = (myPoints[(i+k)].y - plotRect.top)/yScale + yRange[0] ;
						break;
					}
					if(val<=m_vSegColors[0].val)
					{
						cr1 = cr2 = m_vSegColors[0].color;
						colors[i].SetFromCOLORREF(cr1);
					}
					else if(val>=m_vSegColors[m_vSegColors.size()-1].val)
					{
						cr1 = cr2 = m_vSegColors[m_vSegColors.size()-1].color;
						colors[i].SetFromCOLORREF(cr1);
					}
					else
					{
						for(j=0; j<(int)m_vSegColors.size(); j++)
						{
							if(val>=m_vSegColors[j].val && val<m_vSegColors[j+1].val)
							{
								rt1 = m_vSegColors[j].val;
								rt2 = m_vSegColors[j+1].val;
								cr1 = m_vSegColors[j].color;
								cr2 = m_vSegColors[j+1].color;
								
								r1 = GetRValue(cr1);
								r2 = GetRValue(cr2);
								g1 = GetGValue(cr1);
								g2 = GetGValue(cr2);
								b1 = GetBValue(cr1);
								b2 = GetBValue(cr2);
								colors[k].SetFromCOLORREF( RGB(
									((rt2-val)*r1+(val-rt1)*r2)/(rt2-rt1),
									((rt2-val)*g1+(val-rt1)*g2)/(rt2-rt1),
									((rt2-val)*b1+(val-rt1)*b2)/(rt2-rt1) ) );
								break;
							}
						}
					}
				}
				
				LinearGradientBrush lgBrush(points[0], points[1], colors[0], colors[1]);
				
				Pen Mypen(&lgBrush,m_nPlotLineSize);
				Mypen.SetDashStyle((enum Gdiplus::DashStyle)m_nPlotLineStyle);
				Mypen.SetStartCap(LineCapRound);
				Mypen.SetEndCap(LineCapRound);
				graph.DrawLine(&Mypen, points[0], points[1]);
			}
			delete []colors;
			delete []points;*/
		}
		else if(!bCompactDraw && m_bBiColorMode )
		{
			MyDrawLineCrBi(hDC, ((CPlotImpl*)m_pPlot), plotRect, xScale, yScale, xRange, yRange, m_ppfDataRange[0], m_ppfDataRange[1], m_crDataColor, m_crDataColor2, m_nMultiColorInputType, m_nPlotLineSize, m_nPlotLineStyle, pPointsGdi, nPts);
/*			Graphics graph(hDC);
			graph.SetSmoothingMode(SmoothingModeHighQuality);
			
			COLORREF cr1, cr2;
			double val, rt1, rt2;
			int k;
			byte r1, g1, b1, r2, g2, b2;
			
			cr1 = m_crDataColor;
			cr2 = m_crDataColor2;
			
			Point *points = new Point[2];
			Color *colors = new Color[2];
			
			for(i=0; i<nPts-1; i++)
			{
				if(myPoints[i].x == myPoints[i+1].x && myPoints[i].y == myPoints[i+1].y)continue;
				
				for(k=0; k<2; k++)
				{
					points[k].X = myPoints[i+k].x;
					points[k].Y = myPoints[i+k].y;
					switch(m_nMultiColorInputType)
					{
					case kInputColorByPointRatio:
						rt1 = 0.0;
						rt2 = 1.0;
						val = (i+k)/(nPts-1.0);
						break;
					case kInputColorByXVal:
						rt1 = m_pfXDataRange[0];
						rt2 = m_pfXDataRange[1];
						if(!((CPlotImpl*)m_pPlot)->IsXLogarithm())
							val = (myPoints[(i+k)].x - plotRect.left)/xScale + xRange[0];
						else
							val = (plotRect.right - myPoints[(i+k)].x)/xScale + xRange[0];
						break;
					case kInputColorByYVal:
						rt1 = m_pfYDataRange[0];
						rt2 = m_pfYDataRange[1];
						if(!((CPlotImpl*)m_pPlot)->IsYLogarithm())
							val = (plotRect.bottom - myPoints[(i+k)].y)/yScale + yRange[0] ;
						else
							val = (myPoints[(i+k)].y - plotRect.top)/yScale + yRange[0] ;
						break;
					}
					r1 = GetRValue(cr1);
					r2 = GetRValue(cr2);
					g1 = GetGValue(cr1);
					g2 = GetGValue(cr2);
					b1 = GetBValue(cr1);
					b2 = GetBValue(cr2);
					colors[k].SetFromCOLORREF( RGB(
						((rt2-val)*r1+(val-rt1)*r2)/(rt2-rt1),
						((rt2-val)*g1+(val-rt1)*g2)/(rt2-rt1),
						((rt2-val)*b1+(val-rt1)*b2)/(rt2-rt1) ) );
					
					//					colors[k].SetFromCOLORREF( RGB(
					//						((rt2-val)*GetRValue(cr1)+(val-rt1)*GetRValue(cr2))/(rt2-rt1),
					//						((rt2-val)*GetGValue(cr1)+(val-rt1)*GetGValue(cr2))/(rt2-rt1),
					//						((rt2-val)*GetBValue(cr1)+(val-rt2)*GetBValue(cr2))/(rt2-rt1) ) );
				}
				
				LinearGradientBrush lgBrush(points[0], points[1], colors[0], colors[1]);
				
				Pen Mypen(&lgBrush,m_nPlotLineSize);
				Mypen.SetDashStyle((enum Gdiplus::DashStyle)m_nPlotLineStyle);
				Mypen.SetStartCap(LineCapRound);
				Mypen.SetEndCap(LineCapRound);
				graph.DrawLine(&Mypen, points[0], points[1]);
			}
			delete []colors;
			delete []points;*/
		}
		else if( !m_bForceGdiPlus && (m_nPlotLineSize == 1 || (m_nPlotLineSize != 1 && m_nPlotLineStyle == PS_SOLID)) ) 
		{
			if(nPts>1)Polyline(hDC, pPointsGdi, nPts);
		}
		else
		{
			MyDrawPolyline(hDC, m_crPlotColor, m_nPlotLineSize, m_nPlotLineStyle, (Point *)pPointsGdi, nPts, m_bAutoSmooth);
/*			Graphics graph(hDC);
			graph.SetSmoothingMode(SmoothingModeHighQuality);
			
			Color cr(GetRValue(m_crPlotColor), GetGValue(m_crPlotColor), GetBValue(m_crPlotColor));
			Pen pen(cr, (Gdiplus::REAL)m_nPlotLineSize);
			pen.SetDashStyle((enum Gdiplus::DashStyle)m_nPlotLineStyle);

			Point *points = new Point[nPts];
			for(i=0; i<nPts; i++)
			{
				points[i].X = myPoints[i].x;
				points[i].Y = myPoints[i].y;
			}
			if(m_bAutoSmooth)
			{
				graph.DrawCurve(&pen, points, nPts);
			}
			else
			{
				graph.DrawLines(&pen, points, nPts);
			}
			
			delete []points;
			//graph.ReleaseHDC(hDC);*/
		}
		
		break;
	case kXYPlotBezier:
		if(m_nPlotLineSize == 1)
		{
			PolyBezier(hDC, pPointsGdi, nPts);
		}
		else
		{
			MyDrawBeziers(hDC, m_crPlotColor, m_nPlotLineSize, m_nPlotLineStyle, (Point *)pPointsGdi, nPts);
/*			Graphics graph(hDC);
			graph.SetSmoothingMode(SmoothingModeHighQuality);
			
			Color cr(GetRValue(m_crPlotColor), GetGValue(m_crPlotColor), GetBValue(m_crPlotColor));
			Pen pen(cr, (Gdiplus::REAL)m_nPlotLineSize);
			pen.SetDashStyle((enum Gdiplus::DashStyle)m_nPlotLineStyle);
			
			Point *points = new Point[nPts];
			for(int i=0; i<nPts; i++)
			{
				points[i].X = myPoints[i].x;
				points[i].Y = myPoints[i].y;
			}
			graph.DrawBeziers(&pen, points, nPts);
			delete []points;
			//graph.ReleaseHDC(hDC);*/
		}
		
		break;
	case kXYPlotBar:
		POINT *pts;
		DWORD *npts;
		pts = new POINT[2*nPts];
		npts = new DWORD[nPts];
		for(i=0; i<nPts; i++)
		{
			npts[i] = 2;
			switch(m_nBarMode)
			{
			case kBarFromBottom:
				pts[2*i].x = pPointsGdi[i].x;
				pts[2*i].y = plotRect.bottom;
				break;
			case kBarFromTop:
				pts[2*i].x = pPointsGdi[i].x;
				pts[2*i].y = plotRect.top;
				break;
			case kBarFromLeft:
				pts[2*i].x = plotRect.left;
				pts[2*i].y = pPointsGdi[i].y;
				break;
			case kBarFromRight:
				pts[2*i].x = plotRect.right;
				pts[2*i].y = pPointsGdi[i].y;
				break;
			}
			pts[2*i+1].x = pPointsGdi[i].x;
			pts[2*i+1].y = pPointsGdi[i].y;
		}
		PolyPolyline(hDC, pts, npts, nPts);
		
		delete []npts;
		delete []pts;
	
		break;
	default:
		break;
	}
	
	SelectObject( hDC,hOldPen );
	DeleteObject(hPen);
}

void	CPlotDataImpl<DataPoint2D>::DrawSegHided( HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	if(GetDataCount()<=0)return;
	if(m_nPlotType != kXYPlotConnect)return;
	if(!m_bVisible)return;
	if(!m_bSegHideMode)return;
	if(m_vSegHide.size()<=0)return;

	int	i, index;
	POINT pt1, pt2;
	
	HPEN		hPen, hOldPen;
	
	//hPen = CreatePen( PS_SOLID, m_nPlotLineSize+2, ((CPlotImpl*)GetPlot())->GetBkgndColor() );
	hPen = CreatePen( PS_SOLID, m_nPlotLineSize, m_crPlotColor );
	hOldPen = (HPEN)SelectObject(hDC, hPen );

	for(i=0; i<(int)m_vSegHide.size(); i++)
	{
		index = m_vSegHide[i];
		if(index<0 || index>=GetDataCount()-1)continue;

		pt1 = (pPointsGdi)[index];
		pt2 = (pPointsGdi)[index+1];
		MoveToEx(hDC, pt1.x, pt1.y, NULL);
		LineTo(hDC, pt2.x, pt2.y);
	}

	SelectObject(hDC, hPen);
	DeleteObject(hPen);
}


//template<typename DataT>
//void	CPlotDataImpl<DataT>::DrawPlotDataInfo(HDC hDC, RECT plotRect, double *xRange, double *yRange )
void	CPlotDataImpl<DataPoint2D>::DrawPlotDataInfo(HDC hDC, RECT plotRect, double *xRange, double *yRange )
{
	if(!m_pDataSet)return;
	if(!IsInfoShow())return;
	if(GetDataCount()<=0)return;
	if(!xRange || !yRange)return;
	if(xRange[1] == xRange[0] || yRange[1] == yRange[0])return;
	
	HFONT		hOldFont, hFont;
	LOGFONT		logFont;
	SIZE		tmSize;
	
	POINT		ptInfo;
	int			idxPtInfo=0;//this curve draw the info at this point
	int			theIdx;//data set index, some times get from data level(when draw contour)
	
//	double		xScale = m_vpXYDataInfo[which]->xScale;
//	double		yScale = m_vpXYDataInfo[which]->yScale;
	
	//MyVData vData = dataSet->GetData();
	bool		bDrawThis=false;
	if(GetInfo() != _TEXT("") )
	{
		theIdx=GetDataLevel()>=0?GetDataLevel():m_pDataSet->GetIndex(m_nDataID);
		switch(m_pDataSet->GetShowPlotDataInfoMode())
		{
		case kShowPlotDataInfoNot:
			bDrawThis=false;
			break;
		case kShowPlotDataInfoEveryN:
			if( theIdx%m_pDataSet->GetShowPlotDataInfoNum()==0 )bDrawThis=true;
			break;
		case kShowPlotDataInfoTotalN:
			if( theIdx%(int)ceil(m_pDataSet->GetPlotDataLevels()/(double)m_pDataSet->GetShowPlotDataInfoNum())==0 )bDrawThis=true;
			break;
		default:
			break;
		}
	}
	if(!bDrawThis)return;
	
	double		myX, myY;
	bool		bXLog, bYLog;
	bXLog = ((CPlotImpl*)m_pPlot)->IsXLogarithm();
	bYLog = ((CPlotImpl*)m_pPlot)->IsYLogarithm();
	
	logFont = ((CPlotImpl*)m_pPlot)->GetTitleFont();
	logFont.lfHeight=GetInfoFontsize();
	hFont = CreateFontIndirect(&logFont);
	hOldFont = (HFONT)SelectObject( hDC, hFont );
	
	//GetTextExtentPoint(hDC,dataSet->GetInfo().c_str(),dataSet->GetInfo().length(),&tmSize);
	MyGetTextExtentPoint(hDC, &logFont, GetInfo(), DT_LEFT, &tmSize);
	
	if(idxPtInfo>=GetDataCount())idxPtInfo%=GetDataCount();
	
	myX = GetData()[idxPtInfo].val[0];
	if(bXLog)myX = log10(myX);
	myY = GetData()[idxPtInfo].val[1];
	if(bYLog)myY = log10(myY);
	
	if(!((CPlotImpl*)m_pPlot)->IsXOpposite())
		ptInfo.x=(long)(plotRect.left + xScale * (myX-xRange[0]));
	else
		ptInfo.x=(long)(plotRect.right - xScale * (myX-xRange[0]));
	if(!((CPlotImpl*)m_pPlot)->IsYOpposite())
		ptInfo.y = (long)(plotRect.bottom - yScale * (myY-yRange[0]))+tmSize.cy/2;
	else
		ptInfo.y = (long)(plotRect.top + yScale * (myY-yRange[0]))+tmSize.cy/2;
	if(ptInfo.x+tmSize.cx>plotRect.right)ptInfo.x=plotRect.right-tmSize.cx;
	if(ptInfo.y+tmSize.cy>plotRect.bottom)ptInfo.y=plotRect.bottom-tmSize.cy;
	
	if( GetInfo() != _TEXT("") )
	{
		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,m_crPlotColor);
		TextOut(hDC,ptInfo.x,ptInfo.y,GetInfo().c_str(),(int)GetInfo().length());
	}
	idxPtInfo+=m_pDataSet->GetShowPlotDataInfoDataPointStep();
	
	SelectObject(hDC, hOldFont );
	DeleteObject(hFont);
}

//template<typename DataT>
//void	CPlotDataImpl<DataT>::DrawPlotData(HDC hDC, RECT plotRect, double *xRange, double *yRange)
void	CPlotDataImpl<DataPoint2D>::DrawPlotData(HDC hDC, RECT plotRect, double *xRange, double *yRange)
{
	if(GetDataCount()<=0)return;
	if(!m_bVisible)return;
	
	xScale = ((double)Width(plotRect))/(xRange[1] - xRange[0]);
	yScale = ((double)Height(plotRect))/(yRange[1] - yRange[0]);
	
	if(bCompactDraw && !IsClipData())
	{
		if( vDrawData.size()<=0 ||
			vDrawData.size()!=nDrawDataOldSize ||
			nDatas != GetDataCount() ||
			bDrawDataChanged ||
			bDragMoved )
		{
			CreateCompactData(plotRect, xRange, yRange);
		}
	}
	
	if(IsDataScaleNeeded(plotRect, xRange, yRange))
	{
		if(!CreateScreenPoints(hDC, plotRect, xRange, yRange, &pPointsGdi, nPts, nPts4Fill))
		{
			return;
		}
	}
	
	nDatas = GetDataCount();
	
	int nROP2;
	if(m_bSegHideMode)
	{
		nROP2 = SetROP2(hDC, R2_NOTXORPEN);
	}
	DrawCurveOnly(hDC, plotRect, xRange, yRange );
	if(m_bSegHideMode)
	{
		DrawSegHided(hDC, plotRect, xRange, yRange);
	}
	if(m_bSegHideMode)
	{
		SetROP2(hDC, nROP2);
	}
	if(GetInfo() != _TEXT("") )
	{
		DrawPlotDataInfo(hDC, plotRect, xRange, yRange);
	}

	
	if(m_bErrorBarMode) 
	{
		DrawErrorBars(hDC, plotRect, xRange, yRange);
	}
	if(m_bDataFill)
	{
		if(AppendFillPoints(hDC, plotRect, xRange, yRange))
		{
			DrawFillArea(hDC, plotRect, xRange, yRange);
		}
	}
	if(m_bDataBarShow)
	{
		DrawDataBars(hDC, plotRect, xRange, yRange);
	}

	if(m_bHisto)
	{
		DrawHisto(hDC, plotRect, xRange, yRange);
	}
	if(m_bMarkerShow)
	{
		DrawMarkers(hDC, plotRect, xRange, yRange);
	}
	if(GetDataSet()->IsPointSelectionMode())
	{
		DrawPointSelectionMode(hDC, plotRect, xRange, yRange);
	}
	if(m_bDataPointInfoShow)
	{
		DrawDataPointInfo(hDC, plotRect, xRange, yRange);
	}
	if(m_bSpanMode)
	{
		DrawDataSpan(hDC, plotRect, xRange, yRange);
	}
}

Declare_Namespace_End
