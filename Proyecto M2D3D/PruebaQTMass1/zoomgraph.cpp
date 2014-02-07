#include "zoomgraph.h"
#include "ui_zoomgraph.h"

zoomgraph::zoomgraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::zoomgraph)
{
    ui->setupUi(this);

    InitialGraphicSetup(ui->cp_z_graph);
}

zoomgraph::~zoomgraph()
{
    delete ui;
}

void zoomgraph::InitialGraphicSetup(QCustomPlot *customPlot)
{
    customPlot->xAxis->setLabel("Alpha");
    customPlot->yAxis->setLabel("f(Alpha)");
    customPlot->xAxis->setRange(0,2);
    customPlot->yAxis->setRange(0,2);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "F(alpha) vs alpha"));
}

void zoomgraph::UpdateZGraph(double* massresult,int numqs,double qmin,double qmax,double qstep, std::vector<int> dividers, double* miqx_nn, double* miqx_mm)
{
    MassResultZgraph = (double*)malloc(12*numqs*sizeof(double));
    Mi_Qx_NN_zg = (double*)malloc(numqs*dividers.size()*sizeof(double));
    Mi_Qx_MM_zg = (double*)malloc(numqs*dividers.size()*sizeof(double));
    numDivs_zg = dividers.size();
    numQs_zg = numqs;
    Dividers_zg = dividers;
    Qmin_zg = qmin;
    Qmax_zg = qmax;
    Qstep_zg = qstep;

    //copying to internal memory
    for(int i=0;i<12*numQs_zg;i++)
        *(MassResultZgraph+i) = *(massresult+i);
    for(int i=0; i<numqs*numDivs_zg;i++)
    {
        *(Mi_Qx_NN_zg+i)=*(miqx_nn+i);
        *(Mi_Qx_MM_zg+i)=*(miqx_mm+i);
    }

}

void zoomgraph::ReceiveIndexQval(int idx)
{
    index = idx;
}

void zoomgraph::DoZGraph(int sel)
{
    switch(sel)
    {
        case 0: DoGraphFvsA(ui->cp_z_graph);
                break;
        case 1: DoGraphNNvsLL(ui->cp_z_graph);
                break;
        case 2: DoGraphMMvsLL(ui->cp_z_graph);
                break;
        case 3: DoGraphTauvsAlpha(ui->cp_z_graph);
                break;
        case 4: DoGraphDqvsAlpha(ui->cp_z_graph);
                break;
        case 5: DoGraphDqvsQ(ui->cp_z_graph);
                break;
    }
}

void zoomgraph::DoGraphFvsA(QCustomPlot* customPlot)
{
    QVector<double> X(numQs_zg), Y(numQs_zg);
    for (int i=0; i<numQs_zg; ++i)
    {
       X[i] = *(MassResultZgraph+i*12);
       Y[i] = *(MassResultZgraph+1+i*12);
    }

    double Ymin,Ymax,Xmin,Xmax;
    Ymin = MinMaxOfVector(Y,0);
    Ymax = MinMaxOfVector(Y,1);
    Xmin = MinMaxOfVector(X,0);
    Xmax = MinMaxOfVector(X,1);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(X,Y);
    customPlot->graph(0)->setPen(QColor(0,0,255, 255));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(0)->setName("F(alpha) vs Alpha");
    customPlot->xAxis->setLabel("Alpha");
    customPlot->yAxis->setLabel("f(Alpha)");
    customPlot->xAxis->setRange(Xmin-0.05,Xmax+0.05);
    customPlot->yAxis->setRange(Ymin-0.05,Ymax+0.05);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->plotLayout()->removeAt(0);
    customPlot->plotLayout()->simplify();
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "F(alpha) Vs Alpha"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void zoomgraph::DoGraphNNvsLL(QCustomPlot* customPlot)
{
    // X->NN,Y->LL
    QVector<double> X(numDivs_zg), Y(numDivs_zg);

    for(int t=0;t<numDivs_zg;t++)
    {
        Y[t]=*(Mi_Qx_NN_zg+index+t*numQs_zg);
        X[t]=log(Dividers_zg.at(t));
    }

    double Ymin,Ymax,Xmin,Xmax;
    Ymin = MinMaxOfVector(Y,0);
    Ymax = MinMaxOfVector(Y,1);
    Xmin = MinMaxOfVector(X,0);
    Xmax = MinMaxOfVector(X,1);

    customPlot->addGraph();
    customPlot->graph(0)->setData(X,Y);
    customPlot->graph(0)->setPen(QColor(255,0,0, 255));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare,4));
    customPlot->graph(0)->setName("NN vs Log");
    customPlot->xAxis->setLabel("Log");
    customPlot->yAxis->setLabel("NN");
    customPlot->xAxis->setRange(Xmin-0.5,Xmax+0.5);
    customPlot->yAxis->setRange(Ymin-0.5,Ymax+0.5);

    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->plotLayout()->removeAt(0);
    customPlot->plotLayout()->simplify();
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "NN Vs Log"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void zoomgraph::DoGraphMMvsLL(QCustomPlot* customPlot)
{
    // X->NN,Y->LL
    QVector<double> X(numDivs_zg), Y(numDivs_zg);

    for(int t=0;t<numDivs_zg;t++)
    {
        Y[t]=*(Mi_Qx_MM_zg+index+t*numQs_zg);
        X[t]=log(Dividers_zg.at(t));
    }

    double Ymin,Ymax,Xmin,Xmax;
    Ymin = MinMaxOfVector(Y,0);
    Ymax = MinMaxOfVector(Y,1);
    Xmin = MinMaxOfVector(X,0);
    Xmax = MinMaxOfVector(X,1);

    customPlot->addGraph();
    customPlot->graph(0)->setData(X,Y);
    customPlot->graph(0)->setPen(QColor(43,200,70, 255));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare,4));
    customPlot->graph(0)->setName("MM vs Log");
    customPlot->xAxis->setLabel("Log");
    customPlot->yAxis->setLabel("MM");
    customPlot->xAxis->setRange(Xmin-0.5,Xmax+0.5);
    customPlot->yAxis->setRange(Ymin-0.5,Ymax+0.5);

    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->plotLayout()->removeAt(0);
    customPlot->plotLayout()->simplify();
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "MM Vs Log"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();

}

void zoomgraph::DoGraphTauvsAlpha(QCustomPlot* customPlot)
{

    QVector<double> X(numQs_zg), Y(numQs_zg);
    for (int i=0; i<numQs_zg; ++i)
    {
       X[i] = *(MassResultZgraph+i*12);
       Y[i] = *(MassResultZgraph+6+i*12);
    }

    double Ymin,Ymax,Xmin,Xmax;
    Ymin = MinMaxOfVector(Y,0);
    Ymax = MinMaxOfVector(Y,1);
    Xmin = MinMaxOfVector(X,0);
    Xmax = MinMaxOfVector(X,1);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(X,Y);
    customPlot->graph(0)->setPen(QColor(0,128,128, 255));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(0)->setName("Tau vs Alpha");
    customPlot->xAxis->setLabel("Alpha");
    customPlot->yAxis->setLabel("Tau");
    customPlot->xAxis->setRange(Xmin-0.05,Xmax+0.05);
    customPlot->yAxis->setRange(Ymin-0.05,Ymax+0.05);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->plotLayout()->removeAt(0);
    customPlot->plotLayout()->simplify();
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Tau Vs Alpha"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void zoomgraph::DoGraphDqvsAlpha(QCustomPlot* customPlot)
{
    QVector<double> X(numQs_zg), Y(numQs_zg);

    for (int i=0; i<numQs_zg; ++i)
    {
       X[i] = *(MassResultZgraph+i*12);
       Y[i] = *(MassResultZgraph+7+i*12);
    }

    double Ymin,Ymax,Xmin,Xmax;
    Ymin = MinMaxOfVector(Y,0);
    Ymax = MinMaxOfVector(Y,1);
    Xmin = MinMaxOfVector(X,0);
    Xmax = MinMaxOfVector(X,1);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(X,Y);
    customPlot->graph(0)->setPen(QColor(30,179,198, 255));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(0)->setName("Dq vs Alpha");
    customPlot->xAxis->setLabel("Alpha");
    customPlot->yAxis->setLabel("Dq");
    customPlot->xAxis->setRange(Xmin-0.05,Xmax+0.05);
    customPlot->yAxis->setRange(Ymin-0.05,Ymax+0.05);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->plotLayout()->removeAt(0);
    customPlot->plotLayout()->simplify();
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Dq Vs Alpha"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void zoomgraph::DoGraphDqvsQ(QCustomPlot* customPlot)
{
    QVector<double> X(numQs_zg), Y(numQs_zg);
    for (int i=0; i<numQs_zg; ++i)
    {
       X[i] = Qmin_zg + i*Qstep_zg;
       Y[i] = *(MassResultZgraph+7+i*12);
    }

    double Ymin,Ymax,Xmin,Xmax;
    Ymin = MinMaxOfVector(Y,0);
    Ymax = MinMaxOfVector(Y,1);
    Xmin = MinMaxOfVector(X,0);
    Xmax = MinMaxOfVector(X,1);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(X,Y);
    customPlot->graph(0)->setPen(QColor(128,79,179, 255));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(0)->setName("Dq vs Q");
    customPlot->xAxis->setLabel("Q");
    customPlot->yAxis->setLabel("Dq");
    customPlot->xAxis->setRange(Xmin-0.05,Xmax+0.05);
    customPlot->yAxis->setRange(Ymin-0.05,Ymax+0.05);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->plotLayout()->removeAt(0);
    customPlot->plotLayout()->simplify();
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Dq Vs Q"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

double zoomgraph::MinMaxOfVector(QVector<double> Vector, int sel)
{
    double temp;
    if(sel == 0) // Min
    {
        temp = Vector[0];
        for(int i=0; i<Vector.size();i++)
        {
            if(Vector[i]<temp)
                temp = Vector[i];
        }
    }
    else
    {
        temp = Vector[0];
        for(int i=0; i<Vector.size();i++)
        {
            if(Vector[i]>temp)
                temp = Vector[i];
        }
    }
    return temp;
}
