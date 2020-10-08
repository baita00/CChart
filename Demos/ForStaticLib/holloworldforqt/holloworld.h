#ifndef HOLLOWORLD_H
#define HOLLOWORLD_H

#include <QWidget>
#include<tchar.h>


#include "../../ForDynamicLib/PreCompiled/Chart.h"
#if defined (_UNICODE) || defined(UNICODE)
#pragma comment(lib,"../../ForDynamicLib/PreCompiled/CChartu.lib")
#else
#pragma comment(lib,"../../ForDynamicLib/PreCompiled/CChart.lib")
#endif
using namespace NsCChart;


#pragma comment(lib,"user32.lib")

namespace Ui {
class holloworld;
}

class holloworld : public QWidget
{
    Q_OBJECT

public:
    explicit holloworld(QWidget *parent = 0);
    ~holloworld();

private:
    Ui::holloworld *ui;
    CChartWnd m_ChartWnd;
};

#endif //  HOLLOWORLD_H

double contourmap(double x, double y);
