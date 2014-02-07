#include "main.h"
#include "omp.h"
#include <math.h>

Mass2d3d::Mass2d3d()
{
    NumQs = 0;
}

Mass2d3d::~Mass2d3d()
{

}

bool Mass2d3d::MainProcess(bool is2d, double *outputMR, double *outputNN, double *outputMM, double *outputD012)
{
    int* MatrixData;
    double totalsum;

    if(is2d)    MatrixData = (int*)malloc(dataParamDLL.Height*dataParamDLL.Width*sizeof(int));
    else        MatrixData = (int*)malloc(dataParamDLL.Height*dataParamDLL.Width*dataParamDLL.Depth*sizeof(int));

    /*** Summatory Process ***/
    GetDataImage(is2d,MatrixData);
    totalsum = Summatory(is2d,MatrixData);

    /*** SumPiQx ***/
    int* nb_SPQx;
    double* SPQxDivVsNumQ;
    nb_SPQx = (int*)malloc(dataParamDLL.Dividers.size()*sizeof(int));
    SPQxDivVsNumQ = (double*)malloc(NumQs*dataParamDLL.Dividers.size()*sizeof(double));
    GetNumBlocksSPQx(is2d,nb_SPQx);
    CalcSPQx(is2d,nb_SPQx,MatrixData,SPQxDivVsNumQ,totalsum);

    /*** MiQx/Lg1x ***/
    double* MiQxDivVsNumQ_NN;
    double* MiQxDivVsNumQ_MM;
    MiQxDivVsNumQ_NN = (double*)malloc(NumQs*dataParamDLL.Dividers.size()*sizeof(double));
    MiQxDivVsNumQ_MM = (double*)malloc(NumQs*dataParamDLL.Dividers.size()*sizeof(double));
    CalcMiQx(is2d,nb_SPQx,MatrixData,MiQxDivVsNumQ_NN,MiQxDivVsNumQ_MM,SPQxDivVsNumQ,totalsum);

    /*** Correlation ***/
    double* MassResult;
    MassResult = (double*)malloc(12*NumQs*sizeof(double));
    CalcCorrelation(MassResult,MiQxDivVsNumQ_NN,MiQxDivVsNumQ_MM);

    /*** Copying MiQxDivVsNumQ_NN, MiQxDivVsNumQ_MM, MassResult ***/
    int numDivs = dataParamDLL.Dividers.size();
    for(int index=0;index<numDivs*NumQs;index++)
        *(outputNN+index)=*(MiQxDivVsNumQ_NN+index);
    for(int index=0;index<numDivs*NumQs;index++)
        *(outputMM+index)=*(MiQxDivVsNumQ_MM+index);
    for(int index=0;index<12*NumQs;index++)
        *(outputMR+index)=*(MassResult+index);
    for(int index=0;index<3;index++)
        *(outputD012+index)=*(D012+index);

    /** Free Memory **/
    free(nb_SPQx);
    free(SPQxDivVsNumQ);
    free(MiQxDivVsNumQ_NN);
    free(MiQxDivVsNumQ_MM);
    free(MassResult);
    return true;
}

void Mass2d3d::CalcCorrelation(double* massresult, double* miqxdvnq_nn, double* miqxdvnq_mm)
{
    double* VectorNNMM;
    double* LL;
    int numDivs = dataParamDLL.Dividers.size();

    VectorNNMM = (double*)malloc(2*numDivs*sizeof(double));
    LL = (double*)malloc(numDivs*sizeof(double));

    for(int t=0;t<numDivs;t++)
			*(LL+t)=log(dataParamDLL.Dividers.at(t));

    D012[0]=0;
    D012[1]=0;
    D012[2]=0;

    for(int t=0; t<NumQs;t++)
    {
        // Copy vNNMM <- MiQxNN & MiQxMM
        for(int i=0;i<numDivs;i++)
        {
            *(VectorNNMM+i)=*(miqxdvnq_nn+t+i*NumQs);
            *(VectorNNMM+i+numDivs)=*(miqxdvnq_mm+t+i*NumQs);
        }

        PartialCorrelation(t,VectorNNMM,LL,massresult);
    }

    SaveMassResult(massresult);
}

void Mass2d3d::SaveMassResult(double* massresult)
{
    FILE *pFile;
	pFile = fopen("MassResult.csv","w");

	if (pFile == NULL)
	{
		printf("It is not possible to create this file!\n");
	}
	else
	{
		fprintf(pFile,"qx,alpha,f(alpha),R2alpha,R2 f(alpha),B1 error alpha,B1 error f(alpha),Tau,Dq,ErrorDq,D012\n");
		for(int i=0;i<NumQs;i++)
		{
			fprintf(pFile,"%.3lf,",*(qval+i));
			for(int j=0;j<9;j++)
			{
				fprintf(pFile,"%.4lf,",*(massresult+j+i*12));
			}

			if(i<3)
                fprintf(pFile,"%.4lf,",D012[i]);

            fprintf(pFile,"\n");
		}

		fclose(pFile);
	}
}

double Mass2d3d::CorrNNMMLL(double* vNNMM, double* vLL, double* Fsub, double* SumEi, int Sel)
{
    double out;
	int i;
	double SumNN,SumLL,avgLL,avgNN,avgMM,Sum1,Sum2,SumMM;
	double B1,B0,ajus,ei;
	double *reglin,*Ei;
	double *bc,*m1,*m2,*m3;
	SumNN = SumLL = SumMM = 0.0;
	int numDivs = dataParamDLL.Dividers.size();

	reglin =(double*)malloc(numDivs*sizeof(double));
	Ei = (double*)malloc(numDivs*sizeof(double));
	bc = (double*)malloc(numDivs*sizeof(double));
	m1 = (double*)malloc(numDivs*sizeof(double));
	m2 = (double*)malloc(numDivs*sizeof(double));
	m3 = (double*)malloc(numDivs*sizeof(double));
	avgMM = avgNN = avgLL = 0;

	for(i=0;i<numDivs;i++)
	{
		SumLL += *(vLL+i);
		if(Sel == 0)
			SumNN += *(vNNMM+i);
		else
			SumMM += *(vNNMM+i+numDivs);
	}

	avgLL = SumLL/(double)numDivs;

	if(Sel==0)
		avgNN = SumNN/(double)numDivs;
	else
		avgMM = SumMM/(double)numDivs;

	Sum1 = Sum2 = 0.0;

	for(i=0;i<numDivs;i++)
		if(Sel == 0)
		{
			Sum1 += (*(vLL+i)-avgLL)*(*(vNNMM+i)-avgNN);
			Sum2 += (*(vLL+i)-avgLL)*(*(vLL+i)-avgNN);
		}
		else
		{
			Sum1 += (*(vLL+i)-avgLL)*(*(vNNMM+i+numDivs)-avgMM);
			Sum2 += (*(vLL+i)-avgLL)*(*(vLL+i)-avgMM);
		}

    B1 = B0 = 0.0;
	B1 = Sum1/Sum2;

	if(Sel == 0)
		B0 = avgNN -(B1*avgLL);
	else
		B0 = avgMM -(B1*avgLL);

    if(Sel==0)
	{
		B1*= 1000;
		B1 = ceil(B1);
		B1 /= 1000;
	}

	*(Fsub) = B1;

	for(i=0;i<numDivs;i++)
	{
		*(reglin+i)=B0 + B1*(*(vLL+i));
		ajus = *(reglin + i);

		if(Sel == 0)
			ei = ajus - *(vNNMM+i);
		else
			ei = ajus - *(vNNMM+i+numDivs);

		ei = pow(ei,2);
		*(Ei+i) = ei;
	}

    *SumEi = 0.0;

	for (i=0; i<numDivs ; i++)
		*SumEi += *(Ei+i);

	Sum1=Sum2 = 0.0;

	for(i=0;i<numDivs;i++)
	{
		if(Sel == 0)
			*(bc+i) = *(vNNMM+i)-*(reglin+i);
		else
			*(bc+i) = *(vNNMM+i+numDivs)-*(reglin+i);

		*(m1+i)= (*(bc+i))*(*(bc+i));
		Sum2 += *(m1+i);
	}

	for(i=0;i<numDivs;i++)
	{
		if(Sel==0)
			*(m2+i)=*(vNNMM+i)-avgNN;
		else
			*(m2+i)=*(vNNMM+i+numDivs)-avgMM;

		*(m3+i) = (*(m2+i))*(*(m2+i));
		Sum1 += *(m3+i);
	}

	out = 1 - (Sum2/Sum1);

	free(bc);
	free(m1);
	free(m2);
	free(m3);
	free(reglin);
	free(Ei);

	return out;


}

void Mass2d3d::PartialCorrelation(int i, double* vNNMM, double* vLL, double* massresult)
{
    double corr1,corr2,dato,errAl,errF,errD,Tau,dq,D0,D1,D2,Dq;
	double Fsub[2];
	double SumEi[1];
	int numDivs = dataParamDLL.Dividers.size();

	*Fsub = 0.0;
	dq = Dq = D0 = D1 = D2 = Tau = 0.0;
	corr1 = CorrNNMMLL(vNNMM,vLL,Fsub,SumEi,0);
	dato = *SumEi/(numDivs-2);
    errAl = sqrt(dato);
    corr2 = CorrNNMMLL(vNNMM,vLL,Fsub+1,SumEi,1);
    dato = *SumEi/(numDivs-2);
    errF = sqrt(dato);

    //if(*(qval + i)== 1.0)
    if((*(qval + i)>0.999)&&(*(qval + i)<1.0001))
		errD = errF;
	else
		errD = 2.0*errF/((errF/errAl)+1.0);

    Tau = *(qval+i)*(*(Fsub+1)) - *(Fsub);

    // Resolucion de millonesimas//
    Tau*=10000000000000;
    Tau = ceil(Tau);
    Tau /= 10000000000000;

    //if(*(qval + i)== 1.0)
    if((*(qval + i)>0.999)&&(*(qval + i)<1.0001))
    {
    	dq = *Fsub;
        D1 = *Fsub;
        D012[1]= *Fsub;
    }
    else
        dq = (*(Fsub+1)*(*(qval + i))-*(Fsub))/(*(qval + i)-1.0);

    //if(*(qval + i)== 0.0)
    if((*(qval + i)>-0.0001)&&(*(qval + i)<0.0001))
    {
        D0 = *(Fsub);
        D012[0]= *Fsub;
    }
    //if(*(qval + i)== 2.0)
    if((*(qval + i)>1.999)&&(*(qval + i)<2.0001))
    {
        D2 = dq;
        D012[2]= dq;
    }
    Dq = dq;

    *(massresult+i*12) = *(Fsub+1);
    *(massresult+i*12+1) = *Fsub;
    *(massresult+i*12+2) = corr1;
    *(massresult+i*12+3) = corr2;
    *(massresult+i*12+4) = errAl;
    *(massresult+i*12+5) = errF;
    *(massresult+i*12+6) = Tau;
    *(massresult+i*12+7) = Dq;
    *(massresult+i*12+8) = errD;
    *(massresult+i*12+9) = D0;
    *(massresult+i*12+10) = D1;
    *(massresult+i*12+11) = D2;
}

void Mass2d3d::CalcMiQx(bool is2d, int* nb_spqx,int* matrixdata,double* miqxdvsn_nn, double* miqxdvsn_mm,double* spqxdvsn,double totalsum)
{
    int numDivs     = dataParamDLL.Dividers.size();
    int DtCol       = dataParamDLL.Width;
    int DtRow       = dataParamDLL.Height;
    int DtDepth     = dataParamDLL.Depth;
    int divider,i,j;
    int z,v,w;
    double *MiQxThrdvsNumQ_NN;
    double *MiQxThrdvsNumQ_MM;
    int threads = omp_get_num_procs();

    if(is2d)
    {
        MiQxThrdvsNumQ_NN = (double*)malloc(NumQs*threads*sizeof(double));
        MiQxThrdvsNumQ_MM = (double*)malloc(NumQs*threads*sizeof(double));

        for(i=0;i<numDivs;i++)
        {
             for(int q=0;q<NumQs*threads;q++)
             {
                *(MiQxThrdvsNumQ_NN+q)=0;
                *(MiQxThrdvsNumQ_MM+q)=0;
             }

            divider = dataParamDLL.Dividers.at(i);
            z = ceil(double(DtCol)/(divider));
            v = ceil(double(DtRow)/(divider));

            #pragma omp parallel for
            for(j=0;j<*(nb_spqx+i);j++)
            {
                int x,y,iam2;
                double sumBlock;
                double tmp1,tmp2,tmp3,tmp4;

                iam2 = omp_get_thread_num();
                y = j/z;
                x = j%z;
                sumBlock = 0;
                sumBlock = SummatoryBlock2D(x,y,z,v,matrixdata,divider);

                for(int r=0;r<NumQs;r++)
                    if(sumBlock!=0)
                    {
                        tmp1 = pow(sumBlock/totalsum,*(qval+r));
                        tmp2 = tmp1/(*(spqxdvsn+r+i*NumQs));
                        tmp3 = log(sumBlock/totalsum);
                        if(isnan(tmp2)) tmp2=0;
                        if(isnan(tmp3)) tmp3=0;
                        if(tmp2>0)  tmp4 = log(tmp2);
                        else		tmp4 = 0;
                        *(MiQxThrdvsNumQ_NN+r+iam2*NumQs)+= tmp2*tmp4;
                        *(MiQxThrdvsNumQ_MM+r+iam2*NumQs)+= tmp2*tmp3;
                    }
            }
            //Updating miqxdvsn_nn,miqxdvsn_MM <- MiQxThrdvsNumQ_NN,MiQxThrdvsNumQ_MM
            UpdateMiQxDvsN(miqxdvsn_nn,miqxdvsn_mm,MiQxThrdvsNumQ_NN,MiQxThrdvsNumQ_MM,i);
        }

        SaveMiQxNNMM(miqxdvsn_nn,miqxdvsn_mm);
    }
    else
    {
        MiQxThrdvsNumQ_NN = (double*)malloc(NumQs*threads*sizeof(double));
        MiQxThrdvsNumQ_MM = (double*)malloc(NumQs*threads*sizeof(double));

        for(i=0;i<numDivs;i++)
        {
            for(int q=0;q<NumQs*threads;q++)
             {
                *(MiQxThrdvsNumQ_NN+q)=0;
                *(MiQxThrdvsNumQ_MM+q)=0;
             }

            divider = dataParamDLL.Dividers.at(i);
            z = ceil(double(DtCol)/(divider));
            v = ceil(double(DtRow)/(divider));
            w = ceil(double(DtDepth)/(divider));

            #pragma omp parallel for
            for(j=0;j<*(nb_spqx+i);j++)
            {
                int x,y,p,iam2;
                double sumBlock;
                double tmp1,tmp2,tmp3,tmp4;

                iam2 = omp_get_thread_num();
                p = j/(z*v);
                y = (j%(z*v))/z;
                x = (j%(z*v))%z;
                sumBlock = 0;
                sumBlock = SummatoryBlock3D(x,y,p,z,v,w,matrixdata,divider);

                for(int r=0;r<NumQs;r++)
                    if(sumBlock!=0)
                    {
                        tmp1 = pow(sumBlock/totalsum,*(qval+r));
                        tmp2 = tmp1/(*(spqxdvsn+r+i*NumQs));
                        tmp3 = log(sumBlock/totalsum);
                        if(isnan(tmp2)) tmp2=0;
                        if(isnan(tmp3)) tmp3=0;
                        if(tmp2>0)  tmp4 = log(tmp2);
                        else		tmp4 = 0;
                        *(MiQxThrdvsNumQ_NN+r+iam2*NumQs)+= tmp2*tmp4;
                        *(MiQxThrdvsNumQ_MM+r+iam2*NumQs)+= tmp2*tmp3;
                    }
            }
             //Updating miqxdvsn_nn,miqxdvsn_MM <- MiQxThrdvsNumQ_NN,MiQxThrdvsNumQ_MM
            UpdateMiQxDvsN(miqxdvsn_nn,miqxdvsn_mm,MiQxThrdvsNumQ_NN,MiQxThrdvsNumQ_MM,i);
        }

        SaveMiQxNNMM(miqxdvsn_nn,miqxdvsn_mm);
    }

    free(MiQxThrdvsNumQ_NN);
    free(MiQxThrdvsNumQ_MM);
}

void Mass2d3d::SaveMiQxNNMM(double* miqxdvsn_nn,double* miqxdvsn_mm)
{
    FILE *pFile;
	pFile = fopen("MiQx.csv","w");
	int numDivs  = dataParamDLL.Dividers.size();

	if (pFile == NULL)
	{
		printf("It is not possible create this file!\n");
	}
	else
	{
        for(int t=0;t<NumQs;t++)
            fprintf(pFile,"%.3lf,",*(qval+t));

        fprintf(pFile,"\n");

	    for(int p=0;p<numDivs;p++)
        {
            for(int t=0;t<NumQs;t++)
                fprintf(pFile,"%.3lf,",*(miqxdvsn_nn+t+p*NumQs));
            fprintf(pFile,"\n");
        }

        fprintf(pFile,"\n");

        for(int t=0;t<NumQs;t++)
            fprintf(pFile,"%.3lf,",*(qval+t));

        fprintf(pFile,"\n");

	    for(int p=0;p<numDivs;p++)
        {
            for(int t=0;t<NumQs;t++)
                fprintf(pFile,"%.3lf,",*(miqxdvsn_mm+t+p*NumQs));
            fprintf(pFile,"\n");
        }

		fclose(pFile);
	}
	return;
}

void Mass2d3d::UpdateMiQxDvsN(double* miqxdvsn_nn,double* miqxdvsn_mm,double* MiQxThrdvsNumQ_NN,double* MiQxThrdvsNumQ_MM, int i)
{
    double *tmpMiQx_NN;
    double *tmpMiQx_MM;
    tmpMiQx_NN = (double*)malloc(NumQs*sizeof(double));
    tmpMiQx_MM = (double*)malloc(NumQs*sizeof(double));
    int threads = omp_get_num_procs();

   for(int h=0;h<NumQs;h++)
    {
        *(tmpMiQx_NN+h)=0;
        *(tmpMiQx_MM+h)=0;
    }

    for(int h=0;h<NumQs;h++)
        for(int g=0; g<threads;g++)
        {
            *(tmpMiQx_NN+h) += *(MiQxThrdvsNumQ_NN+h+g*NumQs);
            *(tmpMiQx_MM+h) += *(MiQxThrdvsNumQ_MM+h+g*NumQs);
        }

    for(int f=0;f<NumQs;f++)
    {
        *(miqxdvsn_nn+f+i*NumQs)=*(tmpMiQx_NN+f);
        *(miqxdvsn_mm+f+i*NumQs)=*(tmpMiQx_MM+f);
    }
}

void Mass2d3d::CalcSPQx(bool is2d, int* nb_spqx,int* matrixdata,double* spqxdvsn,double totalsum)
{
    int numDivs     = dataParamDLL.Dividers.size();
    int DtCol       = dataParamDLL.Width;
    int DtRow       = dataParamDLL.Height;
    int DtDepth     = dataParamDLL.Depth;
    int divider,i,j;
    int z,v,w;
    double *SPQxThrdvsNumQ;
    int threads = omp_get_num_procs();

    if(is2d)
    {
        SPQxThrdvsNumQ  = (double*)malloc(NumQs*threads*sizeof(double));

        for(i=0;i<numDivs;i++)
        {
            for(int q=0;q<NumQs*threads;q++)
                *(SPQxThrdvsNumQ+q)=0;

            divider = dataParamDLL.Dividers.at(i);
            z = ceil(double(DtCol)/(divider));
            v = ceil(double(DtRow)/(divider));

            #pragma omp parallel for
            for(j=0;j<*(nb_spqx+i);j++)
            {
                int x,y,iam2;
                double sumBlock;

                iam2 = omp_get_thread_num();
                y = j/z;
                x = j%z;
                sumBlock = 0;
                sumBlock = SummatoryBlock2D(x,y,z,v,matrixdata,divider);

                //Power of probability
                for(int r=0;r<NumQs;r++)
                    if(sumBlock!=0)
                        *(SPQxThrdvsNumQ+r+iam2*NumQs)+= pow(sumBlock/totalsum,*(qval+r));
            }
            //Updating spqxdvsn <- SPQxThrdvsNumQ
            UpdateSPQxDvsN(spqxdvsn,SPQxThrdvsNumQ, i);
        }

        SaveSumPiQx(spqxdvsn);
    }
    else
    {
        SPQxThrdvsNumQ  = (double*)malloc(NumQs*threads*sizeof(double));

        for(i=0;i<numDivs;i++)
        {
            for(int q=0;q<NumQs*threads;q++)
                *(SPQxThrdvsNumQ+q)=0;

            divider = dataParamDLL.Dividers.at(i);
            z = ceil(double(DtCol)/(divider));
            v = ceil(double(DtRow)/(divider));
            w = ceil(double(DtDepth)/(divider));

            #pragma omp parallel for
            for(j=0;j<*(nb_spqx+i);j++)
            {
                int x,y,p,iam2;
                double sumBlock;

                iam2 = omp_get_thread_num();
                p = j/(z*v);
                y = (j%(z*v))/z;
                x = (j%(z*v))%z;
                sumBlock = 0;
                sumBlock = SummatoryBlock3D(x,y,p,z,v,w,matrixdata,divider);

                //Power of probability
                for(int r=0;r<NumQs;r++)
                    if(sumBlock!=0)
                        *(SPQxThrdvsNumQ+r+iam2*NumQs)+= pow(sumBlock/totalsum,*(qval+r));
            }
            //Updating spqxdvsn <- SPQxThrdvsNumQ
            UpdateSPQxDvsN(spqxdvsn,SPQxThrdvsNumQ,i);
        }
        SaveSumPiQx(spqxdvsn);
    }
}

double Mass2d3d::SummatoryBlock3D(int x,int y,int p,int z,int v, int w,int* matrixdata,int divider)
{
    double sum;
    int tmp1,tmp2,tmp3,dx,dy,dp;
    int DtCol       = dataParamDLL.Width;
    int DtRow       = dataParamDLL.Height;
    int DtDepth     = dataParamDLL.Depth;

    tmp1 = DtCol%divider;
    tmp2 = DtRow%divider;
    tmp3 = DtDepth%divider;
    sum = 0;

    if(tmp1 == 0 && tmp2 ==0 && tmp3==0)
    {
        for(int p1=0;p1<divider;p1++)           //profundidad
            for(int y1=0;y1<divider;y1++)      //vertical
                for(int x1=0;x1<divider;x1++)  //horizontal
                    sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
    }
    else
    {
        if(tmp1!=0 && tmp2!=0 && tmp3!=0)
        {
            if(x<z-1 && y<v-1 && p<w-1)
            {
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
            }

            if(x==z-1 && y<v-1 && p<w-1)
            {
                dx = divider-(z*(divider)- DtCol);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1 == dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
            }

            if(x<z-1 && y==v-1 && p<w-1)
            {
                dy = divider-(v*(divider)- DtRow);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1 == dy)
                            break;
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                    }
            }

            if(x==z-1 && y==v-1 && p<w-1)
            {
                dx = divider-(z*(divider)- DtCol);
                dy = divider-(v*(divider)- DtRow);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1 == dy)
                            break;
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1 == dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
                    }
            }

            if(x<z-1 && y<v-1 && p==w-1)
            {
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)  //profundidad
                {
                    if(p1 == dp)
                        break;

                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                }
            }

            if(x==z-1 && y<v-1 && p==w-1)
            {
                dx = divider-(z*(divider)- DtCol);
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)  //profundidad
                {
                    if(p1 == dp)
                        break;

                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1 == dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
                }
            }

            if(x<z-1 && y==v-1 && p==w-1)
            {
                dy = divider-(v*(divider)- DtRow);
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)  //profundidad
                {
                    if(p1 == dp)
                        break;

                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1 == dy)
                            break;
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                    }
                }
            }

            if(x==z-1 && y==v-1 && p==w-1)
            {
                dx = divider-(z*(divider)- DtCol);
                dy = divider-(v*(divider)- DtRow);
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)  //profundidad
                {
                    if(p1 == dp)
                        break;

                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1 == dy)
                            break;

                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1 == dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
                    }
                }
            }
        }

        if(tmp1!=0 && tmp2==0 && tmp3==0)
        {
            if(x<z-1 && y<v && p<w)
            {
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
            }

            if(x==z-1 && y<v && p<w)
            {
                dx = divider-(z*(divider)- DtCol);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1 == dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
            }

        }

        if(tmp1==0 && tmp2!=0 && tmp3==0)
        {
            if(x<z && y<v-1 && p<w)
            {
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
            }

            if(x<z && y==v-1 && p<w)
            {
                dy = divider-(v*(divider)- DtRow);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1==dy)
                            break;

                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                    }

            }

        }

        if(tmp1==0 && tmp2==0 && tmp3!=0)
        {
            if(x<z && y<v && p<w-1)
            {
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
            }

            if(x<z && y<v && p==w-1)
            {
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)           //profundidad
                {
                    if(p1 == dp)
                        break;
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                }
            }
        }

        if(tmp1!=0 && tmp2!=0 && tmp3==0)
        {
            if(x<z-1 && y<v-1 && p<w)
            {
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
            }

            if(x==z-1 && y<v-1 && p<w)
            {
                dx = divider-(z*(divider)- DtCol);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1==dx)
                                break;

                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
            }

            if(x<z-1 && y==v-1 && p<w)
            {
                dy = divider-(v*(divider)- DtRow);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1==dy)
                            break;
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                    }
            }

            if(x==z-1 && y==v-1 && p<w)
            {
                dx = divider-(z*(divider)- DtCol);
                dy = divider-(v*(divider)- DtRow);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1==dy)
                            break;
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1==dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
                    }
            }
        }

        if(tmp1!=0 && tmp2==0 && tmp3!=0)
        {
            if(x<z-1 && y<v && p<w-1)
            {
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
            }

            if(x==z-1 && y<v && p<w-1)
            {
                dx = divider-(z*(divider)- DtCol);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1==dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
            }

            if(x<z-1 && y<v && p==w-1)
            {
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)           //profundidad
                {
                    if(p1==dp)
                        break;
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                }
            }

            if(x==z-1 && y<v && p==w-1)
            {
                dx = divider-(z*(divider)- DtCol);
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)           //profundidad
                {
                    if(p1==dp)
                        break;
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                        {
                            if(x1==dx)
                                break;
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                        }
                }
            }
        }

        if(tmp1==0 && tmp2!=0 && tmp3!=0)
        {
            if(x<z && y<v-1 && p<w-1)
            {
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
            }

            if(x<z && y==v-1 && p<w-1)
            {
                dy = divider-(v*(divider)- DtRow);
                for(int p1=0;p1<divider;p1++)           //profundidad
                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1==dy)
                            break;
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                    }

            }

            if(x<z && y<v-1 && p==w-1)
            {
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)           //profundidad
                {
                    if(p1==dp)
                        break;
                    for(int y1=0;y1<divider;y1++)      //vertical
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                }
            }

            if(x<z && y==v-1 && p==w-1)
            {
                dy = divider-(v*(divider)- DtRow);
                dp = divider-(w*(divider)- DtDepth);
                for(int p1=0;p1<divider;p1++)           //profundidad
                {
                    if(p1==dp)
                        break;
                    for(int y1=0;y1<divider;y1++)      //vertical
                    {
                        if(y1==dy)
                            break;
                        for(int x1=0;x1<divider;x1++)  //horizontal
                            sum += *(matrixdata+(x*(divider)+x1)+(y*(divider)+y1)*DtCol+(p*(divider)+p1)*DtCol*DtRow);
                    }
                }
            }
        }
    }

    return sum;
}

double Mass2d3d::SummatoryBlock2D(int x,int y,int z,int v,int* matrixdata,int divider)
{
    double sum;
    int tmp1,tmp2,dx,dy;
    int DtCol       = dataParamDLL.Width;
    int DtRow       = dataParamDLL.Height;

    tmp1 = DtCol%divider;
    tmp2 = DtRow%divider;
    sum = 0;

    if(tmp1 == 0 && tmp2 == 0)
    {
        for(int w=0;w<divider;w++)      //vertical
            for(int m=0;m<divider;m++)  //horizontal
                sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
    }
    else
    {
        if(tmp1!= 0 && tmp2!=0)
        {
            if(x<z-1 && y<v-1)
            {
                for(int w=0;w<divider;w++) //vertical
                    for(int m=0;m<divider;m++) //horizontal
                         sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
            }

            if(x==z-1 && y<v-1)
            {
                dx = divider-(z*(divider)- DtCol);
                for(int w=0;w<divider;w++) //vertical
                    for(int m=0;m<divider;m++) //horizontal
                    {
                        if(m==dx)
                            break;
                        sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
                    }
            }

            if(x<z-1 && y==v-1)
            {
                dy = divider-(v*(divider)- DtRow);
                for(int w=0;w<divider;w++) //vertical
                {
                    if(w==dy)
                        break;
                    for(int m=0;m<divider;m++) //horizontal
                        sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
                }
            }

            if(x==z-1 && y==v-1)
            {
                dx = divider-(z*(divider)- DtCol);
                dy = divider-(v*(divider)- DtRow);
                for(int w=0;w<divider;w++) //vertical
                {
                    if(w==dy)
                        break;
                    for(int m=0;m<divider;m++) //horizontal
                    {
                        if(m==dx)
                            break;
                        sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
                    }
                }
            }
        }

        if(tmp1!=0 && tmp2==0)
        {
            if(x<z-1 && y<v)
            {
                for(int w=0;w<divider;w++) //vertical
                    for(int m=0;m<divider;m++) //horizontal
                        sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
            }

            if(x==z-1 && y<v)
            {
                dx = divider-(z*(divider)- DtCol);
                for(int w=0;w<divider;w++) //vertical
                    for(int m=0;m<divider;m++) //horizontal
                    {
                        if(m==dx)
                            break;
                        sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
                    }
             }
        }

        if(tmp1==0 && tmp2!=0)
        {
            if(x<z && y<v-1)
            {
                for(int w=0;w<divider;w++) //vertical
                    for(int m=0;m<divider;m++) //horizontal
                        sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
            }

            if(x<z && y==v-1)
            {
                dy = divider-(v*(divider)- DtRow);
                for(int w=0;w<divider;w++) //vertical
                {
                    if(w==dy)
                        break;
                    for(int m=0;m<divider;m++) //horizontal
                        sum += *(matrixdata+(x*(divider)+m)+(y*(divider)+w)*DtCol);
                }
            }
        }
    }

  return sum;
}

void Mass2d3d::SaveSumPiQx(double * spqxdvsn)
{
    FILE *pFile;
	pFile = fopen("SumPiQx.csv","w");
    int numDivs  = dataParamDLL.Dividers.size();

	if (pFile == NULL)
	{
		printf("It is not possible create this file!\n");
	}
	else
	{
        for(int t=0;t<NumQs;t++)
            fprintf(pFile,"%.3lf,",*(qval+t));

        fprintf(pFile,"\n");

	    for(int p=0;p<numDivs;p++)
        {
            for(int t=0;t<NumQs;t++)
                fprintf(pFile,"%.3f,",*(spqxdvsn+t+p*NumQs));
            fprintf(pFile,"\n");
        }
		fclose(pFile);
	}
	return;
}

void Mass2d3d::UpdateSPQxDvsN(double* spqxdvsn, double* SPQxThrdvsNumQ, int i)
{
    double *tmpSPQx;
    tmpSPQx  = (double*)malloc(NumQs*sizeof(double));
    int threads = omp_get_num_procs();

    for(int h=0;h<NumQs;h++)
        *(tmpSPQx+h)=0;

    for(int h=0;h<NumQs;h++)
        for(int g=0; g<threads;g++)
            *(tmpSPQx+h) += *(SPQxThrdvsNumQ+h+g*NumQs);

    for(int f=0;f<NumQs;f++)
        *(spqxdvsn+f+i*NumQs)=*(tmpSPQx+f);
}

void Mass2d3d::GetNumBlocksSPQx(bool is2d, int* nb_spqx)
{
    int numDivs     = dataParamDLL.Dividers.size();
    int DtCol       = dataParamDLL.Width;
    int DtRow       = dataParamDLL.Height;
    int DtDepth     = dataParamDLL.Depth;
    int divider;

    if(is2d)
    {
        for(int i=0;i<numDivs; i++)
        {
            divider = dataParamDLL.Dividers.at(i);
            *(nb_spqx+i)=(int)ceil(double(DtCol)/divider)*ceil(double(DtRow)/divider);
        }
    }
    else
    {
        for(int i=0; i<numDivs; i++)
        {
            divider = dataParamDLL.Dividers.at(i);
            *(nb_spqx+i)=(int)ceil(double(DtCol)/divider)*ceil(double(DtRow)/divider)*ceil(double(DtDepth)/divider);
        }
    }
}

double Mass2d3d::Summatory(bool is2d, int* matrixData)
{
    double tsum;
    int numElems;

    tsum = 0;

    if(is2d)    numElems = dataParamDLL.Height*dataParamDLL.Width;
    else        numElems = dataParamDLL.Height*dataParamDLL.Width*dataParamDLL.Depth;

    #pragma omp parallel for reduction(+:tsum)
    for(int i=0; i<numElems;i++)
        tsum += *(matrixData+i);

    return tsum;
}

bool Mass2d3d::GetDataImage(bool is2d, int* matrixData)
{
    int* temp;
    FILE *pfile;
    temp = (int*)malloc(2*sizeof(int));

    if(is2d)
    {
        pfile = fopen(dataParamDLL.FileNames[0].c_str(),"r");
        if(pfile == NULL) return false;
        fscanf(pfile,"%d",temp);fscanf(pfile,"%d",temp+1);
        for(int x=0;x<dataParamDLL.Height*dataParamDLL.Width;x++)
            fscanf(pfile,"%d",matrixData+x);
        fclose(pfile);
    }
    else
    {   for(int y=0;y<dataParamDLL.Depth;y++)
        {
            pfile = fopen(dataParamDLL.FileNames[y].c_str(),"r");
            if(pfile == NULL) break;
            fscanf(pfile,"%d",temp);fscanf(pfile,"%d",temp+1);
            for(int x=0;x<dataParamDLL.Height*dataParamDLL.Width;x++)
                fscanf(pfile,"%d",matrixData+x+y*dataParamDLL.Width*dataParamDLL.Height);
            fclose(pfile);
        }
    }

    return true;
}

bool Mass2d3d::SetParameters(DataParam * dataparamsQT)
{
    dataParamDLL.Height = dataparamsQT->Height;
    dataParamDLL.Width  = dataparamsQT->Width;
    dataParamDLL.Depth  = dataparamsQT->Depth;
    dataParamDLL.Qmin   = dataparamsQT->Qmin;
    dataParamDLL.Qmax   = dataparamsQT->Qmax;
    dataParamDLL.Qstep  = dataparamsQT->Qstep;
    dataParamDLL.FileNames  = (std::vector<std::string>)dataparamsQT->FileNames;
    dataParamDLL.Dividers   = (std::vector<int>)dataparamsQT->Dividers;
    SaveParameters();
    return true;
}

bool Mass2d3d::SaveParameters(void)
{
    FILE *pfile;
    pfile = fopen("log_parameters.txt","w");
    fprintf(pfile,"Height: %d\n",dataParamDLL.Height);
    fprintf(pfile,"Width: %d\n",dataParamDLL.Width);
    fprintf(pfile,"Depth: %d\n",dataParamDLL.Depth);
    fprintf(pfile,"Qmin: %lf\n",dataParamDLL.Qmin);
    fprintf(pfile,"Qmax: %lf\n",dataParamDLL.Qmax);
    fprintf(pfile,"Qstep: %lf\n",dataParamDLL.Qstep);
    fprintf(pfile,"Files to process:\n");
    for(int i=0;i<dataParamDLL.FileNames.size();i++)
        fprintf(pfile,"%s\n",dataParamDLL.FileNames.at(i).c_str());
    fprintf(pfile,"Dividers:\n");
    for(int i=0;i<dataParamDLL.Dividers.size();i++)
        fprintf(pfile,"%d,",dataParamDLL.Dividers.at(i));
    fprintf(pfile,"\nNum thread: %d\n",omp_get_num_procs());
    NumQs = (int)round(((dataParamDLL.Qmax - dataParamDLL.Qmin)/dataParamDLL.Qstep)+1);
    fprintf(pfile,"Num Qs:%d\n", NumQs);
    fprintf(pfile,"Q values:\n");
    qval = (double*)malloc(NumQs*sizeof(double));
    for(int i=0;i<NumQs;i++)
    {
        *(qval+i)= dataParamDLL.Qmin + i*dataParamDLL.Qstep;
        fprintf(pfile,"%lf,",*(qval+i));
    }

    fclose(pfile);
    return true;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
