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

//#include "../Basics/MyGdiPlus.h"
#include "../../Basic/Global.h"
#include "InstruBase.h"

using namespace NsCChart;

CInstruBase::CInstruBase()
{
	m_hPattern = 0;
	SetDefaults();

	//GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL); 
	CGlobal::InitGdiPlus();
}

CInstruBase::~CInstruBase()
{
	DeleteUsedHandles();

	//GdiplusShutdown(m_pGdiToken); 
	CGlobal::FreeGdiPlus();
}

void	CInstruBase::SetDefaults()
{
	m_fCurrValue = 0.0;
	m_crCurrColor = RGB(192, 64, 64);
	m_bFillCurr = true;

	m_nAxisPosition = kInstruPositionWideLarge;
	m_bOpposite = false;

	m_nInstruShape = kInstruShapeHorizontal;
	m_nTickCount = 4;
	m_nMinorTickCount = 5;
	m_pfRange[0] = 0.0;
	m_pfRange[1] = 100.0;
	m_strTitle = _TEXT("");
	m_sizeTitleOffset.cx = m_sizeTitleOffset.cy = 0;
	
#ifdef DEFAULT_WHITE_BACKGROUND
	m_crAxisColor = RGB(32, 32, 32);
	m_crTitleColor = RGB(128, 64, 64);
	m_crBkColor = RGB(224, 224, 224);
#else
	m_crAxisColor = RGB(224, 224, 224);
	m_crTitleColor = RGB(128, 192, 192);
	m_crBkColor = RGB(32, 32, 32);
#endif
	
	m_bTickBar = false;
	m_bMinorTickBar = false;
	m_nTickBarHalfWidthBase = 6;
	m_nTickBarHalfWidthEnd = 4;
	m_nMinorTickBarHalfWidthBase = 5;
	m_nMinorTickBarHalfWidthEnd = 3;
	m_nTickLength = 10;
	m_nSubTickLength = 8;
	m_nMinorTickLength = 5;
	m_nTickPosition = kInstruPosInner;
	m_nMinorTickPosition = kInstruPosInner;
	m_nTickSize = 1;
	m_nSubTickSize = 1;
	m_nMinorTickSize = 1;
	m_crTickColor = m_crAxisColor;
	m_crSubTickColor = m_crAxisColor;
	m_crMinorTickColor = m_crAxisColor;
	//m_bRangeSet = false;
	
	m_nBrushType = kBrushNull;
	
	m_nHatchStyle = HS_CROSS;
	m_crHatchColor = RGB(128, 128, 128);
	if(m_hPattern)
	{
		DeleteObject(m_hPattern);
		m_hPattern = 0;
	}
	m_bShowBorder = true;
	m_nBorderSize = 1;
	m_nBorderStyle = PS_SOLID;
	m_crBorderColor = RGB(128, 128, 128);
	m_bBorder3D = false;
	m_nBorder3DEdge = EDGE_SUNKEN;
	m_bEraseBkgnd = true;
	
	m_bShowTitle = true;
	m_bTitleAsValue = false;
	m_crTickLabelColor = RGB(64, 64, 64);
	m_bShowTickLabel = true;

	m_nMarginHorizontal = 5;
	m_nMarginVertical = 5;
	
	m_nLineSize = 3;
	m_nLineStyle = PS_SOLID;
	
	m_nTitlePosition = kInstruPositionWideSmall;
	m_strUnit = _TEXT("s");
	m_bShowUnit = false;
	
	m_nBarWidth = 20;
	m_nBarMargin = 4;
	m_nBarBorderSize = 1;
	m_crBarBorderColor = RGB(128, 0, 0);
	m_bBarBorder3D = true;
	m_bShowBar = true;
	m_bFillBar = true;
	m_crBarColor = RGB(64, 64, 96);

	m_bShowAxis = true;
	m_bShowAxisLine = true;
	m_bShowMajorTicks = true;
	m_bShowSubMajorTicks = false;
	m_bShowMinorTicks = true;

	m_bHideIdleArc = false;
	
	m_nEdgeGap	= 5;
	m_fAngleStart = 0.0;
	m_fAngleRange	= 2.0*myPi;

	m_bClockWise = true;
	m_nLabelEdgeType = kShowLabelEdgeStart;
	m_nLabelPosition = kInstruPosOuter;
	m_bFillWholeCircle = true;
	m_crWholeCircleColor = RGB(192, 128, 128);

	m_bReplaceLabel = false;
	m_vstrAlternateLabels.clear();

	HFONT hFont;
	
	hFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN, _TEXT("Arial"));
	GetObject(hFont, sizeof(LOGFONT), &m_lfTitleFont);
	DeleteObject(hFont);
	hFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN, _TEXT("Arial"));
	GetObject(hFont, sizeof(LOGFONT), &m_lfLabelFont);
	DeleteObject(hFont);

	m_bShowStaticPointer = false;
	m_nStaticPointerIndex = 2;
	m_nStaticPointerWithdraw = 20;
	m_nStaticPointerBaseHalfWidth = 15;
	m_crStaticPointerColor1 = RGB(192, 164, 128);
	m_crStaticPointerColor2 = RGB(128, 192, 128);
	m_nStaticPointerAdditionalWithdraw = 10;

	m_bShowPointer = true;
	m_nPointerShape = kPointerSingleBar;
	m_nPointerLineSize = 3;
	m_nPointerWithdraw = 5;
	m_crPointerColor1 = RGB(0, 0, 96);
	m_crPointerColor2 = RGB(164, 0, 0);
	m_bShowStick = true;
	m_crStickColor = RGB(150, 150, 150);
	m_nStickRadius = 4;
	m_nPointerHalfWidthBase = 6;
	m_nPointerHalfWidthEnd = 4;
	m_nPointerSwordLenEnd = 15;
	
	m_bPointerShowSwordHandle = true;
	m_nPointerSwordHandleRadiusInc = 3;
	
	m_nPointerArrowDisplaceBase = 30;
	m_nPointerArrowDistFromCenter = 10;
	m_bFillPointerCircle = false;
	m_crFillPointerCircleColor = RGB(228, 255, 255);
	m_bPointerCircle3D = true;

	m_nPointerHalfWidthEndTail = 2;
	m_nPointerLenTail = 30;
	m_bShowPointerTail = true;
	m_bAxisColorful = false;

	m_bWheelHorizontal = true;
	m_nWheelThick = 40;
	m_nWheelEdges = 180;
	m_fWheelEdgeRatio = 0.95;
	m_fWheelWindowAngle = 3.0*myPi/4.0;
	m_fCurrSizeAngle = 2.0*myPi/60.0;

	m_bLogarithm = false;

	InitAxisColors();
}

void	CInstruBase::DeleteUsedHandles()
{
	if(m_hPattern)
	{
		DeleteObject(m_hPattern);
		m_hPattern = 0;
	}
}

tstring		CInstruBase::GetRealTitle()
{
	if(!m_bTitleAsValue)return m_strTitle;

	tstring realtitle;

	tostringstream ostr;
	ostr<<GetCurrValue();
	realtitle = ostr.str();
	if(m_bShowUnit)
		realtitle += (_TEXT(" ") + m_strUnit);
	return realtitle;
}

void		CInstruBase::InitAxisColors()
{
	m_vAxisSegColors.clear();
	
	AxisSegColors acs;
	
	acs.nTicks = 2;
	acs.color = RGB(224, 128, 128);
	m_vAxisSegColors.push_back(acs);

	acs.color = RGB(128, 224, 128);
	m_vAxisSegColors.push_back(acs);

	acs.color = RGB(128, 128, 224);
	m_vAxisSegColors.push_back(acs);

	acs.color = RGB(192, 224, 96);
	m_vAxisSegColors.push_back(acs);
	
	acs.color = RGB(192, 96, 96);
	m_vAxisSegColors.push_back(acs);

	acs.color = RGB(255, 224, 64);
	m_vAxisSegColors.push_back(acs);
}

COLORREF	CInstruBase::GetAxisSegColor(int nIndex)
{
	int i, nmax, now;
	nmax = 0;
	for(i=0; i<(int)m_vAxisSegColors.size(); i++)
	{
		nmax += m_vAxisSegColors[i].nTicks;
	}
	nIndex = int(nIndex - floor(double(nIndex/nmax))*nmax);
	now = 0;
	COLORREF color;
	for(i=0; i<(int)m_vAxisSegColors.size(); i++)
	{
		now += m_vAxisSegColors[i].nTicks;
		if(nIndex<now)
		{
			color = m_vAxisSegColors[i].color;
			break;
		}
	}
	return color;
}