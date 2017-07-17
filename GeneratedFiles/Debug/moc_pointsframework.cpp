/****************************************************************************
** Meta object code from reading C++ file 'pointsframework.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pointsframework.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pointsframework.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PointsFramework_t {
    QByteArrayData data[15];
    char stringdata0[335];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PointsFramework_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PointsFramework_t qt_meta_stringdata_PointsFramework = {
    {
QT_MOC_LITERAL(0, 0, 15), // "PointsFramework"
QT_MOC_LITERAL(1, 16, 23), // "onGenerateNormalClicked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 26), // "onGenerateBlueNoiseClicked"
QT_MOC_LITERAL(4, 68, 32), // "onGenerateBlueNoiseNormalClicked"
QT_MOC_LITERAL(5, 101, 36), // "onGenerateMulticlassBlueNoise..."
QT_MOC_LITERAL(6, 138, 23), // "onGenerateRandomClicked"
QT_MOC_LITERAL(7, 162, 28), // "onGenerateMultiNormalClicked"
QT_MOC_LITERAL(8, 191, 25), // "onGenerateSequenceClicked"
QT_MOC_LITERAL(9, 217, 27), // "onGenerateSequence2DClicked"
QT_MOC_LITERAL(10, 245, 18), // "onCalculateClicked"
QT_MOC_LITERAL(11, 264, 20), // "onCalculateDCClicked"
QT_MOC_LITERAL(12, 285, 16), // "onPickingClicked"
QT_MOC_LITERAL(13, 302, 8), // "bChecked"
QT_MOC_LITERAL(14, 311, 23) // "onGenerateSpiralClicked"

    },
    "PointsFramework\0onGenerateNormalClicked\0"
    "\0onGenerateBlueNoiseClicked\0"
    "onGenerateBlueNoiseNormalClicked\0"
    "onGenerateMulticlassBlueNoiseClicked\0"
    "onGenerateRandomClicked\0"
    "onGenerateMultiNormalClicked\0"
    "onGenerateSequenceClicked\0"
    "onGenerateSequence2DClicked\0"
    "onCalculateClicked\0onCalculateDCClicked\0"
    "onPickingClicked\0bChecked\0"
    "onGenerateSpiralClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PointsFramework[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    0,   75,    2, 0x0a /* Public */,
       4,    0,   76,    2, 0x0a /* Public */,
       5,    0,   77,    2, 0x0a /* Public */,
       6,    0,   78,    2, 0x0a /* Public */,
       7,    0,   79,    2, 0x0a /* Public */,
       8,    0,   80,    2, 0x0a /* Public */,
       9,    0,   81,    2, 0x0a /* Public */,
      10,    0,   82,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    1,   84,    2, 0x0a /* Public */,
      14,    0,   87,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void,

       0        // eod
};

void PointsFramework::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PointsFramework *_t = static_cast<PointsFramework *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onGenerateNormalClicked(); break;
        case 1: _t->onGenerateBlueNoiseClicked(); break;
        case 2: _t->onGenerateBlueNoiseNormalClicked(); break;
        case 3: _t->onGenerateMulticlassBlueNoiseClicked(); break;
        case 4: _t->onGenerateRandomClicked(); break;
        case 5: _t->onGenerateMultiNormalClicked(); break;
        case 6: _t->onGenerateSequenceClicked(); break;
        case 7: _t->onGenerateSequence2DClicked(); break;
        case 8: _t->onCalculateClicked(); break;
        case 9: _t->onCalculateDCClicked(); break;
        case 10: _t->onPickingClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->onGenerateSpiralClicked(); break;
        default: ;
        }
    }
}

const QMetaObject PointsFramework::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PointsFramework.data,
      qt_meta_data_PointsFramework,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PointsFramework::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PointsFramework::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PointsFramework.stringdata0))
        return static_cast<void*>(const_cast< PointsFramework*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PointsFramework::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
