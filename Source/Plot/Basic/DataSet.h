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

#include "PlotData.h"
#include "PlotImpl.h"
#include "CChartNameSpace.h"

Declare_Namespace_CChart

template<typename DataT>
class CDataSet
{
public:
	CDataSet();
	virtual ~CDataSet();

protected:
	void	SetDefaults();

protected:
	std::vector<CPlotDataImpl<DataT>* >	m_vpPlotData;
	int					m_nCurDataID;

	bool				m_bMultiSelect;

	int					m_nReservedDataLength;

	int					m_nMaxPoints;
	bool				m_bMaxPointsReset;

	bool				m_bPointSelectionMode;

	bool				m_bExcludeDataWhenUniteColor;

protected:
	// For data save and load
	bool				m_bSequence;
	bool				m_bShareX;

	int					m_nShowPlotDataInfoMode;
	int					m_nShowPlotDataInfoNum;
	int					m_nShowPlotDataInfoDataPointStep;

	tstring				m_strFileSeparator;
	tstring				m_strDataSeparator;

public:

	inline	int			GetPlotDataCount( void ) const { return (int)m_vpPlotData.size(); }
	inline	int			GetPlotDataLevels( void ) const { return GetPlotDataCount(); }

	int					GetIndexOfFirstVisiblePlotData();
	int					GetIndexOfLastVisiblePlotData();
	int					GetIndexOfNextVisiblePlotData(int idxCurr);
	int					GetIndexOfPrevVisiblePlotData(int idxCurr);
	int					GetVisiblePlotDataCount();
	
public:
	virtual	bool		IsEmpty();
	int					GetMaxPlotDataLength();
protected:
	// CriticalSection
	CRITICAL_SECTION	m_csData;
public:
	void				MyLock(){EnterCriticalSection(&m_csData);}
	void				MyUnlock(){LeaveCriticalSection(&m_csData);}
public:
	bool				DeleteData( int dataID );
	void				DeleteAllData( void );
	bool				EmptyData( int dataID );
	void				EmptyAllData( void );
	
	bool				SetDataModified( int dataID );
	
public:
	int					GetDataID( int index) const;
	int					GetIndex( int dataID ) const;

	int					GetSelectedID();
	int					GetLightedID();
	int					GetSelectedPointIdx();
	int					GetLightedPointIdx();
	
	tstring				GetDataTitle( int dataID );
	void				SetDataTitle( int dataID, tstring title );
	virtual	tstring		GetAbsDataTitle( int which );
	void				SetAbsDataTitle( int which, tstring title );

	void				UpdateXDataRange(double val, int nWhich);
	void				UpdateYDataRange(double val, int nWhich);
	void				UpdateDataRanges(DataT dtVal, int nWhich);

	void				GetDataRange( int dataID );

public:

	std::vector<CPlotDataImpl<DataT>* >	&GetVPlotData(){return m_vpPlotData;}
	CPlotDataImpl<DataT>	*GetPlotData(int dataID);
	CPlotDataImpl<DataT>	*GetAbsPlotData(int nIndex);

	const vector<DataT>	&GetData( int dataID ) const;
	const vector<DataT>		&GetAbsData( int which ) const;

	void				UnsetPlotDataDragMode();
	bool				SetPlotDataOffset(int dataID, double offsetX, double offsetY);
	double				GetPlotDataOffsetX(int dataID);
	double				GetPlotDataOffsetY(int dataID);
	double				GetMinOffsetX();
	double				GetMinOffsetY();
	double				GetMaxOffsetX();
	double				GetMaxOffsetY();

	void				ResetPlotDataOffset(int dataID);
	void				RessetAllPlotDataOffset();

	bool				MovePlotData( int dataID, double offsetx, double offsety);

	double				GetValueFromX(double x, int which);

	int					GetDataIDByTitle(tstring title);
	int					GetIndexByTitle(tstring title);
	CPlotDataImpl<DataT>	*GetPlotDataByTitle( tstring title );
public:
	inline	int			GetIdOfLighted(){for(int i=0;i<GetPlotDataCount();i++)if(m_vpPlotData[i]->IsLighted())return GetDataID(i);return -1;}
	inline	int			GetIndexOfLighted(){for(int i=0;i<GetPlotDataCount();i++)if(m_vpPlotData[i]->IsLighted())return i;return -1;}
	inline	bool		IsAnyPlotDataHiden(){for(int i=0;i<GetPlotDataCount();i++)if(!m_vpPlotData[i]->IsVisible())return true;return false;}
	inline	void		ShowAllPlotDatas(){for(int i=0;i<GetPlotDataCount();i++)SetVisible(GetDataID(i),true);}
	inline	int			GetIdOfSelected(){for(int i=0;i<GetPlotDataCount();i++)if(m_vpPlotData[i]->IsSelected())return GetDataID(i);return -1;}
	inline	int			GetIndexOfSelected(){for(int i=0;i<GetPlotDataCount();i++)if(m_vpPlotData[i]->IsSelected())return i;return -1;}
	inline	bool		IsCompactDraw(int dataID){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->bCompactDraw; }
	inline	void		SetCompactDraw(int dataID, bool compact){ int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->bCompactDraw = compact; }
	inline	bool		IsDrawDataChanged(int dataID){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->bDrawDataChanged; }
	inline	void		SetDrawDataChanged(int dataID, bool changed){ int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->bDrawDataChanged = changed; }
public:
	inline int			GetReservedDataLength(){return m_nReservedDataLength;}
	inline void			SetReservedDataLength(int length){m_nReservedDataLength = length;}

	inline int			GetMaxPoints(){return m_nMaxPoints;}
	inline void			SetMaxPoints(int pts){m_nMaxPoints=pts;}
	inline bool			IsMaxPointsReset(){return m_bMaxPointsReset;}
	inline void			SetMaxPointsReset(bool bReset){m_bMaxPointsReset=bReset;}
	inline bool			IsPointSelectionMode(){return m_bPointSelectionMode;}
	inline void			SetPointSelectionMode(bool ps){m_bPointSelectionMode = ps;}
	inline bool			IsExcludeDataWhenUniteColor(){return m_bExcludeDataWhenUniteColor;}
	inline void			SetExcludeDataWhenUniteColor(bool exclude){m_bExcludeDataWhenUniteColor = exclude;}

	inline	bool		IsPlotDataSelected(){for(int i=0;i<GetPlotDataCount();i++)if(m_vpPlotData[i]->IsSelected())return true;return false;}
	inline	void		SelectAllPlotData(bool bSel){for(int i=0;i<GetPlotDataCount();i++)m_vpPlotData[i]->SetSelected(bSel);}
	inline	bool		IsPlotDataLighted(){for(int i=0;i<GetPlotDataCount();i++)if(m_vpPlotData[i]->IsLighted())return true;return false;}
	inline	void		UnlightAllPlotData(){for(int i=0;i<GetPlotDataCount();i++)m_vpPlotData[i]->SetLighted(false);}

	inline	void		UnselectDataPoints(){for(int i=0;i<GetPlotDataCount();i++)m_vpPlotData[i]->SetIdxPtSelected(-1);}
	inline	void		UnlightDataPoints(){for(int i=0;i<GetPlotDataCount();i++)m_vpPlotData[i]->SetIdxPtLighted(-1);}
	
public:
	inline int			GetPlotType( int dataID )   { int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpPlotData[i]->GetPlotType(); }
	inline void			SetPlotType( int dataID, int type ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetPlotType(type);}
	inline int			GetDataLineSize( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpPlotData[i]->GetDataLineSize(); }
	inline void			SetDataLineSize( int dataID, int lineSize ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataLineSize(lineSize); }
	inline int			GetPlotLineSize( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpPlotData[i]->GetPlotLineSize(); }
	inline void			SetPlotLineSize( int dataID, int lineSize ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetPlotLineSize(lineSize); }
	inline int			GetDataLineStyle( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpPlotData[i]->GetDataLineStyle(); }
	inline void			SetDataLineStyle( int dataID, int lineStyle ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataLineStyle(lineStyle); }
	inline int			GetPlotLineStyle( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return 1; else return m_vpPlotData[i]->GetPlotLineStyle(); }
	inline void			SetPlotLineStyle( int dataID, int lineStyle ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetPlotLineStyle(lineStyle); }
	inline bool			IsAutoLineStyle( int dataID ) { int i = GetIndex( dataID ); if( i < 0 ) return false; else return m_vpPlotData[i]->IsAutoLineStyle(); }
	inline void			SetAutoLineStyle( int dataID, bool bAuto ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetAutoLineStyle(bAuto); };
	inline bool			IsVisible( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->IsVisible(); }
	inline void			SetVisible( int dataID, bool bShow ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetVisible(bShow); };
	inline bool			IsSelected( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->IsSelected(); }
	inline void			SetSelected( int dataID, bool bSel ) { int i = GetIndex( dataID ); if( i < 0 ) return; else {if(!m_bMultiSelect && bSel)SelectAllPlotData(false);m_vpPlotData[i]->SetSelected(bSel);} };
	inline bool			IsLighted( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->IsLighted(); }
	inline void			SetLighted( int dataID, bool bLight ){UnlightAllPlotData();int i = GetIndex( dataID );if( i < 0 ) return;else m_vpPlotData[i]->SetLighted(bLight);}
	inline bool			IsMultiSelect(){return m_bMultiSelect;}
	inline void			SetMultiSelect(bool bSel){m_bMultiSelect=bSel;}
	inline bool			IsDataDragMode( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->bDragMode; }
	inline void			SetDataDragMode( int dataID, bool drag ) { int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->bDragMode = drag; }
public:
	inline void			SetDataID( int dataID, int newID){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->SetDataID(newID);}
	
	inline void			SetDataColor( int dataID, COLORREF color ){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->SetDataColor(color);}
	inline COLORREF		GetDataColor( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return RGB( 0, 0, 0); else return m_vpPlotData[i]->GetDataColor(); }
	inline void			SetPlotColor( int dataID, COLORREF color ){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->SetPlotColor(color);}
	inline COLORREF		GetPlotColor( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return RGB( 0, 0, 0); else return m_vpPlotData[i]->GetPlotColor(); }
	inline void			SetAutoColor( int dataID, bool bAuto){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->SetAutoColor(bAuto);}
	inline bool			IsAutoColor( int dataID ) {int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpPlotData[i]->IsAutoColor();}

	inline void			SetSegHideMode( int dataID, bool bHide ){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->SetSegHideMode(bHide);}
	inline bool			IsSegHideMode( int dataID ) {int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpPlotData[i]->IsSegHideMode();}
	inline void			AddSegHidden( int dataID, int nDataPointIndex ){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->GetSegHide().push_back(nDataPointIndex);}

	inline bool			IsSequence() { return m_bSequence;}
	inline void			SetSequence( bool seq ) { m_bSequence=seq; }
	inline bool			IsShareX() { return m_bShareX;}
	inline void			SetShareX( bool share ) { m_bShareX=share; }

public:
	inline void			SetShowPlotDataInfoMode(int mode){m_nShowPlotDataInfoMode=mode;}
	inline int			GetShowPlotDataInfoMode(){return m_nShowPlotDataInfoMode;}
	inline void			SetShowPlotDataInfoNum(int num){m_nShowPlotDataInfoNum=num;}
	inline int			GetShowPlotDataInfoNum(){return m_nShowPlotDataInfoNum;}
	inline void			SetShowPlotDataInfoDataPointStep(int step){m_nShowPlotDataInfoDataPointStep=step;}
	inline int			GetShowPlotDataInfoDataPointStep(){return m_nShowPlotDataInfoDataPointStep;}
	inline tstring&		GetFileSeparator(){return m_strFileSeparator;}
	inline void			SetFileSeparator(tstring sep){m_strFileSeparator = sep;}
	inline tstring&		GetDataSeparator(){return m_strDataSeparator;}
	inline void			SetDataSeparator(tstring sep){m_strDataSeparator = sep;}

public:
	inline void			SetMarkerShow( int dataID, bool show ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerShow(show);}
	inline bool			IsMarkerShow( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpPlotData[i]->IsMarkerShow(); }
	inline void			SetMarkerType( int dataID, int type ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerType(type);}
	inline int			GetMarkerType( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpPlotData[i]->GetMarkerType(); }
	inline void			SetMarkerSize( int dataID, int size ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerSize(size);}
	inline int			GetMarkerSize( int dataID )  { int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpPlotData[i]->GetMarkerSize(); }
	inline void			SetMarkerFillState( int dataID, bool how ){	int	index = GetIndex( dataID );	if( index < 0 ) return;	m_vpPlotData[index]->SetMarkerFill(how);}
	inline bool			GetMarkerFillState( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpPlotData[i]->IsMarkerFill(); }
	inline int			GetMarkerFillAlpha( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetMarkerFillAlpha(); }
	inline void			SetMarkerFillAlpha( int dataID, int alpha ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetMarkerFillAlpha(alpha); }
	inline void			SetMarkerFrequency( int dataID, int freq ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerFrequency(freq);}
	inline int			GetMarkerFrequency( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetMarkerFrequency(); }
	inline void			SetMarkerStartIndex( int dataID, int sindex ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerStartIndex(sindex);}
	inline int			GetMarkerStartIndex( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetMarkerStartIndex(); }
	inline HBITMAP		GetMarkerImage( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return NULL; else return m_vpPlotData[i]->GetMarkerImage(); }
	inline void			SetMarkerImage( int dataID, HBITMAP image ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetMarkerImage(image); }
	inline bool			IsMarkerImageRound( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpPlotData[i]->IsMarkerImageRound(); }
	inline void			SetMarkerImageRound( int dataID, bool round ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerImageRound(round);}
	inline void			SetMarkerBarVertical( int dataID, bool vert ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerBarVertical(vert);}
	inline bool			IsMarkerBarVertical( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return false; else return m_vpPlotData[i]->IsMarkerBarVertical(); }
	inline void			SetMarkerHHeight( int dataID, int hh ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerBarHHeight(hh);}
	inline int			GetMarkerHHeight( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetMarkerBarHHeight(); }
	inline void			SetMarkerHWidth( int dataID, int hw ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerBarHWidth(hw);}
	inline int			GetMarkerHWidth( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetMarkerBarHWidth(); }
	inline void			SetMarkerExtLen( int dataID, int len ){int index = GetIndex( dataID );if( index < 0 ) return;m_vpPlotData[index]->SetMarkerBarExtLen(len);}
	inline int			GetMarkerExtLen( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetMarkerBarExtLen(); }
	inline bool			IsMarkerFillOtherColor( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->IsMarkerFillOtherColor(); }
	inline void			SetMarkerFillOtherColor( int dataID, bool fc ) { int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetMarkerFillOtherColor(fc); };
	inline COLORREF		GetMarkerFillColor( int dataID ) { int i = GetIndex(dataID); if( i < 0 ) return RGB( 0, 0, 0); else return m_vpPlotData[i]->GetMarkerFillColor(); }
	inline void			SetMarkerFillColor( int dataID, COLORREF color ){int i = GetIndex(dataID); if( i < 0 ) return; else m_vpPlotData[i]->SetMarkerFillColor(color);}

public:
	inline int			GetDataFillMode( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetDataFillMode(); }
	inline void			SetDataFillMode( int dataID, int mode ){ int i = GetIndex( dataID ); if( i < 0 ) return; else {m_vpPlotData[i]->SetDataFillMode(mode); m_vpPlotData[i]->bForceUpdate = true;} }
	inline bool			IsDataFill( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->IsDataFill(); }
	inline void			SetDataFill( int dataID, bool bFill ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataFill(bFill); }
	inline int			GetDataFillBrushType( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetDataFillBrushType(); }
	inline void			SetDataFillBrushType( int dataID, int type ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataFillBrushType(type); }
	inline COLORREF		GetDataFillColor( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetDataFillColor(); }
	inline void			SetDataFillColor( int dataID, COLORREF color ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataFillColor(color); }
	inline int			GetDataFillHatchStyle( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return -1; else return m_vpPlotData[i]->GetDataFillHatchStyle(); }
	inline void			SetDataFillHatchStyle( int dataID, int style ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataFillHatchStyle(style); }
	inline HBITMAP		GetDataFillPattern( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return NULL; else return m_vpPlotData[i]->GetDataFillPattern(); }
	inline void			SetDataFillPattern( int dataID, HBITMAP pattern ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataFillPattern(pattern); }
	inline int			GetDataFillAlpha( int dataID ){ int i = GetIndex(dataID); if( i < 0 ) return 0; else return m_vpPlotData[i]->GetDataFillAlpha(); }
	inline void			SetDataFillAlpha( int dataID, int alpha ){ int i = GetIndex( dataID ); if( i < 0 ) return; else m_vpPlotData[i]->SetDataFillAlpha(alpha); }

	void				SetScatter(int dataID);

};

template<class DataT>
CDataSet<DataT>::CDataSet()
{
	InitializeCriticalSection(&m_csData);
	SetDefaults();
}

template<class DataT>
CDataSet<DataT>::~CDataSet()
{
	DeleteAllData();
	DeleteCriticalSection(&m_csData);
}

template<class DataT>
void	CDataSet<DataT>::SetDefaults()
{
	m_nCurDataID = 0;

	m_bMultiSelect = false;

	m_nReservedDataLength = 0;

	m_nMaxPoints = -1;
	m_bMaxPointsReset = true;

	m_bPointSelectionMode = true;
	m_bExcludeDataWhenUniteColor = false;

	m_bSequence = false;
	m_bShareX = false;

	m_nShowPlotDataInfoMode = kShowPlotDataInfoEveryN;
	m_nShowPlotDataInfoNum = 1;
	m_nShowPlotDataInfoDataPointStep = 32;
	m_strFileSeparator = _TEXT("\r\n//##$$%%//\r\n");
	m_strDataSeparator = _TEXT("\t\t");
}

// Deletes a data set, and moves all the settings
// around in their arrays
template<class DataT>
bool	CDataSet<DataT>::DeleteData( int dataID )
{
	bool bRet = false;
	
	int count=GetPlotDataCount();
	for( int i = 0; i < count; i++ )
	{
		if( m_vpPlotData[i]->GetDataID() == dataID )
		{
			MyLock();
			// Delete this data set
			m_vpPlotData[i]->DeleteData();
			if(m_vpPlotData[i]->pPointsGdi)
			{
				delete []m_vpPlotData[i]->pPointsGdi;
				m_vpPlotData[i]->pPointsGdi = 0;
			}
			// Delete this XYDataInfo
			delete m_vpPlotData[i];
			m_vpPlotData.erase(m_vpPlotData.begin()+i);
			
			MyUnlock();
			
			bRet = true;
			break;
		}
	}
	
	return bRet;
}

template<class DataT>
bool	CDataSet<DataT>::EmptyData( int dataID )
{
	int nIndex = GetIndex(dataID);
	if(nIndex<0)return false;
	
	MyLock();
	m_vpPlotData[nIndex]->DeleteData();
	MyUnlock();
	return true;
}

// Deletes every data set
template<class DataT>
void	CDataSet<DataT>::DeleteAllData( void )
{
	bool		keepGoing = true;
	
	while( keepGoing == true && GetPlotDataCount() > 0 )
	{
		keepGoing = DeleteData( GetDataID(0) );
	}
	//SetXAutoRange();
	//SetYAutoRange();
}

template<class DataT>
void	CDataSet<DataT>::EmptyAllData( void )
{
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		EmptyData( GetDataID(i) );
	}
}

template<class DataT>
bool	CDataSet<DataT>::SetDataModified( int dataID )
{
	unsigned int index;
	if( dataID < 0 )
	{
		for(index=0; index<m_vpPlotData.size(); index++)
		{
			m_vpPlotData[index]->SetDataRangeSet(false);
			m_vpPlotData[index]->GetDataRange( );
		}
		// all have been modified
	}
	else
	{
		index = GetIndex(dataID);
		if(index<0)return false;
		
		m_vpPlotData[index]->SetDataRangeSet(false);
		m_vpPlotData[index]->GetDataRange( );
		// one has been modified
	}
	
	return true;
}

template<class DataT>
int		CDataSet<DataT>::GetMaxPlotDataLength()
{
	int len = 0;
	for(int i = 0; i < GetPlotDataCount(); i++)
	{
		if((int)GetAbsData(i).size()>len)len = (int)GetAbsData(i).size();
	}
	return len;
}

template<class DataT>
bool	CDataSet<DataT>::IsEmpty()
{
	if(m_vpPlotData.size()<=0)return true;
	//if(GetMaxPlotDataLength()<=0)return true;
	
	return false;
}

template<class DataT>
int		CDataSet<DataT>::GetDataID( int index) const
{
	if(index<0 || index>=GetPlotDataCount())
	{
		return -1;
	}
	else
	{
		return m_vpPlotData[index]->GetDataID();
	}
}

template<class DataT>
int		CDataSet<DataT>::GetIndex( int dataID ) const
{
	int			i;
	
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		if( m_vpPlotData[i]->GetDataID() == dataID )
		{
			return i;
		}
	}
	
	return -1;
}

template<class DataT>
int		CDataSet<DataT>::GetSelectedID()
{
	int			i;
	
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		if( m_vpPlotData[i]->IsSelected() )
		{
			return m_vpPlotData[i]->GetDataID();
		}
	}
	
	return -1;
}

template<class DataT>
int		CDataSet<DataT>::GetLightedID()
{
	int			i;
	
	for( i = 0; i < GetPlotDataCount(); i++ )
	{
		if( m_vpPlotData[i]->IsLighted() )
		{
			return m_vpPlotData[i]->GetDataID();
		}
	}
	
	return -1;
}

template<class DataT>
int		CDataSet<DataT>::GetSelectedPointIdx()
{
	int dataID = GetSelectedID();
	if(dataID<0)return -1;

	return GetPlotData(dataID)->GetIdxPtSelected();
}

template<class DataT>
int		CDataSet<DataT>::GetLightedPointIdx()
{
	int dataID = GetLightedID();
	if(dataID<0)return -1;
	
	return GetPlotData(dataID)->GetIdxPtLighted();
}

template<class DataT>
tstring		CDataSet<DataT>::GetDataTitle( int dataID )
{
	int i = GetIndex(dataID);
	if( i < 0 ) return _TEXT("");
	else return m_vpPlotData[i]->GetTitle();
}

template<class DataT>
void		CDataSet<DataT>::SetDataTitle( int dataID, tstring title )
{
	int index = GetIndex( dataID );
	if( index < 0 || index >= GetPlotDataCount() ) return;
	m_vpPlotData[index]->SetTitle( title );
}

template<class DataT>
tstring		CDataSet<DataT>::GetAbsDataTitle( int which )
{
	if(which<0 || which>=GetPlotDataCount()) return _TEXT("");
	else return m_vpPlotData[which]->GetTitle();
}

template<class DataT>
void		CDataSet<DataT>::SetAbsDataTitle( int which, tstring title )
{
	if(which<0 || which>=GetPlotDataCount()) return;
	else m_vpPlotData[which]->SetTitle( title );
}

template<class DataT>
CPlotDataImpl<DataT>	*CDataSet<DataT>::GetPlotData(int dataID)
{
	int index = GetIndex(dataID);
	if( index < 0 || index >= GetPlotDataCount() ) return NULL;
	return m_vpPlotData[index];
}

template<class DataT>
CPlotDataImpl<DataT>	*CDataSet<DataT>::GetAbsPlotData(int nIndex)
{
	if( nIndex < 0 || nIndex >= GetPlotDataCount() ) return NULL;
	return m_vpPlotData[nIndex];
}

template<class DataT>
const vector<DataT> &CDataSet<DataT>::GetData( int dataID ) const
{
	int index=GetIndex(dataID);
	if(index<0)index=0;
	return m_vpPlotData[index]->GetDataConst();
}

template<class DataT>
const vector<DataT> &CDataSet<DataT>::GetAbsData( int which ) const
{
	if(which<0 || which>GetPlotDataCount())which=0;
	return m_vpPlotData[which]->GetDataConst();
}

template<class DataT>
void	CDataSet<DataT>::UpdateXDataRange(double val, int nWhich)
{
	if(nWhich<0 || nWhich>=GetPlotDataCount())return;
	if(!m_vpPlotData[nWhich]->IsDataRangeSet())
	{
		m_vpPlotData[nWhich]->GetDataRange();
		m_vpPlotData[nWhich]->SetDataRangeSet(true);
	}
	if(val < m_vpPlotData[nWhich]->GetDataRange1D(0)[0] )m_vpPlotData[nWhich]->GetDataRange1D(0)[0] = val;
	if(val > m_vpPlotData[nWhich]->GetDataRange1D(0)[1] )m_vpPlotData[nWhich]->GetDataRange1D(0)[1] = val;
}

template<class DataT>
void	CDataSet<DataT>::UpdateYDataRange(double val, int nWhich)
{
	if(nWhich<0 || nWhich>=GetPlotDataCount())return;
	if(!m_vpPlotData[nWhich]->IsDataRangeSet())
	{
		m_vpPlotData[nWhich]->GetDataRange();
		m_vpPlotData[nWhich]->SetDataRangeSet(true);
	}
	if(val < m_vpPlotData[nWhich]->GetDataRange1D(1)[0] )m_vpPlotData[nWhich]->GetDataRange1D(1)[0] = val;
	if(val > m_vpPlotData[nWhich]->GetDataRange1D(1)[1] )m_vpPlotData[nWhich]->GetDataRange1D(1)[1] = val;
}

template<class DataT>
void	CDataSet<DataT>::UpdateDataRanges(DataT dtVal, int nWhich)
{
	if(nWhich<0 || nWhich>=GetPlotDataCount())return;
	if(GetAbsData(nWhich).size()<=1)
	{
		m_vpPlotData[nWhich]->SetDataRangeSet(false);
	}
	UpdateXDataRange(dtVal.val[0], nWhich);
	UpdateYDataRange(dtVal.val[1], nWhich);
}

template<class DataT>
void	CDataSet<DataT>::GetDataRange( int dataID )
{
	int index = GetIndex(dataID);
	if(index<0)return;

	m_vpPlotData[index]->GetDataRange();
}

template<class DataT>
void	CDataSet<DataT>::UnsetPlotDataDragMode()
{
	unsigned int i;
	for(i=0; i<m_vpPlotData.size(); i++)
	{
		m_vpPlotData[i]->bDragMode = false;
	}
}

template<class DataT>
bool	CDataSet<DataT>::SetPlotDataOffset(int dataID, double offsetx, double offsety)
{
	int index = GetIndex(dataID);
	if(index < 0)return false;
	
	double dx0, dy0;
	dx0 = m_vpPlotData[index]->fOffsetX;
	dy0 = m_vpPlotData[index]->fOffsetY;
	
	double dx, dy;
	dx = offsetx - dx0;
	dy = offsety - dy0;
	
	CPlotDataImpl<DataT> *pDataSet = GetAbsPlotData(index);
	for(int i=0; i<(int)pDataSet->GetDataCount(); i++)
	{
		pDataSet->GetData()[i].val[0] += dx;
		pDataSet->GetData()[i].val[1] += dy;
	}
	m_vpPlotData[index]->fOffsetX = offsetx;
	m_vpPlotData[index]->fOffsetY = offsety;
	
	if(fabs(dx)>fMinVal || fabs(dy)>fMinVal)
	{
		m_vpPlotData[index]->bDragMoved = true;
	}
	
	return true;
}

template<class DataT>
double	CDataSet<DataT>::GetPlotDataOffsetX(int dataID)
{
	int index = GetIndex(dataID);
	if(index < 0)return 0;
	
	return m_vpPlotData[index]->fOffsetX;
}

template<class DataT>
double	CDataSet<DataT>::GetPlotDataOffsetY(int dataID)
{
	int index = GetIndex(dataID);
	if(index < 0)return 0;
	
	return m_vpPlotData[index]->fOffsetY;
}

template<class DataT>
double	CDataSet<DataT>::GetMinOffsetX()
{
	double offset = fMaxVal;
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(m_vpPlotData[i]->fOffsetX<offset)offset = m_vpPlotData[i]->fOffsetX;
	}
	return offset;
}

template<class DataT>
double	CDataSet<DataT>::GetMinOffsetY()
{
	double offset = fMaxVal;
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(m_vpPlotData[i]->fOffsetY<offset)offset = m_vpPlotData[i]->fOffsetY;
	}
	return offset;
}

template<class DataT>
double	CDataSet<DataT>::GetMaxOffsetX()
{
	double offset = -fMaxVal;
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(m_vpPlotData[i]->fOffsetX>offset)offset = m_vpPlotData[i]->fOffsetX;
	}
	return offset;
}

template<class DataT>
double	CDataSet<DataT>::GetMaxOffsetY()
{
	double offset = -fMaxVal;
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(m_vpPlotData[i]->fOffsetY>offset)offset = m_vpPlotData[i]->fOffsetY;
	}
	return offset;
}

template<class DataT>
void	CDataSet<DataT>::ResetPlotDataOffset(int dataID)
{
	SetPlotDataOffset(dataID, 0.0, 0.0);
}

template<class DataT>
void	CDataSet<DataT>::RessetAllPlotDataOffset()
{
	for(int i=0; i<GetPlotDataCount(); i++)
	{
		SetPlotDataOffset(GetDataID(i), 0.0, 0.0);
	}
}

template<class DataT>
bool	CDataSet<DataT>::MovePlotData(int dataID, double offsetx, double offsety)
{
	int index = GetIndex(dataID);
	if(index < 0)return false;
	
	int i;
	for(i=0; i<(int)GetAbsData(index).size(); i++)
	{
		GetAbsPlotData(index)->GetData()[i].val[0] += offsetx;
		GetAbsPlotData(index)->GetData()[i].val[1] += offsety;
	}
	return true;
}

template<class DataT>
double		CDataSet<DataT>::GetValueFromX(double x, int which)
{
	if(which<0 || which>=GetPlotDataCount())return 0.0;
	
	if(x < GetAbsData(which)[0].val[0])return GetAbsData(which)[0].val[1];
	int i;
	for(i=0; i<(int)GetAbsData(which).size(); i++)
	{
		if(x>=GetAbsData(which)[i].val[0] && x<=GetAbsData(which)[i+1].val[0])
		{
			double x1, x2, y1, y2;
			x1 = GetAbsData(which)[i].val[0];
			x2 = GetAbsData(which)[i+1].val[0];
			y1 = GetAbsData(which)[i].val[1];
			y2 = GetAbsData(which)[i+1].val[1];
			if(x1==x2)return y1;
			return y1+(x-x1)*(y2-y1)/(x2-x1);
		}
	}
	return GetAbsData(which)[GetAbsData(which).size()-1].val[1];
}

template<class DataT>
int		CDataSet<DataT>::GetIndexOfFirstVisiblePlotData()
{
	int index = -1;
	for(int i=0; i<GetPlotDataCount(); i++)
	{
		if(m_vpPlotData[i]->IsVisible())
		{
			index = i;
			break;
		}
	}
	return index;
}

template<class DataT>
int		CDataSet<DataT>::GetIndexOfLastVisiblePlotData()
{
	int index = -1;
	for(int i=GetPlotDataCount()-1; i>=0; i--)
	{
		if(m_vpPlotData[i]->IsVisible())
		{
			index = i;
			break;
		}
	}
	return index;
}

template<class DataT>
int		CDataSet<DataT>::GetIndexOfNextVisiblePlotData(int idxCurr)
{
	int index = -1;
	if(idxCurr >= GetPlotDataCount())
	{
		return -1;
	}
	index = (idxCurr<0)?0:idxCurr+1;
	while(!IsVisible(GetDataID(index)))
	{
		index++;
		if(index >= GetPlotDataCount())
		{
			index = -1;
			break;
		}
	}
	return index;	
}

template<class DataT>
int		CDataSet<DataT>::GetIndexOfPrevVisiblePlotData(int idxCurr)
{
	int index = -1;
	if(idxCurr >= GetPlotDataCount())
	{
		return -1;
	}
	index = (idxCurr<0)?GetPlotDataCount()-1:idxCurr-1;
	while(!IsVisible(GetDataID(index)))
	{
		index--;
		if(index <0)
		{
			index = -1;
			break;
		}
	}
	return index;
}

template<class DataT>
int		CDataSet<DataT>::GetVisiblePlotDataCount()
{
	int count = 0;
	for(int i=0; i<GetPlotDataCount(); i++)
	{
		if(m_vpPlotData[i]->IsVisible())
			count++;
	}
	return count;
}

template<class DataT>
int		CDataSet<DataT>::GetDataIDByTitle(tstring title)
{
	if(title.length()<=0)return -1;
	int index = GetIndexByTitle(title);
	return GetDataID(index);
}

template<class DataT>
int		CDataSet<DataT>::GetIndexByTitle(tstring title)
{
	if(title.length()<=0)return -1;
	for(int i=0; i<GetPlotDataCount(); i++)
	{
		if(GetAbsDataTitle(i) == title)
		{
			return i;
		}
	}
	return -1;
}

template<class DataT>
CPlotDataImpl<DataT>	*CDataSet<DataT>::GetPlotDataByTitle( tstring title )
{
	if(title.length()<=0)return 0;
	
	int index = GetIndexByTitle(title);
	if(index<0)return 0;
	return GetAbsPlotData(index);
}

template<class DataT>
void	CDataSet<DataT>::SetScatter(int dataID)
{
	int nIndex = GetIndex(dataID); 
	if( nIndex < 0 ) return;
	
	SetPlotType(dataID, kXYPlotScatter);
	SetMarkerShow(dataID, true);
	SetMarkerFrequency(dataID, 1);
	if(GetMarkerType(dataID)==kXYMarkerNone)SetMarkerType(dataID, kXYMarkerCircle);
}

Declare_Namespace_End
