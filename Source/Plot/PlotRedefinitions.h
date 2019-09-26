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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This header file redefined all the plotting classes in CChart library
// classes named CxxImpl is redefined as xxChart
// "I" followed means message interaction

namespace NsCChart{

typedef	CPlotImpl Chart;

typedef CXYPlotImpl XYChart;
typedef CXYPlotImplI XYChartI;

typedef CPolarPlotImpl PolarChart;
typedef CPolarPlotImplI PolarChartI;

typedef CSmithChart SmithChart;
typedef CSmithChartI SmithChartI;

typedef CFixAxisPlotImpl FixAxisChart;
typedef CFixAxisPlotImplI FixAxisChartI;

typedef CRadarPlotImpl RadarChart;
typedef CRadarPlotImplI RadarChartI;

typedef CGanttPlotImpl GanttChart;
typedef CGanttPlotImplI GanttChartI;

typedef CBubblePlotImpl BubbleChart;
typedef CBubblePlotImplI BubbleChartI;

typedef CXY3DPlotImpl XY3DChart;
typedef CXY3DPlotImplI XY3DChartI;

typedef CWaterfallPlotImpl WaterfallChart;
typedef CWaterfallPlotImplI WaterfallChartI;

typedef CSplitPlotImpl<CXYPlotImpl> SplitChart;
typedef CSplitPlotImplI<CXYPlotImpl> SplitChartI;

typedef CMultiPlotImpl MultiChart;
typedef CMultiPlotImplI MultiChartI;

typedef CShareXPlotImpl ShareXChart;
typedef CShareXPlotImplI ShareXChartI;

typedef CLRPlotImpl LRChart;
typedef CLRPlotImplI LRChartI;

typedef CTabbedPlotImpl TabbedChart;
typedef CTabbedPlotImplI TabbedChartI;

typedef CCloudPlotImpl	CloudChart;
typedef CCloudPlotImplI CloudChartI;

typedef CCloudPlotImpl ContourMapChart;
typedef CCloudPlotImplI ContourMapChartI;

typedef CContourLinePlotImpl	ContourLineChart;
typedef CContourLinePlotImplI	ContourLineChartI;

typedef CContourPlotImpl	ContourChart;
typedef CContourPlotImplI	ContourChartI;

typedef	CSplinePlotImpl	SplineChart;
typedef	CSplinePlotImplI	SplineChartI;

typedef CBendingPlotImpl BendChart;
typedef CBendingPlotImplI BendChartI;

typedef CObjectPlotImpl ObjChart;
typedef CObjectPlotImplI ObjChartI;

typedef	CPiePlotImpl	PieChart;
typedef CPiePlotImplI	PieChartI;

typedef	CRingPlotImpl	RingChart;
typedef CRingPlotImplI	RingChartI;

typedef CStemPlotImpl StemChart;
typedef CStemPlotImplI StemChartI;

typedef	StemChart BarChart;
typedef StemChartI BarChartI;

typedef CLayeredPlotImpl LayeredChart;
typedef CLayeredPlotImplI LayeredChartI;

typedef CHistoPlotImpl HistoChart;
typedef CHistoPlotImplI HistoChartI;

typedef CRegionPlotImpl RegionChart;
typedef CRegionPlotImplI RegionChartI;

typedef CBoxPlotImpl BoxChart;
typedef CBoxPlotImplI BoxChartI;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
}