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

class	CGrids
{
public:
	CGrids();
	virtual ~CGrids();

protected:
	void	SetDefaults();

protected:
	bool		m_bMajorHGrids;
	bool		m_bMajorVGrids;
	bool		m_bMinorHGrids;
	bool		m_bMinorVGrids;
	COLORREF	m_crMajorGridColor, m_crMinorGridColor;
	int			m_nMajorGridLineSize, m_nMinorGridLineSize;
	int			m_nMajorGridLineStyle, m_nMinorGridLineStyle;

public:
	inline	bool			IsMajorHGrids(){return m_bMajorHGrids;}
	inline	void			SetMajorHGrids(bool bUse){m_bMajorHGrids=bUse;}
	inline	bool			IsMajorVGrids(){return m_bMajorVGrids;}
	inline	void			SetMajorVGrids(bool bUse){m_bMajorVGrids=bUse;}
	inline	bool			IsMinorHGrids(){return m_bMinorHGrids;}
	inline	void			SetMinorHGrids(bool bUse){m_bMinorHGrids=bUse;}
	inline	bool			IsMinorVGrids(){return m_bMinorVGrids;}
	inline	void			SetMinorVGrids(bool bUse){m_bMinorVGrids=bUse;}

	inline	void			SetMajorGrids(bool bUse){m_bMajorHGrids=bUse;m_bMajorVGrids=bUse;}
	inline	void			SetMinorGrids(bool bUse){m_bMinorHGrids=bUse;m_bMinorVGrids=bUse;}

	inline	int				GetMajorGridLineSize( void ) { return m_nMajorGridLineSize; }
	inline	void			SetMajorGridLineSize( int lineSize ) { if( lineSize > 0 ) m_nMajorGridLineSize = lineSize; }
	inline	int				GetMinorGridLineSize( void ) { return m_nMinorGridLineSize; }
	inline	void			SetMinorGridLineSize( int lineSize ) { if( lineSize > 0 ) m_nMinorGridLineSize = lineSize; }
	inline	int				GetMajorGridLineStyle( void ) { return m_nMajorGridLineStyle; }
	inline	void			SetMajorGridLineStyle( int lineStyle ) { m_nMajorGridLineStyle = lineStyle; }
	inline	int				GetMinorGridLineStyle( void ) { return m_nMinorGridLineStyle; }
	inline	void			SetMinorGridLineStyle( int lineStyle ) { m_nMinorGridLineStyle = lineStyle; }
	inline	COLORREF		GetMajorGridColor( void ) { return m_crMajorGridColor; }
	inline	void			SetMajorGridColor( COLORREF color ) { m_crMajorGridColor = color; }
	inline	COLORREF		GetMinorGridColor( void ) { return m_crMinorGridColor; }
	inline	void			SetMinorGridColor( COLORREF color ) { m_crMinorGridColor = color; }
};

Declare_Namespace_End