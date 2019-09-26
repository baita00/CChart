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

#include "Clock.h"
#include <time.h>

using namespace NsCChart;

CClock::CClock()
{
	SetDefaults();
}

CClock::~CClock()
{

}

void	CClock::SetDefaults()
{
	m_nYear = 2004;
	m_nMonth = 8;
	m_nDay = 17;

	m_nHour = 16;
	m_nMinute = 30;
	m_nSecond = 0;
}

void	CClock::SyncTime()
{
	time_t curTime;
	struct tm *myTm;
	time(&curTime);

#if defined(_MSC_VER) && (_MSC_VER>=1300)
	myTm = new struct tm;
	localtime_s(myTm, &curTime);
#else
	myTm = localtime(&curTime);
#endif
	

	m_nYear = myTm->tm_year + 1900;
	m_nMonth = myTm->tm_mon + 1;
	m_nDay = myTm->tm_mday;
	m_nHour = myTm->tm_hour;
	m_nMinute = myTm->tm_min;
	m_nSecond = myTm->tm_sec;

#if defined(_MSC_VER) && (_MSC_VER>=1300)
	delete myTm;
#endif
}