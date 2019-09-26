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

class CMyGroupBox
{
public:
	CMyGroupBox();
	virtual	~CMyGroupBox();

	void	SetDefaults();

protected:
	bool		m_bShowTitle;
	tstring		m_strTitle;
	LOGFONT		m_lfTitleFont;
	COLORREF	m_crTitleColor;
	bool		m_bTitleInsect;
	int			m_nTitleDisplaceX;
	int			m_nBorderSize;
	int			m_nBorderStyle;
	COLORREF	m_crBorderColor;
	bool		m_bBorder3D;
	bool		m_bSunken;

public:
	inline	bool		IsShowTitle( void ) { return m_bShowTitle; }
	inline	void		SetShowTitle( bool show ) { m_bShowTitle=show; }
	inline	tstring		GetTitle() { return m_strTitle; }
	inline	void		SetTitle( tstring title ) { m_strTitle = title; }
	inline	LOGFONT		&GetTitleFont() { return m_lfTitleFont; }
	inline	void		SetTitleFont( LOGFONT font)	{ m_lfTitleFont=font; }
	inline	COLORREF	GetTitleColor( void ) { return m_crTitleColor; }
	inline	void		SetTitleColor( COLORREF color ) {m_crTitleColor = color; }
	inline	bool		IsTitleInsect( void ) { return m_bTitleInsect; }
	inline	void		SetTitleInsect( bool insect ) { m_bTitleInsect = insect; }
	inline	int			GetTitleDisplaceX( void ) { return m_nTitleDisplaceX; }
	inline	void		SetTitleDisplaceX( int displace ) { m_nTitleDisplaceX = displace; }
	inline	int			GetBorderSize( void ){ return m_nBorderSize; }
	inline	void		SetBorderSize( int size ){ m_nBorderSize = size; }
	inline	int			GetBorderStyle( void ){ return m_nBorderStyle; }
	inline	void		SetBorderStyle( int size ){ m_nBorderStyle = size; }
	inline	COLORREF	GetBorderColor( void ) { return m_crBorderColor; }
	inline	void		SetBorderColor( COLORREF color ) { m_crBorderColor=color; }
	inline	bool		IsBorder3D( void ) { return m_bBorder3D; }
	inline	void		SetBorder3D( bool td ) { m_bBorder3D = td; if(td) m_bTitleInsect = false; }
	inline	bool		IsSunken( void ) { return m_bSunken; }
	inline	void		SetSunken( bool sk ) { m_bSunken = sk; }
};

Declare_Namespace_End
