#include "table.h"
#include "ui_table.h"


table::table(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::table)
{
    ui->setupUi(this);

    model = new QStandardItemModel(10,12,this);
    model->setHorizontalHeaderItem(0,new QStandardItem(QString("qx")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString("L")));
    model->setHorizontalHeaderItem(2,new QStandardItem(QString("Alpha")));
    model->setHorizontalHeaderItem(3,new QStandardItem(QString("f(alpha)")));
    model->setHorizontalHeaderItem(4,new QStandardItem(QString("R2 alpha")));
    model->setHorizontalHeaderItem(5,new QStandardItem(QString("R2 f(alpha)")));
    model->setHorizontalHeaderItem(6,new QStandardItem(QString("B1 err-alpha")));
    model->setHorizontalHeaderItem(7,new QStandardItem(QString("B1 err-f(alpha)")));
    model->setHorizontalHeaderItem(8,new QStandardItem(QString("Tau")));
    model->setHorizontalHeaderItem(9,new QStandardItem(QString("Dq")));
    model->setHorizontalHeaderItem(10,new QStandardItem(QString("ErrorDq")));
    model->setHorizontalHeaderItem(11,new QStandardItem(QString("D012")));

    ui->tv_datatable->setModel(model);
}

table::~table()
{
    delete ui;
}

void table::UpdateTableData(double *massresult, int numqs, double qmin, double qstep, double* d012, std::vector<int> dividers) // The data gets here after pressing the RUN BUTTON
{
    MassResultTable = (double*)malloc(12*numqs*sizeof(double));

    model->removeRows(0,model->rowCount());

    //copying to internal memory
    for(int i=0;i<12*numqs;i++)
        *(MassResultTable+i) = *(massresult+i);

    //For MassResult main values
    for (int row = 0; row < numqs; row++) {
        for (int column = 0; column < 9; column++) {
            QStandardItem *item = new QStandardItem(QString("%0").arg(QString::number(*(MassResultTable+column+row*12),'g',6)));
            model->setItem(row, column+2, item);
         }
    }

   // For Qvalues
    for(int row=0;row<numqs;row++)
    {
        double qvalue = qmin + row*qstep;
        if((qvalue<0.001)&&(qvalue>-0.0001)) qvalue = 0.0;
        QStandardItem *item = new QStandardItem(QString("%0").arg(QString::number(qvalue,'g',3)));
        model->setItem(row,0,item);
    }

    //For D012 values
    for(int row=0;row<3;row++)
    {
        QStandardItem *item = new QStandardItem(QString("%0").arg(QString::number(*(d012+row),'g',5)));
        model->setItem(row,11,item);
    }

    //For Dividers values
    for(int row=0;row<dividers.size();row++)
    {
        QStandardItem *item = new QStandardItem(QString("%0").arg(QString::number(dividers.at(row))));
        model->setItem(row,1,item);
    }
    ui->tv_datatable->resizeColumnsToContents();


    free(MassResultTable);
}
