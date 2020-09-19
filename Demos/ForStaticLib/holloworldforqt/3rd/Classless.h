#pragma once

#include "Chart.h"

namespace Classless
{
	CChart_API void	CreateChart();
	CChart_API void	DestroyChart();
	CChart_API bool	IsCreated();

	// ����һ����������
	CChart_API	HWND	CreatePopupChartWndGlobal(HWND hWndParent, int nChartType, TCHAR *wndTitle=_TEXT(""), int x=0, int y=0, int cx=CW_USEDEFAULT, int cy=CW_USEDEFAULT);
	// ����һ��������Ϣѭ���ĵ�������
	CChart_API	HWND	CreatePopupChartWndGlobalWithMsgLoop(HWND hWndParent, int nChartType, TCHAR *wndTitle=_TEXT(""), int x=0, int y=0, int cx=CW_USEDEFAULT, int cy=CW_USEDEFAULT);
	// ����һ���Ӵ���
	CChart_API	HWND	CreateSubChartWndGlobal(HWND hWndParent, int nChartType, TCHAR *wndTitle=_TEXT(""), int x=0, int y=0, int cx=CW_USEDEFAULT, int cy=CW_USEDEFAULT);

/////////////////////////////////////////////////////////////////////////////
// ���º�������CChartWnd
	// ���ݱ�ʶ������
	CChart_API int	GetChartIndex(int nChartID);
	// ������Ż�ñ�ʶ
	CChart_API int	GetChartID(int nChartIndex);
	// �����ͼ����
	CChart_API int	GetChartType(int nChartIndex=0);
	// �ı���ͼ����
	CChart_API bool	SetChartType(int nType, int nChartIndex=0);
	// �����Attach�Ĵ���
	CChart_API int	GetChartCount();

	// ճ���ڣ��������ʽ
	CChart_API int	Attach(HWND hWnd);
	// ճ���ڣ������ƻ�ͼ��
	CChart_API int	Attach(HWND hWnd, RECT rtClient);
	// ճ���ڣ�ճ���Ի�����, �����ؼ����
	CChart_API int	AttachDlg(HWND hDlg, HWND hCtrl);
	// ճ���ڣ�ճ���Ի����ϣ������ؼ�ID
	CChart_API int	AttachDlg(HWND hDlg, UINT nCtrlID);

	// ճ���ڣ�ͬʱ���û�ͼ����
	CChart_API int	Attach(HWND hWnd, int nType);
	// ճ���ڣ�ͬʱ���û�ͼ���ͣ������ƻ�ͼ��
	CChart_API int	Attach(HWND hWnd, RECT rtClient, int nType);
	// ճ���ڣ�ճ���Ի�����, �����ؼ����
	CChart_API int	AttachDlg(HWND hDlg, HWND hCtrl, int nType);
	// ճ���ڣ�ճ���Ի����ϣ������ؼ�ID
	CChart_API int	AttachDlg(HWND hDlg, UINT nCtrlID, int nType);
	// ж����
	CChart_API bool	Detach();
	CChart_API bool	Detach(HWND hWnd);
	// �ػ�
	CChart_API void	ReDraw();
	CChart_API void	ReDraw(HWND hWnd);
	CChart_API void	ReDraw(int nChartIndex);
	//CChart_API void	ReDraw(CChart *chart);

/////////////////////////////////////////////////////////////////////////////
//CChart_API  ���º�������CChart
	// ���öԻ����Ҽ��˵�ʹ�õ�����
	CChart_API void	SetLangurage(int nLang);
	// �������ݴ���˵��Ƿ���ʾ
	CChart_API void	SetDataProcessing(bool process);
	
	// �����Ƿ����ù�����ʾ
	CChart_API bool	IsEnableToolTip();
	// �����Ƿ����ù�����ʾ
	CChart_API void	SetEnableToolTip(bool enable);
	// �����ͼ���Ƿ���Ч
	CChart_API bool	CheckPlotType(int nType);	
	
	// ����Ĭ����ɫ���ñ�
	CChart_API void	SetColorTable(int nTableIndex);

	// �����Ƿ���ʾͼ������Ҽ��˵�
	CChart_API void	SetImagePopupMenu(bool bShow);

	// ����ͼ�񵽼�����
	CChart_API void		CopyImageToClipBoard(HWND hWnd);
	// ����ͼ���ļ�
	CChart_API void		SaveImageToFile(HWND hWnd, TCHAR *strFileName, TCHAR *strFileExt);
	// ��ӡͼ��
	CChart_API void		PrintImage(HWND hWnd);

	// ��ȡ�ڲ�Plotָ��
	CChart_API void		*GetPlot();
	// ��ȡͼ�����ͱ���
	CChart_API int			GetType();
	// ����ͼ�����ͱ��룬ע�⽫�����������
	CChart_API bool		SetType(int nType);
	// ����ͼ�����ͱ��룬�����û�ͼ���ڣ���Ҫ������ά��ͼ����ά��ͼ������hWnd
	CChart_API bool		SetType(int nType, HWND hWnd);
	// ����ͼ�����ͱ��룬ע�⽫�����������
	CChart_API bool		CreateChart(int nType);
	// ����ͼ�����ͱ��룬�����û�ͼ���ڣ���Ҫ������ά��ͼ����ά��ͼ������hWnd
	CChart_API bool		CreateChart(int nType, HWND hWnd);

	// �����Ƿ����ƻ�ͼ��
	CChart_API bool		IsRectConfined();
	// �����Ƿ����ƻ�ͼ��
	CChart_API void		SetRectConfined(bool confine);
	// ������������
	CChart_API bool		SetConfineRect(RECT ConfineRect);
	// ��ȡ��������
	CChart_API RECT		GetConfineRect();

	// �����ڲ��Ƿ�������
	CChart_API bool		IsEmpty();
	
	// �����������
	CChart_API void		ResetApperance();
	// ������ģʽ���
	CChart_API int		GetApperanceMode();
	// �������ģʽ
	CChart_API void		SetApperanceMode(int mode);

	// ���û�ͼ��ɺ��û�������Խ��Ż�ͼ
	CChart_API void		SetContinuePaint(bool ctn);
	// ������Ϣ������û�������Խ��Ŵ���
	CChart_API void		SetContinueReact(bool ctn);

	// �����Ƿ���Ӧ��������Ϣ
	CChart_API bool		IsReact();
	// �����Ƿ���Ӧ��������Ϣ
	CChart_API void		SetReact(bool react);
	// �����Ƿ���Ӧ�������������Ϣ
	CChart_API void		SetReactLButtonDown(bool react);
	// �����Ƿ���Ӧ�������̧����Ϣ
	CChart_API void		SetReactLButtonUp(bool react);
	// �����Ƿ���Ӧ���˫����Ϣ
	CChart_API void		SetReactLButtonDblClk(bool react);
	// �����Ƿ���Ӧ����ƶ���Ϣ
	CChart_API void		SetReactMouseMove(bool react);
	// �����Ƿ���Ӧ�Ҽ��˵���Ϣ
	CChart_API void		SetReactContextMenu(bool react);
	// �����Ƿ���Ӧ������Ϣ
	CChart_API void		SetReactKeyDown(bool react);
	// �����Ƿ���Ӧ����
	CChart_API void		SetReactTitle(bool react);
	// �����Ƿ���Ӧ������
	CChart_API void		SetReactAxes(bool react);
	// �����Ƿ���Ӧͼ��
	CChart_API void		SetReactLegend(bool react);
	// ������Ϣ��Ӧʱ�Ƿ��Զ�ˢ�£���������ĩβ��R�ļ���������Interactive
	// ��Ҫ����ʵʱ������ʾ,��ʱ����Ϊfalse
	CChart_API bool		IsAutoRedraw();
	// ������Ϣ��Ӧʱ�Ƿ��Զ�ˢ�£���������ĩβ��R�ļ���������Interactive
	// ��Ҫ����ʵʱ������ʾ,��ʱ����Ϊfalse
	CChart_API void		SetAutoRedraw(bool redraw);
	// ���û�ͼ��ƫ��
	CChart_API void		SetOffsetClient(int offsetX, int offsetY);

	// ��SetWnd()���õĴ��ھ����ͼ
	CChart_API void		OnDraw();
	// �Դ��ھ����ͼ
	CChart_API void		OnDraw(HWND hWnd);
	// ���豸�����Ļ�ͼ
	CChart_API void		OnDraw(HDC hDC);
	// �ڴ�����ѡ�������ͼ
	CChart_API void		OnDraw(HDC hDC, RECT destRect);
	// ���豸�����Ķ�Ӧ�Ĵ����ϣ�ѡ�������ͼ
	CChart_API void		OnDraw(HWND hWnd, RECT destRect);

	// ��������������Ϣ�������ػ棬���û����ݷ���ֵ�ػ�
	CChart_API bool		OnLButtonDown( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API bool		OnLButtonUp( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API bool		OnLButtonDblClk( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API bool		OnMouseMove( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API bool		OnMouseLeave( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API bool		OnMouseWheel( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	CChart_API bool		OnKeyDown( HWND hWnd, UINT key );

	// ��������������Ϣ����AutoRedrawΪ��������ػ�
	CChart_API void				OnLButtonDownR( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API void				OnLButtonUpR( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API void				OnLButtonDblClkR( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API void				OnMouseMoveR( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API void				OnMouseLeaveR( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API void				OnMouseWheelR( HWND hWnd, POINT point, UINT ctrlKey );
	CChart_API void				OnContextMenuR( HMENU hMenu, HWND hWnd, POINT point );
	CChart_API void				OnKeyDownR( HWND hWnd, UINT key );

	// ��Ӧ��Ϣ�������ػ棬���û����ݷ���ֵ�ػ�
	CChart_API bool		OnEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// ��Ӧ��Ϣ����AutoRedrawΪ��������ػ�
	CChart_API void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// �����Ҽ��˵������˵�����
	CChart_API void		SetPluginMenu(void (*fcnAddPluginMenu)( void *plot, HMENU addMenu, void *pPara ), int (*fcnInterpretPluginMenu)( void *plot, HWND hWnd, int result, void *pPara ), void *pParaAdd, void *pParaInt);
	// ���ػ���ʾȱʡ�Ҽ��˵�����ϲ���˵����ܿ�ʵ���Ҽ��˵�ȫ���Զ���
	CChart_API void		SetDefMenus(bool bShow);
	// �û��Զ����ͼ����
	CChart_API void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara);
	// �û��Զ����ͼ����
	CChart_API void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex);
	// �û��Զ�����²���
	CChart_API void		SetUserUpdateFunc(void	(*fcnUserUpdate)( void *pPara ), void *pPara);
	// �û��Զ�����²���
	CChart_API void		SetUserUpdateFunc(void	(*fcnUserUpdate)( void *pPara ), void *pPara, int nPlotIndex);
	
	// �û��Զ�������ƶ�ǰ������bContinue����Ϊfalse����Ĭ������
	CChart_API void		SetPreMouseMoveFunc(bool	(*fcnPreMouseMove)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ�����굥��ǰ������bContinue����Ϊfalse����Ĭ������
	CChart_API void		SetPreLButtonDownFunc(bool	(*fcnPreLButtonDown)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ������̧��ǰ������bContinue����Ϊfalse����Ĭ������
	CChart_API void		SetPreLButtonUpFunc(bool	(*fcnPreLButtonUp)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ������˫��ǰ������bContinue����Ϊfalse����Ĭ������
	CChart_API void		SetPreLButtonDblClkFunc(bool	(*fcnPreLButtonDblClk)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ��尴��ǰ������bContinue����Ϊfalse����Ĭ������
	CChart_API void		SetPreKeyDownFunc(bool	(*fcnPreKeyDown)( HDC hDC, UINT key, void *para, bool &bContinue ), void *pPara);
	
	// �û��Զ�������ƶ��������bContinue������
	CChart_API void		SetPostMouseMoveFunc(bool	(*fcnPostMouseMove)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ�����굥���������bContinue������
	CChart_API void		SetPostLButtonDownFunc(bool	(*fcnPostLButtonDown)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ������̧��������bContinue������
	CChart_API void		SetPostLButtonUpFunc(bool	(*fcnPostLButtonUp)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ������˫���������bContinue������
	CChart_API void		SetPostLButtonDblClkFunc(bool	(*fcnPostLButtonDblClk)( HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// �û��Զ��尴���������bContinue������
	CChart_API void		SetPostKeyDownFunc(bool	(*fcnPreKeyDown)( HDC hDC, UINT key, void *para, bool &bContinue ), void *pPara);

	// �����ͼ��ţ���������kTypeSplit
	CChart_API int			GetPlotIndexByMousePos(POINT point);
	
	// ������û�����ݵ�ʱ���Ƿ������ἰ����
	CChart_API void		SetDrawBasicAnyway(bool draw);
	// ������û�����ݵ�ʱ����ͼ�Ƿ������ἰ����
	CChart_API void		SetDrawBasicAnyway(bool draw, int nPlotIndex);
	// ����������ʱĬ����Ļ
	CChart_API void		SetDefScreen( void (*fcnDefScreen)( HDC hDC, RECT plotRect, void *pPara ), void *pPara );
	// ����������ʱ��ͼĬ����Ļ
	CChart_API void		SetDefScreen( void (*fcnDefScreen)( HDC hDC, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex );
	// ����Ĭ����Ļ�����ַ���
	CChart_API void		SetDefMainString(TCHAR *str);
	// ������ͼĬ����Ļ�����ַ���
	CChart_API void		SetDefMainString(TCHAR *str, int nPlotIndex);
	//  ����Ĭ����Ļ�ĸ��ַ���
	CChart_API void		SetDefSubString(TCHAR *str);
	//  ������ͼĬ����Ļ�ĸ��ַ���
	CChart_API void		SetDefSubString(TCHAR *str, int nPlotIndex);
	//  ����Ĭ����Ļ�ı���ͼ
	CChart_API void		SetDefImage(HBITMAP hbm);
	//  ������ͼĬ����Ļ�ı���ͼ
	CChart_API void		SetDefImage(HBITMAP hbm, int nPlotIndex);


	// �����ͼ����Ƿ��ڷ�Χ��
	CChart_API bool		CheckSubPlotIndex(int nPlotIndex);

	// ��ù�����ʾ�ַ���
	CChart_API TCHAR		*GetDescriptionByMousePos(HDC hDC, POINT point);
	// ���ù�����ʾ�ַ�������
	CChart_API void		SetToolTipType(int type);

	// ������������ģʽ���򿪺����Ӧ����Ϸ�
	CChart_API void		SetZoomModeBuildIn(bool zoom);
	// �������ŷ�Χ
	CChart_API void		ResetZoomedRanges();
	// ����ͼ����������ģʽ��ֻ�ǻ��ƣ������ڲ�����CChartWnd�����
	CChart_API void		SetZoomMode(bool zoom);
	// ����ͼ����������ʱX���������ϵ��
	CChart_API void		SetZoomFactorX(double fact);
	// ����ͼ����������ʱY���������ϵ��
	CChart_API void		SetZoomFactorY(double fact);
	// ����ͼ����������ʱ�������������ϵ��
	CChart_API void		SetZoomFactor(double fact);
	// ����ͼ����������ʱ���������ģ����ĵĴ��ż����ļ���ʼ��ע�͵���enum
	CChart_API void		SetZoomCenterMode(int center);
	// �ڲ����ú���
	CChart_API RECT		GetZoomedRect(RECT destRect);

	// ������ߣ�ǰ������������������
	// ���ĸ�������Ҫ���ڷ�����ͼ���ֲ���ͼ������X����ͼ����ʾ��ͼ��š�����������ͬ
	// ����ֵ�����ߵ�ID�ţ�ע�ⲻ����ţ�����-1��ʾ�������ʧ�ܡ�����������ͬ
	CChart_API int			AddCurve(double *pX, double *pY, int nLen, int nPlotIndex=0);
	// ������ߣ�����X����Ϊʱ�䣬���ַ�����ʽ����
	// �ַ�����ʽ��fmtȷ�����ο�C�⺯��strftime
	// ʱ��ֻ�ܾ�ȷ���룬��Ϊ�ڲ�ʹ��time_t
	CChart_API int			AddCurve(TCHAR **pStrTime, TCHAR *fmt, double *pY, int nLen, int nPlotIndex=0);
	// ��ӿ�����
	CChart_API int			AddCurve(int nPlotIndex=0);
	// ������������
	CChart_API int			UpdateCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// ����ID�Ÿ�����������
	CChart_API int			UpdateCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// �滻�������ݣ������·�Χ
	CChart_API int			ReplaceCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// ����ID���滻�������ݣ������·�Χ
	CChart_API int			ReplaceCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// �����ߵ�DataID���Index
	CChart_API int			GetIndex(int dataID, int nPlotIndex=0);
	// �����ߵ�Index���DataID
	CChart_API int			GetDataID(int nIndex, int nPlotIndex=0);
	// �������ߵ�dataID��������;���������콣����������Ҫ������
	CChart_API void		SetDataID(int newID, int nIndex, int nPlotIndex=0);

	// ���2ά���ߵ�һ�����ݵ�
	// nDataIndex��ʾ���ߵ����,
	// nDataIndex������ڵ�ǰ���ߵ�������������һ�����ߣ����С��0���ߴ��ڵ�ǰ���������������ʧ��
	CChart_API int			AddPoint2D(double x, double y, int nDataIndex=0, int nPlotIndex=0);
	// ������ݵ㣬����X����Ϊʱ�䣬���ַ�����ʽ����
	//�ַ���ӦΪ"20130528234428"����������ʱ���룬��14λ���֣��������
	// ʱ��ֻ�ܾ�ȷ���룬��Ϊ�ڲ�ʹ��time_t
	CChart_API int			AddPoint2D(TCHAR *strTime, TCHAR *fmt, double y, int nDataIndex=0, int nPlotIndex=0);
	// ����2ά���ߵ�һ�����ݵ�
	// nPos��ʾ��Ҫ�����λ��,
	CChart_API int			InsertPoint2D(double x, double y, int nPos, int nDataIndex=0, int nPlotIndex=0);

	// �����ά���ߵ�һ�����ݵ�
	CChart_API int			AddPoint3D(double x, double y, double z, int nDataIndex=0);
	// ���һ����ά����
	CChart_API int			AddCurve(double *pX, double *pY, double *pZ, int nLen);	

	// ���������������
	CChart_API void		ClrAllData();
	// �������ͼȫ����������
	CChart_API void		ClrPlotData(int nPlotIndex=0);
	// ���������������
	CChart_API void		ClrSingleData(int nDataIndex, int nPlotIndex=0);
	// ��յ�����������
	CChart_API void		EmptySingleData(int nDataIndex, int nPlotIndex=0);
	// Ϊ����Ԥ���ڴ�ռ䡣��ҪĿ���Ǽӿ��ٶ�
	// CChart�ڲ����ݲ���vector��������ݵ���࣬�������ݵ�����ӣ�CChart�����ϵ�Ϊvector���·����ڴ棬Ӱ���ٶ�
	// �������ʵ�����Ԥ�����ڴ棬���һ���Գɹ������ڴ�
	CChart_API void		SetReservedDataLength(int nLen, int nPlotIndex=0);
	// ������ߵ����ݵ���
	CChart_API int			GetDataPointsCount(int nDataIndex, int nPlotIndex=0);
	// ��ȡ���ݵ�x
	CChart_API double		GetDataPointX(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// ��ȡ���ݵ�y
	CChart_API double		GetDataPointY(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	//��ȡ���ݵ�
	CChart_API bool		GetDataPoint(double data[2], int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// ȥ�������ϵ������ݵ�
	CChart_API int			RemoveSinglePoint(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// ȥ�������ϵ�һ�����ݵ�
	CChart_API int			RemoveFirstPoint(int nDataIndex, int nPlotIndex=0);
	// ȥ�������ϵ�һ�����ݵ㣬ͬʱ�ڲ��������ƣ��ɱ�֤ռ���ڴ��λ�ò��䣬��Ч�ʱ�RemoveFirstPoint�͡�
	CChart_API int			RemoveFirstPointAndShift(int nDataIndex, int nPlotIndex=0);
	// ȥ�����������һ�����ݵ�
	CChart_API int			RemoveLastPoint(int nDataIndex, int nPlotIndex=0);
	// ����X����
	CChart_API int			SlipXValue(int nStep, int nDataIndex, int nPlotIndex=0);
	// ����Y����
	CChart_API int			SlipYValue(int nStep, int nDataIndex, int nPlotIndex=0);

	// ��ӱ�ͼ��һ��ֵ
	CChart_API int			AddPie(double value);
	// ��ӱ�ͼ��һ��ֵ��ͬʱ���øÿ���ı���
	CChart_API int			AddPie(double value, TCHAR* title);
	// �����ͼ��һ������
	CChart_API int			AddStems(double *pData, int nLen);
	// �����ͼ��һ�����У�ͬʱ���ø����еı���
	CChart_API int			AddStems(double *pData, int nLen, TCHAR* title);

	// ���ó����������ڵȸ���ͼ����ͼ
	CChart_API void		SetFieldFcn(double (*_pFieldFcn) (double, double));
#ifndef _WIN64
	// ���ó����������ڵȸ���ͼ����ͼ
	CChart_API void		SetFieldFcn(double (_stdcall *_pFieldFcn) (double, double));
#endif
	// ��ó�����ָ�룬���ڵȸ���ͼ����ͼ
	CChart_API double		(*GetFieldFcn( ))( double, double );
	// ��ӵȸ������ݵ�
	CChart_API void		AddContourPoint(double x, double y, double h);
	// ����ȸ������ݵ�
	CChart_API void		ClrContourPoints();
	// ���ñ�ͼ�������ı���
	CChart_API void		SetPieTitle(const TCHAR* title, int which);
	// ���ñ�ͼ����������ɫ
	CChart_API void		SetPieColor(COLORREF color, int which);
	// ���ñ�ͼ������
	CChart_API void		SetMaxPies(int maxp);
	// ��ñ�ͼ����������
	CChart_API LOGFONT		&GetPieTitleFont();
	
	// ������ͼ�������ӵı�ǩ
	CChart_API void		SetStemLabel(const TCHAR* label, int which);
	// ������ͼ�������еı���
	CChart_API void		SetStemTitle(const TCHAR* title, int nDataIndex);
	// ������ͼ����������ռ����
	CChart_API void		SetStemRatio(double ratio);
	// ������ͼ�����������Ὺʼ��
	CChart_API void		SetStemBase(int base);
	// ������ͼ���������
	CChart_API void		SetStemMaxNum(int num);

	// ���õȸ���ͼ�ȸ��ߵ�����
	CChart_API void		SetContourLineNum(int num);
	// ���õȸ���ͼ�ȸ��߻��Ƶľ��ȣ�nԽ�󣬾���Խ�ߣ�һ��5�Ϳ�����
	CChart_API void		SetContourPrecision(int n);
	// ������ʾ�ȸ�����ֵ
	CChart_API void		SetContourValueShow(bool show);
	
	// ������ͼ����ʼ��ɫ
	CChart_API void		SetContourMapColor1(COLORREF cr);
	// ������ͼ��������ɫ
	CChart_API void		SetContourMapColor2(COLORREF cr);
	
	// ���������ݵ����룬�����ǳ�����
	CChart_API void		SetContourByPoints();
	
	// ������ͼ������������Ϊͼ��
	CChart_API void		SetRightAxisAsLegend(bool as);
	// ������ͼ������������Ϊͼ��
	CChart_API void		SetTopAxisAsLegend(bool as);

	// ����һ��ͼ�㣬���ڷֲ���ͼ������X����ͼ
	CChart_API int			AddLayer();
	// ������ͼ���������ڷ�����ͼ������mode��ʾ����ģʽ��nRows��ʾ������nCols��ʾ����
	// ����ģʽ�μӱ��ļ�ͷ����ע�͵���enum�����Խ��ע�ͣ�����ֱ��ʹ����Щenum�ˡ�
	CChart_API void		ResizePlots(int mode, int nRows, int nCols);
	// ����ͼ�����������ڷֲ���ͼ������X����ͼ
	CChart_API void		ResizePlots(int nLayers);

	// �����ͼ����
	CChart_API int			GetSubPlotCount();

	// ����ͼʱ��õ�ǰ��ͼ���
	CChart_API int			GetTop();

	// ���ö����ͼ��ͼ���Ƿ���ɫ��һ��
	CChart_API void		SetUniteLayerColor(bool bUnite);
	// ���ö����ͼͼ����ɫ��һ��ʱ�Ƿ��������
	CChart_API void		SetExcludeDataWhenUniteColor(bool bExclude, int nPlotIndex);

	// ����������������ڶ����ͼΪ��ͼ��������
	CChart_API int			GetPlotDataCount(int nPlotIndex=0);
	// ���õ��������Ƿ���Ӧ��������Ϣ
	CChart_API void		SetDataReact(bool react, int nDataIndex, int nPlotIndex=0);
	// ǿ��ʹ��GDI+������Ⱦ
	CChart_API void		SetDataForceGdiPlus(bool bForce, int nDataIndex, int nPlotIndex=0);
	// ����������ߵı���,nDataIndex��ʾ���ߵ����
	CChart_API const TCHAR*		GetDataTitle(int nDataIndex, int nPlotIndex=0);
	// �������ݱ���
	CChart_API void		SetDataTitle(const TCHAR* title, int nDataIndex, int nPlotIndex=0);
	// ���������Զ�������ɫ
	CChart_API void		SetAutoDataColorGeneral(bool bAuto, int nPlotIndex=0);
	// �����Զ�������ɫ
	CChart_API void		SetAutoDataColor(bool bAuto, int nDataIndex, int nPlotIndex=0);
	// ���������ɫ
	CChart_API COLORREF	GetDataColor(int nDataIndex, int nPlotIndex=0);
	// ����������ɫ
	CChart_API void		SetDataColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// �Ƿ�˫ɫ����ģʽ
	CChart_API bool		IsBiColorMode(int nDataIndex, int nPlotIndex=0);
	// ����˫ɫ����ģʽ
	CChart_API void		SetBiColorMode(bool bBi, int nDataIndex, int nPlotIndex=0);
	// ���˫ɫ����ģʽ�µڶ�������ɫ
	CChart_API COLORREF	GetDataColor2(int nDataIndex, int nPlotIndex=0);
	// ����˫ɫ����ģʽ�µڶ�������ɫ
	CChart_API void		SetDataColor2(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// �Ƿ��ɫ����ģʽ��������˫ɫ����
	CChart_API bool		IsMultiColorMode(int nDataIndex, int nPlotIndex=0);
	// ���ö�ɫ����ģʽ
	CChart_API void		SetMultiColorMode(bool bMul, int nDataIndex, int nPlotIndex=0);
	// ��ö�ɫ����ģʽ������ڵ�ķ�����0�����ݵ����0.0-1.0��1��X����ֵ��2��Y����ֵ
	CChart_API int			GetMultiColorInputType(int nDataIndex, int nPlotIndex=0);
	// ���ö�ɫ����ģʽ������ڵ�ķ���
	CChart_API void		SetMultiColorInputType(int nType, int nDataIndex, int nPlotIndex=0);
	// ���ö�ɫ����ģʽ�Ľڵ㣬����ratio�ĵķ�Χ�����뷽������
	CChart_API void		AddSegmentColor(double ratio, COLORREF color, int nDataIndex, int nPlotIndex=0);
	// �Ƿ������ݵ㶨ɫ
	CChart_API bool		IsColorPtByPt(int nDataIndex, int nPlotIndex=0);
	// �Ƿ������ݵ㶨ɫ
	CChart_API void		SetColorPtByPt(bool bBy, int nDataIndex, int nPlotIndex=0);
	// ������ݵ���ɫ��ע�⣬��ӵ���ɫ���ݵ��������ݵ������ȫһ������Ȼû������
	CChart_API void		AddDataPointColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// �������ݵ���ɫ
	CChart_API void		SetDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// ɾ��һ�����ݵ���ɫ
	CChart_API void		EraseDataPointColor(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// ����һ�����ݵ���ɫ
	CChart_API void		InsertDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex=0);

	// �����������
	CChart_API int			GetDataLineStyle(int nDataIndex, int nPlotIndex=0);
	// ������������
	CChart_API void		SetDataLineStyle(int nStyle, int nDataIndex, int nPlotIndex=0);
	// ��������߿�
	CChart_API int			GetDataLineSize(int nDataIndex, int nPlotIndex=0);
	// ���������߿�
	CChart_API void		SetDataLineSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// �������ߵĻ��Ʒ�ʽ
	CChart_API void		SetPlotType(int nType, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ����Ƿ���ʾ
	CChart_API void		SetMarkerShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ����Ƿ����
	CChart_API void		SetMarkerFill(bool bFill, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ��ǵ���״
	CChart_API void		SetMarkerType(int nType, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ�Ĵ�С
	CChart_API void		SetMarkerSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ���Ƶ�ʣ�0��ʾ����ǣ���n��ʾÿ��n������һ�Σ���n��ʾһ�����n����
	CChart_API void		SetMarkerFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ��ǵ���ʼ�㣬���ڲ���ȫ�����ʱ
	CChart_API void		SetMarkerStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ��һ������
	CChart_API void		SetMarkFirstPoint(bool bMark, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ����һ������
	CChart_API void		SetMarkLastPoint(bool bMark, int nDataIndex, int nPlotIndex=0);

	// ��������ɢ����ʾģʽ
	CChart_API void		SetScatter(int nDataIndex, int nPlotIndex=0);
	// �������������ɫ
	CChart_API void		SetDataFillColor(bool bFill, COLORREF color, int nOpaque, int nFillMode, int nDataIndex, int nPlotIndex=0);
	// �����������ݰ���ʾ
	CChart_API void		SetDataBarMode(bool bShowBar, int nBarBase, int nBarSize, int nDataIndex, int nPlotIndex=0);
	// ����ѡ��͸���ģʽ
	CChart_API void		SetSLMode(int nMode, int nDataIndex, int nPlotIndex=0);
	// ��ȡ���ݵı�־��������;
	CChart_API bool		GetDataFlag(int nDataIndex, int nPlotIndex=0);
	// �������ݵı�־��������;
	CChart_API void		SetDataFlag(bool bFlag, int nDataIndex, int nPlotIndex=0);
	// ��ȡ���ݿɼ���־
	CChart_API bool		IsDataVisible(int nDataIndex, int nPlotIndex=0);
	//���������Ƿ�ɼ�
	CChart_API void		SetDataVisible(bool bVis, int nDataIndex, int nPlotIndex=0);
	// ����ѹ����ʾģʽ����Ҫ���ڴ�������ʱ����ʾ
	// ѹ����ʾģʽ�У�����������ݵ�����Ļ����ʾʱ��X��ͬ����ѹ����һ�����ݵ㣬ֵȡƽ����
	CChart_API void		SetDataCompactDraw(bool bCompact, int nDataIndex, int nPlotIndex=0);
	// �������ݰ�ֱ��ͼģʽ��ʾ
	CChart_API void		SetDataHistro(bool bHistro, int nDataIndex, int nPlotIndex=0);

	// ��ø������ߵ���ţ�Ŀǰֻ����������ͼ
	CChart_API int			GetIndexOfLighted();
	
	// �����������ݵ���ֵ�Ƿ���ʾ
	CChart_API void		SetDataValueShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ���ֵ��ʾ��ʽ��1ΪX��2ΪY��3ΪX:Y������Ϊ����ʾ
	CChart_API void		SetDataValueType(int nType, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ���ֵ�������С
	CChart_API void		SetDataValueFontSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ���ֵ��ʾƵ�ʣ�0��ʾ����ʾ����n��ʾÿ��n������ʾһ�Σ���n��ʾһ����ʾn����
	CChart_API void		SetDataValueFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ���ֵ��ʾ����ʼ�㣬���ڲ���ȫ����ʾʱ
	CChart_API void		SetDataValueStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex=0);

	// ���ʾ����ģʽ
	CChart_API bool		IsOscilloMode(int nPlotIndex=0);
	// ����ʾ����ģʽ
	CChart_API void		SetOscilloMode(bool bOscillo, int nPlotIndex=0);
	// ����Ƿ�������ʾ����ģʽ
	CChart_API bool		IsStartOscilloScope(int nPlotIndex=0);
	// �����Ƿ�������ʾ����ģʽ
	CChart_API void		SetStartOscilloScope(bool bOscillo, int nPlotIndex=0);
	// ����Ƿ�����϶�����
	CChart_API bool		IsEnableDataDrag();
	// �����Ƿ�����϶�����
	CChart_API void		SetEnableDataDrag(bool enable);
	// ����Ƿ�����϶��������
	CChart_API bool		IsEnableZoneDrag();
	// �����Ƿ�����϶��������
	CChart_API void		SetEnableZoneDrag(bool enable);
	// �ƶ�����
	CChart_API bool		MoveData(double offsetx, double offsety, int nDataIndex, int nPlotIndex=0);
	// ������ߵ�ˮƽƫ��
	CChart_API double		GetDataOffsetX(int nDataIndex, int nPlotIndex=0);
	// �������ߵ�ˮƽƫ��
	CChart_API void		SetDataOffsetX(double offset, int nDataIndex, int nPlotIndex=0);
	// ������ߵ���ֱƫ��
	CChart_API double		GetDataOffsetY(int nDataIndex, int nPlotIndex=0);
	// �������ߵ���ֱƫ��
	CChart_API void		SetDataOffsetY(double offset, int nDataIndex, int nPlotIndex=0);
	// ����X����λ����������ģ��ʾ����
	CChart_API void		SetXUnit(TCHAR *unit, int nDataIndex);
	// ����Y����λ����������ģ��ʾ����
	CChart_API void		SetYUnit(TCHAR *unit, int nDataIndex);
	// ��ͼ���н�����ʾY����ı����ߣ���������ģ��ʾ����
	CChart_API void		SetShowYSCaleOnly(bool only);
	// �����󶨵����
	CChart_API bool		IsHoldCursorToMouse(int nPlotIndex=0);
	// ���ù��󶨵����
	CChart_API void		SetHoldCursorToMouse(bool hold, int nPlotIndex=0);
	// �����󶨵�����
	CChart_API bool		IsHoldCursorToCurve(int nPlotIndex=0);
	// ���ù��󶨵�����
	CChart_API void		SetHoldCursorToCurve(bool hold, int nPlotIndex=0);
	// ��ù��󶨵��������
	CChart_API int			GetIndexToHoldCursor(int nPlotIndex=0);
	// ���ù��󶨵��������
	CChart_API void		SetIndexToHoldCursor(int nCurveIndex, int nPlotIndex=0);
	// �������ͷ����
	CChart_API bool		IsShowZeroArrow(int nPlotIndex=0);
	// ��������ͷ����
	CChart_API void		SetShowZeroArrow(bool show, int nPlotIndex=0);
	// ��ⴥ����ͷ����
	CChart_API bool		IsShowTrigArrow(int nPlotIndex=0);
	// ���ô�����ͷ����
	CChart_API void		SetShowTrigArrow(bool show, int nPlotIndex=0);
	// ��ʾˮƽ����ߣ���������ģ��ʾ����
	CChart_API void		SetShowHCursor(bool show, int nPlotIndex=0);
	// ��ʾ��ֱ����ߣ���������ģ��ʾ����
	CChart_API void		SetShowVCursor(bool show, int nPlotIndex=0);
	// ��ù������ɫ����������ģ��ʾ����
	CChart_API COLORREF	GetCursorColor(int nPlotIndex=0);
	// ���ù������ɫ����������ģ��ʾ����
	CChart_API void		SetCursorColor(COLORREF color, int nPlotIndex=0);
	// ��ù�����߿���������ģ��ʾ����
	CChart_API int			GetCursorSize(int nPlotIndex=0);
	// ���ù�����߿���������ģ��ʾ����
	CChart_API void		SetCursorSize(int size, int nPlotIndex=0);
	// ��ù�������ͣ���������ģ��ʾ����
	CChart_API int			GetCursorStyle(int nPlotIndex=0);
	// ���ù�������ͣ���������ģ��ʾ����
	CChart_API void		SetCursorStyle(int style, int nPlotIndex=0);
	// ���ˮƽ���ֵ����������ģ��ʾ����
	CChart_API double		GetCursorX(int nPlotIndex=0);
	// ����ˮƽ���ֵ����������ģ��ʾ����
	CChart_API void		SetCursorX( double cursor, int nPlotIndex=0);
	// �����ֱ���ֵ����������ģ��ʾ����
	CChart_API double		GetCursorY(int nPlotIndex=0);
	// ������ֱ���ֵ����������ģ��ʾ����
	CChart_API void		SetCursorY( double cursor, int nPlotIndex=0);
	// ��ʾˮƽѡ��������������ģ��ʾ����
	CChart_API void		SetShowHSel(bool show, int nPlotIndex=0);
	// ��ʾ��ֱѡ��������������ģ��ʾ����
	CChart_API void		SetShowVSel(bool show, int nPlotIndex=0);
	// ���ˮƽѡ���������ޣ���������ģ��ʾ����
	CChart_API double		GetHSelLower(int nPlotIndex=0);
	// ����ˮƽѡ���������ޣ���������ģ��ʾ����
	CChart_API void		SetHSelLower(int val, int nPlotIndex=0);
	// ���ˮƽѡ���������ޣ���������ģ��ʾ����
	CChart_API double		GetHSelUpper(int nPlotIndex=0);
	// ����ˮƽѡ���������ޣ���������ģ��ʾ����
	CChart_API void		SetHSelUpper(int val, int nPlotIndex=0);
	// �����ֱѡ���������ޣ���������ģ��ʾ����
	CChart_API double		GetVSelLower(int nPlotIndex=0);
	// ������ֱѡ���������ޣ���������ģ��ʾ����
	CChart_API void		SetVSelLower(int val, int nPlotIndex=0);
	// �����ֱѡ���������ޣ���������ģ��ʾ����
	CChart_API double		GetVSelUpper(int nPlotIndex=0);
	// ������ֱѡ���������ޣ���������ģ��ʾ����
	CChart_API void		SetVSelUpper(int val, int nPlotIndex=0);
	// ���ˮƽѡ�����Ŀ�ȣ���������ģ��ʾ����
	CChart_API double		GetHSelWidth(int nPlotIndex=0);
	// �����ֱѡ�����Ŀ�ȣ���������ģ��ʾ����
	CChart_API double		GetVSelWidth(int nPlotIndex=0);


	// ���ͼ�����
	CChart_API TCHAR*		GetTitle();
	// �����ͼ���⣬�����ڷ�����ͼ���ֲ���ͼ������X����ͼ
	CChart_API TCHAR*		GetTitle(int nPlotIndex);
	// ����ͼ�����
	CChart_API void		SetTitle(const TCHAR* title);
	// ������ͼ����
	CChart_API void		SetTitle(const TCHAR* title, int nPlotIndex);
	// ���ͼ�����λ��
	CChart_API int			GetTitlePosition();
	// �����ͼ����λ��
	CChart_API int			GetTitlePosition(int nPlotIndex);
	// ����ͼ�����λ�á���������0���У���������
	CChart_API void		SetTitlePosition(int position);
	// ������ͼ����λ�á���������0���У���������
	CChart_API void		SetTitlePosition(int position, int nPlotIndex);
	// ͼ������Ƿ���ʾ
	CChart_API bool		IsTitleShow();
	// ��ͼ�����Ƿ���ʾ
	CChart_API bool		IsTitleShow(int nPlotIndex);
	// ������ʾͼ�����
	CChart_API void		SetTitleShow(bool show);
	// ������ʾ��ͼ����
	CChart_API void		SetTitleShow(bool show, int nPlotIndex);
	// ���ͼ�������ɫ
	CChart_API COLORREF	GetTitleColor();
	// �����ͼ������ɫ
	CChart_API COLORREF	GetTitleColor(int nPlotIndex);
	// ����ͼ�������ɫ
	CChart_API void		SetTitleColor(COLORREF color);
	// ������ͼ������ɫ
	CChart_API void		SetTitleColor(COLORREF color, int nPlotIndex);
	// ��ñ�������
	CChart_API LOGFONT		GetTitleFont();
	// �����ͼ��������
	CChart_API LOGFONT		GetTitleFont(int nPlotIndex);
	// ���ñ�������
	CChart_API void		SetTitleFont(LOGFONT logFont);
	// ������ͼ��������
	CChart_API void		SetTitleFont(LOGFONT logFont, int nPlotIndex);
	// �ֲ���ͼ��ʾ������
	CChart_API void		SetShowParentTitle(bool set);

	// ���ͼ�񸱱���
	CChart_API TCHAR*		GetSubTitle();
	// �����ͼ�����⣬�����ڷ�����ͼ���ֲ���ͼ������X����ͼ
	CChart_API TCHAR*		GetSubTitle(int nPlotIndex);
	// ����ͼ�񸱱���
	CChart_API void		SetSubTitle(const TCHAR* title);
	// ������ͼ������
	CChart_API void		SetSubTitle(const TCHAR* title, int nPlotIndex);
	// ���ͼ�񸱱���λ��
	CChart_API int			GetSubTitlePosition();
	// �����ͼ������λ��
	CChart_API int			GetSubTitlePosition(int nPlotIndex);
	// ����ͼ�񸱱���λ�á���������0���У���������
	CChart_API void		SetSubTitlePosition(int position);
	// ������ͼ������λ�á���������0���У���������
	CChart_API void		SetSubTitlePosition(int position, int nPlotIndex);
	// ͼ�񸱱����Ƿ���ʾ
	CChart_API bool		IsSubTitleShow();
	// ��ͼ�������Ƿ���ʾ
	CChart_API bool		IsSubTitleShow(int nPlotIndex);
	// ������ʾͼ�񸱱���
	CChart_API void		SetSubTitleShow(bool show);
	// ������ʾ��ͼ������
	CChart_API void		SetSubTitleShow(bool show, int nPlotIndex);
	// ���ͼ�񸱱�����ɫ
	CChart_API COLORREF	GetSubTitleColor();
	// �����ͼ��������ɫ
	CChart_API COLORREF	GetSubTitleColor(int nPlotIndex);
	// ����ͼ�񸱱�����ɫ
	CChart_API void		SetSubTitleColor(COLORREF color);
	// ������ͼ��������ɫ
	CChart_API void		SetSubTitleColor(COLORREF color, int nPlotIndex);
	// ��ø���������
	CChart_API LOGFONT		GetSubTitleFont();
	// �����ͼ����������
	CChart_API LOGFONT		GetSubTitleFont(int nPlotIndex);
	// ���ø���������
	CChart_API void		SetSubTitleFont(LOGFONT logFont);
	// ������ͼ����������
	CChart_API void		SetSubTitleFont(LOGFONT logFont, int nPlotIndex);
	
	// ���ø����߿���ɫ
	CChart_API void		SetLightColor(COLORREF color);
	// ������ͼ�����߿���ɫ
	CChart_API void		SetLightColor(COLORREF color, int nPlotIndex);
	// ���ø����߿��߿�
	CChart_API void		SetLightLineSize(int nSize);
	// ������ͼ�����߿��߿�
	CChart_API void		SetLightLineSize(int nSize, int nPlotIndex);
	// ���ø����߿�����
	CChart_API void		SetLightLineStyle(int nStyle);
	// ������ͼ�����߿�����
	CChart_API void		SetLightLineStyle(int nStyle, int nPlotIndex);

	// �ڲ����ú���
	CChart_API void		*GetAxis(int location);
	CChart_API void		*GetAxis(int location, int nPlotIndex);

	// ���������ᣬ�������λ��location�����ļ�ͷ����ע�͵���enum
	CChart_API void		AddAxis(int location, int nPlotIndex=0);
	// ������ͼ���������
	CChart_API void		SetAxisTitle(const TCHAR* title, int location, int nPlotIndex);
	// �������������
	CChart_API void		SetAxisTitle(const TCHAR* title, int location);
	// ������ͼ�������������
	CChart_API void		SetAxisTitleFont(LOGFONT logFont, int location, int nPlotIndex);
	// �����������������
	CChart_API void		SetAxisTitleFont(LOGFONT logFont, int location);
	// ������ͼ�������ǩ����
	CChart_API void		SetAxisLabelFont(LOGFONT logFont, int location, int nPlotIndex);
	// �����������ǩ����
	CChart_API void		SetAxisLabelFont(LOGFONT logFont, int location);
	// ������ͼ���������λ��
	CChart_API void		SetAxisTitlePosition(int position, int location, int nPlotIndex);
	// �������������λ��
	CChart_API void		SetAxisTitlePosition(int position, int location);
	// ������ͼ���������ͼ���ļ�϶
	CChart_API void		SetAxisGap(int gap, int location, int nPlotIndex);
	// �������������ͼ���ļ�϶
	CChart_API void		SetAxisGap(int gap, int location);
	// ������ͼ������̶���λ��
	CChart_API void		SetAxisTickPosition(int pos, int location, int nPlotIndex);
	// ���������ḱ�̶���λ��
	CChart_API void		SetAxisMinorTickPosition(int pos, int location);
	// ������ͼ�����ḱ�̶���λ��
	CChart_API void		SetAxisMinorTickPosition(int pos, int location, int nPlotIndex);
	// ����������̶���λ��
	CChart_API void		SetAxisTickPosition(int pos, int location);
	// ������ͼʱ��������
	CChart_API void		SetAxisToTime(bool bTime, int location, int nPlotIndex);
	// ����ʱ��������
	CChart_API void		SetAxisToTime(bool bTime, int location);
	// ������ͼʱ���������ʽ,"%Y%m%d%H%M%S",�ο�strftime������CTime��ĸ�ʽ������
	CChart_API void		SetAxisTimeFormat(TCHAR *format, int location, int nPlotIndex);
	// ����ʱ���������ʽ,"%Y%m%d%H%M%S",�ο�strftime������CTime��ĸ�ʽ������
	CChart_API void		SetAxisTimeFormat(TCHAR *format, int location);
	// ������ͼ��ʾ��ɫ��
	CChart_API void		SetAxisColorBar(bool bShow, int location, int nPlotIndex);
	// ������ʾ��ɫ��
	CChart_API void		SetAxisColorBar(bool bShow, int location);
	// ������ͼ��������ɫ
	CChart_API void		SetAxisColor(COLORREF color, int location, int nPlotIndex);
	// ������������ɫ
	CChart_API void		SetAxisColor(COLORREF color, int location);
	// �����������Ƿ���ʾ�̶�ֵ
	CChart_API void		SetAxisLabelShow(bool bShow, int location);
	// ������ͼ�������Ƿ���ʾ�̶�ֵ
	CChart_API void		SetAxisLabelShow(bool bShow, int location, int nPlotIndex);
	// ������ͼ����ֵ�ĸ�ʽ,�ο�printf�ĸ�ʽ������
	CChart_API void		SetAxisLabelFormat(TCHAR *format, int location, int nPlotIndex);
	// ��������ֵ�ĸ�ʽ,�ο�printf�ĸ�ʽ������
	CChart_API void		SetAxisLabelFormat(TCHAR *format, int location);
	// ������ͼ�������Ƿ���ʾ
	CChart_API void		SetAxisShow(bool bShow, int location, int nPlotIndex);
	// �����������Ƿ���ʾ
	CChart_API void		SetAxisShow(bool bShow, int location);
	// ��ͼ�������Ƿ���ʾ
	CChart_API bool		IsAxisShow(int location, int nPlotIndex);
	//�������Ƿ���ʾ
	CChart_API bool		IsAxisShow(int location);
	// �������������ģʽ
	CChart_API void		SetAxesCompact(int nPlotIndex=0);
	// �����ͼ�����������ǩ��һ�����ݷ�Χ
	CChart_API void		AddRange4LabelHiden(double low, double high, int location, int nPlotIndex);
	// ��������������ǩ��һ�����ݷ�Χ
	CChart_API void		AddRange4LabelHiden(double low, double high, int location);
	// �����������ǩ�滻
	CChart_API void		SetReplaceLabel(bool bReplace, int location);
	// ������ͼ�������ǩ�滻
	CChart_API void		SetReplaceLabel(bool bReplace, int location, int nPlotIndex);
	// ���һ���滻�ı�ǩ
	CChart_API void		AddAlternateLabel(TCHAR *label, int location);
	// ��ͼ���һ���滻�ı�ǩ
	CChart_API void		AddAlternateLabel(TCHAR *label, int location, int nPlotIndex);
	// ����������滻��ǩ
	CChart_API void		ClrAlternateLabel(int location);
	// �����ͼ�������滻��ǩ
	CChart_API void		ClrAlternateLabel(int location, int nPlotIndex);

	// ��ͼ�Զ�����X������̶���
	CChart_API void		SetXAutoTicks(bool bAuto, int nPlotIndex);
	// �Զ�����X������̶���
	CChart_API void		SetXAutoTicks(bool bAuto);
	// ��ͼ�Զ�����Y������̶���
	CChart_API void		SetYAutoTicks(bool bAuto, int nPlotIndex);
	// �Զ�����Y������̶���
	CChart_API void		SetYAutoTicks(bool bAuto);
	// ������ͼX������̶���
	CChart_API void		SetXTickCount(int count, int nPlotIndex);
	// ��������X��̶���
	CChart_API void		SetXTickCount(int count);
	// ������ͼY������̶���
	CChart_API void		SetYTickCount(int count, int nPlotIndex);
	// ��������Y��̶���
	CChart_API void		SetYTickCount(int count);
	// ��ͼ�Զ�����X�����ḱ�̶���
	CChart_API void		SetXAutoMinorTicks(bool bAuto, int nPlotIndex);
	// �Զ�����X�����ḱ�̶���
	CChart_API void		SetXAutoMinorTicks(bool bAuto);
	// ��ͼ�Զ�����Y�����ḱ�̶���
	CChart_API void		SetYAutoMinorTicks(bool bAuto, int nPlotIndex);
	// �Զ�����Y�����ḱ�̶���
	CChart_API void		SetYAutoMinorTicks(bool bAuto);
	// ������ͼX�����ḱ�̶���
	CChart_API void		SetXMinorTickCount(int count, int nPlotIndex);
	// ����X�����ḱ�̶���
	CChart_API void		SetXMinorTickCount(int count);
	// ������ͼY�����ḱ�̶���
	CChart_API void		SetYMinorTickCount(int count, int nPlotIndex);
	// ����Y�����ḱ�̶���
	CChart_API void		SetYMinorTickCount(int count);
	// ������ͼX�����ḡ�����
	CChart_API void		SetXFloatTicks(bool flt, int nPlotIndex);
	// ����X�����ḡ�����
	CChart_API void		SetXFloatTicks(bool flt);
	// ������ͼY�����ḡ�����
	CChart_API void		SetYFloatTicks(bool flt, int nPlotIndex);
	// ����Y�����ḡ�����
	CChart_API void		SetYFloatTicks(bool flt);
	// ������ͼX�������ָ��
	CChart_API void		SetXLogarithm(bool bLog, int nPlotIndex);
	// ����X�������ָ��
	CChart_API void		SetXLogarithm(bool bLog);
	// ������ͼY�������ָ��
	CChart_API void		SetYLogarithm(bool bLog, int nPlotIndex);
	// ����Y�������ָ��
	CChart_API void		SetYLogarithm(bool bLog);
	// ������ͼX���귴�����
	CChart_API void		SetXOpposite(bool bOpposite, int nPlotIndex);
	// ����X���귴�����
	CChart_API void		SetXOpposite(bool bOpposite);
	// ������ͼY���귴�����
	CChart_API void		SetYOpposite(bool bOpposite, int nPlotIndex);
	// ����Y���귴�����
	CChart_API void		SetYOpposite(bool bOpposite);
	// ������ͼ�������������ĳ���ı���
	CChart_API void		SetAtomX(double atom, int nPlotIndex);
	// �����������������ĳ���ı���
	CChart_API void		SetAtomX(double atom);
	// ������ͼ��������������ĳ���ı���
	CChart_API void		SetAtomY(double atom, int nPlotIndex);
	// ������������������ĳ���ı���
	CChart_API void		SetAtomY(double atom);
	// ������ͼ������������ĳ���ı���
	CChart_API void		SetAtomZeroX(double zero, int nPlotIndex);
	// ����������������ĳ���ı���
	CChart_API void		SetAtomZeroX(double zero);
	// ������ͼ�������������ĳ���ı���
	CChart_API void		SetAtomZeroY(double zero, int nPlotIndex);
	// �����������������ĳ���ı���
	CChart_API void		SetAtomZeroY(double zero);
	// ���������귶Χ��ĳ����������£���ͼ�������ָ����
	CChart_API void		SetSegsPreferX(int segs, int nPlotIndex);
	// ���������귶Χ��ĳ����������£��������ָ����
	CChart_API void		SetSegsPreferX(int segs);
	// ���������귶Χ��ĳ����������£���ͼ��������ָ����
	CChart_API void		SetSegsPreferY(int segs, int nPlotIndex);
	// ���������귶Χ��ĳ����������£���������ָ����
	CChart_API void		SetSegsPreferY(int segs);

	// ���������᷶Χ
	CChart_API void		SetPlotRange(double xRange[2], double yRange[2]);
	CChart_API void		SetPlotRange(double xl, double xu, double yl, double yu);
	CChart_API void		SetXRange(double low, double high, int nPlotIndex);
	CChart_API void		SetXRange(double low, double high);
	CChart_API void		SetYRange(double low, double high, int nPlotIndex);
	CChart_API void		SetYRange(double low, double high);
	// ���������᷶Χ�Զ�����
	CChart_API void		SetXAutoRange(bool bAuto);
	CChart_API void		SetYAutoRange(bool bAuto);
	CChart_API void		SetXAutoRange(bool bAuto, int nPlotIndex);
	CChart_API void		SetYAutoRange(bool bAuto, int nPlotIndex);
	// ���������᷶Χ�����ݷ�Χ��ͬ������������չ
	CChart_API void		SetExactXRange(bool bExact, int nPlotIndex=0);
	CChart_API void		SetExactYRange(bool bExact, int nPlotIndex=0);
	// ���������᷶Χ���������ݷ�Χ��ͬ
	CChart_API void		SetOptimalExactXRange(bool bExact, int nPlotIndex=0);
	CChart_API void		SetOptimalExactYRange(bool bExact, int nPlotIndex=0);
	// ���������᷶Χ������������չһ��
	CChart_API void		SetOptimalXExtend(bool bExtend, int nPlotIndex=0);
	CChart_API void		SetOptimalYExtend(bool bExtend, int nPlotIndex=0);
	// ���������᷶Χһ�²���
	CChart_API void		SetStaticRange(bool sr, int nPlotIndex=0);
	// ����X�����᷶Χһ�²���
	CChart_API void		SetStaticXRange(bool sr, int nPlotIndex=0);
	// ����Y�����᷶Χһ�²���
	CChart_API void		SetStaticYRange(bool sr, int nPlotIndex=0);

	// ����������ȳ���ֻ��kTypeXY��Ч
	CChart_API void		SetEqualXYAxis(bool bEqual);
	// ����������ȷ�Χ��ֻ��kTypeXY��Ч
	CChart_API void		SetEqualXYRange(bool bEqual);
	// ����������ȱ����ߣ�ֻ��kTypeXY��Ч
	CChart_API void		SetEqualXYScale(bool bEqual);

	// X���곬����Ļ�ĵ㲻������㣬���Ż��ٶ�
	CChart_API void		SetClipXData(bool clip, int nDataIndex, int nPlotIndex = 0);
	// Y���곬����Ļ�ĵ㲻������㣬���Ż��ٶ�
	CChart_API void		SetClipYData(bool clip, int nDataIndex, int nPlotIndex = 0);

	// ����Xʱ����󣬶Ի�����X���ݷ�Χ����ʱ��ȥһ����ֵ���Ա������ݹ���
	CChart_API void		SetXStartTime(TCHAR *strTime, TCHAR *fmt, int nPlotIndex = 0);
	// ����Yʱ����󣬶Ի�����Y���ݷ�Χ����ʱ��ȥһ����ֵ���Ա������ݹ���
	CChart_API void		SetYStartTime(TCHAR *strTime, TCHAR *fmt, int nPlotIndex = 0);

	// ���ñ�����������һ����ͼ�󶨣����ڹ���X����ͼ
	CChart_API void		SetGridBindLayer(int nPlotIndex);
	// �����ͼ��������
	CChart_API bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV, int nPlotIndex);
	// ���������
	CChart_API bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV);
	// ������ͼ�����ߵĻ���
	CChart_API void		SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV, int nPlotIndex);
	// ���������ߵĻ���
	CChart_API void		SetGridLine(bool MajorH=true, bool MajorV=true, bool MinorH=false, bool MinorV=false);
	// ��������������ɫ
	CChart_API void		SetMajorGridColor(COLORREF color);
	// ������ͼ����������ɫ
	CChart_API void		SetMajorGridColor(COLORREF color, int nPlotIndex);
	// ���ø���������ɫ
	CChart_API void		SetMinorGridColor(COLORREF color);
	// ������ͼ����������ɫ
	CChart_API void		SetMinorGridColor(COLORREF color, int nPlotIndex);
	// �������������߿�
	CChart_API void		SetMajorGridLineSize(int nSize);
	// ������ͼ���������߿�
	CChart_API void		SetMajorGridLineSize(int nSize, int nPlotIndex);
	// ���ø��������߿�
	CChart_API void		SetMinorGridLineSize(int nSize);
	// ������ͼ���������߿�
	CChart_API void		SetMinorGridLineSize(int nSize, int nPlotIndex);
	// ����������������
	CChart_API void		SetMajorGridLineStyle(int nStyle);
	// ������ͼ������������
	CChart_API void		SetMajorGridLineStyle(int nStyle, int nPlotIndex);
	// ���ø�����������
	CChart_API void		SetMinorGridLineStyle(int nStyle);
	// ������ͼ������������
	CChart_API void		SetMinorGridLineStyle(int nStyle, int nPlotIndex);
	// ��ȡ��ͼ������ɫ
	CChart_API COLORREF	GetBkgndColor(int nPlotIndex);
	// ��ȡ������ɫ
	CChart_API COLORREF	GetBkgndColor();
	// ������ͼ������ɫ
	CChart_API void		SetBkgndColor(COLORREF color, int nPlotIndex);
	// ���ñ�����ɫ
	CChart_API void		SetBkgndColor(COLORREF color);
	// ����ɫ�Ƿ񽥱�
	CChart_API bool		IsGradientBkgnd();
	// ����ɫ����
	CChart_API void		SetGradientBkgnd(bool bGrad);

	// ����ͼ��߿���ʾ
	CChart_API void		SetEdgeShow(bool bShow);
	// ����ͼ��߿���ʾ
	CChart_API void		SetEdgeRoundConor(bool bRound);
	// ����ͼ��߿���ɫ
	CChart_API void		SetEdgeColor(COLORREF color);
	// ����ͼ��߿���
	CChart_API void		SetEdgeWidth(int width);

	// ����ͼ����հ�
	CChart_API void		SetMarginLeft(int margin);
	// ���÷�����ͼ��հ�
	CChart_API void		SetMarginLeft(int margin, int nPlotIndex);
	// ����ͼ���ҿհ�
	CChart_API void		SetMarginRight(int margin);
	// ���÷�����ͼ�ҿհ�
	CChart_API void		SetMarginRight(int margin, int nPlotIndex);
	// ����ͼ���Ͽհ�
	CChart_API void		SetMarginTop(int margin);
	// ���÷�����ͼ�Ͽհ�
	CChart_API void		SetMarginTop(int margin, int nPlotIndex);
	// ����ͼ���¿հ�
	CChart_API void		SetMarginBottom(int margin);
	// ���÷�����ͼ�¿հ�
	CChart_API void		SetMarginBottom(int margin, int nPlotIndex);

	// �����������߿���ʾ
	CChart_API void		SetBorderShow(bool bShow);
	// �����������߿���ɫ
	CChart_API void		SetBorderColor(COLORREF color);
	// �����������߿���
	CChart_API void		SetBorderWidth(int width);

	// ����Ƿ�ʹ��ͼ��
	CChart_API bool		IsLegendShow(int nPlotIndex=0);
	// �����Ƿ�ʹ��ͼ��
	CChart_API void		SetLegendShow(bool bUse, int nPlotIndex=0);
	// ����ͼ�����������
	CChart_API void		SetLegendMaxRows(int nRows, int nPlotIndex=0);
	// ����ÿ����ʾ��ͼ����
	CChart_API void		SetLegendMaxCols(int nCols, int nPlotIndex=0);
	// ����ͼ��λ��
	CChart_API void		SetLegendPosition(int nPos, int nPlotIndex=0);
	// ����ͼ������
	CChart_API void		SetLegendFont(LOGFONT logFont, int nPlotIndex=0);
	// ����ͼ������߶�
	CChart_API void		SetLegendFontHeight(int nHeight, int nPlotIndex=0);
	
	// �ڷֲ���ͼ�ͷ�����ͼ�У����õ�һ����ʾ�������ᣬ�ڶ�����ʾ��������
	CChart_API void		SetLRAxis();
	// �ڷֲ���ͼ�У����õ�һ����ʾ�������ᣬ�ڶ�����ʾ��������
	CChart_API void		SetTBAxis();
	// �ײ㺯������ȡ��ͼ�Ļ�ͼ����
	CChart_API RECT		GetLastClientRect(int nPlotIndex);
	// �ײ㺯������ȡ��ͼ����
	CChart_API RECT		GetLastClientRect();
	// �ײ㺯������ȡ��ͼ�Ļ�ͼ����
	CChart_API RECT		GetLastPlotRect(int nPlotIndex);
	// �ײ㺯������ȡ��ͼ����
	CChart_API RECT		GetLastPlotRect();
	// �ײ㺯������ȡ������ķ�Χ
	CChart_API void		GetLastPlotRange(double xRange[2], double yRange[2], int nPlotIndex=0);
	// �ײ㺯������ȡ���ݵķ�Χ
	CChart_API void		GetLastDataRange(double xRange[2], double yRange[2], int nPlotIndex=0);
	// �ײ㺯����������Ļ���������������
	CChart_API void		ClientToData(POINT *point, double *data, int nPlotIndex=0);
	// �ײ㺯���������������������Ļ����
	CChart_API void		DataToClient(double *data, POINT *point, int nPlotIndex=0);

	// ������ά���ߵ�����
	CChart_API void		SetGridLine(bool MajorXY, bool MinorXY, bool MajorYZ, bool MinorYZ, bool MajorZX, bool MinorZX);
	// ����XY��Ļ������
	CChart_API void		SetShowXYMajorGrid( bool show );
	// ����YZ��Ļ������
	CChart_API void		SetShowYZMajorGrid( bool show );
	// ����ZX��Ļ������
	CChart_API void		SetShowZXMajorGrid( bool show );
	// ����XY��Ļ������
	CChart_API void		SetShowXYMinorGrid( bool show );
	// ����YZ��Ļ������
	CChart_API void		SetShowYZMinorGrid( bool show );
	// ����ZX��Ļ������
	CChart_API void		SetShowZXMinorGrid( bool show );

	// ������ͼ���ݵ��ļ�
	CChart_API bool		WriteToFile(TCHAR *pathName, int nPlotIndex=0);
	// ������ͼĳ�������ݵ��ļ�
	CChart_API bool		WriteOneToFile(TCHAR *pathName, int nDataIndex, int nPlotIndex=0);

	
	// ����ڲ�����ָ�룬��ָ��һ��û�ã�ֻ������ĳЩ����´��ݲ����������̺߳���
	CChart_API void		*GetUserPointer();
	// �����ڲ�����ָ��
	CChart_API void		SetUserPointer(void *pUser);
	// ����ڲ������־״̬��������;
	CChart_API bool		GetUserFlag();
	// �����ڲ������־״̬��������;
	CChart_API void		SetUserFlag(bool flag);
}

//#ifdef CChart_API
//#	undef CChart_API
//#endif //CChart_API
