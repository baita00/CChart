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

#include "CChartNameSpace.h"

Declare_Namespace_CChart

enum
{
	kDataBarBaseBottom,
	kDataBarBaseTop,
	kDataBarBaseLeft,
	kDataBarBaseRight,
	
	kDataBarBaseCount
};

class	CPlotDataBar
{
public:
	CPlotDataBar();
	virtual ~CPlotDataBar();

protected:
	void	SetDefaults();

protected:
	bool		m_bDataBarShow;
	int			m_nDataBarBase;
	int			m_nDataBarLineSize;
	int			m_nDataBarLineStyle;
	bool		m_bDataBarSameColor;
	COLORREF	m_crDataBarLineColor;
	int			m_nDataBarAlpha;
	int			m_nDataBarOffset;
public:
	inline	bool	IsDataBarShow(){return m_bDataBarShow;}
	inline	void	SetDataBarShow(bool show){m_bDataBarShow = show;}
	inline	int		GetDataBarBase(){return m_nDataBarBase;}
	inline	void	SetDataBarBase(int base){m_nDataBarBase = base;}
	inline	int		GetDataBarLineSize(){return m_nDataBarLineSize;}
	inline	void	SetDataBarLineSize(int lineSize){m_nDataBarLineSize = lineSize;}
	inline	int		GetDataBarLineStyle(){return m_nDataBarLineStyle;}
	inline	void	SetDataBarLineStyle(int lineStyle){m_nDataBarLineStyle = lineStyle;}
	inline	bool	IsDataBarSameColor(){return m_bDataBarSameColor;}
	inline	void	SetDataBarSameColor(bool same){m_bDataBarSameColor = same;}
	inline	COLORREF	GetDataBarLineColor(){return m_crDataBarLineColor;}
	inline	void		SetDataBarLineColor(COLORREF cr){m_crDataBarLineColor = cr;}
	inline	int		GetDataBarAlpha(){return m_nDataBarAlpha;}
	inline	void	SetDataBarAlpha(int alpha){m_nDataBarAlpha = alpha;}
	inline	int		GetDataBarOffset(){return m_nDataBarOffset;}
	inline	void	SetDataBarOffset(int offset){m_nDataBarOffset = offset;}
};

Declare_Namespace_End