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
#include "Comments.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CCommentsImpl : public CComments
{
public:
	CCommentsImpl();
	virtual	~CCommentsImpl();

public:
	int					AddComment(tstring comment, int x, int y, LOGFONT logFont, COLORREF color, bool bBorderShow, int nBorderSize, int nBorderStyle, COLORREF crBorder, int pos);
	int					AddComment(tstring comment, int x, int y);
	int					AddComment(tstring comment);
	int					AddComment(tstring comment, int flag);
	
	int					AddPictureComment(HBITMAP hBitmap);
	int					AddPictureComment(HBITMAP hBitmap, int x, int y);
	int					AddPictureComment(HBITMAP hBitmap, int x, int y, int pos);

public:
	void				DrawComments(HDC hDC);

public:
	int					GetCommentIdByPoint( HDC hDC, POINT point );

public:
	int					FindSubtitle();
	int					AddSubtitle(tstring subtitle);
};

template<class PlotImplT>
CCommentsImpl<PlotImplT>::CCommentsImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CCommentsImpl<PlotImplT>::~CCommentsImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
int	CCommentsImpl<PlotImplT>::AddComment(tstring comment, int x, int y, LOGFONT logFont, COLORREF color, bool bBorderShow, int nBorderSize, int nBorderStyle, COLORREF crBorder, int pos)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	CCommentImpl *pc = new CCommentImpl(pT);
	pc->SetStrComment(comment);
	pc->SetPosX(x);
	pc->SetPosY(y);
	pc->SetCommentFont(logFont);
	pc->SetCommentColor(color);
	pc->SetBorderShow(bBorderShow);
	pc->SetBorderSize(nBorderSize);
	pc->SetBorderStyle(nBorderStyle);
	pc->SetBorderColor(crBorder);
	pc->SetPosition(pos);
	SIZE zsize = {0, 0};
	pc->SetOffset(zsize);
	pc->SetExcludeTitle(true);
	pc->SetCommentFormat(DT_LEFT | DT_TOP);
	
	pc->SetTextComment(true);
	pc->SetCommentID(m_nCurCommentID);
	m_vpComment.push_back(pc);

	m_nCurCommentID++;

	return m_nCurCommentID-1;
}

template<class PlotImplT>
int	CCommentsImpl<PlotImplT>::AddComment(tstring comment, int x, int y)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	LOGFONT logFont;
	HFONT	hFont=CreateFont(36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, _TEXT("Arial") );
	GetObject(hFont,sizeof(LOGFONT),&logFont);
	DeleteObject(hFont);
	logFont.lfHeight = 16;

	COLORREF crText, crBorder;
#ifndef	DEFAULT_WHITE_BACKGROUND
	// Default is black background
	crText	= RGB( 255, 255, 255 );
#else
	crText	= RGB( 0, 0, 0 );
#endif
	crBorder = crText;
	return	AddComment(comment, x, y, logFont, crText, true, 1, PS_SOLID, crBorder, kCommentRightTopIn);
}

template<class PlotImplT>
int	CCommentsImpl<PlotImplT>::AddComment(tstring comment)
{
	return	AddComment(comment, 0, 0);
}

template<class PlotImplT>
int	CCommentsImpl<PlotImplT>::AddComment(tstring comment, int flag)
{
	if(flag == 0)
	{
		return	AddComment(comment, 0, 0);
	}
	int commentID = FindCommentIDByFlag(flag);
	if(commentID<0)
	{
		commentID = AddComment(comment, 0, 0);
		GetComment(commentID)->SetCommentFlag(flag);
	}
	else
	{
		if(!GetComment(commentID)->IsTextComment())return -1;
		GetComment(commentID)->SetStrComment(comment);
	}
	return commentID;
}

template<class PlotImplT>
int	CCommentsImpl<PlotImplT>::AddPictureComment(HBITMAP hBitmap, int x, int y, int pos)
{
	if(!hBitmap)return -1;
//	if(!IsHandle(hBitmap))return -1;
	
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	CCommentImpl *pc = new CCommentImpl(pT);
	pc->SetCommentBitmap(hBitmap);
	pc->SetPosX(x);
	pc->SetPosY(y);
	pc->SetPosition(pos);
	SIZE zsize = {0, 0};
	pc->SetOffset(zsize);
	pc->SetExcludeTitle(false);
	pc->SetCommentRop(SRCCOPY);
	
	pc->SetTextComment(false);
	pc->SetCommentID(m_nCurCommentID);
	m_vpComment.push_back(pc);
	
	m_nCurCommentID++;

	return m_nCurCommentID-1;
}

template<class PlotImplT>
int	CCommentsImpl<PlotImplT>::AddPictureComment(HBITMAP hBitmap, int x, int y)
{
	return	AddPictureComment(hBitmap, x, y, kCommentDataRegion);
}

template<class PlotImplT>
int	CCommentsImpl<PlotImplT>::AddPictureComment(HBITMAP hBitmap)
{
	return	AddPictureComment(hBitmap, 0, 0);
}

template<class PlotImplT>
void	CCommentsImpl<PlotImplT>::DrawComments(HDC hDC)
{
	unsigned int i;
	for(i=0; i<m_vpComment.size(); i++)
	{
		m_vpComment[i]->OnDraw(hDC);
	}
}

template<class PlotImplT>
int		CCommentsImpl<PlotImplT>::GetCommentIdByPoint( HDC hDC, POINT point )
{
	RECT rect;
	unsigned int i;
	for(i=0; i<m_vpComment.size(); i++)
	{
		rect = m_vpComment[i]->GetCommentRect(hDC);
		if(PtInRect(&rect, point))
		{
			return m_vpComment[i]->GetCommentID();
		}
	}
	return -1;
}

template<class PlotImplT>
int		CCommentsImpl<PlotImplT>::FindSubtitle()
{
	int i, commentID = -1;;
	for(i=0; i<(int)m_vpComment.size(); i++)
	{
		if(!m_vpComment[i]->IsTextComment())continue;
		if(m_vpComment[i]->IsSubtitle())
		{
			commentID = m_vpComment[i]->GetCommentID();
			break;
		}
	}
	return commentID;
}

template<class PlotImplT>
int		CCommentsImpl<PlotImplT>::AddSubtitle(tstring subtitle)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentid;
	commentid = FindSubtitle();
	if(commentid<0)
	{
		commentid = AddComment(subtitle);
	}
	CCommentImpl* pCmm = GetComment(commentid);
	if(!pCmm)return -1;
	pCmm->SetPosition(kCommentCenterHigher);
	
	pCmm->SetCommentFont(pT->GetTitleFont());
	pCmm->GetCommentFont().lfHeight *= 0.75;
	
	pCmm->SetCommentColor(RGB(128, 0, 0));
	pCmm->SetBorderShow(false);
	
	pCmm->SetSubtitle(true);
	return commentid;
}

Declare_Namespace_End
