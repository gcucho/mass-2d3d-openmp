#include "thread.h"
#include "structures.h"
#include "math.h"


Thread::Thread(QObject *parent) :
    QThread(parent)
{

}

void Thread::run()
{

    double * MassResult;
    double * MiQx_NN;
    double * MiQx_MM;
    double * D012;

    MassResult = (double*) malloc(numQs*12*sizeof(double));
    MiQx_NN = (double*)malloc(numDivs*numQs*sizeof(double));
    MiQx_MM = (double*)malloc(numDivs*numQs*sizeof(double));
    D012 = (double*)malloc(3*sizeof(double));

    myMass2d3d.MainProcess(is2d,MassResult,MiQx_NN,MiQx_MM,D012);
    // Update Status
    emit valueDisplayChanged(0);
    // Update Mass Output
    emit UpdateMassOutput(MassResult,MiQx_NN,MiQx_MM,D012);
}

void Thread::UpdateParams(DataParam * dataParamDlg)
{
    if(dataParamDlg->Depth>1) is2d = false;
    else is2d = true;

    myMass2d3d.SetParameters(dataParamDlg);
    numQs = (int)round(((dataParamDlg->Qmax - dataParamDlg->Qmin)/dataParamDlg->Qstep)+1);
    numDivs = dataParamDlg->Dividers.size();
}
