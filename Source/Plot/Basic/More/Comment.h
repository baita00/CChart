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
#include "../MyString.h"
#include "../CChartNameSpace.h"

enum
{
	kCommentDataRegion,
	kCommentClientRect,
	
	kCommentLeftOut,
	kCommentRightOut,
	kCommentTopOut,
	kCommentBottomOut,
	kCommentTopLeftOut,
	kCommentLeftTopOut,
	kCommentBottomLeftOut,
	kCommentLeftBottomOut,
	kCommentTopRightOut,
	kCommentRightTopOut,
	kCommentBottomRightOut,
	kCommentRightBottomOut,
	
	kCommentLeftIn,
	kCommentRightIn,
	kCommentTopIn,
	kCommentBottomIn,
	kCommentTopLeftIn,
	kCommentLeftTopIn,
	kCommentBottomLeftIn,
	kCommentLeftBottomIn,
	kCommentTopRightIn,
	kCommentRightTopIn,
	kCommentBottomRightIn,
	kCommentRightBottomIn,
	
	kCommentLeftHighIn,
	kCommentLeftHigherIn,
	kCommentLeftLowIn,
	kCommentLeftLowerIn,
	kCommentRightHighIn,
	kCommentRightHigherIn,
	kCommentRightLowIn,
	kCommentRightLowerIn,
	
	kCommentCenter,
	kCommentCenterHigh,
	kCommentCenterHigher,
	kCommentCenterLow,
	kCommentCenterLower,
	
	kCommentPositionCount
};

Declare_Namespace_CChart

class	CComment
{
public:
	CComment();
	virtual ~CComment();

protected:
	void		SetDefaults();

	void		DeleteUsedHandles();

protected:
	bool		m_bTextComment;
	int			m_nCommentID;

protected:
	bool		m_bCommentShow;
	tstring		m_strComment;
	LOGFONT		m_lfCommentFont;
	COLORREF	m_crCommentColor;
	bool		m_bTransparent;
	COLORREF	m_crCommentBkColor;
	
	bool		m_bBorderShow;
	int			m_nBorderSize;
	int			m_nBorderStyle;
	COLORREF	m_crBorderColor;
	
	int			m_nFormat;

protected:
	HBITMAP		m_hBitmap;
	int			m_nRop;

protected:
	int			m_nPosX;
	int			m_nPosY;
	
	int			m_nPosition;
	SIZE		m_sizeOffset;
	bool		m_bExcludeTitle;

protected:
	bool		m_bDragMode;
	bool		m_bDragEnable;

protected:
	//bool		m_bSubtitle;
	int			m_nCommentFlag;

public:
	inline	bool		IsTextComment(){return m_bTextComment;}
	inline	void		SetTextComment(bool bT){m_bTextComment = bT;}

	inline	int			GetCommentID(){return m_nCommentID;}
	inline	void		SetCommentID(int id){m_nCommentID = id;}

	inline	bool		IsCommentShow(){return m_bCommentShow;}
	inline	void		SetCommentShow(bool bShow){m_bCommentShow = bShow;}

	inline	tstring		GetStrComment(){return m_strComment;}
	inline	void		SetStrComment(tstring comment){m_strComment = comment;}
	inline	LOGFONT		&GetCommentFont(){return m_lfCommentFont;}
	inline	void		SetCommentFont(LOGFONT logfont){m_lfCommentFont = logfont;}
	inline	COLORREF	GetCommentColor(){return m_crCommentColor;}
	inline	void		SetCommentColor(COLORREF cr){m_crCommentColor = cr;}
	inline	bool		IsTransparent(){return m_bTransparent;}
	inline	void		SetTransparent(bool trans){m_bTransparent = trans;}
	inline	COLORREF	GetCommentBkColor(){return m_crCommentBkColor;}
	inline	void		SetCommentBkColor(COLORREF cr){m_crCommentBkColor = cr;}

	inline	bool		IsBorderShow(){return m_bBorderShow;}
	inline	void		SetBorderShow(bool show){m_bBorderShow = show;}
	inline	int			GetBorderSize(){return m_nBorderSize;}
	inline	void		SetBorderSize(int size){m_nBorderSize = size;}
	inline	int			GetBorderStyle(){return m_nBorderStyle;}
	inline	void		SetBorderStyle(int style){m_nBorderStyle = style;}
	inline	COLORREF	GetBorderColor(){return m_crBorderColor;}
	inline	void		SetBorderColor(COLORREF cr){m_crBorderColor = cr;}

	inline	int			GetCommentFormat(){return m_nFormat;}
	inline	void		SetCommentFormat(int format){m_nFormat = format;}

	inline	HBITMAP		&GetCommentBitmap(){return m_hBitmap;}
	inline	void		SetCommentBitmap(HBITMAP bm){if(m_hBitmap)DeleteObject(m_hBitmap); m_hBitmap=bm;}
	inline	int			GetCommentRop(){return m_nRop;}
	inline	void		SetCommentRop(int rop){m_nRop = rop;}

	inline	int			GetPosX(){return m_nPosX;}
	inline	void		SetPosX(int pos){m_nPosX = pos;}
	inline	int			GetPosY(){return m_nPosY;}
	inline	void		SetPosY(int pos){m_nPosY = pos;}

	inline	int			GetPosition(){return m_nPosition;}
	inline	void		SetPosition(int pos){m_nPosition = pos;}
	inline	SIZE		&GetOffset(){return m_sizeOffset;}
	inline	void		SetOffset(SIZE offset){m_sizeOffset = offset;}
	inline	void		SetOffsetX(int offset){m_sizeOffset.cx = offset;}
	inline	void		SetOffsetY(int offset){m_sizeOffset.cy = offset;}
	inline	bool		IsExcludeTitle(){return m_bExcludeTitle;}
	inline	void		SetExcludeTitle(bool et){m_bExcludeTitle = et;}

	inline	bool		IsDragMode(){return m_bDragMode;}
	inline	void		SetDragMode(bool drag){m_bDragMode = drag; }
	inline	bool		IsDragEnable(){return m_bDragEnable;}
	inline	void		SetDragEnable(bool drag){m_bDragEnable = drag; }

	inline	bool		IsSubtitle(){return m_nCommentFlag == 1;}
	inline	void		SetSubtitle(bool bSt){m_nCommentFlag = bSt?1:0;}

	inline	int			GetCommentFlag(){return m_nCommentFlag;}
	inline	void		SetCommentFlag(int flag){m_nCommentFlag = flag;}

	inline	bool		IsConfined(){return (m_nPosition == kCommentDataRegion || m_nPosition >=kCommentLeftIn); }
};

Declare_Namespace_End