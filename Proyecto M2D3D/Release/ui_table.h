/********************************************************************************
** Form generated from reading UI file 'table.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLE_H
#define UI_TABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_table
{
public:
    QTableView *tv_datatable;

    void setupUi(QDialog *table)
    {
        if (table->objectName().isEmpty())
            table->setObjectName(QStringLiteral("table"));
        table->resize(721, 378);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/mass_main.png"), QSize(), QIcon::Normal, QIcon::Off);
        table->setWindowIcon(icon);
        tv_datatable = new QTableView(table);
        tv_datatable->setObjectName(QStringLiteral("tv_datatable"));
        tv_datatable->setGeometry(QRect(10, 10, 701, 361));
        tv_datatable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tv_datatable->setWordWrap(true);
        tv_datatable->horizontalHeader()->setDefaultSectionSize(100);
        tv_datatable->horizontalHeader()->setHighlightSections(false);
        tv_datatable->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tv_datatable->horizontalHeader()->setStretchLastSection(false);
        tv_datatable->verticalHeader()->setVisible(false);

        retranslateUi(table);

        QMetaObject::connectSlotsByName(table);
    } // setupUi

    void retranslateUi(QDialog *table)
    {
        table->setWindowTitle(QApplication::translate("table", "MASS Report Table", 0));
    } // retranslateUi

};

namespace Ui {
    class table: public Ui_table {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLE_H
