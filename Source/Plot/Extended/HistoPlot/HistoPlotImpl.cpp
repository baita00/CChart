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

#include "HistoPlotImpl.h"

using namespace NsCChart;

CHistoPlotImpl::CHistoPlotImpl()
{

}

CHistoPlotImpl::~CHistoPlotImpl()
{

}

bool	CHistoPlotImpl::Statistic()
{
	if(m_vfHistoData.size()<=0)
		return false;

	int i, j;

	double lower, upper;
	if(!m_bAutoRange)
	{
		lower = m_fRange[0];
		upper = m_fRange[1];
	}
	else
	{
		lower = m_vfHistoData[0];
		upper = m_vfHistoData[0];
		for(i=1; i<(int)m_vfHistoData.size(); i++)
		{
			if(m_vfHistoData[i]<lower)
				lower = m_vfHistoData[i];
			if(m_vfHistoData[i]>upper)
				upper = m_vfHistoData[i];
		}
	}
	if(lower>=upper)
		return false;
	if(m_nDivides<=0)
		return false;

	double dx = (upper - lower)/m_nDivides;

	double *pX, *pY;
	pX = new double[m_nDivides+1];
	pY = new double[m_nDivides+1];

	for(i=0; i<m_nDivides+1; i++)
	{
		pX[i] = lower + i*dx;
		pY[i] = 0.0;
	}
	for(j=0; j<(int)m_vfHistoData.size(); j++)
	{
		for(i=0; i<m_nDivides+1; i++)
		{
			if(m_vfHistoData[j]>=lower+i*dx && m_vfHistoData[j]<lower+(i+1)*dx)
			{
				if(m_vfHistoData[j]==upper)
					pY[i-1] += 1.0;
				else
					pY[i] += 1.0;
				break;
			}
		}
	}

//	m_vpXYDataInfo.clear();
	int dataID = AddCurve(pX, pY, m_nDivides+1);
	SetPlotType(dataID, kXYPlotStepHV);
	//SetHisto(dataID, true);
	GetPlotData(dataID)->SetHisto(true);
	SetDataFill(dataID, true);
	SetDataFillMode(dataID, kDataFillFromBottomAxis);

	delete []pY;
	delete []pX;

	return true;
}