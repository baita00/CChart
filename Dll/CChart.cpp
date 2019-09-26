// CChart.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CChart.h"

#include "../Source/Plot/Wrapper/Chart.h"
#include "../Source/Plot/Wrapper/Classless.h"
#include "../Source/Plot/Basic/Global.h"

using namespace NsCChart;

#ifdef _DEBUG
#	if defined(_UNICODE) || defined(UNICODE)								
#		pragma comment(lib,"..\\Source\\Bin\\Plotud.lib")			
#	else												
#		pragma comment(lib,"..\\Source\\Bin\\Plotd.lib")
#	endif												
#else				
#	if defined(_UNICODE) || defined(UNICODE)								
#		pragma comment(lib,"..\\Source\\Bin\\Plotu.lib")			
#	else												
#		pragma comment(lib,"..\\Source\\Bin\\Plot.lib")
#	endif
#endif


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	TCHAR myName[128];
	static long nRef=0;
    
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
			if(nRef==0)
			{
				::GetModuleFileName((HMODULE)hModule, myName, 128);
				CGlobal::m_bInDll = true;
				CGlobal::m_strDllName = myName;
				
				if(!CGlobal::IsGdiPlusInitialized())
					CGlobal::InitGdiPlus();
				
				RegisterChartControl();
			}
			InterlockedIncrement(&nRef);
			break;
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			InterlockedDecrement(&nRef);
			if(nRef==0)
			{
				CGlobal::m_bInDll = false;
				
				if(CGlobal::IsGdiPlusInitialized())
					CGlobal::FreeGdiPlus();
				
				UnregisterClass(_TEXT("ChartCtrl"), GetModuleHandle(NULL));
			}
			break;
    }
    return TRUE;
}

/*
// This is an example of an exported variable
CChart_API int nCChart=0;

// This is an example of an exported function.
CChart_API int fnCChart(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see CChart.h for the class definition
CCChart::CCChart()
{ 
	return; 
}
*/
