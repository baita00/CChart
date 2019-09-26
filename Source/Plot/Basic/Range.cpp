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

#include "Range.h"

using namespace NsCChart;

CRange::CRange()
{
	SetDefaults();
}

CRange::~CRange()
{

}

void	CRange::SetDefaults()
{
	m_pfPlotRange[0] = -1.0;
	m_pfPlotRange[1] = 1.0;
	m_pfLastPlotRange[0] = -2.0;
	m_pfLastPlotRange[1] = 2.0;
	m_bRangeSet = false;
		
	m_bAutoRange = true;
	m_bStaticRange = false;

	m_bExactRange = false;
	m_bOptimalExactRange = true;
	m_bOptimalZeroRepair = true;
	m_bOptimalSymetryRepair = true;
	m_bOptimalLExtend	= false;
	m_bOptimalUExtend	= false;

	m_bAutoMajorTick = true;
	m_bAutoMinorTick = true;

	m_nMajorTicks = 4;
	m_nMinorTicks = 5;

	m_fTickMin = -1;
	m_fTickMax = 1;

	m_fAtom = 1.0;
	m_fAtomZero = 0.0;
	
	m_bLogarithm = false;

	m_bFloatTicks = false;

	m_bFloatTicksOld = false;

	m_bGapDetermined = false;
	m_fTickGap = 1.0;
	m_bDefExpandWhileGapDet = false;

	m_nZoomCenter = kRangeZoomCenterM;

	m_tmtStartTime = 0;
}

