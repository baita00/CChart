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
#include "../../Basic/DataSet.h"
#include "../../Basic/XYPlot.h"

Declare_Namespace_CChart

template<class PlotImplT>
class CDataSet3DImpl : public CDataSet<DataPoint3D>
{
public:
	CDataSet3DImpl();
	virtual ~CDataSet3DImpl();

public:
	PlotImplT			*GetPlot();

public:
	void				SetPlotColors();
	void				SetPlotLineStyles();
	void				SetPlotLineSizes();

public:
	int					AddEmpty();
	int					AddCurve( MyVData3D vData );
	template<typename T1, typename T2, typename T3>
	int					AddCurve( T1 *pX, T2 *pY, T3 *pZ, int nLen )
	{
		if(!pX || !pY || !pZ || nLen<=0)return -1;
		MyVData3D vData;
		if(m_nReservedDataLength>0)
		{
			vData.reserve(m_nReservedDataLength);
		}
		
		DataPoint3D dp;
		for(int i=0;i<nLen;i++)
		{
			dp.val[0]=pX[i];
			dp.val[1]=pY[i];
			dp.val[2]=pZ[i];
			vData.push_back(dp);
		}
		return AddCurve(vData);
	}

	int					AddPointFinal(DataPoint3D dp3d, int nWhich);
	int					AddPoint(double x, double y, double z, int nWhich);
	int					AddPoint( double x, double y, double z) ;

	template<typename T1, typename T2>
	int					InsertPoint( T1 x, T2 y, int nPos, int nWhich )
	{
		double X = (double)x;
		double Y = (double)y;
		return InsertPoint(X, Y, nPos, nWhich);
	}
protected:
	virtual	int			AddData( MyVData3D vData );
	virtual	int			SetData( int dataID, MyVData3D vData );
};

template<class PlotImplT>
CDataSet3DImpl<PlotImplT>::CDataSet3DImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CDataSet3DImpl<PlotImplT>::~CDataSet3DImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
PlotImplT	*CDataSet3DImpl<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<class PlotImplT>
void	CDataSet3DImpl<PlotImplT>::SetPlotColors()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int nNum=GetPlotDataCount();
	//short r,g,b;
	for( int i = 0; i < nNum; i++ )
	{
		//Set color and line size
		if(pT->IsAutoDataColorGeneral() && GetAbsPlotData(i)->IsAutoColor())
		{
			switch(pT->GetModeOfSetColors())
			{
			case kSetColorFromTwoColor:
				pT->GetAbsPlotData(i)->GetGlPlotColor()[0] = ( pT->m_fvLineColor1[0]*(nNum-i) + pT->m_fvLineColor2[0] )/nNum;
				pT->GetAbsPlotData(i)->GetGlPlotColor()[1] = ( pT->m_fvLineColor1[1]*(nNum-i) + pT->m_fvLineColor2[1] )/nNum;
				pT->GetAbsPlotData(i)->GetGlPlotColor()[2] = ( pT->m_fvLineColor1[2]*(nNum-i) + pT->m_fvLineColor2[2] )/nNum;
				pT->GetAbsPlotData(i)->GetGlPlotColor()[3] = ( pT->m_fvLineColor1[3]*(nNum-i) + pT->m_fvLineColor2[3] )/nNum;
				break;
			default:
				//GetAbsPlotData(i)->SetPlotColor(CPlot::m_pColorTable[(i+pT->GetAutoColorStartIndex())%CPlot::m_nColorsInTable]);
				pT->GetAbsPlotData(i)->GetGlPlotColor()[0] = GetRValue(CPlot::m_pColorTable[(i+pT->GetAutoColorStartIndex())%CPlot::m_nColorsInTable])/255.0f;
				pT->GetAbsPlotData(i)->GetGlPlotColor()[1] = GetGValue(CPlot::m_pColorTable[(i+pT->GetAutoColorStartIndex())%CPlot::m_nColorsInTable])/255.0f;
				pT->GetAbsPlotData(i)->GetGlPlotColor()[2] = GetBValue(CPlot::m_pColorTable[(i+pT->GetAutoColorStartIndex())%CPlot::m_nColorsInTable])/255.0f;
				pT->GetAbsPlotData(i)->GetGlPlotColor()[3] = 0.0f;
				break;
			}
		}
		else
		{
			memcpy(pT->GetAbsPlotData(i)->GetGlPlotColor(),pT->GetAbsPlotData(i)->GetGlDataColor(), 4*sizeof(GLfloat) );
			//GetAbsPlotData(i)->SetPlotColor(GetAbsPlotData(i)->GetDataColor());
		}
		//GetAbsPlotData(i)->SetPlotColor(CPlot::ChooseLineColor(GetAbsPlotData(i)->GetPlotColor(), GetAbsPlotData(i)->IsLighted(), GetAbsPlotData(i)->IsSelected()));
	}
}
/*
void	CTDLinePlot::SetPlotColors()
{
	int nNum=(int)m_vpXYZDataInfo.size();
	for( int i = 0; i < nNum; i++ )
	{
		//Set color and line size
		if(m_bAutoDataColor && m_vpXYZDataInfo[i]->bAutoColor)
		{
			switch(m_nModeOfSetColors)
			{
			case kSetColorFromTwoColor:
				m_vpXYZDataInfo[i]->fvPlotColor[0] = ( m_fvLineColor1[0]*(nNum-i) + m_fvLineColor2[0] )/nNum;
				m_vpXYZDataInfo[i]->fvPlotColor[1] = ( m_fvLineColor1[1]*(nNum-i) + m_fvLineColor2[1] )/nNum;
				m_vpXYZDataInfo[i]->fvPlotColor[2] = ( m_fvLineColor1[2]*(nNum-i) + m_fvLineColor2[2] )/nNum;
				m_vpXYZDataInfo[i]->fvPlotColor[3] = ( m_fvLineColor1[3]*(nNum-i) + m_fvLineColor2[3] )/nNum;
				break;
			default:
				m_vpXYZDataInfo[i]->fvPlotColor[0] = GetRValue( CPlot::m_pColorTable[i%CPlot::m_nColorsInTable] )/255.0f;
				m_vpXYZDataInfo[i]->fvPlotColor[1] = GetGValue( CPlot::m_pColorTable[i%CPlot::m_nColorsInTable] )/255.0f;
				m_vpXYZDataInfo[i]->fvPlotColor[2] = GetBValue( CPlot::m_pColorTable[i%CPlot::m_nColorsInTable] )/255.0f;
				m_vpXYZDataInfo[i]->fvPlotColor[3] = 0.0f;
				break;
			}
		}
		else
		{
			memcpy(m_vpXYZDataInfo[i]->fvPlotColor, m_vpXYZDataInfo[i]->fvDataColor, 4*sizeof(GLfloat) );;
		}
	}
}*/

template<class PlotImplT>
void	CDataSet3DImpl<PlotImplT>::SetPlotLineStyles()
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
void	CDataSet3DImpl<PlotImplT>::SetPlotLineSizes()
{
	for( int i = 0; i < GetPlotDataCount(); i++ )
	{
		GetAbsPlotData(i)->SetPlotLineSize(CPlot::ChooseLineWidth(GetAbsPlotData(i)->GetDataLineSize(), GetAbsPlotData(i)->IsLighted(), GetAbsPlotData(i)->IsSelected()));
	}
}

template<class PlotImplT>
int	CDataSet3DImpl<PlotImplT>::AddData( MyVData3D vData )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	CPlotDataImpl<DataPoint3D> *plotData = new CPlotDataImpl<DataPoint3D>(pT);
	if(pT->GetReservedDataLength()>0)
	{
		plotData->GetData().reserve(pT->GetReservedDataLength());
	}
	int returnVal = plotData->SetData( vData );
	if( returnVal == false ) return -1;
	
	MyLock();
		
	// Now add this to our typed pointer array
	plotData->SetDataID(m_nCurDataID);
	((CPlotData<DataPoint3D>*)plotData)->SetDefaults(  );

	plotData->SetDataSet(this);
	
	m_vpPlotData.push_back(plotData);
	//GetDataRanges();
	plotData->GetDataRange();
		
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
int	CDataSet3DImpl<PlotImplT>::SetData( int dataID, MyVData3D vData )
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
int		CDataSet3DImpl<PlotImplT>::AddEmpty()
{
	MyVData3D vd;
	vd.clear();
	return AddData(vd);
}

template<class PlotImplT>
int		CDataSet3DImpl<PlotImplT>::AddCurve(MyVData3D vData)
{
	return AddData(vData);
}

template<class PlotImplT>
int		CDataSet3DImpl<PlotImplT>::AddPointFinal(DataPoint3D dp3d, int nWhich)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(nWhich < 0 || nWhich > GetPlotDataCount())return -1;
	
	if( nWhich == GetPlotDataCount() )
	{
		double pX[1], pY[1], pZ[1];
		pX[0] = dp3d.val[0];
		pY[0] = dp3d.val[1];
		pZ[0] = dp3d.val[2];
		return AddCurve(pX, pY, pZ, 1);
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
	GetAbsPlotData(nWhich)->GetData().push_back(dp3d);
	UpdateDataRanges(dp3d, nWhich);
	//pT->SetNewDataComming( true );
	GetAbsPlotData(nWhich)->bForceUpdate = true;
	pT->SetRangeSet(false);
	
	MyUnlock();
	
	return GetDataID(nWhich);
}

template<class PlotImplT>
int		CDataSet3DImpl<PlotImplT>::AddPoint(double x, double y, double z, int nWhich)
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
	
	DataPoint3D dp3d;
	dp3d.val[0] = x;
	dp3d.val[1] = y;
	dp3d.val[2] = z;
	return AddPointFinal(dp3d, nWhich);
}

template<class PlotImplT>
int		CDataSet3DImpl<PlotImplT>::AddPoint(double x, double y, double z)
{
	if(GetPlotDataCount()<=0)
	{
		double pX[1], pY[1], pZ[1];
		pX[0] = x;
		pY[0] = y;
		pZ[0] = z;
		return AddCurve(pX, pY, pZ, 1);
	}
	else
	{
		return AddPoint(x, y, z, 0);
	}
}

Declare_Namespace_End
