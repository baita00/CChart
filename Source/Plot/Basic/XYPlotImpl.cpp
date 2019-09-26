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

#include "MyGdiPlus.h"
#include "XYPlotImpl.h" 
#include "MyMemDC.h"
#include "Handlers/Dialogs//ColorControl.h"

#include <iomanip>

using namespace NsCChart;

CXYPlotImpl::CXYPlotImpl()
{
}

CXYPlotImpl::~CXYPlotImpl()
{
	
}

RECT	CXYPlotImpl::AddtionalPlotRectAdjust(HDC hDC, RECT plotRect)
{
	return CLegendImpl<CXYPlotImpl>::AdjustByLegend(hDC, plotRect);
}

int		CXYPlotImpl::GetDataIDByPoint( HDC hDC, POINT point )
{
	if( RegionIdentify(hDC,point) != kXYRegionData )return -1;
	
	
	if(m_bLegendShow)
	{
		int n= LegendRegionIdentify(hDC,point);
		if(n>=0 && n<GetPlotDataCount())
		{
			return GetDataID(n);
		}
	}
	
	return GetNeareastPlotData(hDC,point);
}

void	CXYPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	if(IsEmpty())
	{
		DrawDefScreen(hDC, destRect);
		return;
	}
	
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
	bool bDB = IsDoubleBuffer();
	SetDoubleBuffer(false);
	CXYPlotBasicImpl::FinalDraw(hdc, destRect);
	SetDoubleBuffer(bDB);
	
	DrawLegend(hdc);
}


int		CXYPlotImpl::RegionIdentifyEx(HDC hDC, POINT point, bool &legendRegion, int &centerAxisRegion)
{
	int region = CXYPlotBasicImpl::RegionIdentifyEx(hDC, point, legendRegion, centerAxisRegion);

	int which = LegendRegionIdentify(hDC,point);
	if(which>=0)
	{
		legendRegion = true;
	}
	else
	{
		legendRegion = false;
	}

	return region;
}

void	CXYPlotImpl::UniteToColor( COLORREF color )
{
	CXYPlotBasicImpl::UniteToColor(color);
	SetLegendBorderColor(color);
}


void	CXYPlotImpl::SetApperanceMode(int mode)
{
	CPlotImpl::SetApperanceMode(mode);
	
	switch(mode)
	{
	case kExtraSmall:
		SetLegendFontHeight(8);
		GetLegendFont().lfWeight = FW_THIN;
		break;
	case kSmall:
		SetLegendFontHeight(10);
		GetLegendFont().lfWeight = FW_LIGHT;
		break;
	case kNormal:
		SetLegendFontHeight(12);
		GetLegendFont().lfWeight = FW_NORMAL;
		break;
	case kLarge:
		SetLegendFontHeight(16);
		GetLegendFont().lfWeight = FW_BOLD;
		break;
	case kExtraLarge:
		SetLegendFontHeight(24);
		GetLegendFont().lfWeight = FW_ULTRABOLD;
		break;
	default:
		break;
	}
}


CXYPlotImplI::CXYPlotImplI()
{
	
}

CXYPlotImplI::~CXYPlotImplI()
{
	
}

tstring		CXYPlotImplI::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	int i;
	double data[2];
	
	RECT plotRect = GetLastPlotRect();

	tostringstream ostr;
	tstring str = _TEXT("");
	switch(m_nPointDesType)
	{
	case kDesCoords:
		ostr.str(_TEXT(""));
		if(!PtInRect(&plotRect, point))
		{
			goto Elements;
		}
		else
		{
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				ostr<<_TEXT("Mouse point coords: ")<<point.x<<_TEXT(", ")<<point.y;
				break;
			case kLangChinese:
				ostr<<_TEXT("鼠标点坐标: ")<<point.x<<_TEXT(", ")<<point.y;
				break;
			}
		}
		str = ostr.str();
		break;
	case kDesXY:
		ostr.str(_TEXT(""));
		if(!PtInRect(&plotRect, point))
		{
			goto Elements;
		}
		else
		{
			LPToData(&point, data);
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				ostr<<_TEXT("Mouse point data: ")<<data[0]<<_TEXT(", ")<<data[1];
				break;
			case kLangChinese:
				ostr<<_TEXT("鼠标点数据: ")<<data[0]<<_TEXT(", ")<<data[1];
				break;
			}
		}
		str = ostr.str();
		break;
	case kDesXAndYAll:
		if(GetPlotDataCount()<=0)
		{
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				return _TEXT("No data");
			case kLangChinese:
				return _TEXT("无数据");
			}
		}
		if(!PtInRect(&plotRect, point))
		{
			goto Elements;
		}
		LPToData(&point, data);
		double xRange[2];
		GetXDataRange(xRange);
		if(data[0]-GetMinOffsetX()<xRange[0] || data[0]-GetMaxOffsetX()>xRange[1])
		{
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				return _TEXT("Exceed data range");
			case kLangChinese:
				return _TEXT("超过数据范围");
			}
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("At mouse row");
			ostr<<_TEXT("\r\nX Coord:");
			str = ostr.str();
			break;
		case kLangChinese:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("鼠标所在列");
			ostr<<_TEXT("\r\nX坐标:");
			str = ostr.str();
			break;
		}
		for(i=0; i<GetPlotDataCount(); i++)
		{
			if(GetAbsData(i).size()==0)continue;
			ostr.str(_TEXT(""));
			ostr<<fixed<<setprecision(2)<<data[0]-m_vpPlotData[i]->fOffsetX;
			str += ostr.str();
			if(i!=GetPlotDataCount()-1)
			{
				ostr.str(_TEXT(""));
				ostr<<_TEXT(",");
				str += ostr.str();
			}
		}
		switch(CGlobal::m_nLangurage)
		{
		case kLangEnglish:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("\r\nY Coord:");
			str += ostr.str();
			break;
		case kLangChinese:
			ostr.str(_TEXT(""));
			ostr<<_TEXT("\r\nY坐标:");
			str += ostr.str();
			break;
		}
		for(i=0; i<GetPlotDataCount(); i++)
		{
			if(GetAbsData(i).size()==0)continue;
			ostr.str(_TEXT(""));
			ostr<<fixed<<setprecision(2)<<GetValueFromX(data[0], i)-m_vpPlotData[i]->fOffsetY;
			str += ostr.str();
			if(i!=GetPlotDataCount()-1)
			{
				ostr.str(_TEXT(""));
				ostr<<_TEXT(",");
				str += ostr.str();
			}
		}
		break;
	case kDesXYNearest:
		for(i=0; i<GetPlotDataCount(); i++)
		{
			bool handled = false;
			if(GetAbsData(i).size()==0)continue;
			for(int j=0; j<(int)GetAbsData(i).size(); j++)
			{
				POINT scnPt;
				DataToLP((double *)(GetAbsData(i)[j].val), &scnPt);

				if(abs(point.x - scnPt.x)<=m_nPointDesAccuracy && abs(point.y - scnPt.y)<=m_nPointDesAccuracy)
				{
					ostr.str(_TEXT(""));
					switch(CGlobal::m_nLangurage)
					{
					case kLangEnglish:
						ostr<<fixed<<setprecision(2)<<_TEXT("Curve No:")<<i+1<<_TEXT("\nData point: ")<<GetAbsData(i)[j].def.x<<","<<GetAbsData(i)[j].def.y;
						break;
					case kLangChinese:
						ostr<<fixed<<setprecision(2)<<_TEXT("曲线序号:")<<i+1<<_TEXT("\n数据点: ")<<GetAbsData(i)[j].def.x<<","<<GetAbsData(i)[j].def.y;
						break;
					}
					str += ostr.str();
					handled = true;
					break;
				}
			}
			if(handled)break;
		}
		break;
	case kDesElements:
Elements:
		int centerAxis;
		bool bLegend;
		int region;
		region = RegionIdentifyEx(hDC, point, bLegend, centerAxis);
		if(bLegend)
		{
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				str = _TEXT("Legend region");
				break;
			case kLangChinese:
				str = _TEXT("图例区域");
				break;
			}
		}
		else if(centerAxis>=0)
		{
			str = GetAxisByLocation(centerAxis)->GetDescription();
		}
		else
		{
			switch(region)
			{
			case kXYRegionTitle:
				switch(CGlobal::m_nLangurage)
				{
				case kLangEnglish:
					str = _TEXT("Title region");
					break;
				case kLangChinese:
					str = _TEXT("标题区域");
					break;
				}
				break;
			case kXYRegionLeft:
			case kXYRegionRight:
			case kXYRegionBottom:
			case kXYRegionTop:
				CAxis *axis;
				axis = GetAxisByRegion(region);
				if(axis)
				{
					str = axis->GetDescription();
				}
				break;
			case kXYRegionData:
				int dataID;
				dataID = GetDataIDByPoint(hDC, point);
				if(dataID>=0)
				{
					ostr.str(_TEXT(""));
					switch(CGlobal::m_nLangurage)
					{
					case kLangEnglish:
						ostr<<_TEXT("Curve ")<<GetIndex(dataID)+1<<_TEXT(": ")<<GetDataTitle(dataID);
						break;
					case kLangChinese:
						ostr<<_TEXT("第")<<GetIndex(dataID)+1<<_TEXT("条曲线: ")<<GetDataTitle(dataID);
						break;
					}					
					str = ostr.str();
					if(fabs(GetPlotData(dataID)->fOffsetX)>fMinVal || fabs(GetPlotData(dataID)->fOffsetY)>fMinVal)
					{
						ostr.str(_TEXT(""));
						switch(CGlobal::m_nLangurage)
						{
						case kLangEnglish:
							ostr<<_TEXT("\r\nOffset: (")<<GetPlotData(dataID)->fOffsetX<<_TEXT(", ")<<GetPlotData(dataID)->fOffsetY<<_TEXT(")");
							break;
						case kLangChinese:
							ostr<<_TEXT("\r\n偏移: (")<<GetPlotData(dataID)->fOffsetX<<_TEXT(", ")<<GetPlotData(dataID)->fOffsetY<<_TEXT(")");
							break;
						}
						str += ostr.str();
					}
					break;
				}
				else
				{
					switch(CGlobal::m_nLangurage)
					{
					case kLangEnglish:
						str = _TEXT("Data region");
						break;
					case kLangChinese:
						str = _TEXT("数据区域");
						break;
					}
				}
				break;
			case kXYRegionOut:
				switch(CGlobal::m_nLangurage)
				{
				case kLangEnglish:
					str = _TEXT("Outer region");
					break;
				case kLangChinese:
					str = _TEXT("外部区域");
					break;
				}
				break;
			}
		}
		break;
	}
	if(IsOscilloScopeMode())
	{
		if(m_bEnableXZone)
		{
			ostr.str(_TEXT(""));
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				ostr<<__TEXT("\r\nX zone range:(")<<GetXZoneLower()<<__TEXT(", ")<<GetXZoneUpper()<<_TEXT(")\r\nX zone gap: ")<<GetXZoneGap();
				break;
			case kLangChinese:
				ostr<<__TEXT("\r\nX区间范围:(")<<GetXZoneLower()<<__TEXT(", ")<<GetXZoneUpper()<<_TEXT(")\r\nX区间间距: ")<<GetXZoneGap();
				break;
			}
			str += ostr.str();
		}
		if(m_bEnableYZone)
		{
			ostr.str(_TEXT(""));
			switch(CGlobal::m_nLangurage)
			{
			case kLangEnglish:
				ostr<<__TEXT("\r\nY zone range:(")<<GetYZoneLower()<<__TEXT(", ")<<GetYZoneUpper()<<_TEXT(")\r\nY zone gap: ")<<GetYZoneGap();
				break;
			case kLangChinese:
				ostr<<__TEXT("\r\nY区间范围:(")<<GetYZoneLower()<<__TEXT(", ")<<GetYZoneUpper()<<_TEXT(")\r\nY区间间距: ")<<GetYZoneGap();
				break;
			}
			str += ostr.str();
		}
	}
	return str;
}

void	CXYPlotImplI::FinalDraw(HDC hDC, RECT destRect)
{
	if(IsEmpty())
	{
		DrawDefScreen(hDC, destRect);
		return;
	}

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
	bool bDB = IsDoubleBuffer();
	SetDoubleBuffer(false);
	CXYPlotImpl::FinalDraw(hdc, destRect);
	SetDoubleBuffer(bDB);
	
	if(CGlobal::m_bEnableToolTip)
	{
		DrawTooltip(hdc, GetLastPlotRect());
	}

	DrawOscilloScope(hdc);
}

