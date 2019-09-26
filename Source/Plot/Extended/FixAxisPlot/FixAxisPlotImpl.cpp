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

#include "../../Basic/MyGdiPlus.h"
#include "../../Basic/MyMemDC.h"
#include "FixAxisPlotImpl.h"
#include <iomanip>

using namespace NsCChart;

CFixAxisPlotImpl::CFixAxisPlotImpl()
{
	InitAxes();
	SetLegendShow(true);
	SetLegendPosition(kLegendBottomLeft);
	SetLegendLineLength(16);

	m_nIndexToHoldCursor = 0;

	//CGlobal::m_bMenuAxes = false;
	
	SetBkgndColor(RGB(192, 192, 192));
	SetMajorGrids(true);
	SetMajorGridLineStyle(PS_DOT);

	//CGlobal::m_bOscilloScopeMenu = true;
}

CFixAxisPlotImpl::~CFixAxisPlotImpl()
{

}

void	CFixAxisPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	//InitAxes();
	//if there is no data, doing nothing

	SetLastClientRect(destRect);
	if(IsEmpty())
	{
		//DrawBkgnd( hDC );
		DrawDefScreen(hDC, destRect);
		return;
	}
	if(GetMaxPlotDataLength()<=0)
	{
		DrawBkgnd( hDC );
		return;
	}
	
	// Check Logarithm safety
	if(IsXLogarithm() || IsYLogarithm())
	{
		if(!CheckLogSafety())
		{
			DrawBkgnd( hDC );
			return;
		}
	}
	
	int nOldMapmode = SetMapMode(hDC, MM_TEXT);
	
	//////////////////////////////////////////////////////////////////
	double	xRange[2], yRange[2];
	
	// Make sure our ranges are set
	// Get ranges for plotting
	if(IsRangeUpdateNeeded())
	{
		GetPlotRange( xRange, yRange );
		SetLastPlotRange(xRange, yRange);
	}
	else
	{
		GetLastPlotRange(xRange, yRange);
	}
	
	
	// Get the size of our plotting rectangle
	RECT	plotRect = GetPlotRect( hDC, destRect );

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
	
	// Erase background
	if(!m_bNothingElse)DrawBkgnd(hdc);
	
	// Set line colors,line styles, line sizes
	SetPlotColors();
	SetPlotLineStyles();
	SetPlotLineSizes();

//	EnterCriticalSection(&m_csPlot);
	
	// Draw basics
	CPlotImpl::FinalDraw(hdc,destRect);
	
	//if(CGlobal::m_bOscilloScopeMenu && IsOscilloScopeMode())
	if(IsOscilloScopeMode())
	{
		// now draw oscilloscopes
		int idx = m_nIndexToHoldCursor;
		if(GetDataID(idx)<0) idx=0;

		if(GetDataID(idx)>=0)
		{
			// Draw Selelected zone
			InitializeSelZone();
			DrawSelZone( hdc, plotRect );
			// Draw the cross
			InitializeCursor();
			DrawCursor(hdc, plotRect);
			// Draw trig arrow
			DrawTrigArrow(hdc, plotRect);
			// Draw the dataset zero arrow
			DrawDataSetZeroArrow(hdc, plotRect);
		}
		// Draw the dataset zero arrow
		DrawDataSetZeroArrow(hdc, plotRect);

//		SetLastPlotRange(xrng, yrng);
	}
	
	
	// Set Clip
	// Draw each data set
	DrawDataSet( hdc, xRange, yRange );
	// clear clip
	
	if(m_bLegendShow)
	{
		DrawLegend(hdc);
	}

//	LeaveCriticalSection(&m_csPlot);
	
	SetMapMode(hDC, nOldMapmode);
}

void	CFixAxisPlotImpl::GetPlotRange( double *xRange, double *yRange )
{
	for(int i=0; i<GetPlotDataCount(); i++)
	{
		GetPlotRange(xRange, yRange, GetDataID(i));
	}
}

void	CFixAxisPlotImpl::GetPlotRange( double *xRange, double *yRange, int dataID )
{
	double optRange;
	double maxval;

	int i = GetIndex(dataID);
	if(i<0)return;
	
	CPlotDataImpl<DataPoint2D> *dataInfo = GetAbsPlotData(i);
	if(!dataInfo->GetExtProp())return;
	
	if(!((FixAxisProp *)dataInfo->GetExtProp())->bFixAxisXRangeSet)
	{
		xRange[0] = dataInfo->GetXDataRange()[0];
		xRange[1] = dataInfo->GetXDataRange()[1];
		if(m_bDualX)
		{
			maxval = max(fabs(xRange[0]), fabs(xRange[1]));
			GetMyCeil(maxval, optRange);
			if(fabs(optRange) < fMinVal) optRange = 1.0;
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeXL = -optRange;
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeXH = optRange;
		}
		else
		{
		/*	GetMyCeil(fabs(xRange[1] - xRange[0]), optRange);
			GetMyFloor(xRange[0], optStart);
			dataInfo->fRangeXL = optStart;
			dataInfo->fRangeXH = optStart + optRange;*/
			double oRg[2];
			GetOptimalRange(xRange, oRg, false, IsOptimalExactXRange(), IsOptimalXZeroRepair(), IsOptimalXSymetryRepair(), IsOptimalXlExtend(), IsOptimalXuExtend());
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeXL = oRg[0];
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeXH = oRg[1];
		}
		((FixAxisProp *)dataInfo->GetExtProp())->bFixAxisXRangeSet = true;
	}
	else
	{
		xRange[0] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeXL;
		xRange[1] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeXH;
	}
	
	if(!((FixAxisProp *)dataInfo->GetExtProp())->bFixAxisYRangeSet)
	{
		yRange[0] = dataInfo->GetYDataRange()[0];
		yRange[1] = dataInfo->GetYDataRange()[1];
		if(m_bDualY)
		{
			maxval = max(fabs(yRange[0]), fabs(yRange[1]));
			GetMyCeil(maxval, optRange);
			if(fabs(optRange) < fMinVal) optRange = 1.0;
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeYL = -optRange;
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeYH = optRange;
		}
		else
		{
			/*
			GetMyCeil(fabs(yRange[1] - yRange[0]), optRange);
			GetMyFloor(yRange[0], optStart);
			dataInfo->fRangeYL = optStart;
			dataInfo->fRangeYH = optStart + optRange;
			*/
			double oRg[2];
			GetOptimalRange(yRange, oRg, false, IsOptimalExactYRange(), IsOptimalYZeroRepair(), IsOptimalYSymetryRepair(), IsOptimalYlExtend(), IsOptimalYuExtend());
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeYL = oRg[0];
			((FixAxisProp *)dataInfo->GetExtProp())->fRangeYH = oRg[1];
		}
		((FixAxisProp *)dataInfo->GetExtProp())->bFixAxisYRangeSet = true;
	}
	else
	{
		yRange[0] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeYL;
		yRange[1] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeYH;
	}
}

void	CFixAxisPlotImpl::DrawPlotDataEach( HDC hDC, double *xRange, double *yRange )
{	
	//GetPlotRange(xRange, yRange);
	// Begin draw
	for( int i = 0; i < GetPlotDataCount(); i++ )
	{
		CPlotDataImpl<DataPoint2D> *dataInfo = GetAbsPlotData(i);
		xRange[0] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeXL;
		xRange[1] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeXH;
		yRange[0] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeYL;
		yRange[1] = ((FixAxisProp *)dataInfo->GetExtProp())->fRangeYH;
		DrawSinglePlotData(i, hDC, xRange, yRange);
	}
}

// The default functions do nothing to the passed things
void	CFixAxisPlotImpl::DataToLP( double *data, LPPOINT point, int nWhich )
{
	RECT		plotRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;
	
	if(nWhich < 0 || nWhich >= GetPlotDataCount()) return;
	if( point == NULL ) return;
	
	plotRect = GetLastPlotRect( );
	
	xRange[0] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeXL;
	xRange[1] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeXH;
	yRange[0] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeYL;
	yRange[1] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeYH;
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( scaleX == 0. || scaleY == 0. ) return;
	
	point->x = (long)((data[0]-xRange[0]) / scaleX) + plotRect.left;
	point->y = -(long)((data[1]-yRange[0]) / scaleY) + plotRect.bottom;
}

void	CFixAxisPlotImpl::DataToLP( double *data, LPPOINT point )
{
	DataToLP(data, point, m_nIndexToHoldCursor);
}

void	CFixAxisPlotImpl::LPToData( LPPOINT point, double *data, int nWhich )
{
	RECT		plotRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;

	if(nWhich < 0 || nWhich >= GetPlotDataCount()) return;
	if( point == NULL ) return;
	
	plotRect = GetLastPlotRect( );
	
	xRange[0] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeXL;
	xRange[1] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeXH;
	yRange[0] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeYL;
	yRange[1] = ((FixAxisProp *)GetAbsPlotData(nWhich)->GetExtProp())->fRangeYH;
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( fabs(scaleX) < fMinVal || fabs(scaleY) < fMinVal ) return;
	
	data[0] = (point->x-plotRect.left) * scaleX + xRange[0];
	data[1] = (plotRect.bottom-point->y) * scaleY + yRange[0];
}

void	CFixAxisPlotImpl::LPToData( LPPOINT point, double *data )
{
	LPToData( point, data, m_nIndexToHoldCursor);
}

int		CFixAxisPlotImpl::GetNeareastPlotData( HDC hDC, POINT point )
{
	if(GetPlotDataCount()<=0)return -1;

	CPlotData<DataPoint2D>			*dataSet;

	int					i, j, lo, up;
	double				data[2];//,range[2];
	
	int					nIdx;
	double				dlM,dl0;

	//GetYRange(range);
	//dlM=(range[1]-range[0])/m_fHitPrecision;

	//LPToData( hDC, &point, data );

	double xMin, xMax;
	double xm,x1;

	nIdx=-1;
	for(i=0;i<GetPlotDataCount();i++)
	{
		dlM=(((FixAxisProp *)GetAbsPlotData(i)->GetExtProp())->fRangeYH - ((FixAxisProp *)GetAbsPlotData(i)->GetExtProp())->fRangeYL)/m_fHitPrecision;

		dataSet = GetPlotData( GetDataID( i ) );

		if( dataSet->GetDataCount()<=1 ) continue;

		switch(GetAbsPlotData(i)->nSLMode)
		{
		case kSLByFillArea:
			//if(PtInRegion())
			POINT *myPoints;
			int nPts4Fill;
			myPoints = GetAbsPlotData(i)->pPointsGdi;
			nPts4Fill = GetAbsPlotData(i)->nPts4Fill;
			
			BeginPath(hDC);
			Polyline(hDC, myPoints, nPts4Fill);
			EndPath( hDC );
			
			{
				HRGN hRgn = PathToRegion(hDC);
				Region rgn(hRgn);
				
				if(PtInRegion(hRgn, point.x, point.y))
				{
					nIdx = i;
				}
				DeleteObject(hRgn);
			}
			
			if(nIdx>=0)return GetDataID(nIdx);
			
			break;
		default:
			lo=-1;
			up=-1;
			
			LPToData( &point, data, i );
			CDataSet<DataPoint2D>::GetDataRange( GetDataID(i) );
			
			if(IsXLogarithm())
			{
				xMin = log10(GetAbsPlotData(i)->GetXDataRange()[0]);
				xMax = log10(GetAbsPlotData(i)->GetXDataRange()[1]);
			}
			else
			{
				xMin = GetAbsPlotData(i)->GetXDataRange()[0];
				xMax = GetAbsPlotData(i)->GetXDataRange()[1];
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

int		CFixAxisPlotImpl::GetNeareastDataPoint( int dataID, HDC hDC, POINT point, double data[2])
{
	CPlotData<DataPoint2D> *pDataSet=GetPlotData(dataID);
	if(!pDataSet)return -1;
	if(pDataSet->GetDataCount()<=0)return -1;

	int idx = GetIndex(dataID);

	double xMin, xMax;
	if(IsXLogarithm())
	{
		xMin = log10(GetAbsPlotData(idx)->GetXDataRange()[0]);
		xMax = log10(GetAbsPlotData(idx)->GetXDataRange()[1]);
	}
	else
	{
		xMin = GetAbsPlotData(idx)->GetXDataRange()[0];
		xMax = GetAbsPlotData(idx)->GetXDataRange()[1];
	}
	double xm, x1;

	LPToData( &point, data, GetIndex(dataID) );
	if(data[0]<xMin || data[0]>xMax)return -1;

	int lo = -1, up = -1;
	bool set = false;
	double x0=data[0], y0=data[1];
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

tstring	CFixAxisPlotImpl::GetAbsDataTitle( int which )
{
	if(which < 0 || which >= GetPlotDataCount())return _TEXT("");

	tstring title;// = CXYPlotImpl::GetAbsoluteDataTitle(which);

	tostringstream ostr;
	double low, high;

	ostr<<_TEXT(" Ch")<<which+1<<_TEXT(": ");
	title = ostr.str();

	if(!m_bShowYScaleOnly)
	{
		low = ((FixAxisProp *)GetAbsPlotData(which)->GetExtProp())->fRangeXL;
		high = ((FixAxisProp *)GetAbsPlotData(which)->GetExtProp())->fRangeXH;
		ostr.str(_TEXT(""));
		ostr<<_TEXT("X ")<<(high - low)/(m_nMajorDivsX + 1) << GetAbsPlotData(which)->GetXUnit() <<_TEXT("/div");
		title += ostr.str();
		
		title += _TEXT("--");
	}

	low = ((FixAxisProp *)GetAbsPlotData(which)->GetExtProp())->fRangeYL;
	high = ((FixAxisProp *)GetAbsPlotData(which)->GetExtProp())->fRangeYH;
	ostr.str(_TEXT(""));
	ostr<<_TEXT("Y ")<<(high - low)/(m_nMajorDivsX + 1) << GetAbsPlotData(which)->GetYUnit() <<_TEXT("/div");
	title += ostr.str();

	return title;
}

tstring	CFixAxisPlotImpl::GetAbsDataAppendix( int which )
{
	if(which < 0 || which >= GetPlotDataCount())return _TEXT("");

	tstring appendix = _TEXT( " (" ) + CXYPlotImpl::GetAbsDataTitle(which) + _TEXT( ")" );
	return  appendix;
}

void	CFixAxisPlotImpl::GetMyCeil(double val, double &ceilVal)
{
	int epsl;
	double reside;
	double sig = val>0?1.0:-1.0;
	
	if(val!=0.0)
	{
		epsl=(int)floor( log10( fabs(val) ) );
		reside = fabs(val) / pow(10.0, epsl);
		
		reside = ceil(reside);
		ceilVal = sig * reside * pow(10.0, epsl);
	}
	else
	{
		ceilVal = 0.0;
	}
}

void	CFixAxisPlotImpl::GetMyFloor(double val, double &floorVal)
{
	int epsl;
	double reside;
	double sig = val>0?1.0:-1.0;
	
	if(val!=0.0)
	{
		epsl=(int)floor( log10( fabs(val) ) );
		reside = fabs(val) / pow(10.0, epsl);
		
		reside = floor(reside);
		floorVal = sig * reside * pow(10.0, epsl);
	}
	else
	{
		floorVal = 0.0;
	}
}

void	CFixAxisPlotImpl::InitAxes()
{
//	int TickL, minorTickL;
//	CAxis *axis;
	
	DeleteAllAxes();

	AddLRAxis();
	AddTBAxis();
	if(m_bDualX)
	{
		AddAxis(kLocationCenterVL);
	}
	if(m_bDualY)
	{
		AddAxis(kLocationCenterHB);
	}

	if(m_bDualX)SetXRange(-5.0, 5.0);
	else SetXRange(0.0, 10.0);
	SetXTickCount(m_nMajorDivsX);
	SetXMinorTickCount(m_nMinorDivsX);

	if(m_bDualY)SetYRange(-5.0, 5.0);
	else SetYRange(0.0, 10.0);
	SetYTickCount(m_nMajorDivsY);
	SetYMinorTickCount(m_nMinorDivsY);
	
	int i;
	vector<CAxisImpl *> vAxis;
	
	vAxis = GetXAxes();
	for(i=0; i<(int)vAxis.size(); i++)
	{
		vAxis[i]->SetTickLabelShow(false);
//		vAxis[i]->SetIndependent(true);

		if(vAxis[i]->GetLocation()==kLocationCenterVL)
		{
			vAxis[i]->SetTickPosition(kTickPositionBoth);
			vAxis[i]->SetMinorTickPosition(kTickPositionBoth);
			vAxis[i]->SetTickLength(2*vAxis[i]->GetTickLength());
			vAxis[i]->SetMinorTickLength(2*vAxis[i]->GetMinorTickLength());
		}
	}
	vAxis = GetYAxes();
	for(i=0; i<(int)vAxis.size(); i++)
	{
		vAxis[i]->SetTickLabelShow(false);
//		vAxis[i]->SetIndependent(true);
		
		if(vAxis[i]->GetLocation()==kLocationCenterHB)
		{
			vAxis[i]->SetTickPosition(kTickPositionBoth);
			vAxis[i]->SetMinorTickPosition(kTickPositionBoth);
			vAxis[i]->SetTickLength(2*vAxis[i]->GetTickLength());
			vAxis[i]->SetMinorTickLength(2*vAxis[i]->GetMinorTickLength());
		}
	}
	SetAxesCompact();
}

void	CFixAxisPlotImpl::SetCurrentCurve(int nIndex)
{
	if(nIndex<0 || nIndex>=GetPlotDataCount())return;
	SetIndexToHoldCursor(nIndex);
}

void	CFixAxisPlotImpl::GetLastPlotRange(double xRange[2], double yRange[2])
{
	GetPlotRange(xRange, yRange, GetDataID(m_nIndexToHoldCursor));
}

void	CFixAxisPlotImpl::GetLastXRange(double xRange[2])
{
	double range[2];
	GetPlotRange(xRange, range, GetDataID(m_nIndexToHoldCursor));
}

void	CFixAxisPlotImpl::GetLastYRange(double yRange[2])
{
	double range[2];
	GetPlotRange(range, yRange, GetDataID(m_nIndexToHoldCursor));
}

int		CFixAxisPlotImpl::AddData( MyVData2D vData )
{
	int dataID = CXYPlotImpl::AddData(vData);
	if(dataID>=0)
	{
		CPlotData<DataPoint2D> *plot = GetPlotData(dataID);
		if(!plot)return -1;
		CFixAxisProp *pFP = new CFixAxisProp;
		plot->SetExtProp(pFP);
	}
	return dataID;
}


CFixAxisPlotImplI::CFixAxisPlotImplI()
{

}

CFixAxisPlotImplI::~CFixAxisPlotImplI()
{
	
}

tstring	CFixAxisPlotImplI::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	int i;
	double data[2];
	
	tostringstream ostr;
	tstring str;
	switch(m_nPointDesType)
	{
	case kDesCoords:
	case kDesXY:
		return CXYPlotImpl::GetDescriptionByMousePos(hDC, point);
	case kDesXAndYAll:
		if(GetPlotDataCount()<=0)
		{
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				return _TEXT("No data");
			case kLangChinese:
				return _TEXT("无数据");
			}
		}
		RECT plotRect;
		plotRect = GetLastPlotRect();
		if(!PtInRect(&plotRect, point))
		{
			goto Elements;
		}
		LPToData(&point, data);
		double xRange[2];
		GetXDataRange(xRange);
		if(data[0]-GetMinOffsetX()<xRange[0] || data[0]-GetMaxOffsetX()>xRange[1])
		{
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				return _TEXT("Exceed data range");
			case kLangChinese:
				return _TEXT("超过数据范围");
			}
		}
		
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("At mouse row");
			ostr<<_TEXT("\r\nX Coord:");
			str = ostr.str();
			break;
		case kLangChinese:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("鼠标所在列");
			ostr<<_TEXT("\r\nX坐标:");
			str = ostr.str();
			break;
		}
		for(i=0; i<GetPlotDataCount(); i++)
		{
			if(GetAbsData(i).size()==0)continue;
			LPToData(&point, data, i);
			ostr.str(_TEXT(""));
			ostr<<fixed<<setprecision(2)<<data[0]-GetAbsPlotData(i)->fOffsetX;
			str += ostr.str();
			if(i!=GetPlotDataCount()-1)
			{
				ostr.str(_TEXT(""));
				ostr<<_TEXT(",");
				str += ostr.str();
			}
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("\r\nY Coord:");
			str += ostr.str();
			break;
		case kLangChinese:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("\r\nY坐标:");
			str += ostr.str();
			break;
		}
		for(i=0; i<GetPlotDataCount(); i++)
		{
			if(GetAbsData(i).size()==0)continue;
			ostr.str(_TEXT(""));
			ostr<<fixed<<setprecision(2)<<GetValueFromX(data[0], i)-GetAbsPlotData(i)->fOffsetY;
			str += ostr.str();
			if(i!=GetPlotDataCount()-1)
			{
				ostr.str(_TEXT(""));
				ostr<<_TEXT(",");
				str += ostr.str();
			}
		}
		break;
		case kDesElements:
Elements:
			return CXYPlotImpl::GetDescriptionByMousePos(hDC, point);
	}
	
	if(IsOscilloScopeMode())
	{
		if(m_bEnableXZone)
		{
			ostr.str(_TEXT(""));
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				ostr<<__TEXT("\r\nX zone range:(")<<GetXZoneLower()<<__TEXT(", ")<<GetXZoneUpper()<<_TEXT(")\r\nX zone gap: ")<<GetXZoneGap();
				break;
			case kLangChinese:
				ostr<<__TEXT("\r\nX区间范围:(")<<GetXZoneLower()<<__TEXT(", ")<<GetXZoneUpper()<<_TEXT(")\r\nX区间间距: ")<<GetXZoneGap();
				break;
			}
			str += ostr.str();
		}
		if(m_bEnableYZone)
		{
			ostr.str(_TEXT(""));
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				ostr<<__TEXT("\r\nY zone range:(")<<GetYZoneLower()<<__TEXT(", ")<<GetYZoneUpper()<<_TEXT(")\r\nY zone gap: ")<<GetYZoneGap();
				break;
			case kLangChinese:
				ostr<<__TEXT("\r\nY区间范围:(")<<GetYZoneLower()<<__TEXT(", ")<<GetYZoneUpper()<<_TEXT(")\r\nY区间间距: ")<<GetYZoneGap();
				break;
			}
			str += ostr.str();
		}
	}
	return str;
}
