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

#include "MyGdiPlus.h"
#include "Global.h"
#include "../Resources/PlotResources.h"

using namespace NsCChart;

/////////////////////////////////////////////////////////////////////////
// Class CGlobal static members

bool	CGlobal::m_bInDll = false;
#ifdef _DEBUG
#	if defined(_UNICODE) || defined(UNICODE)
tstring	CGlobal::m_strDllName = _TEXT("CChart_ud.dll");
#	else
tstring	CGlobal::m_strDllName = _TEXT("CChart_d.dll");
#	endif
#else
#	if defined(_UNICODE) || defined(UNICODE)
tstring	CGlobal::m_strDllName = _TEXT("CChart_u.dll");
#	else
tstring	CGlobal::m_strDllName = _TEXT("CChart.dll");
#	endif
#endif

bool	CGlobal::m_bLoadDefPatterns = true;
bool	CGlobal::m_bAddDefAxes = false;

bool	CGlobal::m_bShowDataProcessingMenu = false;

int		CGlobal::m_nLangurage = kLangEnglish;

bool	CGlobal::m_bEnableToolTip = false;

bool	CGlobal::m_bContinuePaint = true;
bool	CGlobal::m_bContinueReact = true;

bool	CGlobal::m_bImagePopupMenu = true;

/////////////////////////////////////////////////////////////////////////
// Class CGlobal static members functions

tstring	CGlobal::GetStrMyVersion()
{
	TCHAR str[nMaxString];
	switch(m_nLangurage)
	{
	case kLangEnglish:
		LoadString(MyGetModuleHandle(), IDS_STRING_VERSION, str, nMaxString);
		break;
	case kLangChinese:
		LoadString(MyGetModuleHandle(), IDS_STRING_VERSION_C, str, nMaxString);
		break;
	}
	return str;
}

tstring	CGlobal::GetStrAuthorInfo()
{
	TCHAR str[nMaxString];
	switch(m_nLangurage)
	{
	case kLangEnglish:
		LoadString(MyGetModuleHandle(), IDS_STRING_AUTHOR, str, nMaxString);
		break;
	case kLangChinese:
		LoadString(MyGetModuleHandle(), IDS_STRING_AUTHOR_C, str, nMaxString);
		break;
	}
	return str;
}

tstring	CGlobal::GetStrCopyright()
{
	TCHAR str[nMaxString];
	switch(m_nLangurage)
	{
	case kLangEnglish:
		LoadString(MyGetModuleHandle(), IDS_STRING_COPYRIGHT, str, nMaxString);
		break;
	case kLangChinese:
		LoadString(MyGetModuleHandle(), IDS_STRING_COPYRIGHT_C, str, nMaxString);
		break;
	}
	return str;
}

tstring	CGlobal::GetStrLicense()
{
	TCHAR str[nMaxString];
	switch(m_nLangurage)
	{
	case kLangEnglish:
		LoadString(MyGetModuleHandle(), IDS_STRING_LICENSE, str, nMaxString);
		break;
	case kLangChinese:
		LoadString(MyGetModuleHandle(), IDS_STRING_LICENSE_C, str, nMaxString);
		break;
	}
	return str;
}

tstring	CGlobal::GetStrStatement()
{
	TCHAR str[nMaxString];
	switch(m_nLangurage)
	{
	case kLangEnglish:
		LoadString(MyGetModuleHandle(), IDS_STRING_STATEMENT, str, nMaxString);
		break;
	case kLangChinese:
		LoadString(MyGetModuleHandle(), IDS_STRING_STATEMENT_C, str, nMaxString);
		break;
	}
	return str;
}

tstring	CGlobal::GetStrContact()
{
	TCHAR str[nMaxString];
	switch(m_nLangurage)
	{
	case kLangEnglish:
		LoadString(MyGetModuleHandle(), IDS_STRING_CONTACT, str, nMaxString);
		break;
	case kLangChinese:
		LoadString(MyGetModuleHandle(), IDS_STRING_CONTACT_C, str, nMaxString);
		break;
	}
	return str;
}

tstring	CGlobal::GetStrUntitle()
{
	switch(m_nLangurage)
	{
	case kLangEnglish:
		return _TEXT("Untitled");
	case kLangChinese:
		return _TEXT("未命名");
	default:
		return _TEXT("");
	}
}

HMODULE	CGlobal::MyGetModuleHandle()
{
	if(!m_bInDll)
	{
		return GetModuleHandle(NULL);
	}
	else
	{
		return GetModuleHandle(m_strDllName.c_str());
	}
}

static CInitGDIPlus InitGDIPlus_Controller;

void	CGlobal::InitGdiPlus()
{
	InitGDIPlus_Controller.Initialize();
}

void	CGlobal::FreeGdiPlus()
{
	InitGDIPlus_Controller.Deinitialize();
}

bool	CGlobal::IsGdiPlusInitialized()
{
	return InitGDIPlus_Controller.IsInitialized();
}
