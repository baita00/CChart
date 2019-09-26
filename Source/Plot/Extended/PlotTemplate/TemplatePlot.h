#pragma once

#pragma warning(disable:4786)

#include <windows.h>
#include <vector>
using namespace std;

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart


class CTemplatePlot
{
public:
	CTemplatePlot();
	virtual ~CTemplatePlot();

	void	SetDefaults();

public:

};

Declare_Namespace_End