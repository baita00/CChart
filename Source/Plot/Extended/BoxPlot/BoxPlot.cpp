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

#include "BoxPlot.h"
#include "../../Basic/PlotUtility.h"

using namespace NsCChart;

bool NsCChart::BoxNodeLess(const BoxNode *d1, const BoxNode *d2)
{
	return d1->fXVal < d2->fXVal;
}

bool NsCChart::BoxNodeGreat(const BoxNode *d1, const BoxNode *d2)
{
	return d1->fXVal > d2->fXVal;
}

CBoxPlot::CBoxPlot()
{
	SetDefaults();
	m_nHighlightedNode = -1;
}

CBoxPlot::~CBoxPlot()
{
	DeleteAll();
}

void	CBoxPlot::SetDefaults()
{
	m_bHorizontal = false;
	m_bNewBoxNodeComming = false;

	m_bSortBox = true;
}

void	CBoxPlot::DeleteAll()
{
	int i;
	for(i=(int)m_vpBoxNodes.size()-1; i>=0; i--)
	{
		delete m_vpBoxNodes[i];
		m_vpBoxNodes.pop_back();
	}
}

void	CBoxPlot::InitBoxNode(pBoxNode pNode)
{
	if(!pNode)return;

	pNode->fXVal = 0.0;
	pNode->fMid = 0.0;
	pNode->fQ1 = 0.0;
	pNode->fQ3 = 0.0;
	pNode->fLowerEdge = 0.0;
	pNode->fUpperEdge = 0.0;

	pNode->vVals.clear();

	pNode->nWidth = 30;
	pNode->bFill = true;
	pNode->crFill = RGB(200, 32, 32);
	pNode->crLine = RGB(200, 32, 32);
	pNode->crCtLine = RGB(128, 128, 128);
	pNode->nLineSize = 1;
	pNode->nCtLineSize = 1;
	pNode->bEndLine = false;
}

void	CBoxPlot::SortBoxNode(pBoxNode pNode)
{
	if(!pNode)return;
	sort(pNode->vVals.begin(), pNode->vVals.end());
	if(pNode->fQ1>pNode->fQ3)swap(pNode->fQ1, pNode->fQ3);
	if(pNode->fLowerEdge>pNode->fUpperEdge)swap(pNode->fLowerEdge, pNode->fUpperEdge);
}


void	CBoxPlot::GetBoxRange(double &vmn, double &vmx, int nIndex)
{
	if(!CheckBoxIndex(nIndex))return;

	pBoxNode pnode = m_vpBoxNodes[nIndex];

	SortBoxNode(pnode);

	vmn = fMaxVal;
	vmx = -fMaxVal;

	if(pnode->fQ1<vmn)vmn=pnode->fQ1;
	if(pnode->fLowerEdge<vmn)vmn=pnode->fLowerEdge;
	if(pnode->vVals.size()>0)
	{
		if(pnode->vVals[0]<vmn)vmn=pnode->vVals[0];
	}

	if(pnode->fQ3>vmx)vmx=pnode->fQ3;
	if(pnode->fUpperEdge>vmx)vmx=pnode->fUpperEdge;
	if(pnode->vVals.size()>0)
	{
		if(pnode->vVals[pnode->vVals.size()-1]>vmx)vmx=pnode->vVals[pnode->vVals.size()];
	}
}
