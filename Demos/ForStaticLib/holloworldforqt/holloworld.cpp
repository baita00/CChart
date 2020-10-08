#include "holloworld.h"
#include "ui_holloworld.h"

holloworld::holloworld(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::holloworld)
{
    //set chart instance name and size
    resize(800, 600);
    setWindowTitle(tr("Paint Demo"));
//    m_ChartWnd.Attach((HWND)this->winId(), kTypeXY);
//    m_ChartWnd.GetChart()->AddPoint2D(1, 1);
//    m_ChartWnd.GetChart()->AddPoint2D(2, 2);

    // pototype a function pointer pfunction
//    double (*pfunction)(double , double);
//    pfunction = contourmap;


    //contour map
//    m_ChartWnd.Attach((HWND)this->winId(), kTypeContourLine);
//    m_ChartWnd.GetChart()->SetFieldFcn(contourmap);
//    m_ChartWnd.GetChart()->SetPlotRange(-2.0, 2.0, -2.0, 2.0);
//    m_ChartWnd.GetChart()->SetContourPrecision(8);


    m_ChartWnd.Attach((HWND)this->winId(), kTypeXY);

    m_ChartWnd.GetChart()->SetAxisTitle(_T("x-axis"), 1);
    m_ChartWnd.GetChart()->SetAxisTitle(_T("y-axis"), 0);

    double pX[360], pY[360];
    double Pi = 3.1415926536;
    int i;

    //add 1 courve
    for(i=0; i<360; i++)
    {
        pX[i] = i;
        pY[i] = 2.0*sin(i*2.0*Pi/360.0*3.0);
    }
    m_ChartWnd.GetChart()->AddCurve(pX, pY, 360);

    ui->setupUi(this);
}


double contourmap(double x, double y)
{
    return 1.0/((x-1.0)*(x-1.0)+y*y+1.0);
}

holloworld::~holloworld()
{
    delete ui;
}
