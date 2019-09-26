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

//#include "StdAfx.h"
#include "StemPlot.h"
#include "../../Basic/PlotUtility.h"

using namespace NsCChart;

CStemPlot::CStemPlot()
{
	SetDefaults();
}

CStemPlot::~CStemPlot()
{

}

void	CStemPlot::SetDefaults()
{
	m_nStemBase = kStemBaseBottom;

	m_nMaxDataSets = 10;
	m_nMaxDataSetLen = 100;
	m_fRatioOccupy = 0.6;
	m_bAddUp = false;

	m_b3DStem = false;
	m_f3DStemThick = 15;
	m_f3DStemAngle = 45;

	m_bForceYZeroStart = true;
	m_crStemEdgeColor = RGB(100, 100, 100);

	m_vstrLabels.resize(m_nMaxDataSetLen);

	m_bShowValOnTop = false;
	HFONT hfont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (hfont!=NULL)
	{
		::GetObject(hfont, sizeof(LOGFONT), &m_lfValOnTop);
		::DeleteObject(hfont);
	}
	m_bStemGradientEffect = true;
	m_nStemGradientType = kGradientVCt2Bd;
}

