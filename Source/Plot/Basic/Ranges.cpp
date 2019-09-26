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

#include "Ranges.h"
#include "PlotUtility.h"
using namespace NsCChart;
/*
CRanges::CRanges()
{
	m_vpRanges.resize(2);
	CRangeImpl *pRI;
	pRI = new CRangeImpl;
	m_vpRanges[0] = pRI;
	pRI = new CRangeImpl;
	m_vpRanges[1] = pRI;
	SetDefaults();
}

CRanges::~CRanges()
{
	while(m_vpRanges.size()>0)
	{
		delete m_vpRanges[0];
		m_vpRanges.erase(m_vpRanges.begin());
	}
}

void	CRanges::SetDefaults()
{
//	m_pfXPlotRange[0] = 0.0;
//	m_pfXPlotRange[1] = 1.0;
//	m_pfYPlotRange[0] = 0.0;
//	m_pfYPlotRange[1] = 1.0;
//	m_pfLastXPlotRange[0] = 0.0;
//	m_pfLastXPlotRange[1] = 1.0;
//	m_pfLastYPlotRange[0] = 0.0;
//	m_pfLastYPlotRange[1] = 1.0;
	//m_bXRangeSet = false;
	//m_bYRangeSet = false;
	
	m_bEqualXYRange = false;
	
//	m_bXAutoRange = true;
//	m_bYAutoRange = true;

//	m_bExactXRange = false;
//	m_bExactYRange = false;
//	m_bOptimalExactXRange = true;
//	m_bOptimalExactYRange = true;
//	m_bOptimalXZeroRepair = true;
//	m_bOptimalYZeroRepair = true;
//	m_bOptimalXSymetryRepair = true;
//	m_bOptimalYSymetryRepair = true;
//	m_bOptimalXlExtend	= false;
//	m_bOptimalXuExtend	= false;
//	m_bOptimalYlExtend	= false;
//	m_bOptimalYuExtend	= false;

//	m_bAutoMajorTickX = true;
//	m_bAutoMajorTickY = true;
//	m_bAutoMinorTickX = true;
//	m_bAutoMinorTickY = true;

//	m_fAtomX = 1.0;
//	m_fAtomY = 1.0;
//	m_fAtomZeroX = 0.0;
//	m_fAtomZeroY = 0.0;
	
//	m_bXLogarithm = false;
//	m_bYLogarithm = false;

//	m_bFloatXTicks = false;
//	m_bFloatYTicks = false;

//	m_bFloatXTicksOld = false;
//	m_bFloatYTicksOld = false;
}

CRangeImpl*		CRanges::GetClsRange(int nIndex)
{
	if(nIndex<0 || nIndex>m_vpRanges.size())
		return 0;
	else
		return m_vpRanges[nIndex];
}

CRangeImpl*		CRanges::GetXClsRange()
{
	return GetClsRange(0);
}

CRangeImpl*		CRanges::GetYClsRange()
{
	return GetClsRange(1);
}
*/