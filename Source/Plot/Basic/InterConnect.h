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

class CInterConnect
{
public:
	CInterConnect();
	virtual	~CInterConnect();

protected:
	void	SetDefaults();

protected:
	bool				m_bInterConnect;
	int					m_nInterConnectLineSize;
	int					m_nInterConnectLineStyle;
	COLORREF			m_crInterConnectLineColor;
	int					m_nInterConnectAlpha;

public:
	inline bool			IsInterConnect() { return m_bInterConnect;}
	inline void			SetInterConnect( bool inter ) { m_bInterConnect=inter; }
	inline int			GetInterConnectLineSize() { return m_nInterConnectLineSize; }
	inline void			SetInterConnectLineSize( int linesize ) { m_nInterConnectLineSize = linesize; }
	inline int			GetInterConnectLineStyle() { return m_nInterConnectLineStyle; }
	inline void			SetInterConnectLineStyle( int linestyle ) { m_nInterConnectLineStyle = linestyle; }
	inline COLORREF		GetInterConnectLineColor() { return m_crInterConnectLineColor; }
	inline void			SetInterConnectLineColor( COLORREF color ) { m_crInterConnectLineColor = color; }
	inline int			GetInterConnectAlpha() { return m_nInterConnectAlpha; }
	inline void			SetInterConnectAlpha( int alpha ) { m_nInterConnectAlpha = alpha; }
};

Declare_Namespace_End
