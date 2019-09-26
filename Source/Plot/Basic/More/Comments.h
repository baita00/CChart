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

#include "CommentImpl.h"
#include <vector>

Declare_Namespace_CChart

class	CComments
{
public:
	CComments();
	virtual	~CComments();
protected:
	void	SetDefaults();

	void	DeleteComments();

protected:
	std::vector<CCommentImpl *>	m_vpComment;
	int		m_nCurCommentID;

public:
	int		GetCommentIndex(int commentID);
	int		GetCommentID(int index);

	int		FindCommentIDByFlag(int flag);
	CCommentImpl*	GetCommentByFlag(int flag);

public:
	void				UnsetCommentsDragMode();
	int					GetCommentDragModeID();
	inline bool			IsCommentDragMode( int commentID ) { int i = GetCommentIndex(commentID); if( i < 0 ) return 0; else return m_vpComment[i]->IsDragMode(); }
	inline void			SetCommentDragMode( int commentID, bool drag ) { int i = GetCommentIndex(commentID); if( i < 0 ) return; else m_vpComment[i]->SetDragMode(drag); }

public:
	inline	int				GetCommentsCount( )const{ return (int)m_vpComment.size(); }
	inline	CCommentImpl	*GetAbsComment( int which ) { if( which >= 0 && which < (int)m_vpComment.size() ) return m_vpComment[which]; else return NULL; }
	inline	CCommentImpl	*GetComment(int commentID) {int index = GetCommentIndex(commentID); if(index>=0)return m_vpComment[index]; else return 0; }
	inline	std::vector<CCommentImpl *>	&GetComments(){return m_vpComment;}
	inline	SIZE			&GetCommentOffset(int commentID){int index = GetCommentIndex(commentID); if(index>=0)return m_vpComment[index]->GetOffset(); else {static SIZE zs = {0, 0}; return zs; } }

};

Declare_Namespace_End
