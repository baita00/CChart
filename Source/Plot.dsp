# Microsoft Developer Studio Project File - Name="Plot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Plot - Win32 DebugU
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Plot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Plot.mak" CFG="Plot - Win32 DebugU"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Plot - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Plot - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Plot - Win32 ReleaseU" (based on "Win32 (x86) Static Library")
!MESSAGE "Plot - Win32 DebugU" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "Plot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /Zm600 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Bin\Plot.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=lib /out:Bin\Plot.lib Bin\Plot.lib plot\accessary\gdiplus\lib\gdiplus.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Plot - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /Zm600 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Bin\Plotd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=lib /out:Bin\Plotd.lib Bin\Plotd.lib plot\accessary\gdiplus\lib\gdiplus.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Plot - Win32 ReleaseU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Plot___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "Plot___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /YX /FD /Zm600 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Bin\Plot.lib"
# ADD LIB32 /nologo /out:"Bin\Plotu.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=lib /out:Bin\Plotu.lib Bin\Plotu.lib plot\accessary\gdiplus\lib\gdiplus.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Plot - Win32 DebugU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Plot___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Plot___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /FR /YX /FD /GZ /Zm600 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Bin\Plotd.lib"
# ADD LIB32 /nologo /out:"Bin\Plotud.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=lib /out:Bin\Plotud.lib Bin\Plotud.lib plot\accessary\gdiplus\lib\gdiplus.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Plot - Win32 Release"
# Name "Plot - Win32 Debug"
# Name "Plot - Win32 ReleaseU"
# Name "Plot - Win32 DebugU"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\AnalogClock.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\AnalogClockImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\AutoSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Axes.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\AxesHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\AxesImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Axis.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\Axis3DImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisArrowsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisColorBarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\AxisImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisMajorTicksDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisMinorTicksDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisMiscellaneousDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\AxisPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisRangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisTickLabelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisTitleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BendingPlot\BendingPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BendingPlot\BendingPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\BkgndDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\BkgndPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\Powell\Bobyqa.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BoxPlot\BoxHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BoxPlot\BoxPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BoxPlot\BoxPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BubblePlot\BubblePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BubblePlot\BubblePlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Canvas.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\CanvasImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\cellrange.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Wrapper\Chart.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Wrapper\Classless.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\Clock.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\close.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudLegend.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudLegendHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudLegendImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudReactStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\ColorControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\Comment.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\CommentImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\Comments.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\CommentsHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\CommentsImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Compass\Compass.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Compass\CompassImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\Contour.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourBasicPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourLineHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourLinePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourLinePlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourPlot\ContourPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourPlot\ContourPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\CopyRightDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\ctype.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\CursorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataChartTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataFillDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DataHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataLineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataManagementDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataMarkersDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\DataPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\DataProcessing.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDLinePlot\DataSet3DImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DataSetImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataTitleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DefaultHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\DefContextMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\DefOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\Dial.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\DialImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Dib\DIB.CPP
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\DigitalClock.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\DigitalClockImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DragStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\endfile.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\err.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\ExtraFunctionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Dib\FASTBLT.CPP
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\FixAxisPlot\FixAxisPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\FixAxisPlot\FixAxisPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\fmt.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\fmtlib.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\ftruncate.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\g_cellseries.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\g_checkbox.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\g_editbox.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\GanttPlot\GanttPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\GanttPlot\GanttPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\GlFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Gradienter\Gradienter.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Gradienter\GradienterImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\grid.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\grid_lib.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\gridcell.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Grids.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\GridsImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HistoPlot\HistoPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HistoPlot\HistoPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HugePlot\HugePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HugePlot\HugePlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\ImagePopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\InstruBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\InstruBaseImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\InterConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\InterConnectImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredAxesHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredDefaultHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredPlotBasicImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredReactStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Legend.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\LegendBasicImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\LegendDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\LegendHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\LegendImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\LegendPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ListContour.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\MajorGridsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\memdc.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergecell.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergecella.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergecellb.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergemanager.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\MinorGridsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\MultiDataOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\MultiPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\MultiPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\MyGdiPlus.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\MyGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\MyGroupBoxImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\MyMemDC.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\MyString.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\mytime.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\NlFit\NlFit.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ObjectPlot\ObjectPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ObjectPlot\ObjectPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\open.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\OscilloPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\OscilloScope.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\OscilloScopeHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\OscilloScopeImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\PageNote.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\PageNoteImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Dib\PALETTE.CPP
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\PartialDial.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\PartialDialImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\PieHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\PiePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\PiePlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Plot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotBasicImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotData.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataBarImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataComplexColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataComplexColorImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataErrorBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataErrorBarImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataFill.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataFillImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataHisto.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataHistoImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataMarkerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataPointInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataPointInfoImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataSpan.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataSpanImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayerBasicImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayerHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PlotWnd\PlotWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PolarPlot\PolarPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PolarPlot\PolarPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\Powell\Powell.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PreferHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RadarPlot\RadarPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RadarPlot\RadarPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Range.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\RangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\RangeImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\RangeManage.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\RangeManageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\RangeManageImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Ranges.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\Ranges3DImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\RangesImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\rectrgn.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RegionPlot\RegionPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RegionPlot\RegionPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\RingPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\RingPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Ruler\Ruler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Ruler\RulerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\ScrollBar\ScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\ScrollBar\ScrollBarImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\SelZoneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\SevenSegmentCode\SevenSegmentCode.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\SevenSegmentCode\SevenSegmentCodeImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\sfe.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXAxesHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXLayerHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXLayerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXReactStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\sig_die.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Slider\Slider.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Slider\SliderImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplinePlot\Spline.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplinePlot\SplinePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplinePlot\SplinePlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\SplitHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\SplitPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\SplitReactStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\StatusPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\StemPlot\StemHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\StemPlot\StemPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\StemPlot\StemPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\SubtitleImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\TabbedPlot\TabbedHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\TabbedPlot\TabbedPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\TabbedPlot\TabbedPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\TDHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDLinePlot\TDLinePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDLinePlot\TDLinePlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\TDPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\TDPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDSurfacePlot\TDSurfacePlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDSurfacePlot\TDSurfacePlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Thermometer\Thermometer.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Thermometer\ThermometerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Title.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\TitleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\TitleHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\TitleImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\TitlePopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\Tooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\TooltipHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\TooltipImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\TrafficLight\TrafficLight.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\TrafficLight\TrafficLightImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\UsedPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\util.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\variant.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\WaterfallPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\WaterfallPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Wheel\Wheel.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Wheel\WheelImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\wref.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\wrtfmt.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\libf2c\wsfe.c
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\XY3DPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\XY3DPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYPlotBasicImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYPlotImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYReactStatus.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\AnalogClock.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\AnalogClockImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\array.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\AutoSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Axes.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\AxesHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\AxesImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Axis.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\Axis3DImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisArrowsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisColorBarDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisColorDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\AxisImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisMajorTicksDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisMinorTicksDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisMiscellaneousDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\AxisPopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisRangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisTickLabelDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs\AxisTitleDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BendingPlot\BendingPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BendingPlot\BendingPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\BkgndDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\BkgndPopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BoxPlot\BoxHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BoxPlot\BoxPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BoxPlot\BoxPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BubblePlot\BubblePlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\BubblePlot\BubblePlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Canvas.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\CanvasImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\CChartNameSpace.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\cellrange.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Wrapper\Chart.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Wrapper\Classless.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\Clock.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudLegend.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudLegendHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudLegendImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\CloudPlot\CloudReactStatus.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\ColorControl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\Comment.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\CommentImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\Comments.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\CommentsHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\CommentsImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Compass\Compass.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Compass\CompassImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\Contour.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourBasicPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourLineHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourLinePlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ContourLinePlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourPlot\ContourPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourPlot\ContourPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\CopyRightDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\CursorDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataChartTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataFillDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DataHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataLineDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataManagementDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataMarkersDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\DataPopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\DataProcessing.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DataSet.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDLinePlot\DataSet3DImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DataSetImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\DataTitleDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DefaultHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefAxisSettingDialogs.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\DefContextMenu.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\DefOperation.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\Dial.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\DialImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Dib\DIB.H
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\DigitalClock.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Clock\DigitalClockImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\DragStatus.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\ExtraFunctionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Dib\FASTBLT.H
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\FixAxisPlot\FixAxisPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\FixAxisPlot\FixAxisPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\g_cellseries.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\g_checkbox.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\g_editbox.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\GanttPlot\GanttPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\GanttPlot\GanttPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\GlFont.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Global.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Gradienter\Gradienter.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Gradienter\GradienterImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\grid.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\grid_lib.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\gridcell.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Grids.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\GridsImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HistoPlot\HistoPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HistoPlot\HistoPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HugePlot\HugePlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\HugePlot\HugePlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\ImagePopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\InstruBase.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\InstruBaseImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\InterConnect.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\InterConnectImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredAxesHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredDefaultHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredPlotBasicImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\LayeredReactStatus.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Legend.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\LegendBasicImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\LegendDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\LegendHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\LegendImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\LegendPopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ContourLinePlot\ListContour.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\MajorGridsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\matrix.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\memdc.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergecell.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergecella.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergecellb.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\mergemanager.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\MinorGridsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefDataSettingDialogs\MultiDataOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\MultiPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\MultiPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\MyGdiPlus.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\MyGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\InstruBase\MyGroupBoxImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\MyMemDC.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\MyMenuID.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\MyString.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\mytime.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\NlFit\NlFit.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ObjectPlot\ObjectPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\ObjectPlot\ObjectPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\OscilloPopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\OscilloScope.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\OscilloScopeHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\OscilloScopeImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\PageNote.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\PageNoteImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Dib\PALETTE.H
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\PartialDial.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Dial\PartialDialImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\PieHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\PiePlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\PiePlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Plot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotBasicImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotData.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataBar.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataBarImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataComplexColor.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataComplexColorImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataErrorBar.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataErrorBarImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataFill.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataFillImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataHisto.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataHistoImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataMarker.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataMarkerImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataPointInfo.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataPointInfoImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataSpan.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotDataSpanImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\PlotHeaders.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\PlotInterface.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayer.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayerBasicImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayerHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\PlotLayerImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\PlotRedefinitions.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PlotUtility.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PlotWnd\PlotWnd.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PolarPlot\PolarPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PolarPlot\PolarPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\Numerical\Powell\Powell.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\PreferHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RadarPlot\RadarPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RadarPlot\RadarPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Range.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\RangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\RangeImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\RangeManage.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\RangeManageHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\RangeManageImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Ranges.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\Ranges3DImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\RangesImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\ReactStatus.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\rectrgn.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RegionPlot\RegionPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\RegionPlot\RegionPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\RingPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\PiePlot\RingPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Ruler\Ruler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Ruler\RulerImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\ScrollBar\ScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\ScrollBar\ScrollBarImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\SelZoneDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\SevenSegmentCode\SevenSegmentCode.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\SevenSegmentCode\SevenSegmentCodeImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXAxesHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXLayer.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXLayerHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXLayerImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\LayeredPlot\ShareXReactStatus.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Slider\Slider.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Slider\SliderImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplinePlot\Spline.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplinePlot\SplinePlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplinePlot\SplinePlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\SplitHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\SplitPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\SplitPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\SplitPlot\SplitReactStatus.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\StatusPopup.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\StemPlot\StemHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\StemPlot\StemPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\StemPlot\StemPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\SubtitleImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\TabbedPlot\TabbedHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\TabbedPlot\TabbedPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\TabbedPlot\TabbedPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\TDHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDLinePlot\TDLinePlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDLinePlot\TDLinePlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\TDPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDPlot\TDPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDSurfacePlot\TDSurfacePlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\ThreeDim\TDSurfacePlot\TDSurfacePlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Thermometer\Thermometer.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Thermometer\ThermometerImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Title.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\Dialogs\DefPlotSettingDialogs\TitleDlg.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\TitleHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\TitleImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\TitlePopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\Tooltip.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\TooltipHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\More\TooltipImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\TrafficLight\TrafficLight.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\TrafficLight\TrafficLightImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\Handlers\ContextMenu\UsedPopup.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Accessary\grid_lib\variant.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\WaterfallPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\WaterfallPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Wheel\Wheel.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Instruments\Wheel\WheelImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\XY3DPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Extended\XY3DPlot\XY3DPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYHandler.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYPlot.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYPlotBasicImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYPlotImpl.h
# End Source File
# Begin Source File

SOURCE=.\Plot\Basic\XYReactStatus.h
# End Source File
# End Group
# End Target
# End Project
