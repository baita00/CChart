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

#include "MyGroupBox.h"

using namespace NsCChart;

CMyGroupBox::CMyGroupBox()
{
	SetDefaults();
}

CMyGroupBox::~CMyGroupBox()
{

}

void	CMyGroupBox::SetDefaults()
{
	m_bShowTitle = true;
	m_strTitle = _TEXT("Group Box");
	HFONT hFont;
	hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN, _TEXT("Arial"));
	GetObject(hFont, sizeof(LOGFONT), &m_lfTitleFont);
	DeleteObject(hFont);
	m_crTitleColor = RGB(92, 92, 92);
	m_bTitleInsect = true;
	m_nTitleDisplaceX = 10;
	m_nBorderSize = 1;
	m_nBorderStyle = PS_SOLID;
	m_crBorderColor = RGB(32, 32, 32);
	m_bBorder3D = false;
	m_bSunken = true;
}