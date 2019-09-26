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
#include "MyString.h"

Declare_Namespace_CChart

// class CPlotBasicImpl is a basement class

class CPlotBasicImpl
{
public:
	CPlotBasicImpl();
	virtual ~CPlotBasicImpl();
public:
	virtual	void		OnDraw(  ) = 0;
	virtual	void		OnDraw( HWND hWnd ) = 0;
	virtual	void		OnDraw( HWND hWnd, RECT destRect ) = 0;
	virtual	void		OnDraw( HDC hDC ) = 0;
	virtual void		OnDraw( HDC hDC, RECT destRect ) = 0;
	virtual	void		FinalDraw( HDC hDC, RECT destRect ) = 0;
	
	virtual	void		Update( HDC hDC, RECT destRect) = 0;
public:
	HWND				m_hWnd;
	bool				m_bRectConfined;
	RECT				m_rctConfined;
	int					m_nPlotID;
public:
	HWND				GetWnd();
	void				SetWnd(HWND hWnd);
	bool				IsRectConfined();
	void				SetRectConfined(bool confined);
	RECT				GetConfineRect();
	void				SetConfineRect(RECT rect);
	int					GetPlotID();
	void				SetPlotID(int id);

	// error handling
protected:
	tstring		m_strLastError;
	
public:
	void		SetLastError(tstring errstring) { m_strLastError = errstring; }
	tstring&	GetErrString() { return m_strLastError; }
	
	tstring		GetLastError(bool bReset=true) { tstring str = m_strLastError; if(bReset)m_strLastError=_TEXT(""); return str; }
	
	bool		IsError(tstring errstr) { return errstr.length()>0; }
	bool		IsError() { return IsError(m_strLastError); }
	
	void		Reset() { m_strLastError=_TEXT(""); }
};

Declare_Namespace_End