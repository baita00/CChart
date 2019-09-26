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

class CDial
{
public:
	CDial();
	virtual	~CDial();

	void	SetDefaults();

protected:
	tstring		m_strSubTitle;
	bool		m_bShowSubTitle;
	SIZE		m_sizeSubTitleOffset;
	LOGFONT		m_lfSubTitleFont;
	COLORREF	m_crSubTitleColor;
	bool		m_bSubTitleAsValue;
	bool		m_bFillSubTitleRect;
	COLORREF	m_crSubTitleRectColor;

public:
	inline	tstring		GetSubTitle(){return m_strSubTitle;}
	inline	void		SetSubTitle(tstring title){m_strSubTitle = title;}
	inline	bool		IsShowSubTitle(){return m_bShowSubTitle;}
	inline	void		SetShowSubTitle(bool show){m_bShowSubTitle = show;}
	inline	SIZE		GetSubTitleOffset(){return m_sizeSubTitleOffset;}
	inline	void		SetSubTitleOffset(SIZE offset){m_sizeSubTitleOffset = offset;}
	inline	LOGFONT		&GetSubTitleFont(){return m_lfSubTitleFont;}
	inline	void		SetSubTitleFont(LOGFONT logFont){m_lfSubTitleFont = logFont;}
	inline	COLORREF	GetSubTitleColor(){return m_crSubTitleColor;}
	inline	void		SetSubTitleColor(COLORREF color){m_crSubTitleColor = color;}
	inline	bool		IsSubTitleAsValue( void ) { return m_bSubTitleAsValue; }
	inline	void		SetSubTitleAsValue( bool as ) { m_bSubTitleAsValue=as; }
	inline	bool		IsFillSubTitleRect( void ) { return m_bFillSubTitleRect; }
	inline	void		SetFillSubTitleRect( bool fill ) { m_bFillSubTitleRect = fill; }
	inline	COLORREF	GetSubTitleRectColor( void ) { return m_crSubTitleRectColor; }
	inline	void		SetSubTitleRectColor( COLORREF color ) { m_crSubTitleRectColor = color; }
};

Declare_Namespace_End
