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

//#include "stdafx.h"

#include "ContourLInePlotImpl.h"
#include "../../Basic/PlotData.h"
#include "ListContour.h"
#include <sstream>

using namespace NsCChart;
namespace NsCChart{
extern double TestFunction(double x, double y);
}
CContourLinePlotImpl::CContourLinePlotImpl()
{
	SetMajorHGrids(false);
	SetMajorVGrids(false);

	SetXAutoRange(false);
	SetYAutoRange(false);
	
	//SetLegendShow(false);
	
	SetNContour(21);
	SetDataInfoShow(false);
}

CContourLinePlotImpl::~CContourLinePlotImpl()
{
	
}

void	CContourLinePlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	int nOldMapmode = SetMapMode(hDC, MM_TEXT);

	if( (m_pListContour->GetNContour()!=m_pListContour->GetNPlanes() 
		|| (m_pListContour->GetNContour()==m_pListContour->GetNPlanes() 
			&& GetFieldFcn()!=TestFunction))
		&& m_pListContour->GetNContour()>=2)
	{
		m_pListContour->SetPlanes(m_pListContour->GetNContour());
	}
	if(!IsContourCreated() || m_bContourChanged)
	{
		CreateContour();
		m_bContourChanged = false;

		if(GetFieldFcn()==ContourHeight)
		{
			vector<double> vX, vY;
			for(int i=0; i<(int)m_vContourPoints.size(); i++)
			{
				vX.push_back(m_vContourPoints[i].c[0]);
				vY.push_back(m_vContourPoints[i].c[1]);
			}
			int dataID = AddCurve(vX, vY);
			SetPlotType(dataID, kXYPlotScatter);
			SetMarkerShow(dataID, true);
			SetDataTitle(dataID, _TEXT("data points"));
		}
		SetExactRange(true);
	}
	CXYPlotBasicImpl::FinalDraw(hDC, destRect);

	SetMapMode(hDC, nOldMapmode);
}

bool CContourLinePlotImpl::IsDataInfoShow()
{
	return m_bDataInfoShow;
}

void CContourLinePlotImpl::SetDataInfoShow(bool bDraw)
{
	m_bDataInfoShow = bDraw;
	for(int i=0;i<GetPlotDataCount();i++)
	{
		GetPlotData(GetDataID(i))->SetInfoShow(bDraw);
	}
}

int CContourLinePlotImpl::GetPlotDataLevels()
{
	return m_pListContour->GetNPlanes();
}

bool CContourLinePlotImpl::CreateContour()
{
	m_pListContour->Generate();

	// draw line strips
	CLineStripList* pStripList;
	CLineStrip* pStrip;
	UINT i,index;
	CLineStripList::iterator pos;
	CLineStrip::iterator pos2;
	//double pLimits[4];
	double x,y;
	MyVData2D vData;
	DataPoint2D datapoint;
	int nPt,cPt;

	tostringstream ostr;

	int dataID;
	tstring info,name;
	int count=0;

	DeleteAllData();

	//m_pListContour->GetLimits(pLimits);
	AddBLAxis();
	for (i=0;i<m_pListContour->GetNPlanes();i++)
	{
		pStripList=m_pListContour->GetLines(i);
		//ASSERT(pStripList);
		info = _TEXT("");
		
		for (pos=pStripList->begin(); pos != pStripList->end() ; pos++)
		{
			pStrip=(*pos);
			//ASSERT(pStrip);
			if (pStrip->empty())
				continue;
			nPt=(int)pStrip->size();
			vData.clear();

			cPt=0;
			for (pos2=pStrip->begin(); pos2 != pStrip->end() ; pos2++)
			{
				// retreiving index
				index=(*pos2);
				// drawing
				x=m_pListContour->GetXi(index);
				y=m_pListContour->GetYi(index);
				datapoint.val[0]=x;
				datapoint.val[1]=y;
				cPt++;
				vData.push_back(datapoint);
			}
			dataID=AddCurve(vData);
			if(info == _TEXT(""))
			{
				ostr.precision(3);
				ostr.str(_TEXT(""));
				//ostr<<m_pListContour->GetValue(x,y);
				ostr<<m_pListContour->GetPlane(i);
				info = ostr.str();
				//info.Format("%g",m_pListContour->GetValue(x,y));
			}
			GetPlotData(dataID)->SetInfo(info);
			GetPlotData(dataID)->SetDataLevel(i);
			SetMarkerType(dataID, kXYMarkerNone);
			SetAutoColor(dataID,true);
			count++;
			
			ostr.str(_TEXT(""));
			ostr.fill('0');
			ostr.width(3);
			ostr<<count;
			name = _TEXT("Contour line ") + ostr.str();
			SetDataTitle(dataID,name);
			SetPlotType(dataID,kXYPlotConnect);
			GetPlotData(dataID)->SetInfoShow(m_bDataInfoShow);
		}
	}
	if(m_bColorContour)
	{
		SetAutoDataColorGeneral(true);
		SetModeOfSetColors(kSetColorFromTwoColor);
		SetLineColor1(m_crColor1);
		SetLineColor2(m_crColor2);
	}
	else
	{
		SetAutoDataColorGeneral(false);
	}

	double limits[4];
	m_pListContour->GetLimits(limits);
	SetXRange(limits[0],limits[1]);
	SetYRange(limits[2],limits[3]);
	m_bNewDataComming = false;

	//m_bContourChanged = false;

	return true;
}

bool CContourLinePlotImpl::CreateContour(int nPlanes)
{
	return CreateContour(nPlanes,false);
}

bool CContourLinePlotImpl::CreateContour(int nPlanes,bool bExponential)
{
	m_pListContour->SetPlanes(nPlanes,bExponential);
	return CreateContour();
}

bool	CContourLinePlotImpl::IsContourCreated()
{
	return m_pListContour->GetLineCounts()>0;
}

CContourLinePlotImplI::CContourLinePlotImplI()
{

}

CContourLinePlotImplI::~CContourLinePlotImplI()
{

}

/*
bool	CContourLinePlotImplI::ShowDlgBySelection(HDC hDC)
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
	else if(IsPlotDataSelected())
	{
		int which;
		which = GetIndexOfSelected();
		if(DefDataSettings(this,hDC,which))
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