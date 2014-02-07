#ifndef TABLE_H
#define TABLE_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class table;
}

class table : public QDialog
{
    Q_OBJECT

public:
    explicit table(QWidget *parent = 0);
    ~table();
    double * MassResultTable;
    QStandardItemModel *model;

private slots:
    void UpdateTableData(double*,int,double,double,double*, std::vector<int>);
private:
    Ui::table *ui;

};

#endif // TABLE_H
