#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include "structures.h"
#include "main.h"

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);
    Mass2d3d myMass2d3d;
    bool is2d;
    int numQs;
    int numDivs;

signals:
    void valueDisplayChanged(int);
    void UpdateMassOutput(double*,double*,double*,double*);

public slots:
    void UpdateParams(DataParam *dataParamDlg);


private:
    void run();
};

#endif // THREAD_H
