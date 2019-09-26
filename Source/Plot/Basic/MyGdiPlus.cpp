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
#include "MyString.h"
#include <tchar.h>
#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER==1200
//#	pragma comment(lib, "../Accessary/GdiPlus/Lib/gdiplus.lib")
#else
#	pragma comment(lib, "gdiplus.lib")
#endif

bool GetEncoderClassID(const TCHAR *format, CLSID *pClsid)
{
	UINT n, size;
	ImageCodecInfo *codecInfos;
	GetImageEncodersSize(&n, &size);
	codecInfos = (ImageCodecInfo *)malloc(size);
	GetImageEncoders(n, size, codecInfos);
	for (UINT i = 0; i < n; i++)
	{
		int result;
#if defined(_UNICODE) || defined(UNICODE)
		result = wcscmp(codecInfos[i].FormatDescription, format);
#else
		wstring fmt;
		if(!NsCChart::mymbs2wcs(format, fmt))return false;
		result = wcscmp(codecInfos[i].FormatDescription, fmt.c_str());
#endif
		// wcscmp类似于strcmp
		if (result == 0)
		{
			*pClsid = codecInfos[i].Clsid;
			free(codecInfos);
			return true;
		}
	}
	free(codecInfos);
	return false;
}

CInitGDIPlus::CInitGDIPlus(bool bInitCtorDtor) : m_bInitCtorDtor(bInitCtorDtor),
	m_bInited(false), m_hMap(NULL), m_gdiplusToken(NULL),
	m_gdiplusStartupInput(NULL), m_initcount(0)
{
	if (m_bInitCtorDtor)
	{
		Initialize();
	}
}
	
	// If GDI+ has not explicitly been Deinitialized, do it in the destructor
CInitGDIPlus::~CInitGDIPlus()
{
	if (m_bInitCtorDtor)
	{
		Deinitialize();
	}
}
	
	// This function creates a file mapping based on the current process id.
	// If the mapping already exists, it knows that another instance of this class
	// elsewhere in the process has already taken care of starting GDI+.
void CInitGDIPlus::Initialize()
{
	if (!m_bInited)
	{
		TCHAR buffer[128];
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(buffer, 128, _TEXT("GDIPlusInitID=%x"), GetCurrentProcessId());
#else
		_stprintf(buffer, _TEXT("GDIPlusInitID=%x"), GetCurrentProcessId());
#endif
		
		m_hMap = CreateFileMapping((HANDLE) INVALID_HANDLE_VALUE, NULL,
			PAGE_READWRITE | SEC_COMMIT, 0, sizeof(long), buffer);
		if (m_hMap != NULL)
		{
			// We might have a winner
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(m_hMap);
			}
			else
			{
				// Yes, we have a winner
				m_bInited = true;
				Gdiplus::GdiplusStartup(&m_gdiplusToken,
					&m_gdiplusStartupInput, NULL);
				//TRACE("Inited GDIPlus\n");
			}
		}
	}
	m_initcount++;
}
	
	// No tricks to this function.  If this was the class that originally started GDI+,
	// and its initialization count has reached zero, it shuts down GDI+.
void CInitGDIPlus::Deinitialize()
{
	m_initcount--;
	if (m_bInited && m_initcount == 0)
	{
		//TRACE("GDIPlus shutdown\n");
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
		CloseHandle(m_hMap);
		m_bInited = false;
	}
}

bool CInitGDIPlus::IsInitialized()
{
	return m_bInited && m_initcount > 0;
}
