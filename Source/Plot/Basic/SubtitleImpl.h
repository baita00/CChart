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

#include "CChartNameSpace.h"
#include "MyString.h"
#include <windows.h>

Declare_Namespace_CChart

template <class PlotImplT>
class CSubtitleImpl					
{
public:
	CSubtitleImpl();
	virtual ~CSubtitleImpl();

public:
	//Subtitle 
	tstring		GetSubTitle();
	void		SetSubTitle(tstring title);
	int			GetSubTitlePosition();
	void		SetSubTitlePosition(int position);
	bool		IsSubTitleShow();
	void		SetSubTitleShow(bool show);
	COLORREF	GetSubTitleColor();
	void		SetSubTitleColor(COLORREF color);
	LOGFONT&	GetSubTitleFont();
	void		SetSubTitleFont(LOGFONT logFont);
};

template<class PlotImplT>
CSubtitleImpl<PlotImplT>::CSubtitleImpl()
{

}

template<class PlotImplT>
CSubtitleImpl<PlotImplT>::~CSubtitleImpl()
{
	
}

template<class PlotImplT>
tstring		CSubtitleImpl<PlotImplT>::GetSubTitle()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)return pT->GetComment(commentID)->GetStrComment();
	else return _TEXT("");
}

template<class PlotImplT>
void		CSubtitleImpl<PlotImplT>::SetSubTitle(tstring title)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)pT->GetComment(commentID)->SetStrComment(title);
	else pT->AddSubtitle(title);
}

template<class PlotImplT>
int			CSubtitleImpl<PlotImplT>::GetSubTitlePosition()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)return pT->GetComment(commentID)->GetPosition();
	else return 0;
}

template<class PlotImplT>
void		CSubtitleImpl<PlotImplT>::SetSubTitlePosition(int position)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)pT->GetComment(commentID)->SetPosition(position);
}

template<class PlotImplT>
bool		CSubtitleImpl<PlotImplT>::IsSubTitleShow()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)return pT->GetComment(commentID)->IsCommentShow();
	else return false;
}

template<class PlotImplT>
void		CSubtitleImpl<PlotImplT>::SetSubTitleShow(bool show)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)pT->GetComment(commentID)->SetCommentShow(show);
}

template<class PlotImplT>
COLORREF	CSubtitleImpl<PlotImplT>::GetSubTitleColor()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)return pT->GetComment(commentID)->GetCommentColor();
	else return RGB(0, 0, 0);
}

template<class PlotImplT>
void		CSubtitleImpl<PlotImplT>::SetSubTitleColor(COLORREF color)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)pT->GetComment(commentID)->SetCommentColor(color);
}

template<class PlotImplT>
LOGFONT&	CSubtitleImpl<PlotImplT>::GetSubTitleFont()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)return pT->GetComment(commentID)->GetCommentFont();
	else
	{
		static LOGFONT logFont={0};
		HFONT hfont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hfont!=NULL)
		{
			::GetObject(hfont, sizeof(LOGFONT), &logFont);
			::DeleteObject(hfont);
		}
		return logFont;
	}
}

template<class PlotImplT>
void		CSubtitleImpl<PlotImplT>::SetSubTitleFont(LOGFONT logFont)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	int commentID = pT->FindSubtitle();
	if(commentID>=0)pT->GetComment(commentID)->SetCommentFont(logFont);
}

Declare_Namespace_End