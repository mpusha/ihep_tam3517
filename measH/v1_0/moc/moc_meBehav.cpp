/****************************************************************************
** Meta object code from reading C++ file 'meBehav.h'
**
** Created: Fri Feb 9 10:43:53 2018
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../meBehav.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meBehav.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TMeasH[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      22,    7,    7,    7, 0x0a,
      36,    7,    7,    7, 0x0a,
      54,    7,    7,    7, 0x0a,
      72,    7,    7,    7, 0x0a,
      85,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TMeasH[] = {
    "TMeasH\0\0timer1Event()\0timer2Event()\0"
    "timer1ToutEvent()\0timer2ToutEvent()\0"
    "meas1Event()\0meas2Event()\0"
};

void TMeasH::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TMeasH *_t = static_cast<TMeasH *>(_o);
        switch (_id) {
        case 0: _t->timer1Event(); break;
        case 1: _t->timer2Event(); break;
        case 2: _t->timer1ToutEvent(); break;
        case 3: _t->timer2ToutEvent(); break;
        case 4: _t->meas1Event(); break;
        case 5: _t->meas2Event(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TMeasH::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TMeasH::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TMeasH,
      qt_meta_data_TMeasH, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TMeasH::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TMeasH::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TMeasH::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TMeasH))
        return static_cast<void*>(const_cast< TMeasH*>(this));
    return QThread::qt_metacast(_clname);
}

int TMeasH::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
