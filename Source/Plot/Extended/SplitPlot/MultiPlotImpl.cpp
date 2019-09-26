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

#include "MultiPlotImpl.h"

using namespace NsCChart;

CMultiPlotImpl::CMultiPlotImpl()
{
	m_bDragRowSpliter = false;
	m_bDragColSpliter = false;
}

CMultiPlotImpl::~CMultiPlotImpl()
{

}

void	CMultiPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	if(m_bSingleMode && m_nTopIndex>=0 && m_nTopIndex<GetPlotCount())
	{
		bool db = GetSubPlot(m_nTopIndex)->IsDoubleBuffer();
		GetSubPlot(m_nTopIndex)->SetDoubleBuffer(true);
		GetSubPlot(m_nTopIndex)->FinalDraw(hDC, destRect);
		GetSubPlot(m_nTopIndex)->SetDoubleBuffer(db);
		
		GetSubPlot(m_nTopIndex)->SetLastClientRect(destRect);
		SetLastClientRect(destRect);
		
		return;
	}

	RECT plotRect = GetPlotRect(hDC, destRect);
	
	SetLastClientRect(destRect);
	SetLastPlotRect(plotRect);
	
	InitSpliters(plotRect);
	UseColOffsets();
	UseRowOffsets();
	
	HDC hdc;
	CMyMemDC memdc;
	if(!m_bDoubleBuffer)
	{
		hdc = hDC;
	}
	else
	{
		memdc->CreateMemDC(hDC, NULL, &destRect);
		hdc = memdc;
	}

	EraseRect(hdc, destRect, m_crBkgndColor);
	
	if(m_bTitleShow)
	{
		DrawTitle( hdc );
	}
	
	int i, count = GetPlotCount(m_nSplitMode, m_nRows, m_nCols);
	for(i=0; i<count; i++)
	{
		RECT plotRect = GetSubPlotRect(destRect,i);
		if(m_vpPlots[i])
		{
			m_vpPlots[i]->FinalDraw(hdc, plotRect);
		}
		else
		{
			HBRUSH hBrush, hOldBrush;
			hBrush = CreateSolidBrush(GetBkgndColor());
			hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			FillRect(hDC, &plotRect, hBrush);
			SelectObject(hdc, hOldBrush);
			DeleteObject(hBrush);
		}
	}
}

RECT	CMultiPlotImpl::GetSubPlotRect(RECT clientRect,int nIndex)
{
	int iRow,iCol;
	RECT rect={0,0,0,0};

	switch( m_nSplitMode )
	{
	case kSplitNot:
		rect = clientRect;
		break;
	case kSplitNM:
		iRow = nIndex / m_nCols;
		iCol = nIndex % m_nCols;

		rect.top = m_vnRowSpliter[iRow];
		rect.bottom = m_vnRowSpliter[iRow+1];
		rect.left = m_vnColSpliter[iCol];
		rect.right = m_vnColSpliter[iCol+1];

		break;
	case kSplit3L2R1:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[1],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[1],m_vnColSpliter[1],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	case kSplit3L1R2:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[1],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[1],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	case kSplit3T2B1:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[1],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[1],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	case kSplit3T1B2:
		switch(nIndex)
		{
		case 0:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[0],m_vnColSpliter[2],m_vnRowSpliter[1]};
				rect = rt;
			}
			break;
		case 1:
			{
				RECT rt={m_vnColSpliter[0],m_vnRowSpliter[1],m_vnColSpliter[1],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		default:
			{
				RECT rt={m_vnColSpliter[1],m_vnRowSpliter[1],m_vnColSpliter[2],m_vnRowSpliter[2]};
				rect = rt;
			}
			break;
		}
		break;
	default:
		rect = clientRect;
		break;
	}
	return rect;
}

tstring	CMultiPlotImplI::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	RECT titleRect = GetTitleRect(hDC);
	
	if(PtInRect(&titleRect, point))
	{
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Main title region");
		case kLangChinese:
			return _TEXT("总标题区域");
		}
	}
/*	else
	{
		int idx = GetPlotIndex( point );
		if( idx < 0 )
		{
			return _TEXT("");
		}
		else
		{
			return GetSubPlot(idx)->GetDescriptionByMousePos(hDC, point);
		}
	}*/
	return _TEXT("");
}

/*
tstring	CMultiPlotImpl::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	RECT titleRect = GetTitleRect(hDC);
	
	if(PtInRect(&titleRect, point))
	{
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			return _TEXT("Main title region");
		case kLangChinese:
			return _TEXT("总标题区域");
		}
	}
	else
	{
		int idx = GetPlotIndex( point );
		if( idx < 0 )
		{
			return _TEXT("");
		}
		else
		{
			return GetSubPlot(idx)->GetDescriptionByMousePos(hDC, point);
		}
	}
	return _TEXT("");
}

void	CMultiPlotImpl::SwitchSelection()
{
	if(!IsTitleRealyShow() && GetPlotsCount()<=0) return;
	if(IsSelectTitle())
	{
		if(GetPlotsCount()<=0)return;
		SetSelectTitle(false);
		if(!m_bSwitchReverse)
			GetPlot(0)->SetPlotSelected(true);
		else
			GetPlot(GetPlotsCount()-1)->SetPlotSelected(true);
	}
	else if(IsSubPlotSelected())
	{
		int i, index = GetIndexOfSelectedSubPlot();
		if(!m_bSwitchReverse)
			i = index+1;
		else
			i = index-1;
		
		GetPlot(index)->SetPlotSelected(false);
		if(i>=0 && i<GetPlotsCount())
		{
			GetPlot(i)->SetPlotSelected(true);
		}
		else
		{
			if(IsTitleRealyShow())
				SetSelectTitle(true);
		}
	}
	else
	{
		if(IsTitleRealyShow())
			SetSelectTitle(true);
	}
}

void	CMultiPlotImpl::UnlightAll()
{
	if(IsTitleRealyShow())
		SetLightTitle(false);
	for(int i=0; i<GetPlotsCount(); i++)
	{
		GetPlot(i)->SetPlotLighted(false);
	}
}

void	CMultiPlotImpl::UnselectAll()
{
	if(IsTitleRealyShow())
		SetSelectTitle(false);
	for(int i=0; i<GetPlotsCount(); i++)
	{
		GetPlot(i)->SetPlotSelected(false);
	}
}
*/