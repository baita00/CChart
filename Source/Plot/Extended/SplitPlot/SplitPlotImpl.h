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

#include "../../Basic/XYHandler.h"
#include "../../Basic/XYPlotImpl.h"
#include "../../Basic/DragStatus.h"
#include "../../Basic/MyMemDC.h"
#include "SplitPlot.h"
#include "SplitHandler.h"
#include "SplitReactStatus.h"

Declare_Namespace_CChart

// This templated class
template <typename PlotImplT>
class CSplitPlotImpl :	public CSplitPlot<PlotImplT>,
						public CPlotImpl
{
public:
	CSplitPlotImpl();
	virtual ~CSplitPlotImpl();

	virtual	void		FinalDraw( HDC hDC, RECT destRect );

	virtual TCHAR*		GetTypeName(){return _TEXT("SplitPlot");}

public:
	//int		GetPlotIndex( POINT point );

	RECT	GetSubPlotRect(RECT clientRect,int nIndex);

	// CPlotImpl is a pure virtual class, so the following functions should be defined
	CAxisImpl	*NewAxis(){ return 0; }
	bool	IsEmpty(){ return CSplitPlot<PlotImplT>::GetPlotDataCount()<=0; }
	void	GetDataRange1D( int whichDim, double *range ){}
	bool	IsNoDataPlot(){return false;}

	//add the following function since m_rctLastClientRect have inherited twice
	RECT			GetLastClientRect(){return CPlotImpl::GetLastClientRect();}
	void			SetLastClientRect(RECT rect){CPlotImpl::SetLastClientRect(rect);}
	RECT			GetLastPlotRect(){return CPlotImpl::GetLastPlotRect();}
	void			SetLastPlotRect(RECT rect){CPlotImpl::SetLastPlotRect(rect);}
public:
	int				AddCurve( double *pX, double *pY, int nLen, int nIndex );
	int				AddCurve( tstring *pStrTime, double *pY, int nLen, int nIndex );

public:
	bool	IsLegendShow(){return false;}//dummy function

};

template <typename PlotImplT>
class CSplitPlotImplI : public CSplitPlotImpl<PlotImplT>,
						public CSplitHandler<CSplitPlotImplI<PlotImplT> >,
						public CSplitReactStatus<CSplitPlotImplI<PlotImplT> >,
						public CDragStatus<CSplitPlotImplI<PlotImplT> >
{
public:
	CSplitPlotImplI();
	virtual	~CSplitPlotImplI();
	virtual	tstring	GetDescriptionByMousePos(HDC hDC, POINT point);

	bool	IsOscilloScopeMode(){return false;}
  
public:
//	POINT	m_ptOrigin,m_ptCurr;
//	int		m_nPositionOrigin;
	int		m_nSpliterIndex;
//	bool	m_bDragRowSpliter, m_bDragColSpliter;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Implimentations of CSplitPlotImpl

template <typename PlotImplT>
CSplitPlotImpl<PlotImplT>::CSplitPlotImpl()
{
	m_bDragRowSpliter = false;
	m_bDragColSpliter = false;

	m_nMarginLeft = 0;
	m_nMarginRight = 0;
	m_nMarginBottom = 0;
	m_nMarginTop = 0;
	m_nMinMargin = 2;

	DeleteAllAxes();
}

template <typename PlotImplT>
CSplitPlotImpl<PlotImplT>::~CSplitPlotImpl()
{
}

template <class XYPlotImplT>
int		CSplitPlotImpl<XYPlotImplT>::AddCurve( double *pX, double *pY, int nLen, int nIndex )
{
	if(nIndex<0 || nIndex>=GetPlotCount(m_nSplitMode, m_nRows, m_nCols))
		return -1;
	return m_vpPlots[nIndex]->AddCurve(pX, pY, nLen);
}

template <class XYPlotImplT>
int		CSplitPlotImpl<XYPlotImplT>::AddCurve( tstring *pStrTime, double *pY, int nLen, int nIndex )
{
	if(nIndex<0 || nIndex>=GetPlotCount(m_nSplitMode, m_nRows, m_nCols))
		return -1;
	m_vpPlots[nIndex]->SetXToTime();
	return m_vpPlots[nIndex]->AddCurve(pStrTime, pY, nLen);
}

template <typename PlotImplT>
void	CSplitPlotImpl<PlotImplT>::FinalDraw(HDC hDC, RECT destRect)
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

	for(int i=0; i<GetPlotCount(m_nSplitMode, m_nRows, m_nCols); i++)
	{
		if(m_vpPlots[i])
		{
			COLORREF bkCr;
			if(m_vpPlots[i]->IsPlotSelected())
			{
				bool bSet=false;
				for(int j=0; j<GetPlotCount(m_nSplitMode, m_nRows, m_nCols); j++)
				{
					if(!m_vpPlots[j]->IsPlotSelected())
					{
						bkCr = m_vpPlots[j]->GetBkgndColor();
						bSet=true;
						break;
					}
				}
				if(!bSet)bkCr = m_vpPlots[i]->GetBkgndColor();
				
				unsigned char r, g, b;
				r = GetRValue(bkCr)>=10?GetRValue(bkCr)-10:GetRValue(bkCr)+10;
				g = GetGValue(bkCr)>=10?GetGValue(bkCr)-10:GetGValue(bkCr)+10;
				b = GetBValue(bkCr)>=10?GetBValue(bkCr)-10:GetBValue(bkCr)+10;
				m_vpPlots[i]->SetBkgndColor(RGB(r, g, b));
			}
			m_vpPlots[i]->FinalDraw(hdc, GetSubPlotRect(plotRect,i) );
			if(m_vpPlots[i]->IsPlotSelected())
			{
				m_vpPlots[i]->SetBkgndColor(bkCr);
			}
		}
	}

//	SetMapMode(hDC, nOldMapmode);
}

template <typename PlotImplT>
RECT	CSplitPlotImpl<PlotImplT>::GetSubPlotRect(RECT clientRect,int nIndex)
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

//////////////////////////////////////////////////////////////////////////////////////////////////
//

template <typename PlotImplT>
CSplitPlotImplI<PlotImplT>::CSplitPlotImplI()
{

}

template <typename PlotImplT>
CSplitPlotImplI<PlotImplT>::~CSplitPlotImplI()
{
	
}

template <typename PlotImplT>
tstring	CSplitPlotImplI<PlotImplT>::GetDescriptionByMousePos(HDC hDC, POINT point)
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

///////////////////////////////////////////////////////////////////////////////////////////
//

HMENU	CreateContextMenu(CSplitPlotImplI<CXYPlotImpl> *plot);
bool DefPlotSettings( CSplitPlotImplI<CXYPlotImpl> *plot, HDC hDC );
bool DefAxisSettings( CSplitPlotImplI<CXYPlotImpl> *plot, HDC hDC, int location );
bool DefDataSettings( CSplitPlotImplI<CXYPlotImpl> *plot, HDC hDC, int whichDataSet );

HMENU	CreateContextMenu(CSplitPlotImplI<CXYPlotImplI> *plot);
bool DefPlotSettings( CSplitPlotImplI<CXYPlotImplI> *plot, HDC hDC );
bool DefAxisSettings( CSplitPlotImplI<CXYPlotImplI> *plot, HDC hDC, int location );
bool DefDataSettings( CSplitPlotImplI<CXYPlotImplI> *plot, HDC hDC, int whichDataSet );

class CShareXPlotImplI;
HMENU	CreateContextMenu(CSplitPlotImplI<CShareXPlotImplI> *plot);
bool DefPlotSettings( CSplitPlotImplI<CShareXPlotImplI> *plot, HDC hDC );
bool DefAxisSettings( CSplitPlotImplI<CShareXPlotImplI> *plot, HDC hDC, int location );
bool DefDataSettings( CSplitPlotImplI<CShareXPlotImplI> *plot, HDC hDC, int whichDataSet );


Declare_Namespace_End
