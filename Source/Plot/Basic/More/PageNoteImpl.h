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
#include "PageNote.h"

Declare_Namespace_CChart

template<class PlotImplT>
class CPageNoteImpl : public CPageNote
{
public:
	CPageNoteImpl();
	virtual	~CPageNoteImpl();

protected:
	RECT				AdjustPlotSizeForHeadernote( HDC hDC, RECT plotRect );
	RECT				AdjustPlotSizeForFootnote( HDC hDC, RECT plotRect );

	RECT				GetHeadernoteRect( HDC hDC );
	RECT				GetFootnoteRect( HDC hDC );
	void				DrawHeadernote( HDC hDC);
	void				DrawFootnote( HDC hDC);

public:
	void				DrawPageNotes( HDC hDC );
};

template<class PlotImplT>
CPageNoteImpl<PlotImplT>::CPageNoteImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CPageNoteImpl<PlotImplT>::~CPageNoteImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
RECT	CPageNoteImpl<PlotImplT>::AdjustPlotSizeForHeadernote( HDC hDC, RECT plotRect )
{
	if( m_strHeadernote != _TEXT("") && m_bShowHeadernote )
	{
		SIZE		tmSize={0,0};
		MyGetTextExtentPoint(hDC, &m_lfHeadernoteFont, m_strHeadernote, DT_LEFT, &tmSize);
		plotRect.top += tmSize.cy;
	}
	return plotRect;
}

template<class PlotImplT>
RECT	CPageNoteImpl<PlotImplT>::AdjustPlotSizeForFootnote( HDC hDC, RECT plotRect )
{
	if( m_strFootnote != _TEXT("") && m_bShowFootnote )
	{
		SIZE		tmSize={0,0};
		MyGetTextExtentPoint(hDC, &m_lfFootnoteFont, m_strFootnote, DT_LEFT, &tmSize);
		plotRect.bottom -= tmSize.cy;
	}
	return plotRect;
}

template<class PlotImplT>
RECT	CPageNoteImpl<PlotImplT>::GetHeadernoteRect( HDC hDC )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	RECT clientRect = pT->GetLastClientRect();

	RECT	headerRect;
	SIZE	tmSize={0,0};
	if( m_strHeadernote != _TEXT("") && m_bShowHeadernote )
	{
		MyGetTextExtentPoint(hDC,&m_lfHeadernoteFont, m_strHeadernote, DT_LEFT, &tmSize);
	}
	headerRect.left = clientRect.left + pT->GetMarginLeft();
	headerRect.right = clientRect.right - pT->GetMarginRight();
	headerRect.top = clientRect.top + pT->GetMarginTop();
	headerRect.bottom = headerRect.top + tmSize.cy;
	
	return headerRect;
}

template<class PlotImplT>
RECT	CPageNoteImpl<PlotImplT>::GetFootnoteRect( HDC hDC )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	RECT clientRect = pT->GetLastClientRect();

	RECT	footnoteRect;
	SIZE	tmSize={0,0};
	if( m_strFootnote != _TEXT("") && m_bShowFootnote )
	{
		MyGetTextExtentPoint(hDC, &m_lfFootnoteFont, m_strFootnote, DT_LEFT, &tmSize);
	}
	footnoteRect.left = clientRect.left + pT->GetMarginLeft();
	footnoteRect.right = clientRect.right - pT->GetMarginRight();
	footnoteRect.bottom = clientRect.bottom - pT->GetMarginBottom();
	footnoteRect.top = footnoteRect.bottom - tmSize.cy;
	
	return footnoteRect;
}

template<class PlotImplT>
void	CPageNoteImpl<PlotImplT>::DrawHeadernote( HDC hDC )
{
	if(m_bShowHeadernote)
	{
		RECT headerRect = GetHeadernoteRect(hDC);
		if(m_strHeadernote.length()>0)
			DrawTextInRect(hDC, headerRect, m_strHeadernote, &m_lfHeadernoteFont, TRANSPARENT, m_crHeadernoteColor, m_nHeadernoteFormat);
	}
}

template<class PlotImplT>
void	CPageNoteImpl<PlotImplT>::DrawFootnote( HDC hDC )
{
	if(m_bShowFootnote)
	{
		RECT footnoteRect = GetFootnoteRect(hDC);
		if(m_strFootnote.length()>0)
			DrawTextInRect(hDC, footnoteRect, m_strFootnote, &m_lfFootnoteFont, TRANSPARENT, m_crFootnoteColor, m_nFootnoteFormat);
	}
}

template<class PlotImplT>
void	CPageNoteImpl<PlotImplT>::DrawPageNotes( HDC hDC )
{
	DrawHeadernote(hDC);
	DrawFootnote(hDC);
}

Declare_Namespace_End