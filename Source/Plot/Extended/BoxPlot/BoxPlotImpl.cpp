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

#include "BoxPlotImpl.h"
using namespace NsCChart;

CBoxPlotImpl::CBoxPlotImpl()
{

}

CBoxPlotImpl::~CBoxPlotImpl()
{
	
}

void	CBoxPlotImpl::GetPlotRange( double *xRange, double *yRange )
{
	if(!IsNewBoxNodeComming())
	{
		GetRange(xRange, yRange);
	}
	else
	{
		CXYPlotBasicImpl::GetPlotRange(xRange, yRange);	
	}
}

void		CBoxPlotImpl::GetDataRange1D( int whichDim, double *range )
{
	bool bX = (whichDim==0)?true:false;

	if(m_bHorizontal)bX = !bX;

	if(bX)
	{
		range[0] = -1;
		range[1] = GetBoxNodeCount();
	}
	else
	{
		int i;
		range[0]=fMaxVal;
		range[1]=-fMaxVal;
		double mn, mx;
		for(i=0; i<GetBoxNodeCount(); i++)
		{
			GetBoxRange(mn, mx, i);
			if(mn<range[0])range[0]=mn;
			if(mx>range[1])range[1]=mx;
		}
	}
}

int		CBoxPlotImpl::AddBoxNode(BoxNode node)
{
	MyLock();
	
	BoxNode *pnode = new BoxNode;
	*pnode = node;
	m_vpBoxNodes.push_back(pnode);
	if(m_bSortBox)std::sort(m_vpBoxNodes.begin(), m_vpBoxNodes.end(), BoxNodeLess);
	SetNewBoxNodeComming(true);
	
	MyUnlock();
	
	return (int)m_vpBoxNodes.size();
}

int		CBoxPlotImpl::AddBoxNode(double fXVal, double fQ1, double fQ3, double fLower, double fUpper)
{
	BoxNode node;
	InitBoxNode(&node);

	node.fXVal = fXVal;
	node.fQ1 = fQ1;
	node.fQ3 = fQ3;
	node.fLowerEdge = fLower;
	node.fUpperEdge = fUpper;
	node.fMid = (fQ1+fQ3)/2.0;

	return AddBoxNode(node);
}

long	CBoxPlotImpl::GetBoxNodeXCoord(int nIndex, RECT plotRect, double *xRange)
{
	if(!CheckBoxIndex(nIndex))return 0;

	if(!m_bHorizontal)
		return plotRect.left+(nIndex-xRange[0])*Width(plotRect)/MyRange(xRange);
	else
		return plotRect.bottom - (nIndex-xRange[0])*Height(plotRect)/MyRange(xRange);
}

long	CBoxPlotImpl::GetBoxNodeYCoord(double fYVal, RECT plotRect, double *yRange)
{
	if(!m_bHorizontal)
		return plotRect.bottom - (fYVal - yRange[0])*Height(plotRect)/MyRange(yRange);
	else
		return plotRect.left + (fYVal - yRange[0])*Width(plotRect)/MyRange(yRange);
}

RECT	CBoxPlotImpl::GetBoxNodeRect( int nIndex, RECT plotRect, double *xRange, double *yRange, bool bFull )
{
	RECT BoxNodeRect = {0, 0, 0, 0};
	
	if(!CheckBoxIndex(nIndex))return BoxNodeRect;
	
	long xC;
	double yQ1, yQ3, yLower, yUpper;

	if(!m_bHorizontal)
	{
		xC = GetBoxNodeXCoord(nIndex, plotRect, xRange);
		yQ1 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ1, plotRect, yRange);
		yQ3 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ3, plotRect, yRange);
		yLower = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fLowerEdge, plotRect, yRange);
		yUpper = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fUpperEdge, plotRect, yRange);
		
		BoxNodeRect.left = xC - GetBoxWidth(nIndex)/2;
		BoxNodeRect.right = xC + GetBoxWidth(nIndex)/2;
		if(!bFull)
		{
			BoxNodeRect.top = yQ3;
			BoxNodeRect.bottom = yQ1;
		}
		else
		{
			BoxNodeRect.top = yUpper;
			BoxNodeRect.bottom = yLower;
		}
	}
	else
	{
		xC = GetBoxNodeXCoord(nIndex, plotRect, yRange);
		yQ1 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ1, plotRect, xRange);
		yQ3 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ3, plotRect, xRange);
		yLower = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fLowerEdge, plotRect, xRange);
		yUpper = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fUpperEdge, plotRect, xRange);
		
		BoxNodeRect.top = xC - GetBoxWidth(nIndex)/2;
		BoxNodeRect.bottom = xC + GetBoxWidth(nIndex)/2;
		if(!bFull)
		{
			BoxNodeRect.right = yQ3;
			BoxNodeRect.left = yQ1;
		}
		else
		{
			BoxNodeRect.right = yUpper;
			BoxNodeRect.left = yLower;
		}
	}
	
	return BoxNodeRect;
}

void	CBoxPlotImpl::DrawPlotDataEach(HDC hDC, double *xRange, double *yRange)
{
	int i;
	RECT plotRect = GetLastPlotRect();

	for(i=0; i<GetBoxNodeCount(); i++)
	{
		DrawSingleBoxNode(i, hDC, plotRect, xRange, yRange);
	}
}

void	CBoxPlotImpl::DrawSingleBoxNode(int nIndex, HDC hDC, RECT plotRect, double *xRange, double *yRange)
{
	if(!CheckBoxIndex(nIndex))return;

	long width;
	width = GetBoxWidth(nIndex);
	while(GetBoxNodeCount()*width>Width(plotRect)*0.9)
	{
		if(width<6)break;
		width--;
	}

	long xC;
	double yQ1, yQ3, yLower, yUpper;

	if(!m_bHorizontal)
	{
		xC = GetBoxNodeXCoord(nIndex, plotRect, xRange);
		yQ1 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ1, plotRect, yRange);
		yQ3 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ3, plotRect, yRange);
		yLower = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fLowerEdge, plotRect, yRange);
		yUpper = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fUpperEdge, plotRect, yRange);
		
		if(yLower<yUpper || yLower<max(yQ1, yQ3) || yUpper>min(yQ1, yQ3))return;//Coordinate is reversed
	}
	else
	{
		xC = GetBoxNodeXCoord(nIndex, plotRect, yRange);
		yQ1 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ1, plotRect, xRange);
		yQ3 = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fQ3, plotRect, xRange);
		yLower = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fLowerEdge, plotRect, xRange);
		yUpper = GetBoxNodeYCoord(m_vpBoxNodes[nIndex]->fUpperEdge, plotRect, xRange);
		
		if(yLower>yUpper || yLower>min(yQ1, yQ3) || yUpper<max(yQ1, yQ3))return;
	}
	

	int lineSize = GetBoxLineSize(nIndex);
	if(m_nHighlightedNode == nIndex)lineSize+=2;

	HPEN hOldPen, hPen;
	hPen = CreatePen(PS_SOLID, lineSize, GetBoxLineColor(nIndex));
	HBRUSH hOldBrush, hBrush;
	hBrush = CreateSolidBrush(GetBoxFillColor(nIndex));

	hOldPen=(HPEN)SelectObject(hDC, hPen);
	if(IsBoxFill(nIndex))hOldBrush=(HBRUSH)SelectObject(hDC, hBrush);

	if(!m_bHorizontal)
	{
		//rectangle
		if(yQ1==yQ3)
		{
			MoveToEx(hDC, xC-width/2, yQ1, 0);
			LineTo(hDC, xC+width/2, yQ3);
		}
		else
		{
			Rectangle(hDC,xC-width/2, yQ3, xC+width/2, yQ1);
		}
		
		//center line
		MoveToEx(hDC, xC, yLower, 0);
		LineTo(hDC, xC, yUpper);
		
		if(IsBoxEndLine(nIndex))
		{
			if(yLower!=max(yQ1, yQ3))
			{
				MoveToEx(hDC, xC-width/2, yLower, 0);
				LineTo(hDC, xC+width/2, yLower);
			}
			if(yUpper!=min(yQ1, yQ3))
			{
				MoveToEx(hDC, xC-width/2, yUpper, 0);
				LineTo(hDC, xC+width/2, yUpper);
			}
		}
	}
	else
	{
		//rectangle
		if(yQ1==yQ3)
		{
			MoveToEx(hDC, yQ1, xC-width/2, 0);
			LineTo(hDC, yQ3, xC+width/2);
		}
		else
		{
			Rectangle(hDC, yQ3, xC-width/2, yQ1, xC+width/2);
		}
		
		//center line
		MoveToEx(hDC, yLower, xC, 0);
		LineTo(hDC, yUpper, xC);
		
		if(IsBoxEndLine(nIndex))
		{
			if(yLower!=min(yQ1, yQ3))
			{
				MoveToEx(hDC, yLower, xC-width/2, 0);
				LineTo(hDC, yLower, xC+width/2);
			}
			if(yUpper!=max(yQ1, yQ3))
			{
				MoveToEx(hDC, yUpper, xC-width/2, 0);
				LineTo(hDC, yUpper, xC+width/2);
			}
		}
	}
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	if(IsBoxFill(nIndex))SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}

int		CBoxPlotImpl::GetBoxNodeIndexByPoint(POINT point)
{
	RECT plotRect = GetLastPlotRect(), BoxNodeRect;
	if(!PtInRect(&plotRect, point))return -1;
	
	double xRange[2], yRange[2];
	GetRange(xRange, yRange);
	
	int i;
	for(i=0; i<(int)m_vpBoxNodes.size(); i++)
	{
		BoxNodeRect = GetBoxNodeRect(i, plotRect, xRange, yRange, true);
		if(PtInRect(&BoxNodeRect, point))return i;
	}
	
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

CBoxPlotImplI::CBoxPlotImplI()
{
	
}

CBoxPlotImplI::~CBoxPlotImplI()
{
	
}
