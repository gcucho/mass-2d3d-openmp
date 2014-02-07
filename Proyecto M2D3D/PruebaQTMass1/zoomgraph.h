#ifndef ZOOMGRAPH_H
#define ZOOMGRAPH_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class zoomgraph;
}

class zoomgraph : public QDialog
{
    Q_OBJECT

public:
    explicit zoomgraph(QWidget *parent = 0);
    ~zoomgraph();
    double* MassResultZgraph;\
    double* Mi_Qx_NN_zg;
    double* Mi_Qx_MM_zg;
    int numQs_zg;
    int numDivs_zg;
    double Qmin_zg;
    double Qmax_zg;
    double Qstep_zg;
    int index;
    std::vector<int>  Dividers_zg;


public slots:
    void InitialGraphicSetup(QCustomPlot*);
    void UpdateZGraph(double*,int,double,double,double,std::vector<int>,double*,double*);
    void DoZGraph(int);
    void DoGraphFvsA(QCustomPlot*);
    void DoGraphNNvsLL(QCustomPlot*);
    void DoGraphMMvsLL(QCustomPlot*);
    void DoGraphTauvsAlpha(QCustomPlot*);
    void DoGraphDqvsAlpha(QCustomPlot*);
    void DoGraphDqvsQ(QCustomPlot*);
    double MinMaxOfVector(QVector<double>,int);
    void ReceiveIndexQval(int);

private:
    Ui::zoomgraph *ui;
};

#endif // ZOOMGRAPH_H
