/********************************************************************************
** Form generated from reading UI file 'zoomgraph.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZOOMGRAPH_H
#define UI_ZOOMGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_zoomgraph
{
public:
    QCustomPlot *cp_z_graph;

    void setupUi(QDialog *zoomgraph)
    {
        if (zoomgraph->objectName().isEmpty())
            zoomgraph->setObjectName(QStringLiteral("zoomgraph"));
        zoomgraph->resize(703, 462);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/mass_main.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomgraph->setWindowIcon(icon);
        cp_z_graph = new QCustomPlot(zoomgraph);
        cp_z_graph->setObjectName(QStringLiteral("cp_z_graph"));
        cp_z_graph->setGeometry(QRect(9, 9, 681, 441));

        retranslateUi(zoomgraph);

        QMetaObject::connectSlotsByName(zoomgraph);
    } // setupUi

    void retranslateUi(QDialog *zoomgraph)
    {
        zoomgraph->setWindowTitle(QApplication::translate("zoomgraph", "MASS Graph", 0));
    } // retranslateUi

};

namespace Ui {
    class zoomgraph: public Ui_zoomgraph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZOOMGRAPH_H
