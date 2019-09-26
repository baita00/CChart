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

#include "PlotData.h"

Declare_Namespace_CChart

void	ClearAdjoinCoincide(MyVData2D& vData)
{
	if (vData.size() <= 1)
		return;
	for (int i = (int)vData.size() - 1; i >= 1; i--)
	{
		if (vData[i].val[0] == vData[i - 1].val[0] && vData[i].val[1] == vData[i - 1].val[1])
			vData.erase(vData.begin() + i);
	}
}

// The following function is to instance the templated functions
// when there is a templated function, it muat be instanced

//下面两个函数需要在VC6中保留，不然会出现链接错误：未定义的外部符号
//在高版本中需要删去，不然会出现编译错误：非法使用显式模板参数
#if defined(_MSC_VER) && (_MSC_VER<=1200)
	bool vLess<DataPoint2D>(const DataPoint2D &d1, const DataPoint2D &d2)
	{
		return d1.val[nWhichDimCmp] < d2.val[nWhichDimCmp];
	}
	bool vGreat<DataPoint2D>(const DataPoint2D &d1, const DataPoint2D &d2)
	{
		return d1.val[nWhichDimCmp] > d2.val[nWhichDimCmp];
	}
#endif

void	InstanceTemplatedDataFunctions()
{
/*	DataPoint2D d1, d2;
	memset(&d1, 0, sizeof(DataPoint2D));
	memset(&d2, 0, sizeof(DataPoint2D));
	Less<DataPoint2D, 0>(d1, d2);
	Less<DataPoint2D, 1>(d1, d2);
	Great<DataPoint2D, 0>(d1, d2);
	Great<DataPoint2D, 1>(d1, d2);*/
	DataPoint2D d1, d2;
	memset(&d1, 0, sizeof(DataPoint2D));
	memset(&d2, 0, sizeof(DataPoint2D));
	bool bret;
	nWhichDimCmp = 0;
	bret = vLess<DataPoint2D>(d1, d2);
	bret = vGreat<DataPoint2D>(d1, d2);
	nWhichDimCmp = 1;
	bret = vLess<DataPoint2D>(d1, d2);
	bret = vGreat<DataPoint2D>(d1, d2);
}

Declare_Namespace_End