#pragma once

#include <windows.h>
#include <tchar.h>

#include <string>
using namespace std;

namespace NsCChart{

class CGlFont                                                            
{
protected:
	HFONT	m_hFont;
	float	m_fExtrusion;
public:
	CGlFont();
	~CGlFont();

	void	DeleteMyFont();
	bool	InitFont(const TCHAR *fontName);
	bool	InitFont(LOGFONT &lf);

	HFONT	GetFontHandle(){return m_hFont;}
	LOGFONT	GetLogFont();

	// Get font bitmap from gdi,then draw in the scene
	void	Draw2DTextByGdi(const TCHAR *str, float x = 0.0, float y = 0.0);
	// Get font bitmap use wglUseFontBitmaps, then draw in the scene
	void	Draw2DText(const TCHAR *str, float x = 0.0, float y = 0.0);
	// Use wglUseFontOutlines,return the length of the str
	float	Draw3DText(const TCHAR *str);

	float	Get3DLength(const TCHAR *str);
	float	Get3DHeight();
	float	GetExtrusion() {return m_fExtrusion;}
	void	SetExtrusion(float extrusion) {m_fExtrusion = extrusion;}
};

//Namespace end
}
