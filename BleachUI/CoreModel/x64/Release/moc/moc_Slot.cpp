/****************************************************************************
** Meta object code from reading C++ file 'Slot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../model/Slot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Slot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_model__InputSlot_t {
    QByteArrayData data[4];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_model__InputSlot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_model__InputSlot_t qt_meta_stringdata_model__InputSlot = {
    {
QT_MOC_LITERAL(0, 0, 16), // "model::InputSlot"
QT_MOC_LITERAL(1, 17, 12), // "dataReceived"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14) // "onDataReceived"

    },
    "model::InputSlot\0dataReceived\0\0"
    "onDataReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_model__InputSlot[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void model::InputSlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InputSlot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataReceived(); break;
        case 1: _t->onDataReceived(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InputSlot::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InputSlot::dataReceived)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject model::InputSlot::staticMetaObject = { {
    QMetaObject::SuperData::link<Slot::staticMetaObject>(),
    qt_meta_stringdata_model__InputSlot.data,
    qt_meta_data_model__InputSlot,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *model::InputSlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *model::InputSlot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_model__InputSlot.stringdata0))
        return static_cast<void*>(this);
    return Slot::qt_metacast(_clname);
}

int model::InputSlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Slot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void model::InputSlot::dataReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_model__OutputSlot_t {
    QByteArrayData data[3];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_model__OutputSlot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_model__OutputSlot_t qt_meta_stringdata_model__OutputSlot = {
    {
QT_MOC_LITERAL(0, 0, 17), // "model::OutputSlot"
QT_MOC_LITERAL(1, 18, 8), // "dataSent"
QT_MOC_LITERAL(2, 27, 0) // ""

    },
    "model::OutputSlot\0dataSent\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_model__OutputSlot[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void model::OutputSlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OutputSlot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataSent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OutputSlot::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputSlot::dataSent)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject model::OutputSlot::staticMetaObject = { {
    QMetaObject::SuperData::link<Slot::staticMetaObject>(),
    qt_meta_stringdata_model__OutputSlot.data,
    qt_meta_data_model__OutputSlot,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *model::OutputSlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *model::OutputSlot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_model__OutputSlot.stringdata0))
        return static_cast<void*>(this);
    return Slot::qt_metacast(_clname);
}

int model::OutputSlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Slot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void model::OutputSlot::dataSent()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
