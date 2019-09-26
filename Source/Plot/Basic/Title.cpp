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

#include "Title.h"
#include "Plot.h"

using namespace NsCChart;

CTitle::CTitle()
{
	m_hTitlePattern	= NULL;
	SetDefaults();
}

CTitle::~CTitle()
{
	
}

void	CTitle::SetDefaults()
{
	m_bTitleShow		=	true;
	m_strTitle			=	_TEXT("");

	HFONT	hFont=CreateFont(36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, _TEXT("Arial") );
	GetObject(hFont,sizeof(LOGFONT),&m_lfTitleFont);
	DeleteObject(hFont);
	
#ifndef	DEFAULT_WHITE_BACKGROUND
	// Default is black background
	m_crTitleColor		= RGB( 255, 255, 255 );
#else
	m_crTitleColor		= RGB( 0, 0, 0 );
#endif

	m_nTitlePosition	=	kPositionT;
	m_nTitleOrientation	=	kOrientationLR;

	m_nTitleBrushType = kBrushNull;
	m_crTitleBgColor = RGB(128, 128, 128);
	m_nTitleHatchStyle = HS_CROSS;
	m_crTitleHatchColor = RGB(128, 128, 128);
	if(m_hTitlePattern)
	{
		DeleteObject(m_hTitlePattern);
		m_hTitlePattern = 0;
	}
	m_bTitleBorderShow	=	false;
	m_crTitleBorderColor	=	RGB(64, 64, 64);
	m_nTitleBorderSize	=	2;
	m_nTitleBorderStyle	=	PS_SOLID;

	m_nTitleAlignment	=	0;
	m_nTitleGap			=	3;

	m_bLightTitle		=	false;
	m_bSelectTitle		=	false;

	m_nTitleSLShow		=	kSLShowEdge;

	m_bReactTitle		=	true;
}
