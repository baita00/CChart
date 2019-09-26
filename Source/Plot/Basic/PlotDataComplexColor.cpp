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

#include "PlotDataComplexColor.h"

Declare_Namespace_CChart

bool segcolor_less(const SegmentColor &s1, const SegmentColor &s2)
{
	return s1.val < s2.val;
}
bool segcolor_great(const SegmentColor &s1, const SegmentColor &s2)
{
	return s1.val > s2.val;
}

CPlotDataComplexColor::CPlotDataComplexColor()
{
	SetDefaults();
}

CPlotDataComplexColor::~CPlotDataComplexColor()
{

}

void	CPlotDataComplexColor::SetDefaults()
{
#ifdef DEFAULT_WHITE_BACKGROUND
	m_crDataColor2 = RGB(0,0,0);
#else
	m_crDataColor2 = RGB(255,255,255);
#endif
	m_bBiColorMode = false;
	m_bMultiColorMode = false;
	m_nMultiColorInputType = kInputColorByPointRatio;
	m_vSegColors.clear();
	m_bColorPtByPt = false;
}

Declare_Namespace_End