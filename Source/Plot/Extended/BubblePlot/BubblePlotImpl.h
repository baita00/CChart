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

#include "BubblePlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class	CBubblePlotImpl : public CBubblePlot, public CXYPlotImplI
{
public:
	CBubblePlotImpl();
	virtual ~CBubblePlotImpl();

public:
	int		AddBubble(double x, double y);
	int		AddBubble(double x, double y, int which);
	void	SetBubbleSize(int size, int which);
	void	SetBubbleColor(COLORREF color, int which);
	void	SetBubbleImage(HBITMAP hImage, int which);
	void	SetBubbleImage(tstring fileName, int which);

};

typedef CBubblePlotImpl CBubblePlotImplI;

Declare_Namespace_End