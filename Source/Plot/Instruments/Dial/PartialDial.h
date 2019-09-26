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

#include "../../Basic/MyString.h"
#include "../../Basic/CChartNameSpace.h"
#include <windows.h>

Declare_Namespace_CChart

enum
{
	kPartialDialTop,
	kPartialDialBottom,
	kPartialDialLeft,
	kPartialDialRight,
	kPartialDialLeftTop,
	kPartialDialRightTop,
	kPartialDialLeftBottom,
	kPartialDialRightBottom,

	kPartialDialTypeCount
};

class CPartialDial
{
public:
	CPartialDial();
	virtual	~CPartialDial();

	void	SetDefaults();

protected:
	int		m_nPartialDialType;
	int		m_nPointerGap;
	bool	m_bSubtitleOuter;
	bool	m_bShowGlass;
	COLORREF	m_crGlassColor;
	int			m_nGlassAlpha;

public:
	inline	int		GetPartialDialType(){return m_nPartialDialType;}
	inline	void	SetPartialDialType(int type){m_nPartialDialType = type;}
	inline	int		GetPointerGap(){return m_nPointerGap;}
	inline	void	SetPointerGap(int gap){m_nPointerGap = gap;}
	inline	bool	IsSubtitleOuter(){return m_bSubtitleOuter;}
	inline	void	SetSubtitleOuter(bool outer){m_bSubtitleOuter = outer;}
	inline	bool	IsShowGlass(){return m_bShowGlass;}
	inline	void	SetShowGlass(bool show){m_bShowGlass = show;}
	inline	COLORREF	GetGlassColor(){return m_crGlassColor;}
	inline	void		SetGlassColor(COLORREF color){m_crGlassColor = color;}
	inline	int		GetGlassAlpha(){return m_nGlassAlpha;}
	inline	void	SetGlassAlpha(int alpha){m_nGlassAlpha = alpha;}

};

Declare_Namespace_End