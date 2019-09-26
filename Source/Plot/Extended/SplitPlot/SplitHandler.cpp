#include "SplitHandler.h"

Declare_Namespace_CChart

class CMultiPlotImplI;

template<class PlotImplIT>
class CSplitPlotImplI;

class CXYPlotImpl;
class CXYPlotImplI;
class CShareXPlotImplI;

HMENU	CreateContextMenu(CMultiPlotImplI *plot)
{
	return NULL;
}
bool DefPlotSettings( CMultiPlotImplI *plot, HDC hDC )
{
	return false;
}
bool DefAxisSettings( CMultiPlotImplI *plot, HDC hDC, int location )
{
	return false;
}
bool DefDataSettings( CMultiPlotImplI *plot, HDC hDC, int whichDataSet )
{
	return false;
}

/*
template<class PlotImplIT>
HMENU	CreateContextMenu(CSplitPlotImplI<PlotImplIT> *plot)
{

}
template<class PlotImplIT>
bool DefPlotSettings( CSplitPlotImplI<PlotImplIT> *plot, HDC hDC )
{

}
template<class PlotImplIT>
bool DefAxisSettings( CSplitPlotImplI<PlotImplIT> *plot, HDC hDC, int location )
{

}
template<class PlotImplIT>
bool DefDataSettings( CSplitPlotImplI<PlotImplIT> *plot, HDC hDC, int whichDataSet )
{

}
*/

HMENU	CreateContextMenu(CSplitPlotImplI<CXYPlotImpl> *plot)
{
	return 0;
}
bool DefPlotSettings( CSplitPlotImplI<CXYPlotImpl> *plot, HDC hDC )
{
	return false;
}
bool DefAxisSettings( CSplitPlotImplI<CXYPlotImpl> *plot, HDC hDC, int location )
{
	return false;
}
bool DefDataSettings( CSplitPlotImplI<CXYPlotImpl> *plot, HDC hDC, int whichDataSet )
{
	return false;
}

HMENU	CreateContextMenu(CSplitPlotImplI<CXYPlotImplI> *plot)
{
	return 0;
}
bool DefPlotSettings( CSplitPlotImplI<CXYPlotImplI> *plot, HDC hDC )
{
	return false;
}
bool DefAxisSettings( CSplitPlotImplI<CXYPlotImplI> *plot, HDC hDC, int location )
{
	return false;
}
bool DefDataSettings( CSplitPlotImplI<CXYPlotImplI> *plot, HDC hDC, int whichDataSet )
{
	return false;
}



HMENU	CreateContextMenu(CSplitPlotImplI<CShareXPlotImplI> *plot)
{
	return 0;
}
bool DefPlotSettings( CSplitPlotImplI<CShareXPlotImplI> *plot, HDC hDC )
{
	return false;
}
bool DefAxisSettings( CSplitPlotImplI<CShareXPlotImplI> *plot, HDC hDC, int location )
{
	return false;
}
bool DefDataSettings( CSplitPlotImplI<CShareXPlotImplI> *plot, HDC hDC, int whichDataSet )
{
	return false;
}

Declare_Namespace_End