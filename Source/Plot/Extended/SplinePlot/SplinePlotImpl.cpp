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

//#include "stdafx.h"
#include "SplinePlotImpl.h"
#include <sstream>

using namespace NsCChart;

CSplinePlotImpl::CSplinePlotImpl()
{
}

CSplinePlotImpl::~CSplinePlotImpl()
{
}

int CSplinePlotImpl::AddSplineByNum(MyVData2D vData, int nSplineMode,
	COLORREF color, int nSegments, int nOrder, int nEdgeMode)
{
	if (nOrder <= 1 || nSegments <= 1)
		return -1;

	CSpline spline;
	MyVData2D vSplData;
	DataPoint2D datapoint;
	tostringstream ostr;
	tstring name;

	int i, j;
	double max, min;

	ClearAdjoinCoincide(vData);

	int dataID;

	spline.SetSegments(nSegments);
	spline.SetEdgeMode(nEdgeMode);
	switch (nSplineMode)
	{
	case kSplModeParabolic:
		for (i = 0; i < (int)vData.size() - 1; i++)
		{
			for (int j = 0; j < spline.GetSegments(); j++)
			{
				if (!spline.GetParSplineValue(datapoint, vData, i, j))
					continue;
				vSplData.push_back(datapoint);
			}
			if (i == vData.size() - 2)
			{
				if (!spline.GetParSplineValue(datapoint, vData, i,
								spline.GetSegments()))
					continue;
				vSplData.push_back(datapoint);
			}
		}
		dataID = AddCurve(vSplData);
		name = _TEXT("parabolic spline");
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;
	case kSplModeCubic:
		for (i = 0; i <= (int)vData.size() - 2; i++)
		{
			for (j = 0; j < spline.GetSegments(); j++)
			{
				if (!spline.GetCubicSplineValue(datapoint, vData, i, j))
					continue;
				vSplData.push_back(datapoint);
			}
			if (i == vData.size() - 2)
			{
				if (!spline.GetCubicSplineValue(datapoint, vData, i,
								spline.GetSegments()))
					continue;
				vSplData.push_back(datapoint);
			}
		}
		dataID = AddCurve(vSplData);
		name = _TEXT("cubic spline");
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);

		return dataID;
	case kSplModeBezier:
		for (i = 0; i <= spline.GetSegments(); i++)
		{
			if (!spline.GetBezierSplineValue(datapoint, vData, i))
				continue;
			vSplData.push_back(datapoint);
		}
		dataID = AddCurve(vSplData);
		name = _TEXT("bezier spline");
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;
	case kSplModeB:
		for (i = 0; i <= (int)vData.size() - nOrder; i++)
		{
			for (j = 0; j < spline.GetSegments(); j++)
			{
				if (!spline.GetBSplineValue(datapoint, vData, i, nOrder, j))
					continue;
				vSplData.push_back(datapoint);
			}
			if (i == vData.size() - nOrder)
			{
				if (!spline.GetBSplineValue(datapoint, vData, i, nOrder,
								spline.GetSegments()))
					continue;
				vSplData.push_back(datapoint);
			}
		}
		dataID = AddCurve(vSplData);
		ostr<<nOrder<<_TEXT(" order B-Spline");
		name=ostr.str();
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;
	case kSplModeLeastSquare:
		min = vData[0].val[0];
		max = vData[0].val[0];
		for (i = 1; i < (int)vData.size(); i++)
		{
			if (vData[i].val[0] < min)
				min = vData[i].val[0];
			if (vData[i].val[0] > max)
				max = vData[i].val[0];
		}
		for (i = 0; i <= spline.GetSegments(); i++)
		{
			datapoint.val[0] = min + i * (max - min) / (spline.GetSegments() - 1);
			spline.GetLeastSqualValue(datapoint, vData, nOrder);
			vSplData.push_back(datapoint);
		}
		dataID = AddCurve(vSplData);
		ostr<<nOrder<<_TEXT(" order least square");
		name=ostr.str();
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;

	default:
		return -1;
	}
}

int CSplinePlotImpl::AddSplineByStep(MyVData2D vData, int nSplineMode,
	COLORREF color, double fMaxStep, int nOrder, int nEdgeMode)
{
	if (nOrder <= 1 || fMaxStep < 0.0)
		return -1;

	CSpline spline;
	MyVData2D vSplData;
	DataPoint2D datapoint, datapoint1;
	
	tostringstream ostr;
	tstring name;

	int i, j;
	double max, min;
	double xlength;

	ClearAdjoinCoincide(vData);

	int dataID;

	int nSeg;

	//spline.SetSegments(nSegments);
	spline.SetEdgeMode(nEdgeMode);
	switch (nSplineMode)
	{
	case kSplModeParabolic:
		for (i = 0; i < (int)vData.size() - 1; i++)
		{
			nSeg = (int) ceil(fabs((vData[i + 1].val[0] - vData[i].val[0]) / fMaxStep));
			if (nSeg <= 0)
				nSeg = 1;
			spline.SetSegments(nSeg);

			for (int j = 0; j < spline.GetSegments(); j++)
			{
				if (!spline.GetParSplineValue(datapoint, vData, i, j))
					continue;
				vSplData.push_back(datapoint);
			}
			if (i == vData.size() - 2)
			{
				if (!spline.GetParSplineValue(datapoint, vData, i,
								spline.GetSegments()))
					continue;
				vSplData.push_back(datapoint);
			}
		}
		dataID = AddCurve(vSplData);
		name = _TEXT("parabolic spline");
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;
	case kSplModeCubic:
		for (i = 0; i <= (int)vData.size() - 2; i++)
		{
			nSeg = (int) ceil(fabs((vData[i + 1].val[0] - vData[i].val[0]) / fMaxStep));
			if (nSeg <= 0)
				nSeg = 1;
			spline.SetSegments(nSeg);
			for (j = 0; j < spline.GetSegments(); j++)
			{
				if (!spline.GetCubicSplineValue(datapoint, vData, i, j))
					continue;
				vSplData.push_back(datapoint);
			}
			if (i == vData.size() - 2)
			{
				if (!spline.GetCubicSplineValue(datapoint, vData, i,
								spline.GetSegments()))
					continue;
				vSplData.push_back(datapoint);
			}
		}
		dataID = AddCurve(vSplData);
		name = _TEXT("cubic spline");
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);

		return dataID;
	case kSplModeBezier:
		xlength = spline.GetXLength(vData);
		nSeg = (int) ceil(xlength / fMaxStep);
		if (nSeg <= 0)
			nSeg = 1;
		spline.SetSegments(nSeg);

		for (i = 0; i <= spline.GetSegments(); i++)
		{
			if (!spline.GetBezierSplineValue(datapoint, vData, i))
				continue;
			vSplData.push_back(datapoint);
		}
		dataID = AddCurve(vSplData);
		name = _TEXT("bezier spline");
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;
	case kSplModeB:
		for (i = 0; i <= (int)vData.size() - nOrder; i++)
		{
			//xlength=spline.GetCentroid(vData,i+1,nOrder).val[0]-spline.GetCentroid(vData,i,nOrder).val[0];
			datapoint = spline.GetBSplCtrlPoint(vData, i, nOrder, true);
			datapoint1 = spline.GetBSplCtrlPoint(vData, i, nOrder, false);
			xlength = datapoint1.val[0] - datapoint.val[0];

			nSeg = (int) ceil(fabs(xlength / fMaxStep));
			if (nSeg <= 0)
				nSeg = 1;
			spline.SetSegments(nSeg);

			for (j = 0; j < spline.GetSegments(); j++)
			{
				if (!spline.GetBSplineValue(datapoint, vData, i, nOrder, j))
					continue;
				vSplData.push_back(datapoint);
			}
			if (i == vData.size() - nOrder)
			{
				if (!spline.GetBSplineValue(datapoint, vData, i, nOrder,
								spline.GetSegments()))
					continue;
				vSplData.push_back(datapoint);
			}
		}
		dataID = AddCurve(vSplData);
		ostr<<nOrder<<_TEXT(" order B-Spline");
		name=ostr.str();
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;
	case kSplModeLeastSquare:
		min = vData[0].val[0];
		max = vData[0].val[0];
		for (i = 1; i < (int)vData.size(); i++)
		{
			if (vData[i].val[0] < min)
				min = vData[i].val[0];
			if (vData[i].val[0] > max)
				max = vData[i].val[0];
		}
		nSeg = (int) ceil(fabs((max - min) / fMaxStep));
		if (nSeg <= 0)
			nSeg = 1;
		spline.SetSegments(nSeg);

		for (i = 0; i <= spline.GetSegments(); i++)
		{
			datapoint.val[0] = min + i * (max - min) / (spline.GetSegments() - 1);
			spline.GetLeastSqualValue(datapoint, vData, nOrder);
			vSplData.push_back(datapoint);
		}
		dataID = AddCurve(vSplData);
		ostr<<nOrder<<_TEXT("order least square");
		name=ostr.str();
		SetDataTitle(dataID, name);
		SetDataColor(dataID, color);
		return dataID;

	default:
		return -1;
	}
}