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

#include <windows.h>
#include <tchar.h>

#include <vector>
using namespace std;

namespace NsCChart{

#define	PLOT_CLASS_NAME		_TEXT("PlotWnd_Defined_by_Yang_Guojun")

//////////////////////////////////////////////////////////////////////////////////////////////
// This is a templated class, but a glabal value pThis must be used. it may cause trouble
//
template<class PlotImplT>
struct stWndProcPara
{
	struct	stWndProc
	{
		HWND hWnd;
#ifndef _WIN64
		UINT		uintOldWndProc;
#else
		LONG_PTR	uintOldWndProc;
#endif
	};
	vector<stWndProc>	vWndProc;
	vector<PlotImplT*>	vpPlot;
	
	static	vector<stWndProcPara*> vpThis;
	
	static	LRESULT	CALLBACK	PlotWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int				nPlotWndID;

	int				nCurrPlotID;
};


template<class PlotImplT>
int	FindHWndByHWnd(HWND hWnd, stWndProcPara<PlotImplT> *para)
{
	int i;
	for(i=0; i<(int)para->vWndProc.size(); i++)
	{
		if(hWnd == para->vWndProc[i].hWnd)
		{
			return i;
		}
	}
	return -1;
}

template<class PlotImplT>
int	FindHWndByChart(HWND hWnd, stWndProcPara<PlotImplT> *para)
{
	int i;
	for(i=0; i<(int)para->vpPlot.size(); i++)
	{
		if(hWnd == para->vpPlot[i]->GetWnd())
		{
			return i;
		}
	}
	return -1;
}


template<class PlotImplT>
class CPlotWnd
{
public:
	CPlotWnd();
	virtual	~CPlotWnd();

private:
	stWndProcPara<PlotImplT>*	m_pWndProcPara;
	void	Release();
	stWndProcPara<PlotImplT>*	GetPara();
	
public:
	PlotImplT	*GetPlot(int nPlotIndex=0);
	PlotImplT	*SetPlot(PlotImplT *pPlot, int nPlotIndex=0);
	int		GetPlotCount();
	int		GetPlotIndex(int nPlotID);
	int		GetPlotID(int nPlotIndex);
	
	int		Attach(HWND hWnd);
	int		Attach(HWND hWnd, RECT rtClient);
	int		AttachDlg(HWND hDlg, HWND hCtrl);
	int		AttachDlg(HWND hDlg, UINT nCtrlID);

	bool	Detach();
	bool	Detach(HWND hWnd);

	void	ReDraw();
	void	ReDraw(HWND hWnd);
	void	ReDraw(int nPlotIndex);
	void	ReDraw(PlotImplT *plot);
};

//////////////////////////////////////////////////////////////////////////////////////////////
//

template<class PlotImplT>
vector<stWndProcPara<PlotImplT> *> stWndProcPara<PlotImplT>::vpThis;

template<class PlotImplT>
CPlotWnd<PlotImplT>::CPlotWnd( )
{
	static	int nPlotWndCount=0;

	m_pWndProcPara = new stWndProcPara<PlotImplT>;
	
	m_pWndProcPara->vWndProc.clear();
	m_pWndProcPara->nPlotWndID = nPlotWndCount;

	stWndProcPara<PlotImplT>::vpThis.push_back(this->m_pWndProcPara);

	nPlotWndCount++;

	m_pWndProcPara->nCurrPlotID = 0;
}

template<class PlotImplT>
CPlotWnd<PlotImplT>::~CPlotWnd()
{
	Release();
	for(int i=0; i<(int)stWndProcPara<PlotImplT>::vpThis.size(); i++)
	{
		if(m_pWndProcPara->nPlotWndID == stWndProcPara<PlotImplT>::vpThis[i]->nPlotWndID)
		{
			stWndProcPara<PlotImplT>::vpThis.erase(stWndProcPara<PlotImplT>::vpThis.begin()+i);
			break;
		}
	}
	
	delete m_pWndProcPara;
	m_pWndProcPara = 0;
}

template<class PlotImplT>
void	CPlotWnd<PlotImplT>::Release( )
{
	if(!m_pWndProcPara)return;
	while(m_pWndProcPara->vpPlot.size()>0)
	{
		delete m_pWndProcPara->vpPlot[0];
		m_pWndProcPara->vpPlot[0] = 0;
		m_pWndProcPara->vpPlot.erase(m_pWndProcPara->vpPlot.begin());
	}

	m_pWndProcPara->vWndProc.clear();
}

template<class PlotImplT>
stWndProcPara<PlotImplT>*	CPlotWnd<PlotImplT>::GetPara()
{
	return m_pWndProcPara;
}

template<class PlotImplT>
PlotImplT	*CPlotWnd<PlotImplT>::GetPlot(int nPlotIndex)
{
	if(!m_pWndProcPara)return 0;
	if(nPlotIndex<0 || nPlotIndex>=(int)m_pWndProcPara->vpPlot.size())return 0;
	
	return m_pWndProcPara->vpPlot[nPlotIndex];
}

template<class PlotImplT>
PlotImplT	*CPlotWnd<PlotImplT>::SetPlot(PlotImplT *pPlot, int nPlotIndex)
{
	if(!m_pWndProcPara || !pPlot)return 0;
	if(nPlotIndex<0 || nPlotIndex>=(int)m_pWndProcPara->vpPlot.size())return 0;
	
	PlotImplT *oldPlot = m_pWndProcPara->vpPlot[nPlotIndex];
	pPlot->SetWnd(oldPlot->GetWnd());
	m_pWndProcPara->vpPlot[nPlotIndex] = pPlot;
	return oldPlot;
}

template<class PlotImplT>
int		CPlotWnd<PlotImplT>::GetPlotCount()
{
	return (int)m_pWndProcPara->vpPlot.size();
}

template<class PlotImplT>
int		CPlotWnd<PlotImplT>::GetPlotIndex(int nPlotID)
{
	int i;
	for(i=0; i<(int)m_pWndProcPara->vpPlot.size(); i++)
	{
		if(m_pWndProcPara->vpPlot[i]->GetPlotID() == nPlotID)
			return i;
	}
	return -1;
}

template<class PlotImplT>
int		CPlotWnd<PlotImplT>::GetPlotID(int nPlotIndex)
{
	if(nPlotIndex<0 || nPlotIndex>=(int)m_pWndProcPara->vpPlot.size())return -1;
	return m_pWndProcPara->vpPlot[nPlotIndex]->GetPlotID();
}

template<class PlotImplT>
LRESULT	CALLBACK	stWndProcPara<PlotImplT>::PlotWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i, j, idx;
	stWndProcPara *pT = 0;

	PAINTSTRUCT ps;
	HDC hDC;
	
	switch(msg)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for(j=0; j<(int)stWndProcPara<PlotImplT>::vpThis.size(); j++)
		{
			if(FindHWndByHWnd<PlotImplT>(hWnd, stWndProcPara<PlotImplT>::vpThis[j])>=0 )
			{
				pT = stWndProcPara<PlotImplT>::vpThis[j];
				
				for(i=0; i<(int)pT->vpPlot.size(); i++)
				{
					if(pT->vpPlot[i]->GetWnd() != hWnd)continue;

					pT->vpPlot[i]->SetWnd(hWnd);
					if(!pT->vpPlot[i]->IsRectConfined())
					{
						pT->vpPlot[i]->OnDraw(hWnd);
					}
					else
					{
						pT->vpPlot[i]->OnDraw(hWnd, pT->vpPlot[i]->GetConfineRect());
					}
				}
			}
		}

		EndPaint(hWnd, &ps);
		if(!CGlobal::m_bContinuePaint)return 0;

		idx = FindHWndByHWnd(hWnd, pT);
		if(idx==-1)return 0;
		return CallWindowProc((WNDPROC)pT->vWndProc[idx].uintOldWndProc, hWnd, msg, wParam, lParam);
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
	case 0x020A://WM_MOUSEWHEEL:
	case WM_CONTEXTMENU:
	case WM_KEYDOWN:
		for(j=0; j<(int)stWndProcPara<PlotImplT>::vpThis.size(); j++)
		{
			if(FindHWndByHWnd(hWnd, stWndProcPara<PlotImplT>::vpThis[j])>=0)
			{
				pT = stWndProcPara<PlotImplT>::vpThis[j];
				
				for(i=0; i<(int)pT->vpPlot.size(); i++)
				{
					if(pT->vpPlot[i]->GetWnd() != hWnd)continue;
					
					if(!pT->vpPlot[i]->IsReact())continue;
					pT->vpPlot[i]->Interactive(hWnd, msg, wParam, lParam);
				}
			}
		}
		if(!CGlobal::m_bContinueReact)return 0;

		idx = FindHWndByHWnd(hWnd, pT);
		if(idx==-1)return 0;
		return CallWindowProc((WNDPROC)pT->vWndProc[idx].uintOldWndProc, hWnd, msg, wParam, lParam);
	case WM_ERASEBKGND:
		bool needErase;
		needErase = false;

		for(j=0; j<(int)stWndProcPara<PlotImplT>::vpThis.size(); j++)
		{
			if(FindHWndByHWnd(hWnd, stWndProcPara<PlotImplT>::vpThis[j])>=0)
			{
				pT = stWndProcPara<PlotImplT>::vpThis[j];
				for(i=0; i<(int)pT->vpPlot.size(); i++)
				{
					if(pT->vpPlot[i]->GetWnd() != hWnd)continue;

					if(pT->vpPlot[i]->IsRectConfined())
					{
						needErase = true;
						break;
					}
				}
				if(needErase)break;
			}
		}
		if(needErase)
		{
			idx = FindHWndByHWnd(hWnd, pT);
			if(idx==-1)return 0;
			return CallWindowProc((WNDPROC)pT->vWndProc[idx].uintOldWndProc, hWnd, msg, wParam, lParam);
		}
		else
		{
			return 0;
		}		
//	case WM_SIZE:
//		GetClientRect(hWnd, &rt);
//		InvalidateRect(hWnd, &rt,TRUE);
//		break;
	case WM_DESTROY:
		// anyone is ok 
		for(j=0; j<(int)stWndProcPara<PlotImplT>::vpThis.size(); j++)
		{
			if(FindHWndByHWnd(hWnd, stWndProcPara<PlotImplT>::vpThis[j])>=0)
			{
				pT = stWndProcPara<PlotImplT>::vpThis[j];
				break;
			}
		}
		if(!pT)return 0;
		idx = FindHWndByHWnd(hWnd, pT);
		if(idx==-1)return 0;
#ifndef _WIN64
		SetWindowLong(hWnd, GWL_WNDPROC, pT->vWndProc[idx].uintOldWndProc);
#else
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, pT->vWndProc[idx].uintOldWndProc);
#endif
		return CallWindowProc((WNDPROC)pT->vWndProc[idx].uintOldWndProc, hWnd, msg, wParam, lParam);

	default:
		for(j=0; j<(int)stWndProcPara<PlotImplT>::vpThis.size(); j++)
		{
			if(FindHWndByHWnd(hWnd, stWndProcPara<PlotImplT>::vpThis[j])>=0)
			{
				pT = stWndProcPara<PlotImplT>::vpThis[j];
				break;
			}
		}
		if(!pT)return 0;
		idx = FindHWndByHWnd(hWnd, pT);
		if(idx==-1)return 0;
		return CallWindowProc((WNDPROC)pT->vWndProc[idx].uintOldWndProc, hWnd, msg, wParam, lParam);
	}
	return 0;
}


template<class PlotImplT>
int	CPlotWnd<PlotImplT>::Attach(HWND hWnd)
{
	if(!IsWindow(hWnd))
	{
		return -1;
	}
	
	if(!m_pWndProcPara)
		return -1;

	////////////////////////////////////////////////////

	PlotImplT *plot;
	plot = new PlotImplT;
	
	int i;
	int nhWndOld = FindHWndByHWnd(hWnd, m_pWndProcPara);

	stWndProcPara<PlotImplT>::stWndProc wp;
	int nPlots = 0;
	for(i=0; i<(int)m_pWndProcPara->vpPlot.size(); i++)
	{
		if(m_pWndProcPara->vpPlot[i]->GetWnd() == hWnd)
			nPlots++;
	}

	if(nPlots == 0 || nhWndOld == -1)
	{
		// the following code ensure that the unitOldWndProc
		// is correct for any hwnd and cchartwnd
		int old = -1;
		for(i=0; i<(int)stWndProcPara<PlotImplT>::vpThis.size(); i++)
		{
			if(FindHWndByHWnd(hWnd, stWndProcPara<PlotImplT>::vpThis[i])==i && stWndProcPara<PlotImplT>::vpThis[i] != m_pWndProcPara)
			{
				old = i;
				break;
			}
		}
		
		wp.hWnd = hWnd;
		if(old<0)
		{
#ifndef _WIN64	
			wp.uintOldWndProc = SetWindowLong(hWnd, GWL_WNDPROC, (long)m_pWndProcPara->PlotWindowProc);
#else
			wp.uintOldWndProc = SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)m_pWndProcPara->PlotWindowProc);
#endif
		}
		else
		{
			for(i=0; i<(int)stWndProcPara<PlotImplT>::vpThis[old]->vWndProc.size(); i++)
			{
				if(hWnd == stWndProcPara<PlotImplT>::vpThis[old]->vWndProc[i].hWnd)
				{
					wp.uintOldWndProc = stWndProcPara<PlotImplT>::vpThis[old]->vWndProc[i].uintOldWndProc;
					break;
				}
			}
		}
		m_pWndProcPara->vWndProc.push_back(wp);
	}
	
	plot->SetPlotID(m_pWndProcPara->nCurrPlotID);
	plot->SetWnd(hWnd);
	m_pWndProcPara->vpPlot.push_back(plot);

	m_pWndProcPara->nCurrPlotID++;
	
	return (m_pWndProcPara->nCurrPlotID - 1);
}

template<class PlotImplT>
int		CPlotWnd<PlotImplT>::Attach(HWND hWnd, RECT rtClient)
{
	int chartID = Attach(hWnd);
	if(chartID>=0)
	{
		m_pWndProcPara->vpPlot[m_pWndProcPara->vpPlot.size()-1]->SetRectConfined(true);
		m_pWndProcPara->vpPlot[m_pWndProcPara->vpPlot.size()-1]->SetConfineRect(rtClient);
		return chartID;
	}
	else
	{
		return -1;
	}
}

template<class PlotImplT>
int		CPlotWnd<PlotImplT>::AttachDlg(HWND hDlg, HWND hCtrl)
{
	if(!IsWindow(hCtrl))
		return false;
	
	RECT rtWCtrl, rtWParent;
	GetWindowRect(hCtrl, &rtWCtrl);
	GetWindowRect(hDlg, &rtWParent);
	
	POINT point = {rtWCtrl.left, rtWCtrl.top};
	ScreenToClient(hDlg, &point);
	
    RECT rtCtrlClient;  
    GetClientRect(hCtrl, &rtCtrlClient);  
    OffsetRect(&rtCtrlClient, point.x, point.y);  
	
    int ret = Attach(hDlg, rtCtrlClient); 
	if(ret)
	{
		GetPlot((int)m_pWndProcPara->vpPlot.size()-1)->SetBkgndColor(GetSysColor(COLOR_3DFACE));
		GetPlot((int)m_pWndProcPara->vpPlot.size()-1)->SetEdgeShow(true);
	}
	return ret;
}

template<class PlotImplT>
int		CPlotWnd<PlotImplT>::AttachDlg(HWND hDlg, UINT nCtrlID)
{
	HWND hCtrl = GetDlgItem(hDlg, nCtrlID);
	if(!hCtrl)
		return -1;
	return AttachDlg(hDlg, hCtrl);
}

template<class PlotImplT>
bool	CPlotWnd<PlotImplT>::Detach()
{
	int i;
	for(i=0; i<(int)m_pWndProcPara->vWndProc.size(); i++)
	{
		if(!IsWindow(m_pWndProcPara->vWndProc[i].hWnd))continue;
		
#ifndef _WIN64
		SetWindowLong(m_pWndProcPara->vWndProc[i].hWnd, GWL_WNDPROC, m_pWndProcPara->vWndProc[i].uintOldWndProc);
#else
		SetWindowLongPtr(m_pWndProcPara->vWndProc[i].hWnd, GWLP_WNDPROC, m_pWndProcPara->vWndProc[i].uintOldWndProc);
#endif
		
	}
	m_pWndProcPara->vWndProc.clear();
	
	for(i=(int)m_pWndProcPara->vpPlot.size()-1; i>=0; i--)
	{
		delete m_pWndProcPara->vpPlot[i];
		m_pWndProcPara->vpPlot[i]=0;
		m_pWndProcPara->vpPlot.erase(m_pWndProcPara->vpPlot.begin()+i);
		
	}
	
	return true;
}

template<class PlotImplT>
bool	CPlotWnd<PlotImplT>::Detach(HWND hWnd)
{
	if(FindHWndByHWnd(hWnd, m_pWndProcPara)==-1)return false;
	
	if(!IsWindow(hWnd))return true;
	//	if(!m_pPara->uintOldWndProc)return false;		
	
	int i;
	for(i=(int)m_pWndProcPara->vWndProc.size()-1; i>=0; i--)
	{
		if(hWnd == m_pWndProcPara->vWndProc[i].hWnd)
		{
#ifndef _WIN64
			SetWindowLong(hWnd, GWL_WNDPROC, m_pWndProcPara->vWndProc[i].uintOldWndProc);
#else
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, m_pWndProcPara->vWndProc[i].uintOldWndProc);
#endif
			m_pWndProcPara->vWndProc.erase(m_pWndProcPara->vWndProc.begin()+i);
		}
	}
	for(i=(int)m_pWndProcPara->vpPlot.size()-1; i>=0; i--)
	{
		if(hWnd == m_pWndProcPara->vpPlot[i]->GetWnd())
		{
			delete m_pWndProcPara->vpPlot[i];
			m_pWndProcPara->vpPlot.erase(m_pWndProcPara->vpPlot.begin()+i);
		}
	}
	return true;
}

template<class PlotImplT>
void	CPlotWnd<PlotImplT>::ReDraw()
{
	int i; 
	for(i=0; i<(int)m_pWndProcPara->vpPlot.size(); i++)
	{
		ReDraw(i);
	}
}

template<class PlotImplT>
void	CPlotWnd<PlotImplT>::ReDraw(HWND hWnd)
{
	int i;
	for(i=0; i<(int)m_pWndProcPara->vpPlot.size(); i++)
	{
		if(m_pWndProcPara->vpPlot[i]->GetWnd() == hWnd)
		{
			m_pWndProcPara->vpPlot[i]->OnDraw(hWnd);
		}
	}
}

template<class PlotImplT>
void	CPlotWnd<PlotImplT>::ReDraw(int nPlotIndex)
{
	if(nPlotIndex<0 || nPlotIndex>=(int)m_pWndProcPara->vpPlot.size())return;
	
	m_pWndProcPara->vpPlot[nPlotIndex]->OnDraw(m_pWndProcPara->vpPlot[nPlotIndex]->GetWnd());
}

template<class PlotImplT>
void	CPlotWnd<PlotImplT>::ReDraw(PlotImplT *plot)
{
	if(!plot)return;
	int i;
	for(i=0; i<(int)m_pWndProcPara->vpPlot.size(); i++)
	{
		if(plot == m_pWndProcPara->vpPlot[i])
		{
			plot->OnDraw(plot->GetWnd());
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//
}