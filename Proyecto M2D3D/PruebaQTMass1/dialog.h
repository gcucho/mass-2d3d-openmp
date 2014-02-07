#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "qcustomplot.h"
#include <string>
#include <vector>
#include "thread.h"
#include "structures.h"
#include "table.h"
#include "zoomgraph.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    bool b_automanual;
    bool b_mass2d3d;
    void InitialGraphicSetup(QCustomPlot *customPlot);
    void CalcDividers(void);
    void BubbleSort(int *pDivs,int Ndivs);
    bool AnalyzeMass2D(int *err);
    bool AnalyzeMass3D(int *err);
    double* MassResult;
    double* MiQx_NN;
    double* MiQx_MM;
    double* D012;

private slots:
    void on_rb_lvalmanual_clicked();
    void on_rb_lvalauto_clicked();
    void on_rb_mass2d_clicked();
    void on_rb_mass3d_clicked();
    void on_pb_addraw_clicked();
    void on_pb_deleteone_clicked();
    void on_pb_deleteall_clicked();
    void on_pb_preshow_clicked();
    void on_pb_saveresult_clicked();
    void on_pb_run_clicked();
    void on_pb_seetable_clicked();
    void on_pb_showresult_clicked();
    void UpdateStatus(int);
    void CopyMassOutput2Dlg(double*,double*,double*,double*);
    void DoGraphFvsA(QCustomPlot*);
    void DoGraphNNvsLL(QCustomPlot*);
    void DoGraphMMvsLL(QCustomPlot*);
    void DoGraphTauvsAlpha(QCustomPlot*);
    void DoGraphDqvsAlpha(QCustomPlot*);
    void DoGraphDqvsQ(QCustomPlot*);
    void UpdateComboBoxQvalues(QComboBox*);
    void UpdateSelGraphQvalue(int index);
    double MinMaxOfVector(QVector<double>, int);
    void CopyingFile(QString,QString);
    bool SaveTxtFile(QString,QImage,int);
    void on_pb_zoomgraph_clicked();
    void on_pb_convert_clicked();

signals:
    void ParamsChanged(DataParam *);
    void UpdateDataTable(double*,int,double,double,double*,std::vector<int>);
    void UpdateZoomGraph(double*,int,double,double,double,std::vector<int>,double*,double*);
    void DoZgraph(int);
    void SendIndexQval(int);

private:
    Ui::Dialog *ui;
    QStringList m_slData;
    int *preDivs;
    int Cols;
    int Rows;
    int Depth;
    int *Dividers;
    int NumDivs;
    int *err;
    Thread *mythread;
    DataParam *dataParam;
    table *mytable;
    zoomgraph *myZgraph;
};

#endif // DIALOG_H
