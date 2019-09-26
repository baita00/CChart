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
#pragma warning(disable:4786)

#include "../../Basic/CChartNameSpace.h"
#include <windows.h>
#include <vector>
using namespace std;
Declare_Namespace_CChart

typedef struct stBoxNode
{
	double fXVal;

	double fMid, fQ1, fQ3;
	double fLowerEdge, fUpperEdge;
	vector<double> vVals;

	int	nWidth;
	bool bFill;
	COLORREF crFill, crLine, crCtLine;
	int	nLineSize, nCtLineSize;
	bool bEndLine;
}BoxNode, *pBoxNode;

bool BoxNodeLess(const BoxNode *d1, const BoxNode *d2);
bool BoxNodeGreat(const BoxNode *d1, const BoxNode *d2);

class CBoxPlot
{
public:
	CBoxPlot();
	virtual	~CBoxPlot();

	void	SetDefaults();

protected:
	vector<pBoxNode>	m_vpBoxNodes;
	bool	m_bHorizontal;

	void	DeleteAll();

	void	InitBoxNode(pBoxNode pNode);
	void	SortBoxNode(pBoxNode pNode);
	void	GetBoxRange(double &vmn, double &vmx, int nIndex);
	

protected:
	bool		m_bNewBoxNodeComming;
	bool		m_bSortBox;

public:
	int			m_nHighlightedNode;

public:
	inline	int		GetBoxNodeCount() { return (int)m_vpBoxNodes.size(); }
	inline	bool	CheckBoxIndex(int nIndex) { return nIndex>=0 && nIndex<(int)m_vpBoxNodes.size();}
	inline	bool	IsBoxHorizontal(){ return m_bHorizontal; }
	inline	void	SetBoxHorizontal(bool horiz){ m_bHorizontal = horiz; }

	inline	bool	IsNewBoxNodeComming(){ return m_bNewBoxNodeComming; }
	inline	void	SetNewBoxNodeComming(bool comming){ m_bNewBoxNodeComming = comming; }
	inline	bool	IsSortBox(){ return m_bSortBox; }
	inline	void	SetSortBox(bool bSort){ m_bSortBox = bSort; }

public:
	inline	pBoxNode	GetBoxNode(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex];}
	inline	double		GetBoxXVal(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->fXVal;}
	inline	void		SetBoxXVal(double fVal, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->fXVal=fVal;}
	inline	double		GetMidVal(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->fMid;}
	inline	void		SetMidVal(double fVal, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->fMid=fVal;}
	inline	double		GetQ1Val(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->fQ1;}
	inline	void		SetQ1Val(double fVal, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->fQ1=fVal;}
	inline	double		GetQ3Val(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->fQ3;}
	inline	void		SetQ3Val(double fVal, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->fQ3=fVal;}
	inline	double		GetLowerEdge(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->fLowerEdge;}
	inline	void		SetLowerEdge(double fVal, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->fLowerEdge=fVal;}
	inline	double		GetUpperEdge(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->fUpperEdge;}
	inline	void		SetUpperEdge(double fVal, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->fUpperEdge=fVal;}
	inline	int			GetBoxWidth(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->nWidth;}
	inline	void		SetBoxWidth(int nWidth, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->nWidth=nWidth;}
	inline	bool		IsBoxFill(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->bFill;}
	inline	void		SetBoxFill(bool bFill, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->bFill=bFill;}
	inline	COLORREF	GetBoxFillColor(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->crFill;}
	inline	void		SetBoxFillColor(COLORREF cr, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->crFill=cr;}
	inline	COLORREF	GetBoxLineColor(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->crLine;}
	inline	void		SetBoxLineColor(COLORREF cr, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->crLine=cr;}
	inline	COLORREF	GetBoxCtLineColor(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->crCtLine;}
	inline	void		SetBoxCtLineColor(COLORREF cr, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->crCtLine=cr;}
	inline	int			GetBoxLineSize(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->nLineSize;}
	inline	void		SetBoxLineSize(int nSize, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->nLineSize=nSize;}
	inline	int			GetBoxCtLineSize(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->nCtLineSize;}
	inline	void		SetBoxCtLineSize(int nSize, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->nCtLineSize=nSize;}
	inline	bool		IsBoxEndLine(int nIndex){if(!CheckBoxIndex(nIndex))return 0; return m_vpBoxNodes[nIndex]->bEndLine;}
	inline	void		SetBoxEndLine(bool bShow, int nIndex){if(!CheckBoxIndex(nIndex))return; m_vpBoxNodes[nIndex]->bEndLine=bShow;}

	inline	vector<double> &GetVals(int nIndex){if(!CheckBoxIndex(nIndex)){static vector<double> tmp; return tmp;};return m_vpBoxNodes[nIndex]->vVals;}
};

Declare_Namespace_End