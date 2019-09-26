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
#include "TabbedPlot.h"

using namespace NsCChart;

CTabbedPlot::CTabbedPlot()
{
	SetDefaults();
}

CTabbedPlot::~CTabbedPlot()
{
	DeleteAll();
}

void	CTabbedPlot::SetDefaults()
{
	HFONT hFont;	
	hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _TEXT("Arial"));
	GetObject(hFont, sizeof(LOGFONT), &m_lfTabFont);
	DeleteObject(hFont);
	m_nCurTab = 0;
	m_nTabMarginH = 2;
	m_nTabMarginV = 2;
	m_nTabTitleMargin = 5;
	m_crCurTabColor = RGB(192, 192, 192);
	m_crTabColor = GetSysColor(COLOR_3DFACE);
	m_crTabEdgeColor = RGB(168, 128, 128);
	m_crTabTitleColor = RGB(192, 108, 168);
}

bool	CTabbedPlot::AddSubPlot(CPlotImpl *pSubPlot, CHandler *pHandler, tstring strTabTitle)
{
	if(!pSubPlot || !pHandler)return false;

	m_vpPlots.push_back(pSubPlot);
	m_vpHandlers.push_back(pHandler);
	m_vstrTabTitle.push_back(strTabTitle);

	return true;
}

void	CTabbedPlot::DeleteAll()
{
	if(m_vpPlots.size()>0)
	{
		for(int i=(int)m_vpPlots.size()-1; i>=0; i--)
		{
			if(m_vpPlots[i])
			{
				delete m_vpPlots[i];
				m_vpPlots[i] = NULL;
			}
			m_vpPlots.pop_back();
		}
		m_vpHandlers.clear();
		m_vstrTabTitle.clear();
	}
}

CPlotImpl	*CTabbedPlot::GetSubPlot(int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpPlots.size())return 0;
	return m_vpPlots[nIndex];
}

int			CTabbedPlot::GetPlotCount()
{
	return (int)m_vpPlots.size();
}

tstring		CTabbedPlot::GetTabTitle(int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpPlots.size())return _TEXT("");
	return m_vstrTabTitle[nIndex];
}

void		CTabbedPlot::SetTabTitle(tstring title, int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpPlots.size())return;
	m_vstrTabTitle[nIndex] = title;
}