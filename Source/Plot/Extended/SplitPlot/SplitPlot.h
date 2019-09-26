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

// This is a templated class 

#ifndef __SPLITPLOT_H_122333444455555__
#define __SPLITPLOT_H_122333444455555__

#pragma warning(disable:4786)

#include <windows.h>
#include <vector>
using namespace std;

#include "../../Basic/XYHandler.h"
#include "../../Basic/PlotData.h"
#include "../../Basic/ReactStatus.h"
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

// enumutation of split modes
enum
{
	kSplitNot=0,
	kSplitNM=1,
	kSplit3L1R2=2,
	kSplit3L2R1=3,
	kSplit3T1B2=4,
	kSplit3T2B1=5,

	kSplitModeCount
};

class CSplitPlotBase
{
public:
	CSplitPlotBase();
	virtual	~CSplitPlotBase();

	void			SetDefaults();

public:
	int				m_nSplitMode;
	int				m_nRows;
	int				m_nCols;

	vector<int>		m_vnRowSpliter, m_vnColSpliter;
	vector<int>		m_vnRowOffset, m_vnColOffset;
	vector<double>	m_vfRowRatio, m_vfColRatio;

	bool	m_bDragRowSpliter, m_bDragColSpliter;
	
	bool			m_bDragMode;

protected:
	bool	m_bSingleMode;
	int		m_nTopIndex;

public:
	inline	bool	IsDragMode() { return m_bDragMode; }
	inline	void	SetDragMode( bool drag ) { m_bDragMode = drag; }
	inline	int		GetSplitMode(){ return m_nSplitMode; }
	inline	int		GetRows(){ return m_nRows; }
	inline	int		GetCols(){ return m_nCols; }

	inline	bool	IsSingleMode(){return m_bSingleMode;}
	inline	void	SetSingleMode(bool mode){m_bSingleMode = mode;}
	inline	int		GetTop(){return m_nTopIndex;}
	inline	void	SetTop(int top){m_nTopIndex = top;}

	inline	bool	IsReallySingle(){return m_bSingleMode && m_nTopIndex>=0 && m_nTopIndex<GetPlotCount();}

protected:
	void			InitSpliters(RECT plotRect);
public:
	int				GetPlotIndex( POINT point );

public:
	int				GetPlotCount(int mode, int nRows, int nCols);
	int				GetPlotCount();
	int				GetSubPlotCount(){return GetPlotCount();}

protected:
//	RECT			m_rctLastClientRect;

public:
//	inline RECT		GetLastClientRect(){return m_rctLastClientRect;}
//	inline void		SetLastClientRect(RECT rt){m_rctLastClientRect=rt;}
	virtual	RECT	GetLastClientRect()=0;
	virtual	void	SetLastClientRect(RECT rect)=0;
	virtual	RECT	GetLastPlotRect()=0;
	virtual	void	SetLastPlotRect(RECT rect)=0;

public:
	bool			HitRowSpliter(POINT point, int &index);
	bool			HitColSpliter(POINT point, int &index);
	void			CalcColOffsets(RECT plotRect);
	void			CalcRowOffsets(RECT plotRect);
	void			UseColOffsets();
	void			UseRowOffsets();

public:
	void			SetRowRatio(double ratio, int index);
	void			SetColRatio(double ratio, int index);
};

// This is a templated class
template <class PlotImplIT>
class CSplitPlot : public CSplitPlotBase
{
public:
	CSplitPlot();
	virtual ~CSplitPlot();

	void			SetDefaults();
	void			CopySettings(CSplitPlot *plot);

public:
	vector<PlotImplIT *>		m_vpPlots;
	vector<CHandler *>	m_vpHandlers;
	vector<CReactStatus *> m_vpReactStatus;
	
public:
	int				GetPlotDataCount();
//	PlotImplIT		*GetPlot(int nIndex);
	PlotImplIT		*GetSubPlot(int nIndex);//{return GetPlot(nIndex);}
	CHandler		*GetHandler(int nIndex);
	CReactStatus	*GetReactStatus(int nIndex);

public:
	bool			IsSubPlotSelected();
	int				GetIndexOfSelectedSubPlot();

public:
	void			DeleteAll();
	void			ResizePlots(int mode, int nRows, int nCols);

public:
	virtual	CPlotData<DataPoint2D>	*GetDataSetByTitle( tstring title );
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Implementations of CSplitPlot

template <class PlotImplIT>
CSplitPlot<PlotImplIT>::CSplitPlot()
{

	m_nSplitMode = kSplitNot;
	m_nRows = 1;
	m_nCols = 1;

	//SetRectEmpty(&m_rctLastClientRect);

	ResizePlots( m_nSplitMode, m_nRows, m_nCols );
}

template <class PlotImplIT>
CSplitPlot<PlotImplIT>::~CSplitPlot()
{
	DeleteAll();
}

template <class PlotImplIT>
void	CSplitPlot<PlotImplIT>::SetDefaults()
{
	ResizePlots( kSplitNot, 1, 1 );
}

template <class PlotImplIT>
void	CSplitPlot<PlotImplIT>::CopySettings(CSplitPlot *plot)
{
	ResizePlots(plot->GetSplitMode(), plot->GetRows(), plot->GetCols());
}

template <class PlotImplIT>
int		CSplitPlot<PlotImplIT>::GetPlotDataCount()
{
	int nPlots = GetPlotCount();
	int counts=0;
	for( int i=0; i<nPlots; i++)
	{
		if(m_vpPlots[i])counts+=m_vpPlots[i]->GetPlotDataCount();
	}
	return counts;
}

template <class PlotImplIT>
PlotImplIT*	CSplitPlot<PlotImplIT>::GetSubPlot(int nIndex)
{
	if(nIndex<0 || nIndex>=GetPlotCount())return NULL;
	return m_vpPlots[nIndex];
}

template <class PlotImplIT>
CHandler*	CSplitPlot<PlotImplIT>::GetHandler(int nIndex)
{
	if(nIndex<0 || nIndex>=GetPlotCount())return NULL;
	return m_vpHandlers[nIndex];
}

template <class PlotImplIT>
CReactStatus *CSplitPlot<PlotImplIT>::GetReactStatus(int nIndex)
{
	if(nIndex<0 || nIndex>=GetPlotCount())return NULL;
	return m_vpReactStatus[nIndex];
}

template <class PlotImplIT>
bool	CSplitPlot<PlotImplIT>::IsSubPlotSelected()
{
	int i;
	for(i=0; i<GetPlotCount(); i++)
	{
		if(GetSubPlot(i)->IsPlotSelected())return true;
	}
	return false;
}

template <class PlotImplIT>
int	CSplitPlot<PlotImplIT>::GetIndexOfSelectedSubPlot()
{
	int i;
	for(i=0; i<GetPlotCount(); i++)
	{
		if(GetSubPlot(i)->IsPlotSelected())return i;
	}
	return -1;
}

template <class PlotImplIT>
void	CSplitPlot<PlotImplIT>::DeleteAll()
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
	}
	m_vpHandlers.clear();
	m_vpReactStatus.clear();
}

template <class PlotImplIT>
CPlotData<DataPoint2D>	*CSplitPlot<PlotImplIT>::GetDataSetByTitle( tstring title )
{
	CPlotData<DataPoint2D>	*pDataSet = 0;
	for(int i=0; i<GetPlotCount(); i++)
	{
		pDataSet = GetSubPlot(i)->GetPlotDataByTitle(title);
		if(pDataSet)return pDataSet;
	}
	return 0;
}

template <class PlotImplIT>
void	CSplitPlot<PlotImplIT>::ResizePlots(int mode, int nRows, int nCols)
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
		m_vpPlots[i] = new PlotImplIT;
		m_vpPlots[i]->AddBLAxis();
		m_vpPlots[i]->SetMargin(2, 2, 2, 2);
		m_vpPlots[i]->SetEdgeShow(true);

		m_vpPlots[i]->SetDoubleBuffer(false);

		m_vpHandlers[i] = m_vpPlots[i];
		m_vpReactStatus[i] = m_vpPlots[i];
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

Declare_Namespace_End

#endif