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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Three Dimension plot use the OpenGL API to draw on the screen

#pragma once

#include "../../Basic/CChartNameSpace.h"
#include "../../Basic/MyString.h"
#include "../../Basic/Axis.h"

#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glaux.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "glaux.lib")

Declare_Namespace_CChart

class CTDPlot
{
public:
	CTDPlot();
	~CTDPlot();

	void			SetDefaults();

protected:
	CAxis			*m_pAxes[3];

	GLfloat			m_fvBkgndColor[4];
	GLfloat			m_fvFrameColor[4];
	GLfloat			m_fvAxisPlaneColor[4];
	GLfloat			m_fvAxisColor[4];
	GLfloat			m_fvTickLabelColor[4];

public:
	GLfloat			m_pfOffset[3];
	GLfloat			m_pfLen[3];
	GLfloat			m_pfAngle[3];
/*	GLfloat			m_fLenX;
	GLfloat			m_fLenY;
	GLfloat			m_fLenZ;
	GLfloat			m_fAngleX;
	GLfloat			m_fAngleY;
	GLfloat			m_fAngleZ;
*/
protected:
	bool			m_bShowXYFrame;
	bool			m_bShowYZFrame;
	bool			m_bShowZXFrame;

	bool			m_bShowXYMajorGrid;
	bool			m_bShowXYMinorGrid;
	bool			m_bShowYZMajorGrid;
	bool			m_bShowYZMinorGrid;
	bool			m_bShowZXMajorGrid;
	bool			m_bShowZXMinorGrid;
	GLfloat			m_fvMajorGridColor[4];
	GLfloat			m_fvMinorGridColor[4];

	int				m_nNurbsPoints;

	GLfloat			m_fProjectionRange;
	GLfloat			m_fZoomFactor;
	GLfloat			m_fSF4AxisLabel;
	GLfloat			m_fSF4Title;
	GLfloat			m_fSF4AxisTickLabel;

	GLfloat			m_fvTitleColor[4];
	tstring			m_strTitle;
	bool			m_bTitleShow;

protected:
	HGLRC			m_hGLRC;
	RECT			m_rctLastClientRect;

	void			InitializeOpenGL(HWND hWnd);
	void			FinallizeOpenGL();

public:
	tstring			GetTitle() { return m_strTitle; }
	void			SetTitle( tstring title ) { m_strTitle = title; }
	bool			IsTitleShow() { return m_bTitleShow; }
	void			SetTitleShow(bool bShow) { m_bTitleShow = bShow; }
	bool			IsShowXYFrame() { return m_bShowXYFrame; }
	void			SetShowXYFrame( bool show ) { m_bShowXYFrame = show; }
	bool			IsShowYZFrame() { return m_bShowYZFrame; }
	void			SetShowYZFrame( bool show ) { m_bShowYZFrame = show; }
	bool			IsShowZXFrame() { return m_bShowZXFrame; }
	void			SetShowZXFrame( bool show ) { m_bShowZXFrame = show; }
	bool			IsShowXYMajorGrid() { return m_bShowXYMajorGrid; }
	void			SetShowXYMajorGrid( bool show ) { m_bShowXYMajorGrid = show; }
	bool			IsShowXYMinorGrid() { return m_bShowXYMinorGrid; }
	void			SetShowXYMinorGrid( bool show ) { m_bShowXYMinorGrid = show; }
	bool			IsShowYZMajorGrid() { return m_bShowYZMajorGrid; }
	void			SetShowYZMajorGrid( bool show ) { m_bShowYZMajorGrid = show; }
	bool			IsShowYZMinorGrid() { return m_bShowYZMinorGrid; }
	void			SetShowYZMinorGrid( bool show ) { m_bShowYZMinorGrid = show; }
	bool			IsShowZXMajorGrid() { return m_bShowZXMajorGrid; }
	void			SetShowZXMajorGrid( bool show ) { m_bShowZXMajorGrid = show; }
	bool			IsShowZXMinorGrid() { return m_bShowZXMinorGrid; }
	void			SetShowZXMinorGrid( bool show ) { m_bShowZXMinorGrid = show; }

	RECT			GetLastClientRect() {return m_rctLastClientRect;}
	void			SetLastClientRect(RECT rt) {m_rctLastClientRect = rt;}
	RECT			GetLastPlotRect() {return m_rctLastClientRect;}
	void			SetLastPlotRect(RECT rt) {m_rctLastClientRect = rt;}

	void			ResetTransform();
};

Declare_Namespace_End