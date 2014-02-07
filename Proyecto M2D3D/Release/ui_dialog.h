/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextEdit>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGroupBox *gb_qvalue;
    QLabel *l_qmax;
    QLabel *l_qmin;
    QLabel *l_qstep;
    QLineEdit *le_qmax;
    QLineEdit *le_qmin;
    QLineEdit *le_qstep;
    QLabel *l_title;
    QGroupBox *gb_lvalue;
    QRadioButton *rb_lvalauto;
    QRadioButton *rb_lvalmanual;
    QTextEdit *te_lvalues;
    QGroupBox *gb_dimension;
    QRadioButton *rb_mass2d;
    QRadioButton *rb_mass3d;
    QGroupBox *gb_datamanag;
    QLabel *l_preshow;
    QLabel *l_deleteall;
    QPushButton *pb_deleteall;
    QLabel *l_convert;
    QLabel *l_add_raw;
    QPushButton *pb_preshow;
    QPushButton *pb_convert;
    QPushButton *pb_addraw;
    QLabel *l_deleteone;
    QListView *lv_filenames;
    QLineEdit *le_height;
    QLineEdit *le_width;
    QLineEdit *le_depth;
    QLabel *l_iheight;
    QLabel *l_iwidth;
    QLabel *l_idepth;
    QPushButton *pb_deleteone;
    QGroupBox *gb_controls;
    QPushButton *pb_run;
    QLabel *l_run;
    QPushButton *pb_showresult;
    QLabel *l_showres;
    QPushButton *pb_seetable;
    QPushButton *pb_saveresult;
    QLabel *l_Table;
    QLabel *l_saveresult;
    QComboBox *cb_selgraph;
    QComboBox *cb_qvalues;
    QLabel *l_selgraph_2;
    QPushButton *pb_zoomgraph;
    QLabel *l_zoomres;
    QGroupBox *gb_graphics;
    QCustomPlot *cp_graph1;
    QGroupBox *gb_Status;
    QTextEdit *te_status;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(770, 523);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/mass_main.png"), QSize(), QIcon::Normal, QIcon::Off);
        Dialog->setWindowIcon(icon);
        Dialog->setWindowOpacity(1);
        gb_qvalue = new QGroupBox(Dialog);
        gb_qvalue->setObjectName(QStringLiteral("gb_qvalue"));
        gb_qvalue->setGeometry(QRect(10, 380, 141, 131));
        l_qmax = new QLabel(gb_qvalue);
        l_qmax->setObjectName(QStringLiteral("l_qmax"));
        l_qmax->setGeometry(QRect(10, 30, 51, 16));
        l_qmin = new QLabel(gb_qvalue);
        l_qmin->setObjectName(QStringLiteral("l_qmin"));
        l_qmin->setGeometry(QRect(10, 60, 51, 16));
        l_qstep = new QLabel(gb_qvalue);
        l_qstep->setObjectName(QStringLiteral("l_qstep"));
        l_qstep->setGeometry(QRect(10, 90, 46, 13));
        le_qmax = new QLineEdit(gb_qvalue);
        le_qmax->setObjectName(QStringLiteral("le_qmax"));
        le_qmax->setGeometry(QRect(60, 30, 61, 20));
        le_qmax->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_qmin = new QLineEdit(gb_qvalue);
        le_qmin->setObjectName(QStringLiteral("le_qmin"));
        le_qmin->setGeometry(QRect(60, 60, 61, 20));
        le_qmin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_qstep = new QLineEdit(gb_qvalue);
        le_qstep->setObjectName(QStringLiteral("le_qstep"));
        le_qstep->setGeometry(QRect(60, 90, 61, 20));
        le_qstep->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        l_title = new QLabel(Dialog);
        l_title->setObjectName(QStringLiteral("l_title"));
        l_title->setGeometry(QRect(10, 10, 651, 20));
        l_title->setAlignment(Qt::AlignCenter);
        l_title->setWordWrap(false);
        gb_lvalue = new QGroupBox(Dialog);
        gb_lvalue->setObjectName(QStringLiteral("gb_lvalue"));
        gb_lvalue->setGeometry(QRect(160, 380, 161, 131));
        rb_lvalauto = new QRadioButton(gb_lvalue);
        rb_lvalauto->setObjectName(QStringLiteral("rb_lvalauto"));
        rb_lvalauto->setGeometry(QRect(10, 20, 82, 17));
        rb_lvalmanual = new QRadioButton(gb_lvalue);
        rb_lvalmanual->setObjectName(QStringLiteral("rb_lvalmanual"));
        rb_lvalmanual->setGeometry(QRect(90, 20, 61, 17));
        te_lvalues = new QTextEdit(gb_lvalue);
        te_lvalues->setObjectName(QStringLiteral("te_lvalues"));
        te_lvalues->setGeometry(QRect(10, 50, 141, 71));
        gb_dimension = new QGroupBox(Dialog);
        gb_dimension->setObjectName(QStringLiteral("gb_dimension"));
        gb_dimension->setGeometry(QRect(10, 40, 311, 51));
        rb_mass2d = new QRadioButton(gb_dimension);
        rb_mass2d->setObjectName(QStringLiteral("rb_mass2d"));
        rb_mass2d->setGeometry(QRect(20, 20, 82, 17));
        rb_mass3d = new QRadioButton(gb_dimension);
        rb_mass3d->setObjectName(QStringLiteral("rb_mass3d"));
        rb_mass3d->setGeometry(QRect(200, 20, 82, 17));
        gb_datamanag = new QGroupBox(Dialog);
        gb_datamanag->setObjectName(QStringLiteral("gb_datamanag"));
        gb_datamanag->setGeometry(QRect(10, 100, 311, 271));
        l_preshow = new QLabel(gb_datamanag);
        l_preshow->setObjectName(QStringLiteral("l_preshow"));
        l_preshow->setGeometry(QRect(250, 80, 51, 31));
        l_preshow->setAlignment(Qt::AlignCenter);
        l_preshow->setWordWrap(true);
        l_deleteall = new QLabel(gb_datamanag);
        l_deleteall->setObjectName(QStringLiteral("l_deleteall"));
        l_deleteall->setGeometry(QRect(190, 80, 51, 31));
        l_deleteall->setAlignment(Qt::AlignCenter);
        l_deleteall->setWordWrap(true);
        pb_deleteall = new QPushButton(gb_datamanag);
        pb_deleteall->setObjectName(QStringLiteral("pb_deleteall"));
        pb_deleteall->setGeometry(QRect(190, 20, 51, 61));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/delete_all.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_deleteall->setIcon(icon1);
        pb_deleteall->setIconSize(QSize(32, 32));
        l_convert = new QLabel(gb_datamanag);
        l_convert->setObjectName(QStringLiteral("l_convert"));
        l_convert->setGeometry(QRect(10, 80, 51, 31));
        l_convert->setAlignment(Qt::AlignCenter);
        l_convert->setWordWrap(true);
        l_add_raw = new QLabel(gb_datamanag);
        l_add_raw->setObjectName(QStringLiteral("l_add_raw"));
        l_add_raw->setGeometry(QRect(70, 80, 51, 31));
        l_add_raw->setAlignment(Qt::AlignCenter);
        l_add_raw->setWordWrap(true);
        pb_preshow = new QPushButton(gb_datamanag);
        pb_preshow->setObjectName(QStringLiteral("pb_preshow"));
        pb_preshow->setGeometry(QRect(250, 20, 51, 61));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/pre_process.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_preshow->setIcon(icon2);
        pb_preshow->setIconSize(QSize(32, 32));
        pb_convert = new QPushButton(gb_datamanag);
        pb_convert->setObjectName(QStringLiteral("pb_convert"));
        pb_convert->setGeometry(QRect(10, 20, 51, 61));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/convertRaw.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_convert->setIcon(icon3);
        pb_convert->setIconSize(QSize(32, 32));
        pb_addraw = new QPushButton(gb_datamanag);
        pb_addraw->setObjectName(QStringLiteral("pb_addraw"));
        pb_addraw->setGeometry(QRect(70, 20, 51, 61));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/add_raw.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_addraw->setIcon(icon4);
        pb_addraw->setIconSize(QSize(32, 32));
        l_deleteone = new QLabel(gb_datamanag);
        l_deleteone->setObjectName(QStringLiteral("l_deleteone"));
        l_deleteone->setGeometry(QRect(130, 80, 51, 31));
        l_deleteone->setAlignment(Qt::AlignCenter);
        l_deleteone->setWordWrap(true);
        lv_filenames = new QListView(gb_datamanag);
        lv_filenames->setObjectName(QStringLiteral("lv_filenames"));
        lv_filenames->setGeometry(QRect(10, 120, 291, 101));
        lv_filenames->setProperty("isWrapping", QVariant(false));
        lv_filenames->setWordWrap(true);
        le_height = new QLineEdit(gb_datamanag);
        le_height->setObjectName(QStringLiteral("le_height"));
        le_height->setGeometry(QRect(50, 240, 51, 20));
        le_height->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_height->setReadOnly(true);
        le_width = new QLineEdit(gb_datamanag);
        le_width->setObjectName(QStringLiteral("le_width"));
        le_width->setGeometry(QRect(150, 240, 51, 20));
        le_width->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_width->setReadOnly(true);
        le_depth = new QLineEdit(gb_datamanag);
        le_depth->setObjectName(QStringLiteral("le_depth"));
        le_depth->setGeometry(QRect(250, 240, 51, 20));
        le_depth->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_depth->setReadOnly(true);
        l_iheight = new QLabel(gb_datamanag);
        l_iheight->setObjectName(QStringLiteral("l_iheight"));
        l_iheight->setGeometry(QRect(10, 240, 51, 16));
        l_iwidth = new QLabel(gb_datamanag);
        l_iwidth->setObjectName(QStringLiteral("l_iwidth"));
        l_iwidth->setGeometry(QRect(110, 240, 41, 16));
        l_idepth = new QLabel(gb_datamanag);
        l_idepth->setObjectName(QStringLiteral("l_idepth"));
        l_idepth->setGeometry(QRect(210, 240, 41, 16));
        pb_deleteone = new QPushButton(gb_datamanag);
        pb_deleteone->setObjectName(QStringLiteral("pb_deleteone"));
        pb_deleteone->setGeometry(QRect(130, 20, 51, 61));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/delete_one.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_deleteone->setIcon(icon5);
        pb_deleteone->setIconSize(QSize(32, 32));
        gb_controls = new QGroupBox(Dialog);
        gb_controls->setObjectName(QStringLiteral("gb_controls"));
        gb_controls->setGeometry(QRect(330, 40, 431, 121));
        pb_run = new QPushButton(gb_controls);
        pb_run->setObjectName(QStringLiteral("pb_run"));
        pb_run->setGeometry(QRect(10, 20, 51, 61));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_run->setIcon(icon6);
        pb_run->setIconSize(QSize(32, 32));
        l_run = new QLabel(gb_controls);
        l_run->setObjectName(QStringLiteral("l_run"));
        l_run->setGeometry(QRect(10, 80, 61, 31));
        l_run->setAlignment(Qt::AlignCenter);
        pb_showresult = new QPushButton(gb_controls);
        pb_showresult->setObjectName(QStringLiteral("pb_showresult"));
        pb_showresult->setGeometry(QRect(190, 20, 51, 61));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/show_result.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_showresult->setIcon(icon7);
        pb_showresult->setIconSize(QSize(32, 32));
        l_showres = new QLabel(gb_controls);
        l_showres->setObjectName(QStringLiteral("l_showres"));
        l_showres->setGeometry(QRect(190, 80, 51, 31));
        l_showres->setAlignment(Qt::AlignCenter);
        l_showres->setWordWrap(true);
        pb_seetable = new QPushButton(gb_controls);
        pb_seetable->setObjectName(QStringLiteral("pb_seetable"));
        pb_seetable->setGeometry(QRect(310, 20, 51, 61));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/table.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_seetable->setIcon(icon8);
        pb_seetable->setIconSize(QSize(32, 32));
        pb_saveresult = new QPushButton(gb_controls);
        pb_saveresult->setObjectName(QStringLiteral("pb_saveresult"));
        pb_saveresult->setGeometry(QRect(370, 20, 51, 61));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_saveresult->setIcon(icon9);
        pb_saveresult->setIconSize(QSize(32, 32));
        l_Table = new QLabel(gb_controls);
        l_Table->setObjectName(QStringLiteral("l_Table"));
        l_Table->setGeometry(QRect(310, 80, 51, 31));
        l_Table->setAlignment(Qt::AlignCenter);
        l_Table->setWordWrap(true);
        l_saveresult = new QLabel(gb_controls);
        l_saveresult->setObjectName(QStringLiteral("l_saveresult"));
        l_saveresult->setGeometry(QRect(370, 80, 51, 31));
        l_saveresult->setAlignment(Qt::AlignCenter);
        l_saveresult->setWordWrap(true);
        cb_selgraph = new QComboBox(gb_controls);
        cb_selgraph->setObjectName(QStringLiteral("cb_selgraph"));
        cb_selgraph->setGeometry(QRect(70, 20, 111, 21));
        cb_qvalues = new QComboBox(gb_controls);
        cb_qvalues->setObjectName(QStringLiteral("cb_qvalues"));
        cb_qvalues->setGeometry(QRect(70, 50, 111, 21));
        cb_qvalues->setEditable(false);
        l_selgraph_2 = new QLabel(gb_controls);
        l_selgraph_2->setObjectName(QStringLiteral("l_selgraph_2"));
        l_selgraph_2->setGeometry(QRect(90, 80, 71, 31));
        l_selgraph_2->setAlignment(Qt::AlignCenter);
        l_selgraph_2->setWordWrap(true);
        pb_zoomgraph = new QPushButton(gb_controls);
        pb_zoomgraph->setObjectName(QStringLiteral("pb_zoomgraph"));
        pb_zoomgraph->setGeometry(QRect(250, 20, 51, 61));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/zoom_extend.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_zoomgraph->setIcon(icon10);
        pb_zoomgraph->setIconSize(QSize(32, 32));
        l_zoomres = new QLabel(gb_controls);
        l_zoomres->setObjectName(QStringLiteral("l_zoomres"));
        l_zoomres->setGeometry(QRect(250, 80, 51, 31));
        l_zoomres->setAlignment(Qt::AlignCenter);
        l_zoomres->setWordWrap(true);
        gb_graphics = new QGroupBox(Dialog);
        gb_graphics->setObjectName(QStringLiteral("gb_graphics"));
        gb_graphics->setGeometry(QRect(330, 170, 431, 271));
        cp_graph1 = new QCustomPlot(gb_graphics);
        cp_graph1->setObjectName(QStringLiteral("cp_graph1"));
        cp_graph1->setGeometry(QRect(10, 20, 411, 241));
        gb_Status = new QGroupBox(Dialog);
        gb_Status->setObjectName(QStringLiteral("gb_Status"));
        gb_Status->setGeometry(QRect(330, 450, 431, 61));
        te_status = new QTextEdit(gb_Status);
        te_status->setObjectName(QStringLiteral("te_status"));
        te_status->setGeometry(QRect(10, 20, 411, 31));
        te_status->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        te_status->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Multifractal Analysis and Scale System 2D & 3D", 0));
        gb_qvalue->setTitle(QApplication::translate("Dialog", "Q Values", 0));
        l_qmax->setText(QApplication::translate("Dialog", "Qmax:", 0));
        l_qmin->setText(QApplication::translate("Dialog", "Qmin:", 0));
        l_qstep->setText(QApplication::translate("Dialog", "Qstep:", 0));
        le_qmax->setText(QApplication::translate("Dialog", "5", 0));
        le_qmin->setText(QApplication::translate("Dialog", "-5", 0));
        le_qstep->setText(QApplication::translate("Dialog", "0.1", 0));
        l_title->setText(QApplication::translate("Dialog", "MASS 2D & 3D  CIP-LIMA V1.0", 0));
        gb_lvalue->setTitle(QApplication::translate("Dialog", "L Values", 0));
        rb_lvalauto->setText(QApplication::translate("Dialog", "Automatic", 0));
        rb_lvalmanual->setText(QApplication::translate("Dialog", "Manual", 0));
#ifndef QT_NO_TOOLTIP
        te_lvalues->setToolTip(QApplication::translate("Dialog", "Insert L values separeted by commas.\n"
"Press \"Pre Process\" button to obtain default values.", 0));
#endif // QT_NO_TOOLTIP
        gb_dimension->setTitle(QApplication::translate("Dialog", "Dimension Selection", 0));
        rb_mass2d->setText(QApplication::translate("Dialog", "MASS 2D", 0));
        rb_mass3d->setText(QApplication::translate("Dialog", "MASS 3D", 0));
        gb_datamanag->setTitle(QApplication::translate("Dialog", "Data Input", 0));
        l_preshow->setText(QApplication::translate("Dialog", "Pre Process", 0));
        l_deleteall->setText(QApplication::translate("Dialog", "Delete All", 0));
        pb_deleteall->setText(QString());
        l_convert->setText(QApplication::translate("Dialog", "Convert to Raw ", 0));
        l_add_raw->setText(QApplication::translate("Dialog", "Add Raw Data", 0));
        pb_preshow->setText(QString());
        pb_convert->setText(QString());
        pb_addraw->setText(QString());
        l_deleteone->setText(QApplication::translate("Dialog", "Delete Item", 0));
#ifndef QT_NO_TOOLTIP
        le_height->setToolTip(QApplication::translate("Dialog", "Data Hight", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        le_width->setToolTip(QApplication::translate("Dialog", "Data Width", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        le_depth->setToolTip(QApplication::translate("Dialog", "Data Depth", 0));
#endif // QT_NO_TOOLTIP
        l_iheight->setText(QApplication::translate("Dialog", "Height:", 0));
        l_iwidth->setText(QApplication::translate("Dialog", "Width:", 0));
        l_idepth->setText(QApplication::translate("Dialog", "Depth:", 0));
        pb_deleteone->setText(QString());
        gb_controls->setTitle(QApplication::translate("Dialog", "Controls", 0));
        pb_run->setText(QString());
        l_run->setText(QApplication::translate("Dialog", "Run", 0));
        pb_showresult->setText(QString());
        l_showres->setText(QApplication::translate("Dialog", "Show Result", 0));
        pb_seetable->setText(QString());
#ifndef QT_NO_TOOLTIP
        pb_saveresult->setToolTip(QApplication::translate("Dialog", "Choose a folder\n"
"to save results", 0));
#endif // QT_NO_TOOLTIP
        pb_saveresult->setText(QString());
        l_Table->setText(QApplication::translate("Dialog", "See Data Table", 0));
        l_saveresult->setText(QApplication::translate("Dialog", "Save Results", 0));
        l_selgraph_2->setText(QApplication::translate("Dialog", "Graph Selection", 0));
        pb_zoomgraph->setText(QString());
        l_zoomres->setText(QApplication::translate("Dialog", "Zoom Graph", 0));
        gb_graphics->setTitle(QApplication::translate("Dialog", "Graphics", 0));
        gb_Status->setTitle(QApplication::translate("Dialog", "Status", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
