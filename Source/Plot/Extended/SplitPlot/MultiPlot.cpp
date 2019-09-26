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

#include "MultiPlot.h"

using namespace NsCChart;

CMultiPlot::CMultiPlot()
{
	m_nSplitMode = kSplitNot;
	m_nRows = 1;
	m_nCols = 1;
	
//	SetRectEmpty(&m_rctLastClientRect);
	
	ResizePlots( m_nSplitMode, m_nRows, m_nCols );
}

CMultiPlot::~CMultiPlot()
{
	DeleteAll();
}

void	CMultiPlot::SetDefaults()
{
	ResizePlots( kSplitNot, 1, 1 );
}

CPlotImpl*	CMultiPlot::GetSubPlot(int nIndex)
{
	if(nIndex<0 || nIndex>=GetPlotCount())return NULL;
	return m_vpPlots[nIndex];
}

CHandler*	CMultiPlot::GetHandler(int nIndex)
{
	if(nIndex<0 || nIndex>=GetPlotCount())return NULL;
	return m_vpHandlers[nIndex];
}

CReactStatus *CMultiPlot::GetReactStatus(int nIndex)
{
	if(nIndex<0 || nIndex>=GetPlotCount())return NULL;
	return m_vpReactStatus[nIndex];
}

bool	CMultiPlot::IsSubPlotSelected()
{
	int i;
	for(i=0; i<GetPlotCount(); i++)
	{
		if(GetSubPlot(i))
			if(GetSubPlot(i)->IsPlotSelected())return true;
	}
	return false;
}

int		CMultiPlot::GetIndexOfSelectedSubPlot()
{
	int i;
	for(i=0; i<GetPlotCount(); i++)
	{
		if(GetSubPlot(i))
			if(GetSubPlot(i)->IsPlotSelected())return i;
	}
	return -1;
}

void	CMultiPlot::DeleteAll()
{
	if(m_vpPlots.size()>0)
	{
		for(int i=(int)m_vpPlots.size()-1; i>=0; i--)
		{
			if(m_vpPlots[i])
			{
				delete m_vpPlots[i];
				m_vpPlots[i] = NULL;
			}
			m_vpPlots.pop_back();
		}
		m_vpHandlers.clear();
		m_vpReactStatus.clear();
	}
}

void	CMultiPlot::ResizePlots(int mode, int nRows, int nCols)
{
	int i;
	
	if(nRows<=0)nRows=1;
	if(nCols<=0)nCols=1;
	
	DeleteAll();
	
	int count = GetPlotCount(mode, nRows, nCols);
	m_vpPlots.resize(count);
	m_vpHandlers.resize(count);
	m_vpReactStatus.resize(count);
	for(i=0; i<count; i++)
	{
		m_vpPlots[i] = 0;
		m_vpHandlers[i] = 0;
		m_vpReactStatus[i] = 0;
	}

	m_nSplitMode = mode;
	switch(mode)
	{
	case kSplitNot:
		m_nRows =1;
		m_nCols =1;
		break;
	case kSplitNM:
		m_nRows = nRows;
		m_nCols = nCols;
		break;
	case kSplit3L1R2:
	case kSplit3L2R1:
	case kSplit3T1B2:
	case kSplit3T2B1:
		m_nRows = 2;
		m_nCols = 2;
		break;
	default:
		m_nRows = 1;
		m_nCols = 1;
		break;
	}
	m_vnRowSpliter.resize(m_nRows+1);
	m_vnColSpliter.resize(m_nCols+1);
	m_vnRowOffset.resize(m_nRows+1);
	m_vnColOffset.resize(m_nCols+1);
	m_vfColRatio.resize(m_nCols+1);
	m_vfRowRatio.resize(m_nRows+1);
	for( i=0; i<m_nCols+1; i++)
	{
		m_vnColOffset[i] = 0;
		m_vfColRatio[i] = i/(double)m_nCols;
	}
	for( i=0; i<m_nRows+1; i++)
	{
		m_vnRowOffset[i] = 0;
		m_vfRowRatio[i] = i/(double)m_nRows;
	}
}

bool	CMultiPlot::SetSubPlot(CPlotImpl *pSubPlot, CHandler *pHandler, CReactStatus *pReactStatus, int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpPlots.size())
		return false;
	if(!pSubPlot)
		return false;

	m_vpPlots[nIndex] = pSubPlot;
	m_vpPlots[nIndex]->AddNormalAxis();
	m_vpPlots[nIndex]->SetMargin(2, 2, 2, 2);
	m_vpPlots[nIndex]->SetEdgeShow(true);
	m_vpPlots[nIndex]->SetDoubleBuffer(false);

	m_vpHandlers[nIndex] = pHandler;
	m_vpReactStatus[nIndex] = pReactStatus;

	return true;
}
