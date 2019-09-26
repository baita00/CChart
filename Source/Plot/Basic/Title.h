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
#include "MyString.h"
#include "PlotUtility.h"

Declare_Namespace_CChart

class CTitle
{
public:
	CTitle();
	virtual ~CTitle();

protected:
	void		SetDefaults();

protected:
	bool		m_bTitleShow;

	tstring		m_strTitle;
	LOGFONT		m_lfTitleFont;
	COLORREF	m_crTitleColor;

	int			m_nTitlePosition;
	int			m_nTitleOrientation;

	bool		m_bTitleRectShow;
	int			m_nTitleRectLineSize;
	int			m_nTitleRectLineStyle;

	int			m_nTitleBrushType;
	COLORREF	m_crTitleBgColor;
	int			m_nTitleHatchStyle;
	COLORREF	m_crTitleHatchColor;
	HBITMAP		m_hTitlePattern;
	
	bool		m_bTitleBorderShow;
	COLORREF	m_crTitleBorderColor;
	int			m_nTitleBorderSize;
	int			m_nTitleBorderStyle;
	
	int			m_nTitleAlignment;

	int			m_nTitleGap;

	bool		m_bLightTitle;
	bool		m_bSelectTitle;
	int			m_nTitleSLShow;

	bool		m_bReactTitle;

public:
	inline		bool		IsTitleShow(){ return m_bTitleShow; }
	inline		void		SetTitleShow(bool bShow){ m_bTitleShow = bShow; }
	inline		bool		IsTitleRealyShow( void ) { return m_bTitleShow && m_strTitle != _TEXT(""); }

	inline		tstring		GetTitle(){ return m_strTitle; }
	inline		void		SetTitle(tstring strTitle){ m_strTitle = strTitle; }
	inline		LOGFONT		&GetTitleFont(){ return m_lfTitleFont; }
	inline		void		SetTitleFont(LOGFONT lfFont){ m_lfTitleFont = lfFont; }
	inline		int			GetTitleHeight(){ return m_lfTitleFont.lfHeight; }
	inline		void		SetTitleHeight(int height){ m_lfTitleFont.lfHeight = height; }
	inline		tstring		GetTitleFaceName(){ return m_lfTitleFont.lfFaceName; }
	inline		void		SetTitleFaceName(tstring facename)
	{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_tcscpy_s(m_lfTitleFont.lfFaceName, 32, facename.c_str());
#else
		_tcscpy(m_lfTitleFont.lfFaceName, facename.c_str());
#endif
	}
	inline		COLORREF	GetTitleColor(){ return m_crTitleColor; }
	inline		void		SetTitleColor(COLORREF crColor){ m_crTitleColor = crColor; }
	inline		int			GetTitlePosition(){ return m_nTitlePosition; }
	inline		void		SetTitlePosition(int nPos){ m_nTitlePosition = nPos; }
	inline		int			GetTitleOrientation(){ return m_nTitleOrientation; }
	inline		void		SetTitleOrientation(int nOri){ m_nTitleOrientation = nOri; }

	inline		int			GetTitleBrushType( void ){ return m_nTitleBrushType; }
	inline		void		SetTitleBrushType( int type ){ m_nTitleBrushType = type; }
	inline		COLORREF	GetTitleBgColor( void ){ return m_crTitleBgColor; }
	inline		void		SetTitleBgColor( COLORREF color ){ m_crTitleBgColor = color; }
	inline		int			GetTitleHatchStyle( void ){ return m_nTitleHatchStyle; }
	inline		void		SetTitleHatchStyle( int style ){ m_nTitleHatchStyle = style; }
	inline		COLORREF	GetTitleHatchColor( void ){ return m_crTitleHatchColor; }
	inline		void		SetTitleHatchColor( COLORREF color ){ m_crTitleHatchColor = color; }
	inline		HBITMAP		GetTitlePattern( void ){ return m_hTitlePattern; }
	inline		void		SetTitlePattern( HBITMAP pattern ){ if(m_hTitlePattern)DeleteObject(m_hTitlePattern); m_hTitlePattern = CopyHBitmap(pattern); }
	
	inline		bool		IsTitleBorderShow( void ){ return m_bTitleBorderShow; }
	inline		void		SetTitleBorderShow( bool show ){ m_bTitleBorderShow = show; }
	inline		COLORREF	GetTitleBorderColor( void ){ return m_crTitleBorderColor; }
	inline		void		SetTitleBorderColor( COLORREF color ){ m_crTitleBorderColor = color; }
	inline		int			GetTitleBorderSize( void ){ return m_nTitleBorderSize; }
	inline		void		SetTitleBorderSize( int size ){ m_nTitleBorderSize = size; }
	inline		int			GetTitleBorderStyle( void ){ return m_nTitleBorderStyle; }
	inline		void		SetTitleBorderStyle( int style ){ m_nTitleBorderStyle = style; }

	inline		int			GetTitleAlignment(){ return m_nTitleAlignment; }
	inline		void		SetTitleAlignment(int nAlignment){ m_nTitleAlignment = nAlignment; }
	inline		int			GetTitleGap(){ return m_nTitleGap; }
	inline		void		SetTitleGap(int nGap){ m_nTitleGap = nGap; }

	inline		int			GetTitleSLShow( void ){ return m_nTitleSLShow; }
	inline		void		SetTitleSLShow( int nShow ){ m_nTitleSLShow = nShow; }

	inline		bool		IsSelectTitle() { return m_bSelectTitle; }
	inline		void		SetSelectTitle( bool select ) { m_bSelectTitle = select; }
	inline		bool		IsLightTitle() { return m_bLightTitle; }
	inline		void		SetLightTitle( bool light ) { m_bLightTitle=light; }

	inline		bool		IsReactTitle() { return m_bReactTitle; }
	inline		void		SetReactTitle( bool react ) { m_bReactTitle=react; }
};

Declare_Namespace_End
