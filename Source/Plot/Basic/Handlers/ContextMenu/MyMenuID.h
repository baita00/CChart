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

#include "../../CChartNameSpace.h"
#include "../../Axis.h"
#include "../../Axes.h"
#include "../../Plot.h"
#include "../../PlotData.h"
#include "../../PlotDataMarker.h"
#include "../../Legend.h"

Declare_Namespace_CChart

const int		fontSizes[] = {8, 10, 12, 16, 20, 24, 30, 36, 42, 48, 54, 60, 72, 84, 96, 108 };
const int		nFontSizes = sizeof(fontSizes)/sizeof(fontSizes[0]);

// Some constants for the popup menus
const int		tickSizes[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
const int		nTickSizes				=	sizeof(tickSizes)/sizeof(tickSizes[0]);

const int		nLineSizes				=	10;

// These constants are for the popup menu support. They
// facilitate making and interpretation of the menus

const int		kMenuPlotSettings		=	32768;

// for title
const int		kMenuTitle				=	10;
const int		kMenuTitlePosition		=	kMenuTitle + 1 + nFontSizes + 2;
const int		nMenuTitlePosition		=	3;
const int		nMenuTitle				=	nFontSizes+nMenuTitlePosition+5;
const int		kMenuSubTitle			=	kMenuTitle+nMenuTitle;
const int		kMenuSubTitlePosition	=	kMenuSubTitle + 1 + nFontSizes + 2;
const int		nMenuSubTitle			=	nFontSizes+nMenuTitlePosition+5;

// for axes
const int		nMenuAxisArrows			=	2;
const int		nMenuAxisColorBar		=	5;

const int		kMenuAxisRange			=	100;
const int		kMenuAxisMinimum		=	(1*kMenuAxisRange);
const int		kMenuAxisAddtional		=	20;

const int		kMenuDataRange			=	100;
const int		kMenuDataMinimum		=	(1*kMenuDataRange+1000);

const int		kMenuAxisTitleShow		=	1;
const int		kMenuAxisTitleSize		=	2;
const int		kMenuAxisTitlePosition	=	(kMenuAxisTitleSize+nFontSizes);
const int		kMenuAxisTitleYVerticle	=	kMenuAxisTitlePosition+3;
const int		kMenuAxisLabelShow		=	(kMenuAxisTitleYVerticle+1);
const int		kMenuAxisLabelSize		=	kMenuAxisLabelShow + 2;
const int		kMenuAxisMajorTickCount	=	(kMenuAxisLabelSize+nFontSizes);
// The +1 below is because there is one additional tick size (which is the "Auto" item)
const int		kMenuAxisMajorTickPos	=	(kMenuAxisMajorTickCount+nTickSizes+1);
const int		kMenuAxisMinorTickCount	=	kMenuAxisMajorTickPos + kTickPositionCount;
const int		kMenuAxisMinorTickPos	=	(kMenuAxisMinorTickCount+nTickSizes+1);
const int		kMenuAxisArrows			=	kMenuAxisMinorTickPos + kTickPositionCount;
const int		kMenuAxisIndependent	=	kMenuAxisArrows + nMenuAxisArrows;
const int		kMenuAxisColorBar		=	kMenuAxisIndependent + 1;

// for background
const int		kHatchTypeCount			=	6;
const int		nGridsSizes				=	6;
const int		nGradientSizes			=	12;
const int		nBkgndImageSizes		=	2;
const int		nBkgndSizes				=	3+kBrushTypeCount+kHatchTypeCount+nBkgndImageSizes;
const int		nDataRegionSizes		=	4+kBrushTypeCount+kHatchTypeCount+nGradientSizes;
const int		nBorderSizes			=	1+nLineSizes;
const int		kMenuBackground			=	(kMenuAxisRange*kMaxAxes)+kMenuAxisMinimum+kMenuAxisAddtional;
const int		kMenuGrids				=	kMenuBackground;
const int		kMenuGradient			=	kMenuGrids + nGridsSizes;
const int		kMenuBkgnd				=	kMenuGradient + nGradientSizes;
const int		kMenuBkgndBrushType		=	kMenuBkgnd + 1;
const int		kMenuBkgndHatchType		=	kMenuBkgndBrushType + kBrushTypeCount;
const int		kMenuBkgndImage			=	kMenuBkgndHatchType + kHatchTypeCount + 2;
const int		kMenuDataRegion			=	kMenuBkgnd + nBkgndSizes;
const int		kMenuDataRegionBrushType=	kMenuDataRegion + 2;
const int		kMenuDataRegionHatchType=	kMenuDataRegionBrushType + kBrushTypeCount;
const int		kMenuDataRegionGradient	=	kMenuDataRegionHatchType + kHatchTypeCount + 2;
const int		kMenuBorder				=	kMenuDataRegion + nDataRegionSizes;

const int		kMenuBackgroundRange	=	100;

// for legend
const int		nMenuLegendMaxColumn	=	5;
const int		nMenuLegendFeature		=	5;
//const int		nMenuLegendPositionCount=	13;
const int		kMenuLegend				=	kMenuBackground+kMenuBackgroundRange;
const int		kMenuLegendFeature		=	kMenuLegend + 1;
const int		kMenuLegendBorder		=	kMenuLegendFeature + nMenuLegendFeature;
const int		kMenuLegendBkgnd		=	kMenuLegendBorder + 2 + nLineSizes;
const int		kMenuLegendBrushType	=	kMenuLegendBkgnd + 1;
const int		kMenuLegendHatchType	=	kMenuLegendBrushType + kBrushTypeCount;
const int		kMenuLegendPosition		=	kMenuLegendHatchType + kHatchTypeCount + 2;
const int		kMenuLegendColumns		=	kMenuLegendPosition + kLegendPositionCount;
const int		kMenuLegendRange		=	60;

// for data set
const int		kMenuChartType			=	0;
const int		kMenuLineSize			=	(1+kPlotTypeCount);
const int		kMenuLineStyle			=	(1+kMenuLineSize+nLineSizes);
const int		kMenuMarker				=	(1+kMenuLineStyle+5);
const int		kMenuMarkerType			=	1;
const int		kMenuMarkerFill			=	(kMenuMarkerType + kMarkerTypeCount);
const int		kMenuMarkerSize			=	(kMenuMarkerFill+1);
const int		nMarkerSizes			=	20;
const int		kMenuMarkerFrequency	=	(kMenuMarkerSize+nMarkerSizes);
const int		nMenuMarkerFrequency	=	4;

const int		kMenuDataManagement		=	(kMenuMarker + kMenuMarkerFrequency + nMenuMarkerFrequency);
const int		nMenuDataManagement		=	10;

const int		kMenuDataProcessing		=	(kMenuDataManagement + nMenuDataManagement);
const int		nMenuDataProcessing		=	20;
const int		kMenuDataProcTrans		=	0;
const int		nMenuDataProcTrans		=	3;
const int		kMenuDataProcCalculus	=	kMenuDataProcTrans + nMenuDataProcTrans;
const int		nMenuDataProcCalculus	=	2;
const int		kMenuDataProcSmoothing	=	kMenuDataProcCalculus + nMenuDataProcCalculus;
const int		nMenuDataProcSmoothing	=	2;
const int		kMenuDataProcFitting	=	kMenuDataProcSmoothing + nMenuDataProcSmoothing;
const int		nMenuDataProcFitting	=	10;	

const int		nMaxDataSetsMenus		=	256;
const int		kMenuDataSetsEnd		=	kMenuDataMinimum + kMenuDataRange * nMaxDataSetsMenus;

const int		kenuDataDelLightPoint	=	kMenuDataSetsEnd - 4;
const int		kMenuDataGlobalLoad		=	kMenuDataSetsEnd - 2;
const int		kMenuDataGlobalSave		=	kMenuDataSetsEnd - 1;
const int		kMenuDataGlobalShow		=	kMenuDataSetsEnd;

// for oscilloscope
const	int		nMenuOscilloStart = 1;
const	int		nMenuOscilloCursors = 8;
const	int		nMenuOscilloSelZone = 5;
const	int		nMenuOscilloDataDrag = 5;
const	int		kMenuOscillo = kMenuDataSetsEnd + 1;
const	int		kMenuOscilloStart = kMenuOscillo;
const	int		kMenuOscilloCursor = kMenuOscilloStart + nMenuOscilloStart;
const	int		kMenuOscilloSelZone = kMenuOscilloCursor + nMenuOscilloCursors;
const	int		kMenuOscilloDataDrag = kMenuOscilloSelZone + nMenuOscilloSelZone;
const	int		nMenuOscillo = nMenuOscilloStart + nMenuOscilloCursors + nMenuOscilloSelZone + nMenuOscilloDataDrag;

// for status
const	int		kMenuStatus				=	kMenuOscillo + nMenuOscillo;
const	int		kMenuStatusExtraFunciton=	kMenuStatus+2;
const	int		kMenuStatusEFRangeDrag	=	kMenuStatusExtraFunciton + 3;
const	int		kMenuStatusPointSelection	=	kMenuStatusEFRangeDrag + 4;
const	int		nMenuStatusExtraFunction=	10;
const	int		kMenuStatusTooltip		=	kMenuStatusExtraFunciton+nMenuStatusExtraFunction;
const	int		nMenuStatusTooltip		=	7;
const	int		kMenuStatusFloatTicks	=	kMenuStatusTooltip + nMenuStatusTooltip;
const	int		nMenuStatusFloatTicks	=	2;
const	int		kMenuStatusEqual		=	kMenuStatusFloatTicks + nMenuStatusFloatTicks;
const	int		nMenuStatusEqual		=	3;
const	int		kMenuStatusApperance	=	kMenuStatusEqual + nMenuStatusEqual;
const	int		nMenuStatus				=	2 + nMenuStatusExtraFunction + nMenuStatusTooltip + nMenuStatusFloatTicks + nMenuStatusEqual + kApperanceModeCount;

// for image
const	int		kMenuImage				=	kMenuStatus + nMenuStatus;
const	int		nMenuImage				=	3;

// for langurage
const	int		kMenuLang				=	kMenuImage + nMenuImage;
const	int		nMenuLang				=	kLangCount;

Declare_Namespace_End