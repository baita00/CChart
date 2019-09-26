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

#include "Comments.h"

using namespace NsCChart;

CComments::CComments()
{
	SetDefaults();
}

CComments::~CComments()
{
	DeleteComments();
}

void	CComments::SetDefaults()
{
	m_nCurCommentID = 0;
}

void	CComments::DeleteComments()
{
	int i;
	for(i=(int)m_vpComment.size()-1; i>=0; i--)
	{
		if(m_vpComment[i])delete m_vpComment[i];
		m_vpComment.erase(m_vpComment.begin()+i);
	}
}

int		CComments::GetCommentIndex(int commentID)
{
	int i;
	for(i=0; i<(int)m_vpComment.size(); i++)
	{
		if(m_vpComment[i]->GetCommentID() == commentID)return i;
	}
	return -1;
}

int		CComments::GetCommentID(int index)
{
	if(index>=0 && index<(int)m_vpComment.size())
	{
		return m_vpComment[index]->GetCommentID();
	}
	else
	{
		return -1;
	}
}

int		CComments::FindCommentIDByFlag(int flag)
{
	int i;
	for(i=0; i<(int)m_vpComment.size(); i++)
	{
		if(m_vpComment[i]->GetCommentFlag() == flag)
			return GetCommentID(i);
	}
	return -1;
}

CCommentImpl*	CComments::GetCommentByFlag(int flag)
{
	int commentID = FindCommentIDByFlag(flag);
	if(commentID<0)return 0;
	else return GetComment(commentID);
}

void		CComments::UnsetCommentsDragMode()
{
	unsigned int i;
	for(i=0; i<m_vpComment.size(); i++)
	{
		m_vpComment[i]->SetDragMode(false);
	}
}

int			CComments::GetCommentDragModeID()
{
	unsigned int i;
	for(i=0; i<m_vpComment.size(); i++)
	{
		if(m_vpComment[i]->IsDragMode())
		{
			return m_vpComment[i]->GetCommentID();
		}
	}
	return -1;
}
