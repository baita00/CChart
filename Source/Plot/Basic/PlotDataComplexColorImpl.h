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

#include "PlotDataComplexColor.h"

Declare_Namespace_CChart

template<class PlotDataT>
class	CPlotDataComplexColorImpl : public CPlotDataComplexColor
{
public:
	CPlotDataComplexColorImpl();
	virtual	~CPlotDataComplexColorImpl();

public:
	bool			CheckSegColors();
};

template<class PlotDataT>
CPlotDataComplexColorImpl<PlotDataT>::CPlotDataComplexColorImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
CPlotDataComplexColorImpl<PlotDataT>::~CPlotDataComplexColorImpl()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);
}

template<class PlotDataT>
bool	CPlotDataComplexColorImpl<PlotDataT>::CheckSegColors()
{
	PlotDataT* pT = static_cast<PlotDataT*>(this);

	double xmin, xmax;
	switch(m_nMultiColorInputType)
	{
	case kInputColorByPointRatio:
		xmin = 0.0;
		xmax = 1.0;
		break;
	case kInputColorByXVal:
		xmin = pT->GetXDataRange()[0];
		xmax = pT->GetXDataRange()[1];
		break;
	case kInputColorByYVal:
		xmin = pT->GetYDataRange()[0];
		xmax = pT->GetYDataRange()[1];
		break;
	}
	int j; 
	for(j=(int)m_vSegColors.size()-1; j>=0; j--)
	{
		if(m_vSegColors[j].val<xmin || m_vSegColors[j].val>xmax)
		{
			m_vSegColors.erase(m_vSegColors.begin()+j);
		}
	}
	if(m_vSegColors.size()<2)return false;
	
	sort(m_vSegColors.begin(), m_vSegColors.end(), segcolor_less);
	
	for(j=(int)m_vSegColors.size()-1; j>0; j--)
	{
		if(m_vSegColors[j].val == m_vSegColors[j-1].val)
		{
			m_vSegColors.erase(m_vSegColors.begin()+j); 
		}
	}
	if(m_vSegColors.size()<2)return false;
	
	return true;
}

Declare_Namespace_End