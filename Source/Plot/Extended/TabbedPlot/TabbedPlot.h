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

#pragma once

#pragma warning(disable:4786)

#include <windows.h>
#include <vector>
using namespace std;

#include "../../Basic/CChartNameSpace.h"
#include "../../Basic/PlotImpl.h"
#include "../../Basic/Handler.h"
#include "../../Basic/ReactStatus.h"

Declare_Namespace_CChart


class CTabbedPlot
{
public:
	CTabbedPlot();
	virtual ~CTabbedPlot();

	void			SetDefaults();

protected:
	int						m_nCurTab;
	int						m_nTabMarginH, m_nTabMarginV;
	int						m_nTabTitleMargin;
	LOGFONT					m_lfTabFont;
	COLORREF				m_crTabColor, m_crCurTabColor, m_crTabEdgeColor, m_crTabTitleColor;
	vector<tstring>			m_vstrTabTitle;
public:
	vector<CPlotImpl *>		m_vpPlots;
	vector<CHandler *>		m_vpHandlers;

public:
	bool			AddSubPlot(CPlotImpl *pSubPlot, CHandler *pHandler, tstring strTabTitle);

	void			DeleteAll();

	CPlotImpl		*GetSubPlot(int nIndex);
	int				GetPlotCount();
	tstring			GetTabTitle(int nIndex);
	void			SetTabTitle(tstring title, int nIndex);

public:
	inline	int			GetCurTab(){return m_nCurTab;}
	inline	void		SetCurTab(int cur){m_nCurTab = cur;}
	inline	int			GetTabMarginH(){return m_nTabMarginH;}
	inline	void		SetTabMarginH(int margin){m_nTabMarginH = margin;}
	inline	int			GetTabMarginV(){return m_nTabMarginV;}
	inline	void		SetTabMarginV(int margin){m_nTabMarginV = margin;}
	inline	int			GetTabTitleMargin(){return m_nTabTitleMargin;}
	inline	void		SetTabTitleMargin(int margin){m_nTabTitleMargin = margin;}
	inline	LOGFONT		&GetTabFont(){return m_lfTabFont;}
	inline	void		SetTabFont(LOGFONT font){m_lfTabFont = font;}
	inline	COLORREF	GetTabColor(){return m_crTabColor;}
	inline	void		SetTabColor(COLORREF cr){m_crTabColor = cr;}
	inline	COLORREF	GetCurTabColor(){return m_crCurTabColor;}
	inline	void		SetCurTabColor(COLORREF cr){m_crCurTabColor = cr;}
	inline	COLORREF	GetTabEdgeColor(){return m_crTabEdgeColor;}
	inline	void		SetTabEdgeColor(COLORREF cr){m_crTabEdgeColor = cr;}
	inline	COLORREF	GetTabTitleColor(){return m_crTabTitleColor;}
	inline	void		SetTabTitleColor(COLORREF cr){m_crTabTitleColor = cr;}
};

Declare_Namespace_End