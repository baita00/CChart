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

#include "HistoPlot.h"

using namespace NsCChart;

CHistoPlot::CHistoPlot()
{
	SetDefaults();
}

CHistoPlot::~CHistoPlot()
{

}

void	CHistoPlot::SetDefaults( )
{
	m_vfHistoData.clear();

	m_nDivides = 16;
	m_fRange[0] = 0.0;
	m_fRange[1] = 0.0;
	m_bAutoRange = true;
}

void	CHistoPlot::AddHistoData(double *pVal, int nLen)
{
	if(!pVal || nLen<=0 )
		return;

	m_vfHistoData.clear();
	for(int i=0; i<nLen; i++)
	{
		m_vfHistoData.push_back(pVal[i]);
	}
	Statistic();
}