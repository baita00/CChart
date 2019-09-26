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

#include "../../Basic/PlotData.h"
#include "math.h"

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

enum
{
	kSplModeParabolic		=0,
	kSplModeCubic,
	kSplModeBezier,
	kSplModeB,
	kSplModeLeastSquare,
	kSplModeCount
};

enum
{
	kSplEdgeModeFree		= 0,
	kSplEdgeModeClose		= 1,
	kSplEdgeModeTangent		= 2,
	kSplEdgeModeClamp		= 3,
	kSplEdgeModeCount
};

void	ClearAdjoinCoincide(MyVData2D &vData);

class CSpline
{
public:
	CSpline();
	virtual ~CSpline();
private:
	int m_nSegments;
	int m_nEdgeMode;
	DataPoint2D m_dpTanStart, m_dpTanEnd;
public:
	int GetEdgeMode()
	{
		return m_nEdgeMode;
	}
	void SetEdgeMode(int mode)
	{
		m_nEdgeMode = mode;
	}
	int GetSegments()
	{
		return m_nSegments;
	}
	void SetSegments(int seg)
	{
		m_nSegments = seg;
	}
	DataPoint2D GetTanEnd()
	{
		return m_dpTanEnd;
	}
	void SetTanEnd(DataPoint2D tan)
	{
		m_dpTanEnd.val[0] = tan.val[0];m_dpTanEnd.val[1] = tan.val[1];
	}
	DataPoint2D GetTanStart()
	{
		return m_dpTanStart;
	}
	void SetTanStart(DataPoint2D tan)
	{
		m_dpTanStart.val[0] = tan.val[0];m_dpTanStart.val[1] = tan.val[1];
	}

	DataPoint2D GetBSplCtrlPoint(const MyVData2D& vData, int index, int order, bool bLeft);
	double GetXLength(const MyVData2D& vData);

	bool GetParSplineValue(DataPoint2D& val, const MyVData2D& vData, int idxLeft, int idxSeg);
	bool GetCubicSplineValue(DataPoint2D& val, const MyVData2D& vData, int idxLeft, int idxSeg);
	bool GetBezierSplineValue(DataPoint2D& val, const MyVData2D& vData, int idxSeg);
	bool GetBSplineValue(DataPoint2D& val, const MyVData2D& vData, int m, int n, int idxSeg);
	bool GetLeastSqualValue(DataPoint2D& val, const MyVData2D& vData, int n);

private:
	DataPoint2D GetEdgePoint(int realIdx, const MyVData2D& vData);
	double Factorial(int n)
	{
		if (n <= 1)
			return 1.0;
		else
			return n * Factorial(n - 1);
	}
	double Combination(int n, int i)
	{
		return Factorial(n) / (Factorial(i) * Factorial(n - i));
	}
	double Bernstain(int n, int i, double t)
	{
		return Combination(n, i) * pow(t, i) * pow(1 - t, n - i);
	}
	double Bernstnew(int n, int k, double t);
	bool Tridag(double* a, double* b, double* c, double* r, double* u, int n);//追赶法解三对角方程
};

Declare_Namespace_End