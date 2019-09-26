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

#include "SplitPlot.h"

using namespace NsCChart;

CSplitPlotBase::CSplitPlotBase()
{
	SetDefaults();
}

CSplitPlotBase::~CSplitPlotBase()
{

}

void	CSplitPlotBase::SetDefaults()
{
	m_bSingleMode = false;
	m_nTopIndex = -1;
}

int		CSplitPlotBase::GetPlotCount(int mode, int nRows, int nCols)
{
	switch(mode)
	{
	case kSplitNot:
		return 1;
	case kSplitNM:
		if(nRows<1)nRows=1;
		if(nCols<1)nCols=1;
		return nRows*nCols;
	case kSplit3L1R2:
	case kSplit3L2R1:
	case kSplit3T1B2:
	case kSplit3T2B1:
		return 3;
	default:
		return 1;
	}
}

int		CSplitPlotBase::GetPlotCount()
{
	return GetPlotCount(m_nSplitMode, m_nRows, m_nCols);
}

void	CSplitPlotBase::InitSpliters(RECT plotRect)
{
	int i;
	for( i=0; i<m_nCols+1; i++)
	{
		//m_vnColSpliter[i] = plotRect.left + i * ( plotRect.right - plotRect.left )/m_nCols;
		m_vnColSpliter[i] = plotRect.left + Width(plotRect)*m_vfColRatio[i];
	}
	for( i=0; i<m_nRows+1; i++)
	{
		//m_vnRowSpliter[i] = plotRect.top + i * ( plotRect.bottom - plotRect.top )/m_nRows;
		m_vnRowSpliter[i] = plotRect.top + Height(plotRect)*m_vfRowRatio[i];
	}
}

int		CSplitPlotBase::GetPlotIndex( POINT point )
{
	int i;
	
	RECT plotRect = GetLastPlotRect();
	if(!PtInRect(&plotRect, point))return -1;
	
	int iCol=-1,iRow=-1;
	
	for( i=0; i<m_nCols; i++)
	{
		if( point.x >= m_vnColSpliter[i] && point.x < m_vnColSpliter[i+1] )
		{
			iCol = i;
			break;
		}
	}
	for( i=0; i<m_nRows; i++)
	{
		if( point.y >= m_vnRowSpliter[i] && point.y < m_vnRowSpliter[i+1] )
		{
			iRow = i;
			break;
		}
	}
	if(iCol<0 || iRow<0)
	{
		return -1;
	}
	
	int idx;
	switch(m_nSplitMode)
	{
	case kSplitNot:
		return 0;
	case kSplitNM:
		return iRow * m_nCols + iCol;
	case kSplit3L2R1://(iR,iC),(0,0)=0,(0,1)=2,(1,0)=1,(1,1)=2
		idx = iRow + 2*iCol;
		if(idx==3)idx=2;
		return idx;
	case kSplit3L1R2://(iR,iC),(0,0)=0,(0,1)=1,(1,0)=0,(1,1)=2
		idx = iRow + iCol;
		if(iRow ==1 && iCol ==0 )idx=0;
		return idx;
	case kSplit3T2B1://(iR,iC),(0,0)=0,(0,1)=1,(1,0)=2,(1,1)=2
		idx = 2*iRow + iCol;
		if(idx==3)idx=2;
		return idx;
	case kSplit3T1B2://(iR,iC),(0,0)=0,(0,1)=0,(1,0)=1,(1,1)=2
		idx = iRow + iCol;
		if(iRow ==0 && iCol ==1 )idx=0;
		return idx;
	default:
		return -1;
	}
}

bool		CSplitPlotBase::HitRowSpliter(POINT point, int &index)
{
	for(int i=1; i<m_nRows; i++)
	{
		if( abs(point.y - m_vnRowSpliter[i]) <= 2 )
		{
			index = i;
			return true;
		}
	}
	return false;
}

bool		CSplitPlotBase::HitColSpliter(POINT point, int &index)
{
	for(int i=1; i<m_nCols; i++)
	{
		if( abs(point.x - m_vnColSpliter[i]) <= 2 )
		{
			index = i;
			return true;
		}
	}
	return false;
}

void	CSplitPlotBase::CalcColOffsets(RECT plotRect)
{
	RECT rect = plotRect;
	for( int i=1; i<m_nCols; i++ )
	{
		m_vnColOffset[i] = m_vnColSpliter[i] - 
			//( rect.left + i * ( rect.right - rect.left )/m_nCols );
			(rect.left + Width(rect)*m_vfColRatio[i]);
	}
}

void	CSplitPlotBase::CalcRowOffsets(RECT plotRect)
{
	RECT rect = plotRect;
	for( int i=1; i<m_nRows; i++ )
	{
		m_vnRowOffset[i] = m_vnRowSpliter[i] - 
			//( rect.top + i * ( rect.bottom - rect.top )/m_nRows );
			(rect.top + Height(rect)*m_vfRowRatio[i]);
	}
}

void	CSplitPlotBase::UseColOffsets()
{
	for( int i=1; i<m_nCols; i++ )
	{
		m_vnColSpliter[i] += m_vnColOffset[i];
	}
}

void	CSplitPlotBase::UseRowOffsets()
{
	for( int i=1; i<m_nRows; i++ )
	{
		m_vnRowSpliter[i] += m_vnRowOffset[i];
	}
}

void	CSplitPlotBase::SetRowRatio(double ratio, int index)
{
	if(index<=0 || index>=m_nRows)return;
	if(index>0)
	{
		if(ratio<=m_vfRowRatio[index-1])return;
	}
	if(index<m_nRows)
	{
		if(ratio>=m_vfRowRatio[index+1])return;
	}
	m_vfRowRatio[index] = ratio;
}

void	CSplitPlotBase::SetColRatio(double ratio, int index)
{
	if(index<=0 || index>=m_nCols)return;
	if(index>0)
	{
		if(ratio<=m_vfColRatio[index-1])return;
	}
	if(index<m_nCols)
	{
		if(ratio>=m_vfColRatio[index+1])return;
	}
	m_vfColRatio[index] = ratio;
}
