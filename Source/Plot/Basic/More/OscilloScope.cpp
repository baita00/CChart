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

#include "OscilloScope.h"
#include "../PlotUtility.h"
#include "../CChartNameSpace.h"

using namespace NsCChart;

COscilloScope::COscilloScope()
{
	SetDefaults();
}

COscilloScope::~COscilloScope()
{

}

void	COscilloScope::SetDefaults()
{
	m_nDraggingID = -1;
	m_nSteppingID = -1;
	m_nSteppingZoneEdgeIdx = -1;
	m_nSteppingPrecisionLow = 100;
	m_nSteppingPrecisionHigh = 500;
	m_bSteppingHighPrecision = false;

	m_bCursorByMouse = true;
	m_bShowHCursor = false;
	m_bShowVCursor = false;
	m_fCursorX = -fMaxVal;
	m_fCursorY = -fMaxVal;
	m_bHoldCursorToCurve = false;
	m_nIndexToHoldCursor = 0;
	m_nCursorSize	= 1;
	m_nCursorStyle	= PS_SOLID;
	m_crCursorColor = RGB(128,0,0);
	
	m_bEnableXZone = false;
	m_bEnableYZone = false;
	m_fXZoneLower = -fMaxVal;
	m_fXZoneUpper = fMaxVal;
	m_fYZoneLower = -fMaxVal;
	m_fYZoneUpper = fMaxVal;
	m_crZoneColor = RGB(224,224,255);
	m_nZoneAlpha	= 64;
	m_nZoneEdgeWidth = 2;
	
	//memset(m_bDragZoneEdge, 0, 4*sizeof(bool));
	m_nCurZoneEdge = -1;
	
	m_bStartOscilloScope = false;
	m_bOscilloScopeMode = false;
	m_bEnableZoneDrag = true;
	m_bEnableDataDrag = true;
	
	m_fTrigLevel = 0.0;
	m_bShowTrigArrow = false;
	m_bShowZeroArrow = false;
	m_crTrigArrowColor = RGB(32, 255, 32);
	
	m_bLayerOscilloScopeMode = true;
}