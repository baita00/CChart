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
#include "Comment.h"

Declare_Namespace_CChart

class CPlotImpl;

class	CCommentImpl : public CComment
{
public:
	CCommentImpl(CPlotImpl	*plot);
	virtual	~CCommentImpl();
protected:
	CPlotImpl	*m_pPlot;
	
public:
	void		SetPlot(CPlotImpl *pPlot){m_pPlot = pPlot;}
	CPlotImpl	*GetPlot(){return m_pPlot;}

	void		OnDraw( HDC hDC );

	SIZE		GetCommentSize(HDC hDC);
	RECT		GetCommentRect(HDC hDC);

protected:
	void		DrawTextComments( HDC hDC );
	void		DrawPictureComments( HDC hDC );

protected:
	SIZE		GetTextCommentSize(HDC hDC);
	SIZE		GetPictureCommentSize();
	RECT		GetTextCommentRect(HDC hDC);
	RECT		GetPictureCommentRect(HDC hDC);
	void		RecalcCommentRect(int nPosition, SIZE sizeOffset, RECT &commentRect);
	POINT		GetInitCommentPoint(HDC hDC, int nPosition, int nPosX, int nPosY, bool bExcludeTitle);
};

Declare_Namespace_End
