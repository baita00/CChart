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

#include "Comment.h"

using namespace NsCChart;

CComment::CComment()
{
	m_hBitmap = NULL;
	SetDefaults();
}

CComment::~CComment()
{
	DeleteUsedHandles();
}

void	CComment::SetDefaults()
{
	m_bTextComment = true;
	m_nCommentID = 0;

	m_bCommentShow = true;

	m_strComment = _TEXT("comment");
	HFONT	hFont=CreateFont(36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, _TEXT("Arial") );
	GetObject(hFont,sizeof(LOGFONT),&m_lfCommentFont);
	DeleteObject(hFont);
	m_crCommentColor = RGB(64, 64, 64);
	m_bTransparent = true;
	m_crCommentBkColor = RGB(228, 228, 228);

	m_bBorderShow = false;;
	m_nBorderSize = 2;
	m_nBorderStyle = PS_SOLID;
	m_crBorderColor = RGB(128, 128, 128);
	
	m_nFormat = DT_CENTER;

	m_hBitmap = NULL;
	m_nRop = SRCCOPY;

	m_nPosX = 0;
	m_nPosY = 0;
	
	m_nPosition = kCommentCenterHigher;
	m_sizeOffset.cx = 0;
	m_sizeOffset.cy = 0;
	m_bExcludeTitle = true;

	//m_bSubtitle = false;
	m_nCommentFlag = 0;

	m_bDragMode = false;
	m_bDragEnable = true;
}

void	CComment::DeleteUsedHandles()
{
	if(m_hBitmap)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
}

