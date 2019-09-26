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
#include "DataSet.h"

#include "../Accessary/grid_lib/grid_lib.h"
#include "InterConnectImpl.h"

Declare_Namespace_CChart

#define  AddSeries AddCurve

template<class PlotImplT>
class CDataSetImpl : public CDataSet<DataPoint2D>,
					 public CInterConnectImpl<CDataSetImpl<PlotImplT> >
{
public:
	CDataSetImpl();
	virtual ~CDataSetImpl();

public:
	PlotImplT			*GetPlot();

public:
	void				SetPlotColors();
	void				SetPlotLineStyles();
	void				SetPlotLineSizes();

public:
	void				DrawDataSet(HDC hDC, double *xRange, double *yRange);
	virtual	void		DrawPlotDataEach(HDC hDC, double *xRange, double *yRange);
	virtual	void		DrawSinglePlotData(int which, HDC hDC, double *xRange, double *yRange);

public:
	void				SetDataDragMode( int dataID, bool bDrag, POINT pointStart );
	void				SetDataDragMode( int dataID, bool drag );

public:
	int					AddPlotData( MyVData2D vData, int plottype );
	int					AddEmpty();
	int					AddCurve( MyVData2D vData,int plottype = kXYPlotConnect );
	int					AddCurve( tstring *pStrTime, tstring fmt, double *pY, int nLen )
	{
		return AddCurveByStrTime(pStrTime, fmt, pY, nLen);
	}
	int					AddCurveByStrTime( tstring *pStrTime, tstring fmt, double *pY, int nLen );
	int					AddCurve(double *pX, double (*func)(double x), int nLen);
	int					AddCurve(double *pV, double (*funcx)(double v), double (*funcy)(double v),int nLen);
	int					AddCurve( double *pY, int nLen );
	template<typename T1, typename T2>
	int					AddCurve( vector<T1> vX, vector<T2> vY )
	{
		if(vX.size()<=0 || vY.size()<=0 )return -1;
		MyVData2D vData;
		if(m_nReservedDataLength>0)
		{
			vData.reserve(m_nReservedDataLength);
		}
		
		DataPoint2D dp;
		int nLen = (int)min(vX.size(), vY.size());
		for(int i=0;i<nLen;i++)
		{
			dp.val[0]=vX[i];
			dp.val[1]=vY[i];
			vData.push_back(dp);
		}
		return AddCurve(vData);
	}
	template<typename T1, typename T2>
	int					AddCurve( T1 *pX, T2 *pY, int nLen )
	{
		if(!pX || !pY || nLen<=0)return -1;
		MyVData2D vData;
		if(m_nReservedDataLength>0)
		{
			vData.reserve(m_nReservedDataLength);
		}
		
		DataPoint2D dp;
		for(int i=0;i<nLen;i++)
		{
			dp.val[0]=pX[i];
			dp.val[1]=pY[i];
			vData.push_back(dp);
		}
		return AddCurve(vData);
	}
	template<typename T1, typename T2>
	int					AddCurve( T1 *pX, T2 *pY, double *pErrLB, double *pErrUB, int nLen )
	{		
		if(!pX || !pY || nLen<=0)return -1;
		
		MyVData2D vData;
		DataPoint2D dp;
		vector<double> vlb, vub;
		if(GetReservedDataLength()>0)
		{
			vData.reserve(GetReservedDataLength());
			vlb.reserve(GetReservedDataLength());
			vub.reserve(GetReservedDataLength());
		}
		
		for(int i=0;i<nLen;i++)
		{
			dp.val[0]=pX[i];
			dp.val[1]=pY[i];
			vData.push_back(dp);
			vlb.push_back(pErrLB[i]);
			vub.push_back(pErrUB[i]);
		}
		int dataID = AddCurve(vData);

		GetPlotData(dataID)->SetLBound(vlb);
		GetPlotData(dataID)->SetUBound(vub);
		GetPlotData(dataID)->SetErrorBarMode(true);
		
		GetPlotData(dataID)->SetDataRangeSet(false);
		GetDataRange(dataID);
		
		return dataID;
	}
	template<typename T1, typename T2>
	int					AddCurve( T1 *pX, T2 *pY, double *pErrB, int nLen )
	{
		return AddCurve(pX, pY, pErrB, pErrB, nLen);
	}
	template<typename T1, typename T2>
	int					UpdateCurve( int dataID, T1 *pX, T2 *pY, int nLen )
	{
		if(GetIndex(dataID)<0)return -1;
		if(!pX || !pY || nLen<=0)return -1;
		
		MyVData2D vData;
		if(m_nReservedDataLength>0)
		{
			vData.reserve(m_nReservedDataLength);
		}
		
		DataPoint2D dp;
		for(int i=0;i<nLen;i++)
		{
			dp.val[0]=pX[i];
			dp.val[1]=pY[i];
			vData.push_back(dp);
		}
		return SetData(dataID, vData);
	}

	// no range update!!!
	template<typename T1, typename T2>
	int					ReplaceCurve( int dataID, T1 *pX, T2 *pY, int nLen )
	{
		if(GetIndex(dataID)<0)return -1;
		if(!pX || !pY || nLen<=0)return -1;
		
		MyVData2D *pvData = new MyVData2D;
		if(m_nReservedDataLength>0)
		{
			pvData->reserve(m_nReservedDataLength);
		}
		
		DataPoint2D dp;
		for(int i=0;i<nLen;i++)
		{
			dp.val[0]=pX[i];
			dp.val[1]=pY[i];
			pvData->push_back(dp);
		}

		GetPlotData(dataID)->SetData(pvData);
		return dataID;
	}

	int					AddPointFinal(DataPoint2D dp2d, int nWhich);
	
	template<typename T1, typename T2>
	int					AddPoint(T1 x, T2 y, int nWhich)
	{
		if(nWhich < 0 || nWhich > GetPlotDataCount())return -1;
		
		if( nWhich == GetPlotDataCount() )
		{
			double pX[1], pY[1];
			pX[0] = x;
			pY[0] = y;
			return AddCurve(pX, pY, 1);
		}
		
		DataPoint2D dp2d;
		dp2d.val[0] = x;
		dp2d.val[1] = y;
		return AddPointFinal(dp2d, nWhich);
	}

	template<typename T1, typename T2>
	int					AddPoint(T1 x, T2 y)
	{
		if(GetPlotDataCount()<=0)
		{
			double pX[1], pY[1];
			pX[0] = x;
			pY[0] = y;
			return AddCurve(pX, pY, 1);
		}
		else
		{
			return AddPoint(x, y, 0);
		}
	}
	
	template<typename T1>
	int					AddPoint( T1 y )
	{
		if(GetPlotDataCount() <= 0)
		{
			return AddPoint(1, y);
		}
		else
		{
			double x;
			x = GetAbsData(0)[GetAbsData(0).size()-1].val[0] + 1;
			return AddPoint(x, y);
		}
	}
	int					AddPointByStrTime( tstring strTime, tstring fmt, double y, int nWhich );
	int					AddPointByStrTime( tstring strTime, tstring fmt, double y);

	template<typename T1, typename T2>
	int					InsertPoint( T1 x, T2 y, int nPos, int nWhich )
	{
		double X = (double)x;
		double Y = (double)y;
		return InsertPoint(X, Y, nPos, nWhich);
	}
	int					InsertPoint( double x, double y, int nPos, int nWhich );
	virtual	int			InsertPointFinal( DataPoint2D dp2d, int nPos, int nWhich );

	int					RemovePoint( int dataID, int nPointIndex );
	int					RemoveFirstPoint( int dataID);
	int					RemoveLastPoint( int dataID);
	int					RemovePointAndShift( int dataID, int nPointIndex );
	int					RemoveFirstPointAndShift( int dataID);
	int					ReplaceFirstPoint( DataPoint2D dp2d, int dataID );
	int					ReplaceFirstPoint( double x, double y, int dataID);
	int					ReplaceLastPoint( DataPoint2D dp2d, int dataID );
	int					ReplaceLastPoint( double x, double y, int dataID );
	int					ReplacePointOnly( DataPoint2D dp2d, int nPointIndex, int dataID );
	int					ReplacePointOnly( double x, double y, int nPointIndex, int dataID );

	int					CrowdOutPoint( double x, double y, int nPointIndex, int nWhich);
	int					CrowdOutFirstPoint( double x, double y, int nWhich);
	
	int					SlipXValue(int step, int dataID);
	int					SlipYValue(int step, int dataID);
	
	int					AddPointByID( double x, double y, int dataID );
	int					AddPointByID( tstring strTime, tstring fmt, double y, int dataID );
	int					AddPointFinalByID( DataPoint2D dp2d, int dataID );

public:
	void				SwapXY(int dataID);
	void				SortData(int dataID, int nSortType);

public:
	virtual	void		ShowDataInGrid(HWND hWndParent);
	virtual	void		ShowDataInGrid(int nWhich, HWND hWndParent);

public:
	virtual	bool		WriteToFile( tstring pathName );
	virtual	bool		WriteToFile( int which, tstring pathName );
	virtual	bool		WriteToBuff( tstring &buff );
	virtual	bool		ReadFromFile( tstring pathName );
	virtual	bool		ReadFromFile( int which, tstring pathName );
	virtual	bool		ReadFromBuff( tstring buff );
	int					GetNumsInStr( tstring buff);

protected:
	virtual	int			AddData( MyVData2D vData );
	virtual	int			SetData( int dataID, MyVData2D vData );
	virtual	int			SetData( int dataID, MyVData2D *pvData );
};

template<class PlotImplT>
CDataSetImpl<PlotImplT>::CDataSetImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CDataSetImpl<PlotImplT>::~CDataSetImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
PlotImplT	*CDataSetImpl<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::SetPlotColors()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int nNum=GetPlotDataCount();
	short r,g,b;
	for( int i = 0; i < nNum; i++ )
	{
		//Set color and line size
		if(pT->IsAutoDataColorGeneral() && GetAbsPlotData(i)->IsAutoColor())
		{
			switch(pT->GetModeOfSetColors())
			{
			case kSetColorFromTwoColor:
				r= ( GetRValue(pT->GetLineColor1())*i + GetRValue(pT->GetLineColor2())*(nNum-i) )/nNum;
				g= ( GetGValue(pT->GetLineColor1())*i + GetGValue(pT->GetLineColor2())*(nNum-i) )/nNum;
				b= ( GetBValue(pT->GetLineColor1())*i + GetBValue(pT->GetLineColor2())*(nNum-i) )/nNum;
				GetAbsPlotData(i)->SetPlotColor(RGB(r,g,b));
				break;
			default:
				GetAbsPlotData(i)->SetPlotColor(CPlot::m_pColorTable[(i+pT->GetAutoColorStartIndex())%CPlot::m_nColorsInTable]);
				break;
			}
		}
		else
		{
			GetAbsPlotData(i)->SetPlotColor(GetAbsPlotData(i)->GetDataColor());
		}
		GetAbsPlotData(i)->SetPlotColor(CPlot::ChooseLineColor(GetAbsPlotData(i)->GetPlotColor(), GetAbsPlotData(i)->IsLighted(), GetAbsPlotData(i)->IsSelected()));
	}
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::SetPlotLineStyles()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	for( int i = 0; i < GetPlotDataCount(); i++ )
	{
		if(pT->IsAutoLineStyleGeneral() && GetAbsPlotData(i)->IsAutoLineStyle())
			GetAbsPlotData(i)->SetPlotLineStyle(i%5);
		else
			GetAbsPlotData(i)->SetPlotLineStyle(GetAbsPlotData(i)->GetDataLineStyle());
	}
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::SetPlotLineSizes()
{
	for( int i = 0; i < GetPlotDataCount(); i++ )
	{
		GetAbsPlotData(i)->SetPlotLineSize(CPlot::ChooseLineWidth(GetAbsPlotData(i)->GetDataLineSize(), GetAbsPlotData(i)->IsLighted(), GetAbsPlotData(i)->IsSelected()));
	}
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::DrawDataSet(HDC hDC, double *xRange, double *yRange)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	RECT plotRect = pT->GetLastPlotRect();

	// Create a region for clipping
	int saveDC = SaveDC(hDC);
	
	//PlotRect.InflateRect( 1, -1, -1, 1 );
	//InflateRect(&plotRect,2,2);
	LPtoDP( hDC,(LPPOINT) &plotRect, 2); 
	
	HRGN hRgn;
	if(!pT->IsClipEllipse())
		hRgn=CreateRectRgn( plotRect.left, plotRect.top, plotRect.right, plotRect.bottom );
	else
		hRgn = CreateEllipticRgn(plotRect.left, plotRect.top, plotRect.right, plotRect.bottom);
	SelectObject(hDC,hRgn);
	
	DPtoLP( hDC,(LPPOINT) &plotRect, 2);
	// Begin draw
/*	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		//m_vpPlotData[i]->DrawPlotData(hDC, plotRect, xRange, yRange);
		DrawSinglePlotData(i, hDC, xRange, yRange);
	}
*/
	DrawPlotDataEach(hDC, xRange, yRange);
	if(m_bInterConnect)DrawInterConnection(hDC, plotRect, xRange, yRange);
	
	RestoreDC(hDC, saveDC );
	
	DeleteObject(hRgn);
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::DrawPlotDataEach(HDC hDC, double *xRange, double *yRange)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	// Begin draw
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		//m_vpPlotData[i]->DrawPlotData(hDC, plotRect, xRange, yRange);
		DrawSinglePlotData(i, hDC, xRange, yRange);
	}
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::DrawSinglePlotData(int which, HDC hDC, double *xRange, double *yRange)
{
	if(which<0 || which>=GetPlotDataCount())return;

	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	RECT plotRect = pT->GetLastPlotRect();

	m_vpPlotData[which]->DrawPlotData(hDC, plotRect, xRange, yRange);
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::SetDataDragMode( int dataID, bool bDrag, POINT pointStart )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int index = GetIndex(dataID);
	if(index < 0)return;
	
	m_vpPlotData[index]->bDragMode = bDrag;
	
	pT->m_ptOrigin = pointStart;
	pT->m_ptCurr = pointStart;
	
	pT->m_fOffsetXOrigin = m_vpPlotData[index]->fOffsetX;
	pT->m_fOffsetYOrigin = m_vpPlotData[index]->fOffsetY;
}

template<class PlotImplT>
void	CDataSetImpl<PlotImplT>::SetDataDragMode( int dataID, bool bDrag )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	int index = GetIndex(dataID);
	if(index < 0)return;
	
	m_vpPlotData[index]->bDragMode = bDrag;
}

template<class PlotImplT>
int	CDataSetImpl<PlotImplT>::AddData( MyVData2D vData )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	CPlotDataImpl<DataPoint2D> *plotData = new CPlotDataImpl<DataPoint2D>(pT);
	if(pT->GetReservedDataLength()>0)
	{
		plotData->GetData().reserve(pT->GetReservedDataLength());
	}
	int returnVal = plotData->SetData( vData );
	if( returnVal == false ) return -1;
	
	if(!pT->IsKeepFormer())DeleteAllData();

	MyLock();
	
	// Now add this to our typed pointer array
	plotData->SetDataID(m_nCurDataID);
	((CPlotData<DataPoint2D>*)plotData)->SetDefaults(  );

	plotData->SetDataSet(this);
	
	m_vpPlotData.push_back(plotData);
	//GetDataRanges();
	plotData->GetDataRange();
	
	pT->SetNewDataComming(true);
	plotData->SetSorted(false);
	
	//Rename the data name seqencely
	tostringstream ostr;
	ostr.fill('0');
	ostr.width(3);
	ostr<<GetIndex( m_nCurDataID  );
	SetDataTitle(m_nCurDataID, GetDataTitle(m_nCurDataID)+ostr.str());
	
	m_nCurDataID++;
	
	MyUnlock();
	
	// Post action
	//	if(m_fcnPostAddData)
	//	{
	//		int dataID = m_nCurDataID - 1;
	//		(*m_fcnPostAddData)((CXYPlotImpl *)this, &dataID);
	//	}
	
	return (m_nCurDataID -1);
}

template<class PlotImplT>
int	CDataSetImpl<PlotImplT>::SetData( int dataID, MyVData2D vData )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int index = GetIndex( dataID );
	if( index >= GetPlotDataCount() || index < 0 ) return -1;
	
	if(pT->GetReservedDataLength()>0)
	{
		m_vpPlotData[index]->GetData().reserve(pT->GetReservedDataLength());
	}
	
	MyLock();
	if( m_vpPlotData[index]->SetData( vData ) )
	{
		m_vpPlotData[index]->SetDataRangeSet(false);
		m_vpPlotData[index]->GetDataRange();
		pT->SetNewDataComming(true);
		m_vpPlotData[index]->SetSorted(false);
		m_vpPlotData[index]->bForceUpdate = true;
		//m_bRangeChanged = true;
	}
	else
	{
		dataID = -1;
	}
	MyUnlock();
	return dataID;
}

template<class PlotImplT>
int	CDataSetImpl<PlotImplT>::SetData( int dataID, MyVData2D *pvData )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	int index = GetIndex( dataID );
	if( index >= GetPlotDataCount() || index < 0 ) return -1;
	
	if(pT->GetReservedDataLength()>0)
	{
		m_vpPlotData[index]->GetData().reserve(pT->GetReservedDataLength());
	}
	
	MyLock();
	if( m_vpPlotData[index]->SetData( pvData ) )
	{
		m_vpPlotData[index]->SetDataRangeSet(false);
		m_vpPlotData[index]->GetDataRange();
		pT->SetNewDataComming(true);
		m_vpPlotData[index]->bForceUpdate = true;
		//m_bRangeChanged = true;
	}
	else
	{
		dataID = -1;
	}
	MyUnlock();
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddPlotData(MyVData2D vData, int plottype)
{
	int dataID = AddData(vData);
	if(dataID>=0)
	{
		m_vpPlotData[GetIndex(dataID)]->SetPlotType(plottype);
		return dataID;
	}
	return -1;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddEmpty()
{
	MyVData2D vd;
	vd.clear();
	return AddData(vd);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddCurve(MyVData2D vData, int plottype)
{
	return AddPlotData(vData, plottype);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddCurve(double *pX, double (*func)(double x), int nLen)
{
	if(!pX || !func || nLen<=0)return -1;
	
	double *pY = new double[nLen];
	int i;
	for(i=0; i<nLen; i++)
	{
		pY[i] = func(pX[i]);
	}
	int dataID = AddCurve(pX, pY, nLen);

	delete []pY;
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddCurve(double *pV, double (*funcx)(double v), double (*funcy)(double v),int nLen)
{
	if(!pV || !funcx || !funcy || nLen<=0)return -1;
	double *pX = new double[nLen];
	double *pY = new double[nLen];
	int i;
	for(i=0; i<nLen; i++)
	{
		pX[i] = funcx(pV[i]);
		pY[i] = funcy(pV[i]);
	}
	int dataID = AddCurve(pX, pY, nLen);

	delete []pX;
	delete []pY;

	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddCurveByStrTime(tstring *pStrTime, tstring fmt, double *pY, int nLen)
{
	if(!pStrTime || !pY || nLen<=0)return -1;
	int dataID;
	
	double *pTime = new double[nLen];
	for(int i=0; i<nLen; i++)
	{
		if(!MyStr2Time(pStrTime[i], fmt, pTime[i]))
		{
			delete []pTime;
			return -1;
		}
	}
	
	dataID = AddCurve(pTime, pY, nLen);
	
	delete []pTime;
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddCurve( double *pY, int nLen )
{
	if(!pY || nLen<=0)return -1;
	double *pX = new double[nLen];
	for(int i=0; i<nLen; i++)
	{
		pX[i] = i + 1.0;
	}
	int dataID = AddCurve(pX, pY, nLen);
	delete []pX;
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddPointFinal(DataPoint2D dp2d, int nWhich)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(nWhich < 0 || nWhich > GetPlotDataCount())return -1;
	
	if( nWhich == GetPlotDataCount() )
	{
		double pX[1], pY[1];
		pX[0] = dp2d.val[0];
		pY[0] = dp2d.val[1];
		return AddCurve(pX, pY, 1);
	}
	
	MyLock();
	if(m_nMaxPoints>0)
	{
		if(m_bMaxPointsReset)
		{
			if((int)GetAbsData(nWhich).size()>=m_nMaxPoints)
			{
				GetAbsPlotData(nWhich)->GetData().clear();
			}
		}
		else
		{
			while((int)GetAbsData(nWhich).size()>=m_nMaxPoints)
			{
				GetAbsPlotData(nWhich)->GetData().erase(GetAbsPlotData(nWhich)->GetData().begin());
			}
		}
	}
	GetAbsPlotData(nWhich)->GetData().push_back(dp2d);
	UpdateDataRanges(dp2d, nWhich);
	pT->SetNewDataComming( true );
	GetAbsPlotData(nWhich)->SetSorted(false);
	GetAbsPlotData(nWhich)->bForceUpdate = true;
	pT->SetXRangeSet(false);
	pT->SetYRangeSet(false);
	
	MyUnlock();
	
	return GetDataID(nWhich);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddPointByStrTime( tstring strTime, tstring fmt, double y, int nWhich )
{
	double tTime;
	if(!MyStr2Time(strTime, fmt, tTime))
		return -1;
	
	return AddPoint(tTime, y, nWhich);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddPointByStrTime( tstring strTime, tstring fmt, double y )
{	
	return AddPointByStrTime(strTime, fmt, y, 0);
}



template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::InsertPoint( double x, double y, int nPos, int nWhich )
{
	if(nWhich < 0 || nWhich >= GetPlotDataCount())return -1;
	if(nPos<0 || nPos>=(int)GetAbsData(nWhich).size())return -1;
	
	DataPoint2D dp2d;
	dp2d.val[0] = x;
	dp2d.val[1] = y;
	return InsertPointFinal(dp2d, nPos, nWhich);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::InsertPointFinal( DataPoint2D dp2d, int nPos, int nWhich )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(nWhich < 0 || nWhich > GetPlotDataCount())return -1;
	if(nPos<0 || nPos>(int)GetAbsData(nWhich).size())return -1;
	
	MyLock();
	
	if(m_nMaxPoints>0)
	{
		if(m_bMaxPointsReset)
		{
			if((int)GetAbsData(nWhich).size()>=m_nMaxPoints)
			{
				GetAbsPlotData(nWhich)->GetData().clear();
			}
		}
		else
		{
			while((int)GetAbsData(nWhich).size()>=m_nMaxPoints)
			{
				GetAbsPlotData(nWhich)->GetData().erase(GetAbsPlotData(nWhich)->GetData().begin());
			}
		}
	}
	GetAbsPlotData(nWhich)->GetData().insert( GetAbsPlotData(nWhich)->GetData().begin()+nPos, dp2d );
	UpdateDataRanges(dp2d, nWhich);
	pT->SetNewDataComming(true);
	GetAbsPlotData(nWhich)->bForceUpdate = true;
	
	MyUnlock();
	
	return GetDataID(nWhich);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::RemovePoint( int dataID, int nPointIndex )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int nIndex = GetIndex(dataID);
	if(nIndex < 0)return -1;
	
	if(nPointIndex<0 || nPointIndex>= (int)GetAbsData(nIndex).size())return -1;
	
	MyLock();
	GetAbsPlotData(nIndex)->GetData().erase(GetAbsPlotData(nIndex)->GetData().begin()+nPointIndex);
	MyUnlock();
	
	GetAbsPlotData(nIndex)->SetDataRangeSet(false);
	GetDataRange(dataID);
	pT->SetNewDataComming(true);
	GetAbsPlotData(nIndex)->bForceUpdate = true;
	
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::RemoveFirstPoint(int dataID)
{
	return RemovePoint(dataID, 0);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::RemoveLastPoint(int dataID)
{
	int nIndex = GetIndex(dataID);
	if(nIndex < 0)return -1;
	
	return	RemovePoint(dataID, (int)GetAbsData(nIndex).size()-1);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::RemovePointAndShift( int dataID, int nPointIndex )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int nIndex = GetIndex(dataID);
	if(nIndex < 0)return -1;
	
	if(nPointIndex<0 || nPointIndex>= (int)GetAbsData(nIndex).size())return -1;
	
	MyLock();
	for(int i=nPointIndex; i<(int)GetAbsPlotData(nIndex)->GetData().size()-1; i++)
	{
		GetAbsPlotData(nIndex)->GetData()[i] = GetAbsPlotData(nIndex)->GetData()[i+1];
	}
	GetAbsPlotData(nIndex)->GetData().pop_back();
	MyUnlock();
	
	GetAbsPlotData(nIndex)->SetDataRangeSet(false);
	GetDataRange(dataID);
	pT->SetNewDataComming(true);
	GetAbsPlotData(nIndex)->bForceUpdate = true;
	
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::RemoveFirstPointAndShift(int dataID)
{
	return RemovePointAndShift(dataID, 0);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::ReplaceFirstPoint(DataPoint2D dp2d, int dataID)
{
	RemoveFirstPoint(dataID);
	return InsertPoint(dp2d.def.x, dp2d.def.y, 0, GetIndex(dataID));
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::ReplaceFirstPoint(double x, double y, int dataID)
{
	RemoveFirstPoint(dataID);
	return InsertPoint(x, y, 0, GetIndex(dataID));
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::ReplaceLastPoint(DataPoint2D dp2d, int dataID)
{
	RemoveLastPoint(dataID);
	return AddPoint(dp2d.def.x, dp2d.def.y, GetIndex(dataID));
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::ReplaceLastPoint(double x, double y, int dataID)
{
	RemoveLastPoint(dataID);
	return AddPoint(x, y, GetIndex(dataID));
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::ReplacePointOnly(DataPoint2D dp2d, int nPointIndex, int dataID)
{
	int nIndex = GetIndex(dataID);
	if(nIndex<0)return -1;
	MyLock();
	m_vpPlotData[nIndex]->ReplacePoint(dp2d, nPointIndex);
	MyUnlock();
	return nIndex;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::ReplacePointOnly(double x, double y, int nPointIndex, int dataID)
{
	DataPoint2D dp2d;
	dp2d.def.x = x;
	dp2d.def.y = y;
	return ReplacePointOnly(dp2d, nPointIndex, dataID);
}


template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::CrowdOutPoint(double x, double y, int nPointIndex, int nWhich)
{
	int dataID;
	dataID = AddPoint(x, y, nWhich);
	if(dataID<0)return dataID;
	
	return RemovePointAndShift(dataID, nPointIndex);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::CrowdOutFirstPoint(double x, double y, int nWhich)
{
	return CrowdOutPoint(x, y, 0, nWhich);
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::SlipXValue(int step, int dataID)
{
	int nIndex = GetIndex(dataID);
	if(nIndex < 0)return -1;
	
	if(step == 0)return -1;
	
	if((int)GetAbsData(nIndex).size()<=abs(step))return -1;
	
	int id1, id2, nP;
	nP = (int)GetAbsPlotData(nIndex)->GetData().size();
	MyLock();
	for(int i=0; i<nP-abs(step); i++)
	{
		id1 = i;
		id2 = i+step;
		while(id2 < 0) id2 += nP;
		while(id2 >= nP)id2 -= nP;
		swap(GetAbsPlotData(nIndex)->GetData()[id1].val[0], GetAbsPlotData(nIndex)->GetData()[id2].val[0]);
	}
	MyUnlock();
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::SlipYValue(int step, int dataID)
{
	int nIndex = GetIndex(dataID);
	if(nIndex < 0)return -1;
	
	if(step == 0)return -1;
	
	if((int)GetAbsData(nIndex).size()<=abs(step))return -1;
	
	int id1, id2, nP;
	nP = (int)GetAbsPlotData(nIndex)->GetData().size();
	MyLock();
	for(int i=0; i<nP-abs(step); i++)
	{
		id1 = i;
		id2 = i-step;
		while(id2 < 0) id2 += nP;
		while(id2 >= nP)id2 -= nP;
		swap(GetAbsPlotData(nIndex)->GetData()[id1].val[1], GetAbsPlotData(nIndex)->GetData()[id2].val[1]);
	}
	MyUnlock();
	return dataID;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddPointByID( double x, double y, int dataID )
{
	return AddPoint(x, y, GetIndex(dataID));
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddPointByID( tstring strTime, tstring fmt, double y, int dataID )
{
	return AddPoint(strTime, fmt, y, GetIndex(dataID));
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::AddPointFinalByID( DataPoint2D dp2d, int dataID )
{
	return AddPointFinal(dp2d, GetIndex(dataID));
}

template<class PlotImplT>
void		CDataSetImpl<PlotImplT>::SwapXY(int dataID)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int nIndex = GetIndex(dataID);
	if(nIndex<0)return;
	
	MyLock();
	for(int i=0; i<(int)GetAbsData(nIndex).size(); i++)
	{
		swap(GetAbsPlotData(nIndex)->GetData()[i].val[0], GetAbsPlotData(nIndex)->GetData()[i].val[1]);
	}
	MyUnlock();
	m_vpPlotData[nIndex]->SetDataRangeSet(false);
	GetDataRange(dataID);
	pT->SetNewDataComming(true);
}

template<class PlotImplT>
void		CDataSetImpl<PlotImplT>::SortData(int dataID, int nSortType)
{
	int nIndex = GetIndex(dataID);
	if(nIndex<0)return;
	
	GetAbsPlotData(nIndex)->SetSortType(nSortType);
	MyLock();
	GetAbsPlotData(nIndex)->SortData();
	MyUnlock();
	
	m_vpPlotData[nIndex]->bForceUpdate = true;
}

template<class PlotImplT>
bool		CDataSetImpl<PlotImplT>::WriteToFile( tstring pathName )
{
	tofstream			ofs;
	
	if( GetPlotDataCount() <= 0 ) return false;

	tstring buff;
	if(!WriteToBuff(buff))return false;
	
#if defined(_UNICODE) || defined(UNICODE)
	string name;
	if(!mywcs2mbs(pathName, name))return false;
	ofs.open(name.c_str(), tios::out);
#else
	ofs.open(pathName.c_str(),tios::out);
#endif
	
	if(!ofs.is_open())return false;
	ofs.write(buff.c_str(), buff.length());
	ofs.close();
	
	return true;
}

template<class PlotImplT>
bool		CDataSetImpl<PlotImplT>::WriteToFile( int which, tstring pathName )
{
	if( which < 0 || which >= GetPlotDataCount() ) return false;
	
	CPlotData<DataPoint2D>		*dataSet = GetAbsPlotData(which);
	
	if( dataSet == NULL ) return false;
	
	if(!dataSet->WriteToFile(pathName))return false;
	
	return true;
}

template<class PlotImplT>
bool		CDataSetImpl<PlotImplT>::WriteToBuff( tstring &buff )
{
	int					whichDataSet;
	CPlotData<DataPoint2D>		*dataSet;
	
	if( GetPlotDataCount() <= 0 ) return false;
	int nCount = GetPlotDataCount();
	
	buff = _TEXT("");
	if(m_bSequence)
	{
		for( whichDataSet = 0; whichDataSet < nCount; whichDataSet++ )
		{
			dataSet = GetAbsPlotData(whichDataSet);
			
			if( dataSet == NULL ) continue;
			if(!dataSet->AppendToBuff(buff))
			{
				return false;
			}
			if( whichDataSet != nCount - 1 )
			{
				buff += _TEXT("\r\n");
				buff += m_strFileSeparator;
				buff += _TEXT("\r\n");
			}
		}
		return true;
	}
	else
	{
		tstring *buffs = new tstring[nCount];
		tstring::size_type *pPos = new tstring::size_type[nCount];
		int lines, maxlines = 0;

		for( whichDataSet = 0; whichDataSet < nCount; whichDataSet++ )
		{
			dataSet = GetAbsPlotData(whichDataSet);
			
			if( dataSet == NULL )
			{
				delete []buffs;
				delete []pPos;
				return false;
			}
			if(!dataSet->WriteToBuff(buffs[whichDataSet]))
			{
				delete []buffs;
				delete []pPos;
				return false;
			}
			lines = get_line_count(buffs[whichDataSet]);
			if(lines > maxlines)maxlines = lines;

			pPos[whichDataSet] = 0;
		}

		tostringstream ostr;
		tstring line, lbuf;
		double x, y;
		int i, j;
		for( i=0; i<maxlines; i++)
		{
			line = _TEXT("");
			
			for(j=0; j<nCount; j++)
			{
				lbuf = readline(buffs[j], pPos[j]);
				if(!m_bShareX)
				{
					if(lbuf.empty())
					{
						ostr.str(_TEXT(""));
						ostr<<0<<m_strDataSeparator.c_str()<<0<<_TEXT("\n");
						lbuf = ostr.str();
					}
					
					trimspace(lbuf);
					
					if(j > 0)
					{
						lbuf = m_strDataSeparator + lbuf;
					}
				}
				else
				{
					if(lbuf.empty())
					{
						x = 0.0;
						y = 0.0;
					}
					else
					{
						tistringstream istr(lbuf);
						istr>>x>>y;
					} 

					ostr.str(_TEXT(""));
					if(j == 0)
					{
						ostr << x << m_strDataSeparator.c_str() << y;
					}
					else
					{
						ostr << m_strDataSeparator.c_str() << y;
					}
					lbuf = ostr.str();
				}
				
				line += lbuf;
			}
			
			line += _TEXT("\n");
			buff += line;
		}

		delete []buffs;
		delete []pPos;
		return true;
	}
}

template<class PlotImplT>
bool		CDataSetImpl<PlotImplT>::ReadFromFile( tstring pathName )
{
	tifstream			ifs;
#if defined(_UNICODE) || defined(UNICODE)
	string name;
	if(!mywcs2mbs(pathName, name))return false;
	ifs.open(name.c_str());
#else
	ifs.open(pathName.c_str());
#endif
	if(!ifs.is_open())return false;
	
	tstring content;
	int filesize=get_file_size(pathName.c_str());
	TCHAR *pBuff=new TCHAR[filesize+1];
	memset(pBuff, 0, (filesize+1)*sizeof(TCHAR));
	ifs.read(pBuff, filesize);
	content=pBuff;
	delete []pBuff;
	ifs.close();
	
	return ReadFromBuff(content);
}

template<class PlotImplT>
bool		CDataSetImpl<PlotImplT>::ReadFromFile( int which, tstring pathName )
{
	if( which < 0 || which >= GetPlotDataCount() ) return false;
	
	CPlotData<DataPoint2D>	*dataSet = GetAbsPlotData(which);
	if( dataSet == NULL ) return false;
	
	if( !dataSet->ReadFromFile( pathName ) )return false;
	SetDataModified( GetDataID( which ) );
	
	return true;
}

template<class PlotImplT>
int		CDataSetImpl<PlotImplT>::GetNumsInStr(tstring buff)
{
	tstring	numpatt;
	numpatt = _TEXT("-?\\d+(\\.\\d*)?([eE][+-]?\\d*)?");

	CRegexpT <TCHAR> numReg(numpatt.c_str());
	MatchResult rstnum;
	int matchnum;
	int pos1, pos2;
	
	tstring aline;
	
	int count = 0;
	aline = buff;
	matchnum = 0;
	do
	{
		trimspace(aline);
		
		rstnum=numReg.Match(aline.c_str());
		matchnum=rstnum.IsMatched();
		if(matchnum)
		{
			pos1 = rstnum.GetStart();
			pos2 = rstnum.GetEnd();

			aline = aline.substr(pos2);
			
			count ++;
		}
	}while(matchnum);

	return count;
}

template<class PlotImplT>
bool		CDataSetImpl<PlotImplT>::ReadFromBuff( tstring buff )
{
	tstring strSep = m_strFileSeparator;
	tstring::size_type pos;
	
	if(m_bSequence)
	{
		trimspace(strSep);
		
		tstring buffUsed, buffLeft;
		
		//pos = buff.find_first_of(strSep);
		pos = find_first_matchstring(buff, strSep);
		if(pos == tstring::npos)
		{
			buffUsed = buff;
			buffLeft = _TEXT("");
		}
		else
		{
			buffUsed = buff.substr(0, pos);
			buffLeft = buff.substr(pos + m_strFileSeparator.length());
		}
		
		CPlotData<DataPoint2D> dataSet;
		int count = 0;
		while(buffUsed != _TEXT(""))
		{
			if( !dataSet.ReadFromBuff( buffUsed ) )continue;
			int dataID = AddData(dataSet.GetData());
			if(!dataSet.GetTitle().empty() && dataSet.GetTitle()!=CGlobal::GetStrUntitle())
			{
				SetDataTitle( dataID, dataSet.GetTitle() );
			}
			count ++;
			
			//pos = buffLeft.find_first_of(strSep);
			pos = find_first_matchstring(buffLeft, strSep);
			if(pos == tstring::npos)
			{
				buffUsed = buffLeft;
				buffLeft = _TEXT("");
			}
			else
			{
				buffUsed = buffLeft.substr(0, pos);
				buffLeft = buffLeft.substr(pos + strSep.length());
			}
		}
		return count>0;
	}
	else
	{
		tstring	pattern;
		pattern = _TEXT("^(-?\\d+(\\.\\d*)?([eE][+-]?\\d*)?\\s*){2,}$");

		tstring aline, aline1, subline;
		CRegexpT <TCHAR> datalineReg(pattern.c_str());
		MatchResult rst1;
		int match1;
		int pos1, pos2;

		tstring	numpatt;
		numpatt = _TEXT("-?\\d+(\\.\\d*)?([eE][+-]?\\d*)?");
		CRegexpT <TCHAR> numReg(numpatt.c_str());
		MatchResult rstnum;
		int matchnum;

		tstring::size_type curpos;

		double x, y, val;
		int cols, fstCols;;
		bool bFirstLine = true;
		int nCurves, i;

		int *pIndex = 0;
		int dataID;

		curpos=0;
		do
		{
			aline=readline(buff,curpos);
			aline1 = aline;
			trimspace(aline1);

			rst1=datalineReg.MatchExact(aline1.c_str());
			match1=rst1.IsMatched();
			if(match1)
			{
				cols = GetNumsInStr(aline1);
				if(bFirstLine)
				{
					fstCols = cols;

					if(cols < 2)
					{
						return false;
					}

					if(m_bShareX)
					{
						nCurves = cols-1;
					}
					else
					{
						nCurves = cols/2;
					}

					pIndex = new int[nCurves];
					for(i=0; i<nCurves; i++)
					{
						dataID = AddEmpty();
						pIndex[i] = GetIndex(dataID);
					}
					bFirstLine = false;
				}
				if(cols != fstCols)break;

				for(i=0; i<cols; i++)
				{
					rstnum = numReg.Match(aline1.c_str());
					matchnum = rstnum.IsMatched();
					if(matchnum)
					{
						pos1 = rstnum.GetStart();
						pos2 = rstnum.GetEnd();
						subline = aline1.substr(pos1, pos2);
						aline1 = aline1.substr(pos2);
						
						tistringstream istr(subline);
						istr>>val;
					}
					if(m_bShareX)
					{
						if(i == 0)
						{
							x = val;
						}
						else
						{
							y = val;
							AddPoint(x, y, pIndex[i-1]);
						}
					}
					else
					{
						if(i%2 == 0)
						{
							x = val;
						}
						else
						{
							y = val;
							AddPoint(x, y, pIndex[i/2]);
						}
					}
				}
			}
		}while(curpos != tstring::npos);

		if(pIndex)delete []pIndex;
		return false;
	}
}

template<class PlotImplT>
void		CDataSetImpl<PlotImplT>::ShowDataInGrid(HWND hWndParent)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(IsEmpty())return;
	
	POINT lt={0, 0};
	if(IsWindow(hWndParent))
	{
		ClientToScreen(hWndParent, &lt);
	}
	
	HWND	hWndGrid = CreateGrid(hWndParent, CGlobal::MyGetModuleHandle(), WS_OVERLAPPEDWINDOW, lt.x, lt.y, CW_USEDEFAULT, 500);
	if(!IsWindow(hWndGrid))return;
	
	SetWindowText(hWndGrid, pT->GetTitle().c_str());
	
	CGrid *pGrid = GetGrid(hWndGrid);
	
	int datasetcount = GetPlotDataCount();
	int maxpoints = GetMaxPlotDataLength();
	int npoints;
	
	int i, j;
	CellID cid;
	
	pGrid->SetRows(maxpoints+1);
	pGrid->SetCols(2*datasetcount+1);
	//	pGrid->SetFixRowCount(2);
	
	for(i=0; i<datasetcount; i++)
	{
		npoints = GetAbsPlotData(i)->GetDataCount();
		for(j=0; j<npoints; j++)
		{
			cid.x = 2*i+1;
			cid.y = j+1;
			pGrid->SetCellDouble(cid, GetAbsData(i)[j].val[0]);
			cid.x = 2*i+2;
			cid.y = j+1;
			pGrid->SetCellDouble(cid, GetAbsData(i)[j].val[1]);
		}
	}
	
	SendMessage(hWndGrid, WM_SETICON, ICON_BIG, (WPARAM)LoadIcon(CGlobal::MyGetModuleHandle(), LPCTSTR(IDI_ICON_GRID_LIB)));
	ShowWindow(hWndGrid, SW_SHOW);
}

template<class PlotImplT>
void		CDataSetImpl<PlotImplT>::ShowDataInGrid(int nWhich, HWND hWndParent)
{
	if(nWhich<0 || nWhich>=GetPlotDataCount())return;
	if(GetAbsData(nWhich).size()<=0)return;
	
	POINT lt={0, 0};
	if(IsWindow(hWndParent))
	{
		ClientToScreen(hWndParent, &lt);
	}
	HWND	hWndGrid = CreateGrid(hWndParent, CGlobal::MyGetModuleHandle(), WS_OVERLAPPEDWINDOW, lt.x, lt.y, CW_USEDEFAULT, 500);
	if(!IsWindow(hWndGrid))return;
	
	SetWindowText(hWndGrid, GetDataTitle(GetDataID(nWhich)).c_str());
	
	CGrid *pGrid = GetGrid(hWndGrid);
	
	int datasetcount = GetPlotDataCount();
	int npoints;
	
	int j;
	CellID cid;
	
	npoints = (int)GetAbsData(nWhich).size();
	pGrid->SetRows(npoints+1);
	pGrid->SetCols(3);
	//	pGrid->SetFixRowCount(2);
	
	for(j=0; j<npoints; j++)
	{
		cid.x = 1;
		cid.y = j+1;
		pGrid->SetCellDouble(cid, GetAbsData(nWhich)[j].val[0]);
		cid.x = 2;
		cid.y = j+1;
		pGrid->SetCellDouble(cid, GetAbsData(nWhich)[j].val[1]);
	}
	
	SendMessage(hWndGrid, WM_SETICON, ICON_BIG, (WPARAM)LoadIcon(CGlobal::MyGetModuleHandle(), LPCTSTR(IDI_ICON_GRID_LIB)));
	ShowWindow(hWndGrid, SW_SHOW);
}

Declare_Namespace_End
