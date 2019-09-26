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

#include "SevenSegmentCode.h"
#include "../../Basic/MyConst.h"

using namespace NsCChart;

CSevenSegmentCode::CSevenSegmentCode()
{
	SetDefaults();
}

CSevenSegmentCode::~CSevenSegmentCode()
{
}

void	CSevenSegmentCode::SetDefaults()
{
	m_nHBarLen = 15;
	m_nVBarLen = 30;
	m_nHBarThick = 6;
	m_nVBarThick = 6;

	m_bSlope = true;
	m_fAngleSlope = 5.0*myDeg;

	m_bSunkenVCenter = true;

	
	m_nInnerGap = 1;
	m_nPointCodeSize = 4;

	m_crBkColor = RGB(128, 128, 128);
	m_crCodeColor = RGB(255, 0, 0);
	m_crSegEdgeColor = RGB(224, 224, 224);
	m_crOffColor = RGB(245, 245, 245);
}