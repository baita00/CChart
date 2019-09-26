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

#include "PageNote.h"
using namespace NsCChart;

CPageNote::CPageNote()
{
	SetDefaults();
}

CPageNote::~CPageNote()
{

}

void	CPageNote::SetDefaults()
{
	m_bShowHeadernote = false;
	m_bShowFootnote = false;
	m_strHeadernote = _TEXT("CChart header");
	m_strFootnote = _TEXT("CChart footnote");
	m_crHeadernoteColor = RGB(128, 128, 128);
	m_crFootnoteColor = RGB(128, 128, 128);
	m_nHeadernoteFormat = DT_BOTTOM | DT_RIGHT;
	m_nFootnoteFormat = DT_BOTTOM | DT_RIGHT;
}