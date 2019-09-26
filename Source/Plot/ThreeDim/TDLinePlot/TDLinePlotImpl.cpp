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

//#include "StdAfx.h"
#include <windows.h>
#include "TDLinePlotImpl.h"
#include "../TDPlot/Axis3DImpl.h"
#include "../../Basic/PlotUtility.h"

using namespace NsCChart;

CTDLinePlotImpl::CTDLinePlotImpl(HWND hWnd) : CTDPlotImpl(hWnd)
{
}

CTDLinePlotImpl::~CTDLinePlotImpl()
{
}

void	CTDLinePlotImpl::GetDataRange1D( int whichDim, double *range )
{
	int i;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		GetAbsPlotData(i)->GetDataRange();
	}
	range[0] = 1e63;
	range[1] = -1e63;
	for(i=0; i<GetPlotDataCount(); i++)
	{
		if(range[0]>GetAbsPlotData(i)->GetDataRange1D(whichDim)[0])range[0]=GetAbsPlotData(i)->GetDataRange1D(whichDim)[0];
		if(range[1]<GetAbsPlotData(i)->GetDataRange1D(whichDim)[1])range[1]=GetAbsPlotData(i)->GetDataRange1D(whichDim)[1];
	}
}

void	CTDLinePlotImpl::GetPlotRange( double *xRange, double *yRange, double *zRange )
{
	GetDataRanges(xRange, yRange);
	GetDataRange1D(2, zRange);
	
	double *pRange;
	double optRange[2];
	int	optNTicks, optMinorTicks;
	for(int i=0; i<3; i++)
	{
		if(i==0)pRange = xRange;
		else if(i==1)pRange = yRange;
		else pRange = zRange;
		
		if(pRange[0]==pRange[1])
		{
			if(pRange[0]==0.0)
			{
				pRange[0]-=0.5;
				pRange[1]+=0.5;
			}
			else
			{
				pRange[0]-=pow(10,floor(log10(fabs(pRange[0]))));;
				pRange[1]+=pow(10,floor(log10(fabs(pRange[0]))));;
			}
		}
		GetOptimalRange( pRange, optRange, &optNTicks, &optMinorTicks, false, true, true, true, false, false );
		memcpy(pRange, optRange, 2*sizeof(double));
		GetClsRange(i)->SetRange(pRange[0],pRange[1]);
		GetClsRange(i)->SetTickCount(optNTicks);
		GetClsRange(i)->SetMinorTickCount(optMinorTicks);
	}
}

void	CTDLinePlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	if(GetPlotDataCount() <= 0)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	double xRange[2], yRange[2], zRange[2];
	GetPlotRange(xRange, yRange, zRange);

	wglMakeCurrent( hDC, m_hGLRC);
	glPushMatrix();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);
	

	CTDPlotImpl::FinalDraw( hDC, destRect );
	SetPlotColors();
	DrawDataSet(xRange, yRange, zRange);
	
	glFlush();
	SwapBuffers( hDC );
	
	glPopMatrix();
	wglMakeCurrent(	hDC, NULL);
	m_rctLastClientRect = destRect;
}

void	CTDLinePlotImpl::DrawDataSet( double *xRange, double *yRange, double *zRange )
{
	for(int i=0; i<GetPlotDataCount(); i++)
	{
		glPushMatrix();
		DrawSinglePlotData(i, xRange, yRange, zRange);
		glPopMatrix();
	}
}

void		CTDLinePlotImpl::DrawSinglePlotData( int which, double *xRange, double *yRange, double *zRange )
{
	if(which<0 || which >= GetPlotDataCount() )return;
	CPlotData<DataPoint3D> *pDataSet = GetAbsPlotData(which);
	if(pDataSet->GetDataCount()<=0)return;

	double xScale, yScale, zScale;
	double x, y, z;
	DataPoint3D dp;
	xScale = m_pfLen[0]/(xRange[1] - xRange[0]);
	yScale = m_pfLen[1]/(yRange[1] - yRange[0]);
	zScale = m_pfLen[2]/(zRange[1] - zRange[0]);

	GLfloat *CtrlPoints;
	int NumOfCP;

	glColor3fv(GetAbsPlotData(which)->GetGlPlotColor());
	
	int plottype = GetAbsPlotData(which)->GetPlotType();
	if(plottype == kTDPlotBezier && pDataSet->GetDataCount()<4)
		plottype = kTDPlotConnected;

	int i,j;
	switch(plottype)
	{
	case kTDPlotScatter:
		glPointSize((GLfloat)GetAbsPlotData(which)->GetMarkerSize());
		glBegin(GL_POINTS);
		for(i=0; i<(int)pDataSet->GetDataCount(); i++)
		{
			dp = pDataSet->GetData()[i];
			x = xScale * ( dp.val[0] - xRange[0] );
			y = yScale * ( dp.val[1] - yRange[0] );
			z = zScale * ( dp.val[2] - zRange[0] );
			
			glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
		}
		glEnd();
		break;
	case kTDPlotConnected:
		glBegin(GL_LINE_STRIP);
		for(i=0; i<(int)pDataSet->GetDataCount(); i++)
		{
			dp = pDataSet->GetData()[i];
			x = xScale * ( dp.val[0] - xRange[0] );
			y = yScale * ( dp.val[1] - yRange[0] );
			z = zScale * ( dp.val[2] - zRange[0] );
			
			glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
		}
		glEnd();
		break;
	case kTDPlotBezier:
		if(pDataSet->GetDataCount() <= 32)
			NumOfCP = pDataSet->GetDataCount();
		else
			NumOfCP = 32;
		CtrlPoints = new GLfloat[3*NumOfCP];
		for(i=0; i<NumOfCP; i++)
		{
			if(pDataSet->GetDataCount() <= 32)
			{
				j=i;
			}
			else
			{
				j=int(i*(pDataSet->GetDataCount()-1.0f)/GLfloat(NumOfCP-1.0f));
			}
			dp = pDataSet->GetData()[j];
			x = xScale * ( dp.val[0] - xRange[0] );
			y = yScale * ( dp.val[1] - yRange[0] );
			z = zScale * ( dp.val[2] - zRange[0] );
			
			CtrlPoints[3*i+0] = (GLfloat)x;
			CtrlPoints[3*i+1] = (GLfloat)y;
			CtrlPoints[3*i+2] = (GLfloat)z;
		}

		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, NumOfCP, CtrlPoints);
		glEnable(GL_MAP1_VERTEX_3);
		glShadeModel(GL_FLAT);
		
		glBegin(GL_LINE_STRIP);
		for(i=0; i<=m_nNurbsPoints; i++)
		{
			glEvalCoord1f((GLfloat)i/(GLfloat)m_nNurbsPoints);
			//glVertex3f(CtrlPoints[3*i+0], CtrlPoints[3*i+1], CtrlPoints[3*i+2]);
		}
		glEnd();

		glDisable(GL_MAP1_VERTEX_3);

		delete []CtrlPoints;
		
		break;
	default:
		break;
	}

	if(GetAbsPlotData(which)->IsMarkerShow())
		DrawMarkers(which, xRange, yRange, zRange);
}

void	CTDLinePlotImpl::DrawMarkers( int which, double *xRange, double *yRange, double *zRange )
{
	if(which<0 || which >= GetPlotDataCount() )return;
	CPlotDataImpl<DataPoint3D> *pDataSet = GetAbsPlotData(which);
	
	double xScale, yScale, zScale;
	double x, y, z;
	DataPoint3D dp;
	xScale = m_pfLen[0]/(xRange[1] - xRange[0]);
	yScale = m_pfLen[1]/(yRange[1] - yRange[0]);
	zScale = m_pfLen[2]/(zRange[1] - zRange[0]);

	glColor3fv(GetAbsPlotData(which)->GetGlPlotColor());

	glPointSize((GLfloat)GetAbsPlotData(which)->GetMarkerSize());
	glBegin(GL_POINTS);
	for(int i=0; i<(int)pDataSet->GetDataCount(); i++)
	{
		dp = pDataSet->GetData()[i];
		x = xScale * ( dp.val[0] - xRange[0] );
		y = yScale * ( dp.val[1] - yRange[0] );
		z = zScale * ( dp.val[2] - zRange[0] );
		
		glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
	}
	glEnd();
}

//////////////////////////////////////////////////////////////////////////////////
//

CTDLinePlotImplI::CTDLinePlotImplI( HWND hWnd) : CTDLinePlotImpl(hWnd)
{

}

CTDLinePlotImplI::~CTDLinePlotImplI()
{

}
