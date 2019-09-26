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
#include <windows.h>

#include "../CChartNameSpace.h"

Declare_Namespace_CChart

// range drag mode
enum
{
	kRangeDragX,
	kRangeDragY,
	kRangeDragXY,
	
	kRangeDragCount
};

class CRangeManage
{
public:
	CRangeManage();
	virtual ~CRangeManage();
protected:
	void	SetDefaults();

protected:
	bool				m_bRangeDrag;
	bool				m_bRangeDragging;
	int					m_nRangeDragType;

	bool				m_bRangeZoomMode;
	bool				m_bRangeZoomming;

public:
	double				m_fXRange[2], m_fYRange[2];

public:
	inline	bool		IsRangeZoomMode(){return m_bRangeZoomMode;}
	inline	void		SetRangeZoomMode(bool zoom){m_bRangeZoomMode = zoom;}
	inline	bool		IsRangeZoomming(){return m_bRangeZoomming;}
	inline	void		SetRangeZoomming(bool zoom){m_bRangeZoomming = zoom; if(zoom){m_bRangeDrag = false;}}
	
	inline	bool		IsRangeDrag(){return m_bRangeDrag;}
	inline	void		SetRangeDrag(bool drag){m_bRangeDrag = drag; if(drag){m_bRangeZoomming = false;}}
	inline	bool		IsRangeDragging(){return m_bRangeDragging;}
	inline	void		SetRangeDragging(bool drag){m_bRangeDragging = drag;}
	inline	int			GetRangeDragType(){return m_nRangeDragType;}
	inline	void		SetRangeDragType(int mode){m_nRangeDragType = mode; }
};

Declare_Namespace_End