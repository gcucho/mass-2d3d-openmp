#include "dialog.h"
#include "ui_dialog.h"
#include "table.h"
#include "zoomgraph.h"
#include "structures.h"
#include <QFile>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //Initialize Font Title
    QFont font = ui->l_title->font();
    font.setPointSize(16);
    font.setBold(true);
    ui->l_title->setFont(font);

    //Initialize Radio Button
    // By default MASS2D - AUTO
    ui->rb_mass2d->setChecked(true);
    ui->rb_lvalauto->setChecked(true);
    ui->te_lvalues->setEnabled(false);
    b_automanual = true;
    b_mass2d3d = true;

    //Initialize Graphics
    ui->cb_selgraph->addItem("f(alpha) vs. alpha");
    ui->cb_selgraph->addItem("f(alpha) vs. log");
    ui->cb_selgraph->addItem("Alpha vs. log");
    ui->cb_selgraph->addItem("Tau vs. alpha");
    ui->cb_selgraph->addItem("Dq vs. alpha");
    ui->cb_selgraph->addItem("Dq vs. q");
    InitialGraphicSetup(ui->cp_graph1);

    //Initialize ListView
    ui->lv_filenames->setModel(new QStringListModel(m_slData));

    //Initialize variables
    err = (int*)malloc(1*sizeof(int));

    //Initialize buttons
    ui->pb_run->setEnabled(false);
    ui->cb_selgraph->setEnabled(false);
    ui->cb_qvalues->setEnabled(false);
    ui->pb_showresult->setEnabled(false);
    ui->pb_zoomgraph->setEnabled(false);
    ui->pb_seetable->setEnabled(false);
    ui->pb_saveresult->setEnabled(false);
    ui->cp_graph1->setEnabled(false);

    // Initialize Thread
    mythread = new Thread(this);

    // Initialize Table
    mytable = new table(this);

    // Initialize Z Graph
    myZgraph = new zoomgraph(this);

    // Initialize Data Param struct
    dataParam = new DataParam();

    // Initialize connection between Dialog and Qthread
    connect(this,SIGNAL(ParamsChanged(DataParam*)),mythread,SLOT(UpdateParams(DataParam*))); // For Updating
    connect(mythread,SIGNAL(valueDisplayChanged(int)),this,SLOT(UpdateStatus(int))); // For Status text box
    connect(mythread,SIGNAL(UpdateMassOutput(double*,double*,double*,double*)),this,SLOT(CopyMassOutput2Dlg(double*,double*,double*,double*))); //For copying output

    // Tools connections
    connect(ui->cb_selgraph,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateSelGraphQvalue(int)));
    connect(this,SIGNAL(UpdateDataTable(double*,int,double,double,double*,std::vector<int>)),mytable,SLOT(UpdateTableData(double*,int,double,double,double*,std::vector<int>)));
    connect(this,SIGNAL(UpdateZoomGraph(double*,int,double,double,double,std::vector<int>,double*,double*)),myZgraph,SLOT(UpdateZGraph(double*,int,double,double,double,std::vector<int>,double*,double*)));
    connect(this,SIGNAL(DoZgraph(int)),myZgraph,SLOT(DoZGraph(int)));
    connect(this,SIGNAL(SendIndexQval(int)),myZgraph,SLOT(ReceiveIndexQval(int)));
}

void Dialog::InitialGraphicSetup(QCustomPlot *customPlot)
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
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->cp_graph1, "F(alpha) vs alpha"));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_rb_lvalauto_clicked()
{
    b_automanual = true;
    ui->te_lvalues->setEnabled(false);
}

void Dialog::on_rb_lvalmanual_clicked()
{
    b_automanual = false;
    ui->te_lvalues->setEnabled(true);
}

void Dialog::on_rb_mass2d_clicked()
{
   b_mass2d3d = true;
}

void Dialog::on_rb_mass3d_clicked()
{
    b_mass2d3d = false;
}

void Dialog::on_pb_addraw_clicked()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Text File(*.txt);;All file (*.*)"));

   if(fileName!="")
       {
           m_slData.append(fileName);
           ((QStringListModel*) ui->lv_filenames->model())->setStringList(m_slData);
       }
}

void Dialog::on_pb_deleteone_clicked()
{
    QModelIndexList index = ui->lv_filenames->selectionModel()->selectedIndexes();
    if(!index.isEmpty())
    {
        ui->lv_filenames->model()->removeRow(index.at(0).row()); //Remover en el listview
        m_slData.removeAt(index.at(0).row());                     // Remover en la lista de strings
    }
}

void Dialog::on_pb_deleteall_clicked()
{
    int cont_data = m_slData.count();

    if(cont_data>0)
    {
        for(int i=0;i<cont_data;i++)
        {
            m_slData.removeLast();
        }
        ((QStringListModel*) ui->lv_filenames->model())->setStringList(m_slData);
    }

    ui->pb_run->setEnabled(false);
    ui->cb_selgraph->setEnabled(false);
    ui->pb_showresult->setEnabled(false);
    ui->pb_zoomgraph->setEnabled(false);
    ui->pb_seetable->setEnabled(false);
    ui->pb_saveresult->setEnabled(false);
    ui->te_lvalues->clear();
}

void Dialog::on_pb_preshow_clicked()
{

    if(b_mass2d3d) //Mass 2D
    {
        int datacnt = m_slData.count();
        if(datacnt > 1)
        {
          QMessageBox::warning(this,"Warning","MASS 2D:\nPlease select only one file to process",QMessageBox::Ok);
          return;
        }
        else
        {
            if(datacnt==0)
            {
                QMessageBox::warning(this,"Warning","MASS 2D:\nPlease select one file to process",QMessageBox::Ok);
                return;
            }
            else
            {
                if (!AnalyzeMass2D(err))
                {
                    QMessageBox::warning(this,"Warning","MASS 2D:\n ERROR: The file cannot be opened.");
                    return;
                }

                CalcDividers();
                ui->te_lvalues->clear();
                //Load dividers to str
                QString str;
                for(int k=0;k<NumDivs;k++)
                {
                    str.push_back(QString("%1").arg(*(Dividers+k)));
                    if(k!=NumDivs-1)
                        str.append(",");
                }
                //update text edit with str.
                ui->te_lvalues->append(str);
                //enable Run button
                ui->pb_run->setEnabled(true);
                // Status display
                ui->te_status->append("Pre process ... done!");
            }
        }
    }
    else // Mass 3D
    {
        int datacnt = m_slData.count();
        if(datacnt <= 1)
        {
          QMessageBox::warning(this,"Warning","MASS 3D:\nPlease select more than one file to process",QMessageBox::Ok);
          return;
        }
        else
        {
            if (!AnalyzeMass3D(err))
            {
                QMessageBox::warning(this,"Warning","MASS 3D:\n ERROR: Files cannot be opened or the height and width are not the same in all files.");
                return;
            }

            CalcDividers();
            ui->te_lvalues->clear();
            //Load dividers to str
            QString str;
            for(int k=0;k<NumDivs;k++)
            {
                str.push_back(QString("%1").arg(*(Dividers+k)));
                if(k!=NumDivs-1)
                    str.append(",");
            }
            //update text edit with str.
            ui->te_lvalues->append(str);
            //enable Run button
            ui->pb_run->setEnabled(true);
        }
    }

}

bool Dialog::AnalyzeMass3D(int *err)
{
    FILE * pFile;
    int cntFiles = m_slData.count();
    bool sameDim = true;
    int tmpRows;
    int tmpCols;

    *err = 0;

    for(int i=0; i<cntFiles ; i++)
    {
      pFile = fopen(m_slData.at(i).toStdString().c_str(),"r");
      if(pFile == NULL)
      {
          *err = 2;
          break;
      }
      fscanf(pFile,"%d",&Cols);
      fscanf(pFile,"%d",&Rows);

      if(i==0)
      {
          tmpRows=Rows;
          tmpCols=Cols;

      }
      else
      {
          if((tmpRows != Rows)||(tmpCols!=Cols))
          {
            *err = 3;
            fclose(pFile);
            break;
          }
      }

      fclose(pFile);
    }

    if(*err > 0)
    {
        return false;
    }
    else
    {
        Depth = cntFiles;
        ui->le_width->setText(QString::number(Rows));
        ui->le_height->setText(QString::number(Cols));
        ui->le_depth->setText(QString::number(Depth));
        return true;
    }
}

bool Dialog::AnalyzeMass2D(int * err)
{
    FILE * pFile;
    *err = 0;
    pFile = fopen(m_slData.at(0).toStdString().c_str(),"r");

    if(pFile == NULL)
    {
        *err = 1;
        return false;
    }
    fscanf(pFile,"%d",&Rows);
    fscanf(pFile,"%d",&Cols);
    Depth = 1;
    ui->le_width->setText(QString::number(Rows));
    ui->le_height->setText(QString::number(Cols));
    ui->le_depth->setText("1");
    fclose(pFile);
    return true;
}

void Dialog::CalcDividers()
{
    preDivs = (int*)malloc(100*sizeof(int));
    int j=0;
    int countDivs = 0;
    int cntDivPartial;
    bool lsequal = false;

    for(int i=1;i<Cols;i++)
        if(Cols%i == 0)
        {
            *(preDivs+j)=i;
            countDivs++;
            j++;
        }

    cntDivPartial=countDivs;

    for(int i=1;i<Rows;i++)
        if(Rows%i==0)
        {
            lsequal = false;

            for(int k=0; k<cntDivPartial;k++)
                if(i==*(preDivs+k))
                    lsequal = true;

            if(lsequal == false)
            {
                *(preDivs+j)=i;
                countDivs++;
                j++;
            }

         }

    cntDivPartial=countDivs;

    if(!b_mass2d3d) //MASS3D
    {
        for(int i=1;i<Depth;i++)
        {
            if(Depth%i==0)
            {
                lsequal = false;

                for(int k=0; k<cntDivPartial; k++)
                    if(i==*(preDivs+k))
                        lsequal = true;

                if(lsequal == false)
                {
                    *(preDivs+j)=i;
                    countDivs++;
                    j++;
                }
            }
        }
    }

    BubbleSort(preDivs,countDivs);
    Dividers = (int*)malloc(countDivs*sizeof(int));
    memcpy(Dividers,preDivs,countDivs*sizeof(int));
    NumDivs = countDivs;
    free(preDivs);
}

void Dialog::BubbleSort(int *pDivs,int Ndivs)
{
    int aux;
    for(int i=1; i<Ndivs;i++)
        for(int j=0;j<=Ndivs-2;j++)
        {
            if(*(pDivs+j)>*(pDivs+j+1))
            {
                aux= *(pDivs+j);
                *(pDivs+j)=*(pDivs+j+1);
                *(pDivs+j+1)=aux;
            }
        }
}

void Dialog::on_pb_saveresult_clicked()
{
    QString dirName =  QFileDialog::getExistingDirectory(this,tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if(dirName != "")
    {

        ui->te_status->append("Saving files in directory ... ");
        CopyingFile(QString ("MassResult.csv"), dirName);
        CopyingFile(QString ("MiQx.csv"), dirName);
        CopyingFile(QString ("SumPiQx.csv"), dirName);
        CopyingFile(QString ("log_parameters.txt"), dirName);
        QMessageBox::information(this,"Saved Files","The files were saved in the directory.",QMessageBox::Ok);
        ui->te_status->append("Saving files in directory ... done!");
    }
}

void Dialog::CopyingFile(QString filename, QString dirName)
{
    QFile destinationFile, sourceFile;
    destinationFile.setFileName(dirName+"/"+filename);
    sourceFile.setFileName(filename);

    if ( !destinationFile.exists() || destinationFile.remove() )
    {
        if ( !sourceFile.copy(destinationFile.fileName()) )
            QMessageBox::warning(this,"Warning","Cannot copy files to your selected folder",QMessageBox::Ok);
    }
        else
        QMessageBox::warning(this,"Warning","Cannot remove previous version of file",QMessageBox::Ok);

}

void Dialog::on_pb_run_clicked()
{
    ui->cb_selgraph->setEnabled(false);
    ui->pb_seetable->setEnabled(false);
    ui->pb_showresult->setEnabled(false);
    ui->pb_zoomgraph->setEnabled(false);
    ui->pb_saveresult->setEnabled(false);
    ui->cp_graph1->setEnabled(false);
    ui->cb_qvalues->setEnabled(false);

    //General values
    dataParam->Depth    = Depth;
    dataParam->Height   = Rows;
    dataParam->Width    = Cols;
    dataParam->Qmin     = ui->le_qmin->text().toDouble();
    dataParam->Qmax     = ui->le_qmax->text().toDouble();
    dataParam->Qstep    = ui->le_qstep->text().toDouble();

    //File Names
    dataParam->FileNames.clear();
    if(m_slData.isEmpty())
    {
        QMessageBox::warning(this,"Warning","The process needs more than 1 file",QMessageBox::Ok);
        return;
    }
    else
    {
        for(int i=0; i<m_slData.count();i++)
        {
            dataParam->FileNames.push_back(m_slData.at(i).toStdString());
        }
    }

    //Dividers
    dataParam->Dividers.clear();
    if(b_automanual) //automatic
    {
        for(int i=0;i<NumDivs;i++)
            dataParam->Dividers.push_back(*(Dividers+i));
    }
    else //manual
    {
        //Obtain L values from Text Edit box
        QString temp = ui->te_lvalues->toPlainText();
        QStringList list = temp.split(",");

        for(int i=0; i<list.size();i++)
            dataParam->Dividers.push_back(list[i].toInt());

        NumDivs = dataParam->Dividers.size();
   }

    //Pass parameters to thread
    emit ParamsChanged(dataParam);

    //RUN THREAD!
    mythread->start();
    // Status display
    ui->te_status->append("Running Process ...");

    //if is correct:
    int numQs;
    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);
    MassResult = (double*) malloc(numQs*12*sizeof(double));
    MiQx_NN = (double*)malloc(NumDivs*numQs*sizeof(double));
    MiQx_MM = (double*)malloc(NumDivs*numQs*sizeof(double));
    D012 = (double*)malloc(3*sizeof(double));
}

void Dialog::CopyMassOutput2Dlg(double* massoutput, double* miqx_nn, double* miqx_mm, double * d012)
{
    int numQs;
    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    for(int i=0;i<12*numQs;i++)
        *(MassResult+i)= *(massoutput+i);
    for(int i=0; i<NumDivs*numQs;i++)
        *(MiQx_NN+i)= *(miqx_nn+i);
    for(int i=0; i<NumDivs*numQs;i++)
        *(MiQx_MM+i)= *(miqx_mm+i);
    for(int i=0;i<3;i++)
        *(D012+i)=*(d012+i);

    emit UpdateDataTable(MassResult,numQs,dataParam->Qmin,dataParam->Qstep,D012,dataParam->Dividers); // Envio Datos a Clase Table
    emit UpdateZoomGraph(MassResult,numQs,dataParam->Qmin,dataParam->Qmax,dataParam->Qstep,dataParam->Dividers,MiQx_NN,MiQx_MM); //Envio Datos a Clase ZoomGraph
}

void Dialog::on_pb_seetable_clicked()
{
   mytable->exec();
}

void Dialog::on_pb_showresult_clicked()
{
    int sel;
    sel = ui->cb_selgraph->currentIndex();

    switch(sel)
    {
        case 0: DoGraphFvsA(ui->cp_graph1);
                break;
        case 1: DoGraphNNvsLL(ui->cp_graph1);
                break;
        case 2: DoGraphMMvsLL(ui->cp_graph1);
                break;
        case 3: DoGraphTauvsAlpha(ui->cp_graph1);
                break;
        case 4: DoGraphDqvsAlpha(ui->cp_graph1);
                break;
        case 5: DoGraphDqvsQ(ui->cp_graph1);
                break;
    }

    emit DoZgraph(sel);
}

void Dialog::DoGraphFvsA(QCustomPlot * customPlot)
{
    int numQs;
    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    QVector<double> X(numQs), Y(numQs);
    for (int i=0; i<numQs; ++i)
    {
       X[i] = *(MassResult+i*12);
       Y[i] = *(MassResult+1+i*12);
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
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->cp_graph1, "F(alpha) Vs Alpha"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void Dialog::DoGraphNNvsLL(QCustomPlot * customPlot)
{
    int index = ui->cb_qvalues->currentIndex();
    emit SendIndexQval(index);
    int numQs;

    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    // X->NN,Y->LL
    QVector<double> X(NumDivs), Y(NumDivs);

    for(int t=0;t<NumDivs;t++)
    {
        Y[t]=*(MiQx_NN+index+t*numQs);
        X[t]=log(dataParam->Dividers.at(t));
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
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->cp_graph1, "NN Vs Log"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void Dialog::DoGraphMMvsLL(QCustomPlot * customPlot)
{
    int index = ui->cb_qvalues->currentIndex();
    emit SendIndexQval(index);
    int numQs;

    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    // X->NN,Y->LL
    QVector<double> X(NumDivs), Y(NumDivs);

    for(int t=0;t<NumDivs;t++)
    {
        Y[t]=*(MiQx_MM+index+t*numQs);
        X[t]=log(dataParam->Dividers.at(t));
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
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->cp_graph1, "MM Vs Log"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void Dialog::DoGraphTauvsAlpha(QCustomPlot * customPlot)
{
    int numQs;
    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    QVector<double> X(numQs), Y(numQs);
    for (int i=0; i<numQs; ++i)
    {
       X[i] = *(MassResult+i*12);
       Y[i] = *(MassResult+6+i*12);
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
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->cp_graph1, "Tau Vs Alpha"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void Dialog::DoGraphDqvsAlpha(QCustomPlot * customPlot)
{
    int numQs;
    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    QVector<double> X(numQs), Y(numQs);
    for (int i=0; i<numQs; ++i)
    {
       X[i] = *(MassResult+i*12);
       Y[i] = *(MassResult+7+i*12);
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
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->cp_graph1, "Dq Vs Alpha"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void Dialog::DoGraphDqvsQ(QCustomPlot * customPlot)
{
    int numQs;
    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    QVector<double> X(numQs), Y(numQs);
    for (int i=0; i<numQs; ++i)
    {
       X[i] = dataParam->Qmin + i*dataParam->Qstep;
       Y[i] = *(MassResult+7+i*12);
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
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->cp_graph1, "Dq Vs Q"));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->replot();
}

void Dialog::UpdateStatus(int disp)
{
    switch (disp)
    {
        case 0: ui->te_status->append(QString ("Process... Done!"));
                //Update Buttons for graphics and table
                ui->cb_selgraph->setEnabled(true);
                ui->pb_seetable->setEnabled(true);
                ui->pb_showresult->setEnabled(true);
                ui->pb_zoomgraph->setEnabled(true);
                ui->pb_saveresult->setEnabled(true);
                ui->cp_graph1->setEnabled(true);
                ui->cb_qvalues->setEnabled(false);
                UpdateComboBoxQvalues(ui->cb_qvalues);
                break;
    }
}

void Dialog::UpdateComboBoxQvalues(QComboBox * cb_qvalues)
{
    int numQs;
    numQs = (int)round(((dataParam->Qmax - dataParam->Qmin)/dataParam->Qstep)+1);

    cb_qvalues->clear();
    QString str;
    double Qval;

    for(int j=0; j<numQs ; j++)
    {
        Qval = dataParam->Qmin+j*dataParam->Qstep;
        if((Qval<0.0001)&&Qval>-0.0001) Qval = 0;
        str = QString("Qval=%1").arg(QString::number(Qval,'g',3));
        cb_qvalues->addItem(QString(str));
    }

}

void Dialog::UpdateSelGraphQvalue(int index)
{
    if ((index>0)&&(index<3)) ui->cb_qvalues->setEnabled(true);
    else ui->cb_qvalues->setEnabled(false);
}

double Dialog::MinMaxOfVector(QVector<double> Vector, int sel)
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

void Dialog::on_pb_zoomgraph_clicked()
{
    myZgraph->exec();
}

bool Dialog::SaveTxtFile(QString filename,QImage img, int layer)
{
    uchar *bits = img.bits();

    FILE *pfile;
    pfile = fopen(filename.toStdString().c_str(),"w");
    if(pfile == NULL)
        return false;
    fprintf(pfile,"%d\n",img.width());
    fprintf(pfile,"%d\n",img.height());
    for(int i=0;i<img.byteCount();i+=4)
        fprintf(pfile,"%d ",(int)bits[i+layer]);
    fclose(pfile);

    return true;
}


void Dialog::on_pb_convert_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("Images(*.png *.bmp *.jpg *.tif);;All file (*.*)"));
    QString fileSaveName;
    QImage img1,img2;
    QPixmap img3;
    QMessageBox* msgBox;
    msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    QPushButton *Red_btn = msgBox->addButton(tr("Red"),QMessageBox::ActionRole);
    QPushButton *Green_btn = msgBox->addButton(tr("Green"),QMessageBox::ActionRole);
    QPushButton *Blue_btn = msgBox->addButton(tr("Blue"),QMessageBox::ActionRole);
    QPushButton *Cancel_btn = msgBox->addButton(QMessageBox::Cancel);

    if(fileName!="")
    {
        img1 = QImage(fileName);
        int formatImage = img1.format();
        int resp = QMessageBox::question(this,"Question","Do you want to save the generated *.txt file in the same directory?",QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(resp == QMessageBox::Cancel)
               return;
        if(resp == QMessageBox::No)
                fileSaveName = QFileDialog::getSaveFileName(this,tr("Save File"),"untitled.txt",tr("Text File(*.txt)"));

        // Mono
        if(formatImage == 1)
        {
            img3 = QPixmap(fileName);
            img2 = img3.toImage();
            QChar* mio = fileName.end();
            *(mio-1)='t';*(mio-2)='x';*(mio-3)='t';

            if(resp == QMessageBox::Yes)
            {
                if(!SaveTxtFile(fileName,img2,0))
                    QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
            }
            else //QMessageBox::No
            {
                 if(!SaveTxtFile(fileSaveName,img2,0))
                    QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
            }
        }
        // Indexed 8
        else if(formatImage == 3)
        {
            img3 = QPixmap(fileName);
            img2 = img3.toImage();

            QChar* mio = fileName.end();
            *(mio-1)='t';*(mio-2)='x';*(mio-3)='t';

            if(img2.isGrayscale())
            {
                if(resp == QMessageBox::Yes)
                {
                    if(!SaveTxtFile(fileName,img2,0))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
                else
                {
                    if(!SaveTxtFile(fileSaveName,img2,0))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
            }
            else
            {
                int sel_layer;
                msgBox->setText("This is not a grayscale image file.\nPlease select a layer to work: ");
                msgBox->exec();
                if(msgBox->clickedButton()== Red_btn)        sel_layer = 2;
                else if(msgBox->clickedButton()== Green_btn) sel_layer = 1;
                else if(msgBox->clickedButton()== Blue_btn)  sel_layer = 0;
                else return;

                if(resp == QMessageBox::Yes)
                {
                    if(!SaveTxtFile(fileName,img2,sel_layer))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
                else
                {
                    if(!SaveTxtFile(fileSaveName,img2,sel_layer))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
            }

        }
        // RGB32
        else if(formatImage == 4)
        {
            img3 = QPixmap(fileName);
            img2 = img3.toImage();

            QChar* mio = fileName.end();
            *(mio-1)='t';*(mio-2)='x';*(mio-3)='t';

            if(img2.isGrayscale())
            {
                if(resp == QMessageBox::Yes)
                {
                    if(!SaveTxtFile(fileName,img2,0))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
                else
                {
                    if(!SaveTxtFile(fileSaveName,img2,0))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
            }
            else
            {
                int sel_layer;
                msgBox->setText("This is not a grayscale image file.\nPlease select a layer to work: ");
                msgBox->exec();
                if(msgBox->clickedButton()== Red_btn)        sel_layer = 2;
                else if(msgBox->clickedButton()== Green_btn) sel_layer = 1;
                else if(msgBox->clickedButton()== Blue_btn)  sel_layer = 0;
                else return;

                if(resp == QMessageBox::Yes)
                {
                    if(!SaveTxtFile(fileName,img2,sel_layer))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
                else
                {
                    if(!SaveTxtFile(fileSaveName,img2,sel_layer))
                        QMessageBox::warning(this,"Warning","The file can not be created.", QMessageBox::Ok);
                }
            }

        }
        // NO FORMAT
        else
            QMessageBox::warning(this,"Warning","Can not recognize this image format");
    }
}

