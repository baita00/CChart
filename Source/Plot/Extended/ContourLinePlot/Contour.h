// Contour.h: interface for the CContour class.
//
// CContour implements Contour plot algorithm descrided in 
//		IMPLEMENTATION OF
//		AN IMPROVED CONTOUR
//		PLOTTING ALGORITHM
//		BY 
//
//		MICHAEL JOSEPH ARAMINI 
//
//		B.S., Stevens Institute of Technology, 1980 
// See http://www.ultranet.com/~aramini/thesis.html
//
// Ported to C++ by Jonathan de Halleux.
//
// Using CContour :
//
// CContour is not directly usable. The user has to 
//	1. derive the function ExportLine that is 
//		supposed to draw/store the segment of the contour
//	2. Set the function draw contour of. (using  SetFieldFn
//		The function must be declared as follows
//		double (*myF)(double x , double y);
//
//	History:
//		31-07-2002: 
//			- A lot of contribution from Chenggang Zhou (better strip compressions, merging, area, weight),
//			- Got rid of ugly MFC lists for STL.
//      20-09-2009
//          - Change the plane to stl by Yang Guojun
//////////////////////////////////////////////////////////////////////

#ifndef __CONTOUR_H_122333444455555__
#define __CONTOUR_H_122333444455555__

#include <vector>
#include <windows.h>

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

// A structure used internally by CContour
struct CFnStr
{
	double m_dFnVal;
	short m_sLeftLen;
	short m_sRightLen;
	short m_sTopLen;
	short m_sBotLen;
};

class CContour
{
public:
	CContour();
	virtual ~CContour();

protected:
	// Initialize memory. Called in Generate
	virtual void InitMemory();
	// Clean work arrays
	virtual void CleanMemory();
	// Generates contour
	// Before calling this functions you must
	//	1. derive the function ExportLine that is 
	//		supposed to draw/store the segment of the contour
	//	2. Set the function draw contour of. (using  SetFieldFn
	//		The function must be declared as follows
	//		double (*myF)(double x , double y);
	virtual void Generate();

	int		m_nContourLines;

public:
	// Set the dimension of the primary grid
	void SetFirstGrid(int iCol, int iRow);
	// Set the dimension of the base grid
	void SetSecondaryGrid(int iCol, int iRow);
	// Sets the region [left, right, bottom,top] to generate contour 
	void SetLimits(double pLimits[4]);
	// Sets the isocurve values
	void SetPlanes(const std::vector<double>& vPlanes);
	void SetPlanes(int nPlanes);
	void SetPlanes(int nPlanes, bool bExponential);
	// Sets the pointer to the F(x,y) funtion
	void SetFieldFcn(double (*_pFieldFcn) (double, double));
	double	(*GetFieldFcn( ))( double, double );

	int		GetNContour(){return m_nContourLines;}
	void	SetNContour(int n){m_nContourLines = n;SetPlanes(n);}

	// Retrieve dimension of grids, contouring region and isocurve
	int GetColFir() const
	{
		return m_iColFir;
	};
	int GetRowFir() const
	{
		return m_iRowFir;
	};
	int GetColSec() const
	{
		return m_iColSec;
	};
	int GetRowSec() const
	{
		return m_iRowSec;
	};
	void GetLimits(double pLimits[4]);
	UINT GetNPlanes() const
	{
		return (UINT)m_vPlanes.size();
	};
	const std::vector<double>& GetPlanes() const
	{
		return m_vPlanes;
	};
	double GetPlane(UINT i)	const
	{
		//ASSERT(i < m_vPlanes.size());
		return m_vPlanes[i];
	};

	// For an indexed point i on the sec. grid, returns x(i)
	double GetXi(int i) const
	{
		return m_pLimits[0] +	i %	(m_iColSec + 1) * (m_pLimits[1] - m_pLimits[0]) / (double) (m_iColSec);
	};
	// For an indexed point i on the fir. grid, returns y(i)
	double GetYi(int i) const
	{
		//ASSERT(i >= 0);
		return m_pLimits[2] + i /	(m_iColSec + 1) * (m_pLimits[3] - m_pLimits[2]) / (double) (m_iRowSec);
	};
	static void f4min(int *n, double x[2], double *y);
	static void f4max(int *n, double x[2], double *y);
	//static void f4min(double x[2], double &y);
	//static void f4max(double x[2], double &y);

protected:
	// Accesibles variables
	std::vector<double> m_vPlanes;			// value of contour planes
	double m_pLimits[4];						// left, right, bottom, top
	int m_iColFir;								// primary	grid, number of columns
	int m_iRowFir;								// primary	grid, number of rows
	int m_iColSec;								// secondary grid, number of columns
	int m_iRowSec;								// secondary grid, number of rows
	double (*m_pFieldFcn)(double x, double y); // pointer to F(x,y) function

	// Protected function
	virtual void ExportLine(int iPlane, int x1, int y1, int x2, int y2) = 0; // plots a line from (x1,y1) to (x2,y2)

	// Work functions and variables
	double m_dDx;
	double m_dDy;
	CFnStr** m_ppFnData;	// pointer to mesh parts
	CFnStr* FnctData(int i, int j)
	{
		return (m_ppFnData[i] + j);
	};
	double Field(int x, int y);	 /* evaluate funct if we must,	*/
	void Cntr1(int x1, int x2, int y1, int y2);
	void Pass2(int x1, int x2, int y1, int y2);	  /* draws the contour lines */

private:
	static	CContour	*m_pThis;
};

Declare_Namespace_End

#endif
