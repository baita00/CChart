
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CChart_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CChart_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CChart_EXPORTS
#define CChart_API __declspec(dllexport)
#else
#define CChart_API __declspec(dllimport)
#endif
/*
// This class is exported from the CChart.dll
class CChart_API CCChart {
public:
	CCChart(void);
	// TODO: add your methods here.
};

extern CChart_API int nCChart;

CChart_API int fnCChart(void);
*/
