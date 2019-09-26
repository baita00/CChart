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

#include "../CChartNameSpace.h"
#include "../MyString.h"
#include <windows.h>

Declare_Namespace_CChart

class	CTooltip
{
public:
	CTooltip();
	virtual	~CTooltip();

	void	SetDefaults();

public:
	bool				m_bShowTooltip;
	POINT				m_pointTooltip;
	tstring				m_strTooltip;

protected:
	LOGFONT				m_lfTooltipFont;
	COLORREF			m_crTooltip;
	bool				m_bTooltipRectShow;
	COLORREF			m_crTooltipRect;
	int					m_nTooltipRectLineSize;
	int					m_nTooltipRectLineStyle;

public:
	inline	LOGFONT		&GetTooltipFont(){return m_lfTooltipFont;}
	inline	void		SetTooltipFont(LOGFONT lf){m_lfTooltipFont = lf;}
	inline	COLORREF	GetTooltipColor(){return m_crTooltip;}
	inline	void		SetTooltipColor(COLORREF cr){m_crTooltip = cr;}
	inline	bool		IsTooltipRectShow(){return m_bTooltipRectShow;}
	inline	void		SetTooltipRectShow(bool show){m_bTooltipRectShow = show;}
	inline	COLORREF	GetTooltipRectColor(){return m_crTooltipRect;}
	inline	void		SetTooltipRectColor(COLORREF cr){m_crTooltipRect = cr;}
	inline	int			GetTooltipRectLineSize(){return m_nTooltipRectLineSize;}
	inline	void		SetTooltipRectLineSize(int linesize){m_nTooltipRectLineSize = linesize;}
	inline	int			GetTooltipRectLineStyle(){return m_nTooltipRectLineStyle;}
	inline	void		SetTooltipRectLineStyle(int linestyle){m_nTooltipRectLineStyle = linestyle;}
};

Declare_Namespace_End