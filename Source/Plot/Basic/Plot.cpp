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

#include "Global.h"
#include "Plot.h"
#include "ColorTable.h"

using namespace NsCChart;

int			CPlot::m_nColorsInTable = sizeof(crTableDef)/sizeof(crTableDef[0]);
COLORREF	*CPlot::m_pColorTable = (COLORREF *)crTableDef;

void	CPlot::SetColorTable(int nTableIndex)
{
	if(nTableIndex>=0 && nTableIndex<sizeof(ColorTables)/sizeof(ColorTables[0]))
	{
		m_nColorsInTable	=	ColorTables[nTableIndex].nColors;
		m_pColorTable		=	ColorTables[nTableIndex].pColorTable;
	}
	else
	{
		m_nColorsInTable	=	ColorTables[0].nColors;
		m_pColorTable		=	ColorTables[0].pColorTable;
	}
}

/////////////////////////////////////////////////////////////////////////
// Class CPlot

CPlot::CPlot()
{
//	CGlobal::InitGdiPlus();
	SetDefaults();

	m_fcnDefScreen = 0;
	m_pParaDefScreen = 0;
	
	m_fcnUserDrawing = 0;
	m_pParaUserDrawing = 0;
	
	m_fcnUserUpdate = 0;
	m_pParaUserUpdate = 0;

	InitializeCriticalSection(&m_csPlot);
}

CPlot::~CPlot()
{
	CGlobal::FreeGdiPlus();	
	DeleteCriticalSection(&m_csPlot);
}

void	CPlot::SetDefaults()
{
	m_nMarginLeft	=	4;
	m_nMarginRight	=	4;
	m_nMarginBottom	=	4;
	m_nMarginTop	=	4;

	m_nMinMargin = 10;

	m_crSelectColor =	RGB(192,64,128);
	m_crLightColor	=	RGB(128,64,192);
	m_nSelectLineSize=	2;
	m_nSelectLineStyle=	PS_SOLID;
	m_nLightLineSize=	2;
	m_nLightLineStyle=	PS_SOLID;

	m_nApperanceMode = kNormal;

	m_nPointDesType = kDesXYNearest;
	m_nPointDesAccuracy = 4;

	m_bPlotSelected = false;
	m_bPlotLighted = false;
	m_bNewDataComming = false;
	m_bFirstDraw = true;
	m_bNothingElse = false;
	
	m_bEqualXYAxis	= false;
	m_bEqualXYScale = false;

	m_bXOpposite	= false;
	m_bYOpposite	= false;

	m_bSwitchReverse	= false;
	m_bDrawBasicAnyway	= false;

	m_bDoubleBuffer = true;

	switch(CGlobal::m_nLangurage)
	{
	case kLangChinese:
		m_strDefaultMain = _TEXT("CChart数据可视化库");
		m_strDefaultSub = _TEXT("版权所有。\n保留所有权利。\nwww.cchart.net");
		break;
	case kLangEnglish:
	default:
		m_strDefaultMain = _TEXT("CChart Library");
		m_strDefaultSub = _TEXT("Designed by Yang Guojun\nAll Right Reserved.\nwww.cchart.net");
		break;
	}

}

int		CPlot::ChooseLineWidth(int linewidth, bool bLighted, bool bSelected)
{
	if(linewidth <= 0)
		linewidth =1;
	if(bSelected)
		return linewidth + 2;
	else if(bLighted)
		return linewidth + 1;
	else
		return linewidth;
}

COLORREF	CPlot::ChooseLineColor(COLORREF linecolor, bool bLighted, bool bSelected)
{
	if(bSelected)
		return RGB((GetRValue(linecolor)+255)/2,GetGValue(linecolor),(GetBValue(linecolor)+255)/2);
	else if(bLighted)
		return RGB((GetRValue(linecolor)+255)/2,(GetGValue(linecolor)+255)/2,GetBValue(linecolor));
	else
		return linecolor;
}

int		CPlot::GetApperanceMode()
{
	return m_nApperanceMode;
}

void	CPlot::SetApperanceMode(int mode)
{
	m_nApperanceMode = mode;
}


