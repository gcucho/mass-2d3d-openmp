#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include "structures.h"
/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

class DLL_EXPORT Mass2d3d{

private:
 //  float *qval;
  // int NumQs;

public:
    DataParam dataParamDLL;
    int NumQs;
    double *qval;
    double D012[3];
    Mass2d3d();
    ~Mass2d3d();
    bool SetParameters(DataParam*);
    bool SaveParameters(void);
    bool GetDataImage(bool,int*);
    double Summatory(bool, int*);
    void GetNumBlocksSPQx(bool,int*);
    void CalcSPQx(bool,int*,int*,double*,double);
    double SummatoryBlock2D(int,int,int,int,int*,int);
    double SummatoryBlock3D(int,int,int,int,int,int,int*,int);
    void UpdateSPQxDvsN(double*, double*,int);
    void SaveSumPiQx(double*);
    void CalcMiQx(bool,int*,int*,double*, double*,double*,double);
    void UpdateMiQxDvsN(double*,double*,double*,double*, int);
    void SaveMiQxNNMM(double*,double*);
    void CalcCorrelation(double*, double*, double*);
    void PartialCorrelation(int, double*, double*, double*);
    double CorrNNMMLL(double*,double*,double*,double*,int);
    void SaveMassResult(double*);
    bool MainProcess(bool,double*,double*,double*,double*);
};

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
