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

#include <windows.h>

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class CSlider
{
public:
	CSlider();
	virtual	~CSlider();

	void	SetDefaults();

protected:
	bool		m_bShowBarLine;
	int			m_nSliderHalfLen;
	COLORREF	m_crSliderColor;
	bool		m_bShowBarCenter;
	int			m_nBarCenterWidth;

public:
	inline	bool		IsShowBarLine(){return m_bShowBarLine;}
	inline	void		SetShowBarLine(bool show){m_bShowBarLine = show;}
	inline	int			GetSliderHalfLen(){return m_nSliderHalfLen;}
	inline	void		SetSliderHalfLen(int len){m_nSliderHalfLen = len;}
	inline	COLORREF	GetSliderColor(){return m_crSliderColor;}
	inline	void		SetSliderColor(COLORREF color){m_crSliderColor = color;}
	inline	bool		IsShowBarCenter(){return m_bShowBarCenter;}
	inline	void		SetShowBarCenter(bool show){m_bShowBarCenter = show;}
	inline	int			GetBarCenterWidth(){return m_nBarCenterWidth;}
	inline	void		SetBarCenterWidth(int width){m_nBarCenterWidth = width;}
};

Declare_Namespace_End
