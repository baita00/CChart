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
#include "../MyString.h"

#include "../CChartNameSpace.h"

Declare_Namespace_CChart

class CPageNote
{
public:
	CPageNote();
	virtual ~CPageNote();
protected:
	void	SetDefaults();

protected:
	bool		m_bShowHeadernote;
	bool		m_bShowFootnote;
	tstring		m_strHeadernote;
	tstring		m_strFootnote;
	LOGFONT		m_lfHeadernoteFont;
	LOGFONT		m_lfFootnoteFont;
	COLORREF	m_crHeadernoteColor;
	COLORREF	m_crFootnoteColor;
	int			m_nHeadernoteFormat;
	int			m_nFootnoteFormat;

public:
	inline	bool			IsShowHeadernote() { return m_bShowHeadernote; }
	inline	void			SetShowHeadernote( bool show ) { m_bShowHeadernote=show; }
	inline	bool			IsShowFootnote() { return m_bShowFootnote; }
	inline	void			SetShowFootnote( bool show ) { m_bShowFootnote=show; }
	inline	tstring			GetHeadernote() { return m_strHeadernote; }
	inline	void			SetHeadernote( tstring header ) { m_strHeadernote = header; }
	inline	tstring			GetFootnote() { return m_strFootnote; }
	inline	void			SetFootnote( tstring footnote ) { m_strFootnote = footnote; }
	inline	COLORREF		GetHeadernoteColor( void ) { return m_crHeadernoteColor; }
	inline	void			SetHeadernoteColor( COLORREF color ) { m_crHeadernoteColor = color; }
	inline	COLORREF		GetFootnoteColor( void ) { return m_crFootnoteColor; }
	inline	void			SetFootnoteColor( COLORREF color ) { m_crFootnoteColor = color; }
	inline	LOGFONT			&GetHeadernoteFont() { return m_lfHeadernoteFont; }
	inline	void			SetHeadernoteFont( LOGFONT font ) { m_lfHeadernoteFont = font; }
	inline	LOGFONT			&GetFootnoteFont() { return m_lfFootnoteFont; }
	inline	void			SetFootnoteFont( LOGFONT font ) { m_lfFootnoteFont = font; }
	inline	int				GetHeadernoteFormat() { return m_nHeadernoteFormat; }
	inline	void			SetHeadernoteFormat(int format) { m_nHeadernoteFormat = format; }
	inline	int				GetFootnoteFormat() { return m_nFootnoteFormat; }
	inline	void			SetFootnoteFormat(int format) { m_nFootnoteFormat = format; }
};

Declare_Namespace_End
