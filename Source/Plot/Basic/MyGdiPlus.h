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

#ifndef __MYGDIPLUS_H_122333444455555__
#define __MYGDIPLUS_H_122333444455555__

#include <comdef.h>

#ifndef _WIN64
typedef ULONG ULONG_PTR;
#endif

#if defined(_MSC_VER) && _MSC_VER<=1200
#	include "../Accessary/GdiPlus/Includes/GdiPlus.h"
#else
#	include "GdiPlus.h"
#endif
using namespace Gdiplus;

//#pragma comment(lib, "Plot/Accessary/GdiPlus/Lib/gdiplus.lib")

bool GetEncoderClassID(const TCHAR* format, CLSID* pClsid);

class CInitGDIPlus
{
private:
	HANDLE m_hMap;
	bool m_bInited, m_bInitCtorDtor;
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	long m_initcount;

public:
	CInitGDIPlus(bool bInitCtorDtor = false);

	// If GDI+ has not explicitly been Deinitialized, do it in the destructor
	virtual ~CInitGDIPlus();

	// This function creates a file mapping based on the current process id.
	// If the mapping already exists, it knows that another instance of this class
	// elsewhere in the process has already taken care of starting GDI+.
	void Initialize();

	// No tricks to this function.  If this was the class that originally started GDI+,
	// and its initialization count has reached zero, it shuts down GDI+.
	void Deinitialize();

	// Check if initialized
	bool IsInitialized();
};

#endif