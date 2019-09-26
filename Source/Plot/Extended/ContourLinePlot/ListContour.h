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

// ListContour.h: interface for the CListContour class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __LISTCOUNTOUR_H_122333444455555__
#define __LISTCOUNTOUR_H_122333444455555__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "Contour.h"
#include <list>
#include <tchar.h>

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

// a list of point index referring to the secondary grid
// Let i the index of a point,
typedef std::list<UINT> CLineStrip;
typedef std::list<CLineStrip*> CLineStripList;
typedef std::vector<CLineStripList> CLineStripListVector;


//Declare_Namespace_CChart
class CListContour : public CContour
{
public:
	CListContour();
	virtual ~CListContour();

	// retreiving list of line strip for the i-th contour
	CLineStripList* GetLines(UINT iPlane)
	{
		//ASSERT(iPlane < GetNPlanes());
		return &m_vStripLists[iPlane];
	};
public:
	int	GetLineCounts(){return (int)m_vStripLists.size();}

protected:
	// Initializing memory
	virtual void InitMemory();
	// Cleaning memory and line strips
	virtual void CleanMemory();
public:
	// Generate contour strips
	virtual void Generate();

	// Adding segment to line strips
	// See CContour::ExportLine for further details
	void ExportLine(int iPlane, int x1, int y1, int x2, int y2);

	// Basic algorithm to concatanate line strip. Not optimized at all !
	void CompactStrips();
	/// debuggin
	void DumpPlane(UINT iPlane) const;

	// Area given by this function can be positive or negative depending on the winding direction of the contour.
	double Area(CLineStrip* Line);

	double EdgeWeight(CLineStrip* pLine, double R);
	bool PrintContour(char* fname);
	double GetValue(double x, double y)
	{
		return CContour::m_pFieldFcn(x, y);
	};
protected:
	// Merges pStrip1 with pStrip2 if they have a common end point
	bool MergeStrips(CLineStrip* pStrip1, CLineStrip* pStrip2);
	// Merges the two strips with a welding threshold.
	bool ForceMerge(CLineStrip* pStrip1, CLineStrip* pStrip2);
	// returns true if contour is touching boundary
	bool OnBoundary(CLineStrip* pStrip);

private:
	// array of line strips
	CLineStripListVector m_vStripLists;
};

Declare_Namespace_End

#endif
