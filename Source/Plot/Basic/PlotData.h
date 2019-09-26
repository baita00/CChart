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

#include "../Accessary/Deelx/deelx.h"
#include "MyString.h"
#include "Global.h"
#include "CChartNameSpace.h"
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "gl/gl.h"


Declare_Namespace_CChart


typedef struct stDataPoint2D_Def
{
	double x, y;
}DataPoint2D_Def, *pDataPoint2D_Def;
typedef struct stDataPoint2D
{
	union
	{
		DataPoint2D_Def def;
		double val[2];
	};
}DataPoint2D, *pDataPoint2D;

typedef struct stDataPoint3D_Def
{
	double x, y, z;
}DataPoint3D_Def, *pDataPoint3D_Def;
typedef struct stDataPoint3D
{
	union
	{
		DataPoint3D_Def def;
		double val[3];
	};
}DataPoint3D, *pDataPoint3D;

//typedef vector<double> MyVData1D;
typedef vector<DataPoint2D> MyVData2D;
typedef vector<DataPoint3D> MyVData3D;

enum
{
	kSortXInc,
	kSortXDec,
	kSortYInc,
	kSortYDec,
	kSortZInc,
	kSortZDec,
	
	kSortTypeCount
};

// Plot type
enum
{
	kXYPlotScatter = 0,
	kXYPlotConnect = 1,
	kXYPlotStepHV = 2,
	kXYPlotStepVH = 3,
	kXYPlotStepHVH = 4,
	kXYPlotStepVHV = 5,
	kXYPlotBezier = 6,
	kXYPlotBar = 7,
	
	kPlotTypeCount
};

enum
{
	kBarFromBottom,
	kBarFromTop,
	kBarFromLeft,
	kBarFromRight,
	
	kBarModeCount
};

// how to show dataset infomation
enum
{
	kShowPlotDataInfoNot = 0,
	kShowPlotDataInfoEveryN = 1,
	kShowPlotDataInfoTotalN = 2,
	
	kShowPlotDataInfoModeCount
};

// Select and Light mode

enum
{
	kSLByNearestY,
	kSLByFillArea,

	kSLModeCount
};

void ClearAdjoinCoincide(MyVData2D& vData);

template<typename DataT>
void GetDataPoint(tstring aline, DataT &result, int dim)
{
	tistringstream istr(aline);
	int i;
	for(i=0; i<dim; i++)
	{
		istr>>result.val[i];
	}
}
template<typename DataT>
void GetDataLine(DataT datapoint, int dim, tstring space, tstring &result)
{
	tostringstream ostr;
	int i;
	for(i=0; i<dim; i++)
	{
		ostr<<datapoint.val[i];
		if(i<dim-1)	ostr<<space.c_str();
		else	ostr<<_TEXT("\n");
	}
	result = ostr.str();
}

static int nWhichDimCmp = 0;

template<typename DataT>
bool vLess(const DataT &d1, const DataT &d2)
{
	return d1.val[nWhichDimCmp] < d2.val[nWhichDimCmp];
}
template<typename DataT>
bool vGreat(const DataT &d1, const DataT &d2)
{
	return d1.val[nWhichDimCmp] > d2.val[nWhichDimCmp];
}

//void	InstanceTemplatedDataFunctions();

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

template<typename DataT>
class CPlotData
{
public:
	CPlotData();
	virtual ~CPlotData();

	void				SetDefaults();

protected:
	std::vector<DataT>	*m_pvData;
	std::vector<DataT>	*m_pvDataSaved;
	std::vector<int>	m_vSegHide;
	bool				m_bSegHideMode;

	tstring				m_strTitle;
	tstring				m_strInfo;//Draw at the first point
	tstring				m_strAppendix;
	bool				m_bInfoShow;
	int					m_nInfoFontsize;

	int					m_nDataLevel;

	int					m_nSortType;

	tstring				m_pStrPara[3], m_pStrUnit[3];
//	tstring				m_strXPara;
//	tstring				m_strYPara;
//	tstring				m_strZPara;
//	tstring				m_strXUnit;
//	tstring				m_strYUnit;
//	tstring				m_strZUnit;

	tstring				m_strSpaceSeparator;

protected:
	int			m_nDataID;

	int			m_nPlotType; // see above enum

	// for color
	COLORREF	m_crDataColor; // An RGB value for initial color
	COLORREF	m_crPlotColor; // An RGB value for really plotting
	bool		m_bAutoColor; // Let the program choose color

	GLfloat		m_fvDataColor[4]; // 3D
	GLfloat		m_fvPlotColor[4]; // 3D, An RGB value

	// for data range
//	double		m_pfXDataRange[2];
//	double		m_pfYDataRange[2];
//	double		m_pfZDataRange[2];
	double		m_ppfDataRange[sizeof(DataT)/sizeof(double)][2];
	bool		m_bDataRangeSet;

	// for the line property
	int			m_nDataLineSize;
	int			m_nPlotLineSize;
	int			m_nDataLineStyle;
	int			m_nPlotLineStyle;
	bool		m_bAutoLineStyle;
	int			m_nBarMode;

	bool		m_bVisible;
	bool		m_bSelected;
	bool		m_bLighted;
	int			m_nIdxPtSelected;
	int			m_nIdxPtLighted;
	bool		m_bReact;

	bool		m_bAutoSmooth;

	bool		m_bForceGdiPlus;

	bool		m_pbClipData[sizeof(DataT)/sizeof(double)];
	bool		m_bSorted;

	// for extended properties
	void		*m_pExtProp;

public:
	// for drag and offset
	double		fOffsetX;
	double		fOffsetY;
	bool		bDragMode;

	// for redraw
	int			nDatas, nPts, nPts4Fill;
	POINT		*pPointsGdi;
	double		xScale, yScale;
	int			nOldType;
	//double		fOldXRange[2], fOldYRange[2], fOldZRange[2];
	double		pfOldRange[sizeof(DataT)/sizeof(double)][2];
	RECT		rctOldPlot;
	bool		bCompactDraw;
	MyVData2D	vDrawData;
	bool		bDrawDataChanged;
	int			nDrawDataOldSize;
	bool		bForceUpdate;

	int			pnClipRange[2];

	// for oscilloscope
	bool		bDragMoved;

	// for interaction
	int			nSLMode;

	// for special use
	bool		bFlag;

public:
	inline	const vector<DataT>		&GetDataConst() const {return *m_pvData;}
	inline	vector<DataT>			&GetData() {return *m_pvData;}
	inline	vector<DataT>			*GetPData() {return m_pvData;}
	inline	vector<DataT>			&GetDataSaved() {return *m_pvDataSaved;}
	inline	vector<DataT>			*GetPDataSaved() {return m_pvDataSaved;}
	inline	int						GetDataCount() {return (int)m_pvData->size();}
	inline	vector<int>				&GetSegHide(){return m_vSegHide;}

	inline	void		ReplacePoint(DataT data, int nPointIndex){if(nPointIndex<0 || nPointIndex>=GetDataCount())return; (*m_pvData)[nPointIndex]=data;}

	void				DeleteData();
	//virtual	void		GetDataRange();
	inline bool			IsSegHideMode(){return m_bSegHideMode;}
	inline void			SetSegHideMode(bool seghide){m_bSegHideMode = seghide;}
	inline tstring		GetTitle(){if(m_strTitle==_TEXT("") )return CGlobal::GetStrUntitle();	else return m_strTitle;	};
	inline void			SetTitle(tstring title){m_strTitle=title;}
	inline tstring		GetInfo(){return m_strInfo;}
	inline void			SetInfo(tstring info){m_strInfo=info;}
	inline tstring		GetAppendix(){return m_strAppendix;}
	inline void			SetAppendix(tstring append){m_strAppendix=append;};

	inline bool			IsInfoShow(){return m_bInfoShow;}
	inline void			SetInfoShow(bool bDraw){m_bInfoShow=bDraw;}
	inline int			GetInfoFontsize(){return m_nInfoFontsize;}
	inline void			SetInfoFontsize(int fontsize){if(fontsize>4)m_nInfoFontsize=fontsize;else m_nInfoFontsize=4;}
	inline int			GetDataLevel(){return m_nDataLevel;}
	inline void			SetDataLevel(int level){m_nDataLevel=level;}
	inline tstring		GetXPara(){return m_pStrPara[0];}
	inline void			SetXPara(tstring para){m_pStrPara[0]=para;}
	inline tstring		GetYPara(){return m_pStrPara[1];}
	inline void			SetYPara(tstring para){m_pStrPara[1]=para;}
	inline tstring		GetZPara(){return m_pStrPara[2];}
	inline void			SetZPara(tstring para){m_pStrPara[2]=para;}
	inline tstring		GetXUnit(){return m_pStrUnit[0];}
	inline void			SetXUnit(tstring unit){m_pStrUnit[0]=unit;}
	inline tstring		GetYUnit(){return m_pStrUnit[1];}
	inline void			SetYUnit(tstring unit){m_pStrUnit[1]=unit;}
	inline tstring		GetZUnit(){return m_pStrUnit[2];}
	inline void			SetZUnit(tstring unit){m_pStrUnit[2]=unit;}

	inline int			GetSortType(){return m_nSortType;}
	inline void			SetSortType(int type){m_nSortType = type;}

	inline tstring		GetStrSpaceSeparator(){return m_strSpaceSeparator;}
	inline void			SetStrSpaceSeparator(tstring sep){m_strSpaceSeparator == sep; }
public:
	inline	int			GetDataID() {return m_nDataID;}
	inline	void		SetDataID(int id) { m_nDataID = id; }
	inline	int			GetPlotType() {return m_nPlotType;}
	inline	void		SetPlotType(int type) { m_nPlotType = type; }
	inline	COLORREF	GetDataColor() {return m_crDataColor;}
	inline	void		SetDataColor(COLORREF cr) { m_crDataColor = cr; }
	inline	COLORREF	GetPlotColor() {return m_crPlotColor;}
	inline	void		SetPlotColor(COLORREF cr) { m_crPlotColor = cr; }
	inline	GLfloat*	GetGlDataColor() {return m_fvDataColor;}
	inline	void		SetGlDataColor(GLfloat* cr) { memcpy(m_fvDataColor, cr, 4*sizeof(GLfloat)); }
	inline	GLfloat*	GetGlPlotColor() {return m_fvPlotColor;}
	inline	void		SetGlPlotColor(GLfloat* cr) { memcpy(m_fvPlotColor, cr, 4*sizeof(GLfloat)); }
	inline	bool		IsAutoColor() {return m_bAutoColor;}
	inline	void		SetAutoColor(bool bAuto) { m_bAutoColor = bAuto; }
	inline	double		*GetDataRange1D(int whichDim){ return m_ppfDataRange[whichDim]; }
	inline	double		*GetXDataRange(){ return m_ppfDataRange[0]; }
	inline	double		*GetYDataRange(){ return m_ppfDataRange[1]; }
	inline	double		*GetZDataRange(){ return m_ppfDataRange[2]; }
	inline	void		GetXDataRange(double *range){ memcpy(range, m_ppfDataRange[0], 2*sizeof(double)); }
	inline	void		GetYDataRange(double *range){ memcpy(range, m_ppfDataRange[1], 2*sizeof(double)); }
	inline	void		GetZDataRange(double *range){ memcpy(range, m_ppfDataRange[2], 2*sizeof(double)); }
	inline	void		GetDataRange(double *xRange, double *yRange){ memcpy(xRange, m_ppfDataRange[0], 2*sizeof(double)); memcpy(yRange, m_ppfDataRange[1], 2*sizeof(double));}
	inline	double		GetXDataRangeLow(){return m_ppfDataRange[0][0];}
	inline	double		GetXDataRangeHigh(){return m_ppfDataRange[0][1];}
	inline	double		GetYDataRangeLow(){return m_ppfDataRange[1][0];}
	inline	double		GetYDataRangeHigh(){return m_ppfDataRange[1][1];}
	inline	double		GetZDataRangeLow(){return m_ppfDataRange[2][0];}
	inline	double		GetZDataRangeHigh(){return m_ppfDataRange[2][1];}
	inline	void		SetXDataRange(double low, double high) { m_ppfDataRange[0][0] = low; m_ppfDataRange[0][1] = high; }
	inline	void		SetYDataRange(double low, double high) { m_ppfDataRange[1][0] = low; m_ppfDataRange[1][1] = high; }
	inline	void		SetZDataRange(double low, double high) { m_ppfDataRange[2][0] = low; m_ppfDataRange[2][1] = high; }
	inline	void		SetXDataRange(double *range) { memcpy(m_ppfDataRange[0], range, 2*sizeof(double)); }
	inline	void		SetYDataRange(double *range) { memcpy(m_ppfDataRange[1], range, 2*sizeof(double)); }
	inline	void		SetZDataRange(double *range) { memcpy(m_ppfDataRange[2], range, 2*sizeof(double)); }
	inline	void		SetDataRange(double *xRange, double *yRange) {SetXDataRange(xRange); SetYDataRange(yRange); }
	inline	bool		IsDataRangeSet(){ return m_bDataRangeSet; }
	inline	void		SetDataRangeSet(bool set){ m_bDataRangeSet = set; }
	inline	int			GetDataLineSize() {return m_nDataLineSize;}
	inline	void		SetDataLineSize(int size) { m_nDataLineSize = size; }
	inline	int			GetPlotLineSize() {return m_nPlotLineSize;}
	inline	void		SetPlotLineSize(int size) { m_nPlotLineSize = size; }
	inline	int			GetDataLineStyle() {return m_nDataLineStyle;}
	inline	void		SetDataLineStyle(int style) { m_nDataLineStyle = style; }
	inline	int			GetPlotLineStyle() {return m_nPlotLineStyle;}
	inline	void		SetPlotLineStyle(int style) { m_nPlotLineStyle = style; }
	inline	bool		IsAutoLineStyle() {return m_bAutoLineStyle;}
	inline	void		SetAutoLineStyle(bool bAuto) { m_bAutoLineStyle = bAuto; }
	inline	int			GetBarMode() {return m_nBarMode;}
	inline	void		SetBarMode(int mode) { m_nBarMode = mode; }
	inline	bool		IsVisible() {return m_bVisible;}
	inline	void		SetVisible(bool vis) { m_bVisible = vis; }
	inline	bool		IsSelected() {return m_bSelected;}
	inline	void		SetSelected(bool sel) { m_bSelected = sel; }
	inline	bool		IsLighted() {return m_bLighted;}
	inline	void		SetLighted(bool light) { m_bLighted = light; }
	inline	int			GetIdxPtSelected() {if(m_nIdxPtSelected>=0 && m_nIdxPtSelected<GetDataCount())return m_nIdxPtSelected; else return -1;}
	inline	void		SetIdxPtSelected(int idx) { if(idx>=0 && idx<GetDataCount())m_nIdxPtSelected = idx; else m_nIdxPtSelected = -1;}
	inline	int			GetIdxPtLighted() {return m_nIdxPtLighted;}
	inline	void		SetIdxPtLighted(int idx) { if(idx>=0 && idx<GetDataCount())m_nIdxPtLighted = idx; else m_nIdxPtLighted = -1;}
	inline	bool		IsReact() {return m_bReact;}
	inline	void		SetReact(bool react) { m_bReact = react; }
	inline	bool		IsAutoSmooth() { return m_bAutoSmooth; }
	inline	void		SetAutoSmooth(bool smooth) { m_bAutoSmooth = smooth; }
	inline	bool		IsForceGdiPlus() { return m_bForceGdiPlus; }
	inline	void		SetForceGdiPlus(bool bForce) { m_bForceGdiPlus = bForce; }

	inline	double		*GetClipData(int whichDim){ return m_pbClipData[whichDim]; }
	inline	bool		IsClipXData(){ return m_pbClipData[0]; }
	inline	void		SetClipXData(bool clip){ m_pbClipData[0] = clip; }
	inline	bool		IsClipYData(){ return m_pbClipData[1]; }
	inline	void		SetClipYData(bool clip){ m_pbClipData[1] = clip; }
	inline	bool		IsClipZData(){ return m_pbClipData[2]; }
	inline	void		SetClipZData(bool clip){ m_pbClipData[2] = clip; }
	inline	bool		IsClipData(){int i; for(i=0; i<sizeof(DataT)/sizeof(double); i++){if(m_pbClipData[i])return true;} return false;}
	inline	bool		IsSorted(){ return m_bSorted; }
	inline	void		SetSorted(bool sorted){ m_bSorted = sorted; }

	//	inline	SIZE		&GetTitleOffset(){ return m_sizeTitleOffset;}
//	inline	void		SetTitleOffset(SIZE offset){ m_sizeTitleOffset = offset;}
//	inline	double		GetScale(){ return m_fScale;}
//	inline	void		SetScale(double scale){ m_fScale = scale; }

	inline	void		*GetExtProp(){return m_pExtProp;}
	inline	void		SetExtProp(void *pEP){m_pExtProp = pEP;}

public:
	inline	bool		IsCompactDraw() {return bCompactDraw; }
	inline	void		SetCompactDraw(bool bCD) { bCompactDraw = bCD; }

	inline	int			GetSLMode() {return nSLMode; }
	inline	void		SetSLMode(int mode) { nSLMode = mode; }

	inline	bool		GetFlag() {return bFlag; }
	inline	void		SetFlag(bool flag) { bFlag = flag; }

public:

	bool				SetData( vector<DataT> vData );
	bool				SetData( vector<DataT> *pvData );

	void				SortData(vector<DataT> &vData, int nSortType);
	void				SortData();

	bool				ReadFromBuff(tstring buff);
	bool				ReadFromFile(tstring filename);
	bool				WriteToFile(tofstream &ofs);
	bool				WriteToFile(tstring filename);
	bool				AppendToFile(tstring filename);
	bool				WriteToBuff(tstring &buff);
	bool				AppendToBuff(tstring &buff);
};

//void	CPlotData<DataPoint2D>::SortData(vector<DataPoint2D> &vdata, int nSortType);
//void	CPlotData<DataPoint3D>::SortData(vector<DataPoint3D> &vdata, int nSortType);

template<typename DataT>
CPlotData<DataT>::CPlotData()
{
	m_pvData = new std::vector<DataT>;
	m_pvDataSaved = new std::vector<DataT>;
	SetDefaults();
	pPointsGdi = 0;

	m_pExtProp = 0;
}

template<typename DataT>
CPlotData<DataT>::~CPlotData()
{
	DeleteData();

	if(m_pExtProp)
	{
		delete m_pExtProp;
		m_pExtProp = 0;
	}
	if(m_pvData)
	{
		delete m_pvData;
		m_pvData = 0;
	}
	if(m_pvDataSaved)
	{
		delete m_pvDataSaved;
		m_pvDataSaved = 0;
	}
}

template<typename DataT>
void CPlotData<DataT>::SetDefaults()
{
	m_strTitle = CGlobal::GetStrUntitle();
	m_strInfo = _TEXT("");
	m_strAppendix = _TEXT("");
	m_strSpaceSeparator = _TEXT("\t\t");
	m_bInfoShow = false;
	m_nInfoFontsize = 12;

	m_bSegHideMode = false;
	
	m_nDataLevel = -1;
	
	m_nSortType = kSortXInc;
	
	m_pStrPara[0] = _TEXT("Time");
	m_pStrPara[1] = _TEXT("Voltage");
	m_pStrPara[2] = _TEXT("Voltage");
	m_pStrUnit[0] = _TEXT("ns");
	m_pStrUnit[1] = _TEXT("mV");
	m_pStrUnit[2] = _TEXT("mV");


#ifdef DEFAULT_WHITE_BACKGROUND
	m_crDataColor = RGB(0,0,0);
	m_crPlotColor = RGB(0,0,0);
#else
	m_crDataColor = RGB(255,255,255);
	m_crPlotColor = RGB(255,255,255);
#endif
	m_fvDataColor[0] = m_fvDataColor[1] = m_fvDataColor[2] = m_fvDataColor[3] = 0; 
	m_fvPlotColor[0] = m_fvPlotColor[1] = m_fvPlotColor[2] = m_fvPlotColor[3] = 0;
	m_bAutoColor = true;
  
	m_bDataRangeSet = false;

	m_nPlotType = kXYPlotConnect;

	m_nDataLineSize = 1;
	m_nPlotLineSize = 1;
	m_nDataLineStyle = PS_SOLID;
	m_nPlotLineStyle = PS_SOLID;
	m_bAutoLineStyle = false;
	m_nBarMode	= kBarFromBottom;

	m_bVisible = true;
	m_bSelected = false;
	m_bLighted = false;
	m_nIdxPtSelected = -1;
	m_nIdxPtLighted = -1;

	m_bReact = true;

	m_bAutoSmooth = true;

	m_bForceGdiPlus = false;

	int i;
	for(i=0; i<sizeof(DataT)/sizeof(double); i++)
	{
		m_pbClipData[i] = false;
	}
	m_bSorted = false;
	pnClipRange[0] = pnClipRange[1] = -1;

	bDragMode = false;
	fOffsetX = 0.0;
	fOffsetY = 0.0;

	pPointsGdi = 0;
	nDatas = 0;
	nPts = 0;
	nPts4Fill = 0;
	xScale = 1.0;
	yScale = 1.0;
	nOldType = kXYPlotConnect;
	//memset(fOldXRange, 0, 2*sizeof(double));
	//memset(fOldYRange, 0, 2*sizeof(double));
	for(i=0; i<sizeof(DataT)/sizeof(double); i++)
	{
		memset(pfOldRange, 0, 2*sizeof(double));
	}
	bCompactDraw = false;
	SetRectEmpty(&rctOldPlot);
	vDrawData.clear();
	bDrawDataChanged = true;
	nDrawDataOldSize = 0;
	bForceUpdate = true;
	
	// for oscilloscope
	bDragMoved = false;

	nSLMode = 0;
	bFlag = false;
}

// Delete the data set
template<typename DataT>
void CPlotData<DataT>::DeleteData(void)
{
	// Check if valid data. If so, delete it
	m_strTitle = _TEXT("");
	m_pvData->clear();
}

template<typename DataT>
bool CPlotData<DataT>::SetData(vector<DataT> vData)
{
	m_pvData->clear();
	*m_pvData = vData;
	return true;
}

template<typename DataT>
bool CPlotData<DataT>::SetData(vector<DataT> *pvData)
{
	m_pvData->clear();
	m_pvData = pvData;
	return true;
}

template<typename DataT>
void CPlotData<DataT>::SortData(vector<DataT> &vData, int nSortType)
{
	// sort failed now
	if (vData.size() <= 0)
		return;
	switch(nSortType)
	{
	case kSortXInc:
		nWhichDimCmp = 0;
		sort(vData.begin(), vData.end(), vLess<DataT>);
		break;
	case kSortXDec:
		nWhichDimCmp = 0;
		sort(vData.begin(), vData.end(), vGreat<DataT>);
		break;
	case kSortYInc:
		nWhichDimCmp = 1;
		sort(vData.begin(), vData.end(), vLess<DataT>);
		break;
	case kSortYDec:
		nWhichDimCmp = 1;
		sort(vData.begin(), vData.end(), vGreat<DataT>);
		break;
	case kSortZInc:
		nWhichDimCmp = 2;
		sort(vData.begin(), vData.end(), vLess<DataT>);
		break;
	case kSortZDec:
		nWhichDimCmp = 2;
		sort(vData.begin(), vData.end(), vGreat<DataT>);
		break;
	}
}

template<typename DataT>
void CPlotData<DataT>::SortData()
{
	SortData(*m_pvData, m_nSortType);
}
/*
template<typename DataT>
void	CPlotData<DataT>::GetDataRange(  )
{
	int			j;
	double		minX, minY, maxX, maxY;
	double		lb = 0.0, ub = 0.0;
		
	if( m_bDataRangeSet == false )
	{
		minX = fMaxVal; minY = fMaxVal;
		maxX = -fMaxVal; maxY = -fMaxVal;
		m_bDataRangeSet = true;
				
		// Just find min and max of x
		for( j = 0; j < GetDataCount(); j++)
		{
			if( m_vData[j].val[0] < minX ) minX = m_vData[j].val[0];
			if( m_vData[j].val[0] > maxX ) maxX = m_vData[j].val[0];
			
			if( m_vData[j].val[1] < minY ) minY = m_vData[j].val[1];
			if( m_vData[j].val[1] > maxY ) maxY = m_vData[j].val[1];
		}
		
		m_pfXDataRange[0] = minX;
		m_pfXDataRange[1] = maxX;
		m_pfYDataRange[0] = minY;
		m_pfYDataRange[1] = maxY;
	}
}*/

template<typename DataT>
bool CPlotData<DataT>::ReadFromBuff(tstring buff)
{
	int	dim = sizeof(DataT)/sizeof(double);
	if(dim<=0 || dim>3)return false;
	
	tstring	pattern;
	if(dim==1)
	{
		pattern = _TEXT("^-?\\d+(\\.\\d*)?([eE][+-]?\\d*)?\\s*$");
	}
	else if(dim==2)
	{
		pattern = _TEXT("^(-?\\d+(\\.\\d*)?([eE][+-]?\\d*)?\\s*){2}$");
	}
	else if(dim==3)
	{
		pattern = _TEXT("^(-?\\d+(\\.\\d*)?([eE][+-]?\\d*)?\\s*){3}$");
	}

	tstring aline;
	CRegexpT <TCHAR> dataline(pattern.c_str());
	MatchResult rst1;
	int match1;
	
	DataT datapoint;

	vector<DataT> vData;
	tstring value;
	tstring::size_type curpos;
	
	curpos=0;
	do
	{
		aline=readline(buff,curpos);
		if(aline.length()==0)
			break;
		trimspace(aline);
		rst1=dataline.MatchExact(aline.c_str());
		match1=rst1.IsMatched();
		if(match1)
		{
			GetDataPoint(aline, datapoint, dim);
			vData.push_back(datapoint);
		}
	}while(curpos != tstring::npos);

	if(vData.size()<=0)
	{
		return false;
	}
	else
	{
		m_pvData->clear();
		*m_pvData = vData;
		return true;
	}
}

template<typename DataT>
bool CPlotData<DataT>::ReadFromFile(tstring filename)
{
	tifstream ifs;
#if defined(_UNICODE) || defined(UNICODE)
	string name;
	if(!mywcs2mbs(filename, name))return false;
	ifs.open(name.c_str());
#else
	ifs.open(filename.c_str());
#endif
	if(!ifs.is_open())return false;
	
	int filesize=get_file_size(filename.c_str());
	TCHAR *pBuff=new TCHAR[filesize+1];
	memset(pBuff,0,(filesize+1)*sizeof(TCHAR));
	ifs.read(pBuff,filesize);
	tstring content=pBuff;
	delete []pBuff;
	ifs.close();

	return ReadFromBuff(content);
}

template<typename DataT>
bool	CPlotData<DataT>::WriteToFile(tofstream &ofs)
{
	if(!ofs.is_open() || m_pvData->size()<=0)
		return false;
	
	tstring content;
	if(!WriteToBuff(content))
		return false;

	ofs.write(content.c_str(), content.length());
	return true;
}

template<typename DataT>
bool	CPlotData<DataT>::WriteToFile(tstring filename)
{
	if(m_pvData->size()<=0)
		return false;
	tofstream ofs;
#if defined(_UNICODE) || defined(UNICODE)
	string name;
	if(!mywcs2mbs(filename, name))return false;
	ofs.open(name.c_str());
#else
	ofs.open(filename.c_str());
#endif
	if(!ofs.is_open())return false;
	bool ret = WriteToFile(ofs);
	ofs.close();

	return ret;
}

template<typename DataT>
bool	CPlotData<DataT>::AppendToFile(tstring filename)
{
	if(m_pvData->size()<=0)
		return false;

	tofstream ofs;
	tofstream ofs;
#if defined(_UNICODE) || defined(UNICODE)
	string name;
	if(!mywcs2mbs(filename, name))return false;
	ofs.open(name.c_str(), tios::app);
#else
	ofs.open(filename.c_str(),tios::app);
#endif
	if(!ofs::is_open())return false;
	bool ret = WriteToFile(ofs);
	ofs.close();
	return ret;
}

template<typename DataT>
bool	CPlotData<DataT>::WriteToBuff(tstring &buff)
{
	if(m_pvData->size()<=0)
		return false;
	
	buff = _TEXT("");
	return AppendToBuff(buff);
}

template<typename DataT>
bool	CPlotData<DataT>::AppendToBuff(tstring &buff)
{
	int	dim = sizeof(DataT)/sizeof(double);
	if(dim<=0 || dim>3)return false;

	if(m_pvData->size()<=0)
		return false;
	
	tstring aline;
	int i;

	for(i=0; i<(int)m_pvData->size(); i++)
	{
		GetDataLine((*m_pvData)[i], dim, m_strSpaceSeparator, aline);
		buff += aline;
	}
	
	return true;
}

Declare_Namespace_End