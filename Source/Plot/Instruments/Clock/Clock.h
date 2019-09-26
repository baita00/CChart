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
#include <windows.h>

Declare_Namespace_CChart

class CClock
{
public:
	CClock();
	virtual	~CClock();

	void	SetDefaults();

public:
	int		m_nYear, m_nMonth, m_nDay;
	int		m_nHour, m_nMinute, m_nSecond;

public:
	inline	int		GetYear(){return m_nYear;}
	inline	void	SetYear(int val){m_nYear = val;}
	inline	int		GetMonth(){return m_nMonth;}
	inline	void	SetMonth(int val){m_nMonth = val;}
	inline	int		GetDay(){return m_nDay;}
	inline	void	SetDay(int val){m_nDay = val;}
	inline	int		GetHour(){return m_nHour;}
	inline	void	SetHour(int val){m_nHour = val;}
	inline	int		GetMinute(){return m_nMinute;}
	inline	void	SetMinute(int val){m_nMinute = val;}
	inline	int		GetSecond(){return m_nSecond;}
	inline	void	SetSecond(int val){m_nSecond = val;}

public:
	void	SyncTime();
};

Declare_Namespace_End
