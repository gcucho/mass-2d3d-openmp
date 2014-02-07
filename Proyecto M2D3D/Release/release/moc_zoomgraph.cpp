/****************************************************************************
** Meta object code from reading C++ file 'zoomgraph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PruebaQTMass1/zoomgraph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'zoomgraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_zoomgraph_t {
    QByteArrayData data[17];
    char stringdata[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_zoomgraph_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_zoomgraph_t qt_meta_stringdata_zoomgraph = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 19),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 12),
QT_MOC_LITERAL(4, 44, 12),
QT_MOC_LITERAL(5, 57, 7),
QT_MOC_LITERAL(6, 65, 16),
QT_MOC_LITERAL(7, 82, 8),
QT_MOC_LITERAL(8, 91, 11),
QT_MOC_LITERAL(9, 103, 13),
QT_MOC_LITERAL(10, 117, 13),
QT_MOC_LITERAL(11, 131, 17),
QT_MOC_LITERAL(12, 149, 16),
QT_MOC_LITERAL(13, 166, 12),
QT_MOC_LITERAL(14, 179, 14),
QT_MOC_LITERAL(15, 194, 15),
QT_MOC_LITERAL(16, 210, 16)
    },
    "zoomgraph\0InitialGraphicSetup\0\0"
    "QCustomPlot*\0UpdateZGraph\0double*\0"
    "std::vector<int>\0DoZGraph\0DoGraphFvsA\0"
    "DoGraphNNvsLL\0DoGraphMMvsLL\0"
    "DoGraphTauvsAlpha\0DoGraphDqvsAlpha\0"
    "DoGraphDqvsQ\0MinMaxOfVector\0QVector<double>\0"
    "ReceiveIndexQval\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_zoomgraph[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x0a,
       4,    8,   72,    2, 0x0a,
       7,    1,   89,    2, 0x0a,
       8,    1,   92,    2, 0x0a,
       9,    1,   95,    2, 0x0a,
      10,    1,   98,    2, 0x0a,
      11,    1,  101,    2, 0x0a,
      12,    1,  104,    2, 0x0a,
      13,    1,  107,    2, 0x0a,
      14,    2,  110,    2, 0x0a,
      16,    1,  115,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double, 0x80000000 | 6, 0x80000000 | 5, 0x80000000 | 5,    2,    2,    2,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Double, 0x80000000 | 15, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void zoomgraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        zoomgraph *_t = static_cast<zoomgraph *>(_o);
        switch (_id) {
        case 0: _t->InitialGraphicSetup((*reinterpret_cast< QCustomPlot*(*)>(_a[1]))); break;
        case 1: _t->UpdateZGraph((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< std::vector<int>(*)>(_a[6])),(*reinterpret_cast< double*(*)>(_a[7])),(*reinterpret_cast< double*(*)>(_a[8]))); break;
        case 2: _t->DoZGraph((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->DoGraphFvsA((*reinterpret_cast< QCustomPlot*(*)>(_a[1]))); break;
        case 4: _t->DoGraphNNvsLL((*reinterpret_cast< QCustomPlot*(*)>(_a[1]))); break;
        case 5: _t->DoGraphMMvsLL((*reinterpret_cast< QCustomPlot*(*)>(_a[1]))); break;
        case 6: _t->DoGraphTauvsAlpha((*reinterpret_cast< QCustomPlot*(*)>(_a[1]))); break;
        case 7: _t->DoGraphDqvsAlpha((*reinterpret_cast< QCustomPlot*(*)>(_a[1]))); break;
        case 8: _t->DoGraphDqvsQ((*reinterpret_cast< QCustomPlot*(*)>(_a[1]))); break;
        case 9: { double _r = _t->MinMaxOfVector((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 10: _t->ReceiveIndexQval((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCustomPlot* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            }
            break;
        }
    }
}

const QMetaObject zoomgraph::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_zoomgraph.data,
      qt_meta_data_zoomgraph,  qt_static_metacall, 0, 0}
};


const QMetaObject *zoomgraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *zoomgraph::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_zoomgraph.stringdata))
        return static_cast<void*>(const_cast< zoomgraph*>(this));
    return QDialog::qt_metacast(_clname);
}

int zoomgraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
