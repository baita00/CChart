#pragma once

#include "TemplatePlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class CTemplatePlotImpl : public CTemplatePlot, public CXYPlotImpl
{
public:
	CTemplatePlotImpl();
	virtual ~CTemplatePlotImpl();

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("TemplatePlot");}
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

Declare_Namespace_End