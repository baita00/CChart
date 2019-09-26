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

#pragma warning(disable:4786)
#include "Axis.h"
#include "Plot.h"
#include "Global.h"

using namespace NsCChart;

CAxis::CAxis()
{
	m_hPattern = 0;
	m_nApperanceMode = kNormal;
	SetDefaults();
}

CAxis::~CAxis()
{
	DeleteUsedHandles();
}

void	CAxis::SetDefaults()
{
	m_bVisible = true;
	m_bShowAxisLine = true;
	m_strTitle = _TEXT("");
	m_nLocation = kLocationBottom;

#ifdef DEFAULT_WHITE_BACKGROUND
	m_crAxisColor = RGB(0, 100, 0);
	m_crTitleColor = RGB(0, 0, 0);
#else
	m_crAxisColor = RGB(0, 255, 0);
	m_crTitleColor = RGB(255, 255, 255);
#endif
	m_bShowTitleRect = false;
	m_nTitleRectLineSize = 1;
	m_nTitleRectLineStyle = PS_SOLID;

	m_nTickLength = 10;
	m_nMinorTickLength = 5;
	m_nTickPosition = kTickPositionInner;
	m_nMinorTickPosition = kTickPositionInner;
	m_nTickSize = 3;
	m_nMinorTickSize = 1;
	m_crTickColor = m_crAxisColor;
	m_crMinorTickColor = m_crAxisColor;

	m_bTickLabelSEOnly = false;

	m_bShowMajorTicks = true;
	m_bShowMinorTicks = true;

	m_nBrushType = kBrushNull;
	m_crBgColor = RGB(128, 128, 128);
	m_nHatchStyle = HS_CROSS;
	m_crHatchColor = RGB(128, 128, 128);
	if(m_hPattern)
	{
		DeleteObject(m_hPattern);
		m_hPattern = 0;
	}
	m_bBorderShow = false;
	m_nBorderSize = 1;

	m_bArrowStart = false;
	m_bArrowEnd = false;
	m_nArrowSize = 6;
	m_nArrowRatio = 4;
	m_bArrowFlatBottom = false;
	m_bArrowReverse = false;

	m_bShowTitle = true;
	m_crTickLabelColor = m_crAxisColor;
	m_bTickLabelShow = true;
	m_nTitleGap = 0;

	m_nLineSize = 2;
	m_nLineStyle = PS_SOLID;

	m_nMarginHorizontal = 0;
	m_nMarginVertical = 0;

	m_nAxisGap = 3;
	m_nLabelGap = 3;
	m_bDespiteLow = false;
	m_bDespiteHigh = false;

	m_bLighted = false;
	m_bSelected = false;

	m_nTitlePosition = 0;
	SIZE offset = {0, 0};
	m_sizeTitleOffset = offset;
	m_sizeLabelOffset = offset;
	m_strLabelSuffix = _TEXT("");
	m_strLabelPrefix = _TEXT("");
	m_strLabelFormat = _TEXT("%g");

	m_bSameColor = true;

	m_bReplaceLabel = false;
	m_vstrAlternateLabels.clear();

	m_bMapAlternateMode = false;
	m_fMapAlternateStartVal = 0.0;

	HFONT hFont;

	hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
				DEFAULT_PITCH | FF_SWISS, _TEXT("Arial"));
	GetObject(hFont, sizeof(LOGFONT), &m_lfTitleFont);
	DeleteObject(hFont);
	hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
				DEFAULT_PITCH | FF_SWISS, _TEXT("Arial"));
	GetObject(hFont, sizeof(LOGFONT), &m_lfLabelFont);
	DeleteObject(hFont);
	
	m_bTime = false;
	m_strTimeFormat = _TEXT("%Y/%m/%d\n%H:%M:%S");

	m_bColorBar = false;
	m_nColorBarWidth = 20;
	m_crBarColor1 = RGB(255, 0, 0);
	m_crBarColor2 = RGB(0, 0, 255);
	m_nColorBarMarginOuter = 4;
	m_nColorBarMarginInner = 0;

	//m_bIndependent = false;

	m_bMinusTicks = false;
	m_bTwoStoreyX = false;
	m_bSciNotation = false;
	m_fSnFontRatio = 0.75;

	m_bYTitleVerticle = true;

	m_vRanges4LabelHiden.clear();

	SetApperanceMode(m_nApperanceMode);
}

int		CAxis::GetApperanceMode()
{
	return m_nApperanceMode;
}

void	CAxis::SetApperanceMode(int mode)
{
	m_nApperanceMode = mode;

	switch(mode)
	{
	case kExtraSmall:
		m_lfTitleFont.lfHeight = 10;
		m_lfLabelFont.lfHeight = 8;

		m_lfTitleFont.lfWeight = FW_THIN;
		m_lfLabelFont.lfWeight = FW_THIN;

		m_nTickLength = 4;
		m_nMinorTickLength = 2;
		break;
	case kSmall:
		m_lfTitleFont.lfHeight = 14;
		m_lfLabelFont.lfHeight = 12;

		m_lfTitleFont.lfWeight = FW_LIGHT;
		m_lfLabelFont.lfWeight = FW_LIGHT;

		m_nTickLength = 6;
		m_nMinorTickLength = 3;
		break;
	case kNormal:
		m_lfTitleFont.lfHeight = 18;
		m_lfLabelFont.lfHeight = 15;

		m_lfTitleFont.lfWeight = FW_NORMAL;
		m_lfLabelFont.lfWeight = FW_NORMAL;

		m_nTickLength = 8;
		m_nMinorTickLength = 4;
		break;
	case kLarge:
		m_lfTitleFont.lfHeight = 30;
		m_lfLabelFont.lfHeight = 24;

		m_lfTitleFont.lfWeight = FW_BOLD;
		m_lfLabelFont.lfWeight = FW_BOLD;

		m_nTickLength = 10;
		m_nMinorTickLength = 5;
		break;
	case kExtraLarge:
		m_lfTitleFont.lfHeight = 36;
		m_lfLabelFont.lfHeight = 28;

		m_lfTitleFont.lfWeight = FW_ULTRABOLD;
		m_lfLabelFont.lfWeight = FW_ULTRABOLD;

		m_nTickLength = 14;
		m_nMinorTickLength = 7;
		break;
	default:
		break;
	}
}

void	CAxis::DeleteUsedHandles()
{
	if(m_hPattern)
		DeleteObject(m_hPattern);
	m_hPattern = 0;
}

bool	CAxis::IsXAxis()
{
	if(m_nLocation == kLocationBottom
		|| m_nLocation == kLocationTop
		|| m_nLocation == kLocationCenterHB
		|| m_nLocation == kLocationCenterHT)
		return true;
	else return false;
}

tstring		CAxis::GetDescription()
{
	switch(GetLocation())
	{
	case kLocationLeft:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Left axis");
		case kLangChinese:
			return _TEXT("左边坐标轴");
		}
		break;
	case kLocationRight:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Right axis");
		case kLangChinese:
			return _TEXT("右边坐标轴");
		}
		break;
	case kLocationBottom:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Bottom axis");
		case kLangChinese:
			return _TEXT("下边坐标轴");
		}
		break;
	case kLocationTop:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Top axis");
		case kLangChinese:
			return _TEXT("上边坐标轴");
		}
		break;
	case kLocationCenterVL:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Center vertical axis, left side");
		case kLangChinese:
			return _TEXT("中间坐标轴，竖直偏左");
		}
		break;
	case kLocationCenterVR:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Center vertical axis, right side");
		case kLangChinese:
			return _TEXT("中间坐标轴，竖直偏右");
		}
		break;
	case kLocationCenterHB:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Center horizontal axis, bottom side");
		case kLangChinese:
			return _TEXT("中间坐标轴，水平偏下");
		}
		break;
	case kLocationCenterHT:
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Center horizontal axis, top side");
		case kLangChinese:
			return _TEXT("中间坐标轴，水平偏上");
		}
		break;
	}
	return _TEXT("");
}

bool	CAxis::IsXAxis(int location)
{
	if(location == kLocationBottom ||
		location == kLocationTop ||
		location == kLocationCenterHB ||
		location == kLocationCenterHT)
	{
		return true;
	}
	else
		return false;
}

void	CAxis::SetAlternateMap(int nVal, tstring strVal)
{
	if(nVal<0)return;
	map<int, tstring>::iterator iter;
	iter = m_mapAlternateLabels.find(nVal);
	if(iter != m_mapAlternateLabels.end() )
	{
		iter->second = strVal;
	}
	else
	{
		m_mapAlternateLabels.insert(pair<int, tstring>(nVal, strVal));
	}
}

tstring	CAxis::GetAlternateMap(int nVal)
{
	if(nVal<0)return _TEXT("");
	map<int, tstring>::iterator iter;
	iter = m_mapAlternateLabels.find(nVal);
	if(iter != m_mapAlternateLabels.end() )
	{
		return iter->second;
	}
	else
	{
		return _TEXT("");
	}
}
