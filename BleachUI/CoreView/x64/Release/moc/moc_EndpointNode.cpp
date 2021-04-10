/****************************************************************************
** Meta object code from reading C++ file 'EndpointNode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../logic/EndpointNode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EndpointNode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_model__EndpointNode_t {
    QByteArrayData data[24];
    char stringdata0[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_model__EndpointNode_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_model__EndpointNode_t qt_meta_stringdata_model__EndpointNode = {
    {
QT_MOC_LITERAL(0, 0, 19), // "model::EndpointNode"
QT_MOC_LITERAL(1, 20, 17), // "httpMethodChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 14), // "timeoutChanged"
QT_MOC_LITERAL(4, 54, 10), // "urlChanged"
QT_MOC_LITERAL(5, 65, 18), // "contentTypeChanged"
QT_MOC_LITERAL(6, 84, 17), // "consoleLogChanged"
QT_MOC_LITERAL(7, 102, 16), // "userAgentChanged"
QT_MOC_LITERAL(8, 119, 15), // "processResponse"
QT_MOC_LITERAL(9, 135, 14), // "QNetworkReply*"
QT_MOC_LITERAL(10, 150, 5), // "reply"
QT_MOC_LITERAL(11, 156, 9), // "onTimeout"
QT_MOC_LITERAL(12, 166, 3), // "url"
QT_MOC_LITERAL(13, 170, 7), // "timeout"
QT_MOC_LITERAL(14, 178, 10), // "httpMethod"
QT_MOC_LITERAL(15, 189, 11), // "contentType"
QT_MOC_LITERAL(16, 201, 10), // "consoleLog"
QT_MOC_LITERAL(17, 212, 9), // "userAgent"
QT_MOC_LITERAL(18, 222, 10), // "HttpMethod"
QT_MOC_LITERAL(19, 233, 3), // "GET"
QT_MOC_LITERAL(20, 237, 4), // "POST"
QT_MOC_LITERAL(21, 242, 3), // "PUT"
QT_MOC_LITERAL(22, 246, 5), // "PATCH"
QT_MOC_LITERAL(23, 252, 3) // "DEL"

    },
    "model::EndpointNode\0httpMethodChanged\0"
    "\0timeoutChanged\0urlChanged\0"
    "contentTypeChanged\0consoleLogChanged\0"
    "userAgentChanged\0processResponse\0"
    "QNetworkReply*\0reply\0onTimeout\0url\0"
    "timeout\0httpMethod\0contentType\0"
    "consoleLog\0userAgent\0HttpMethod\0GET\0"
    "POST\0PUT\0PATCH\0DEL"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_model__EndpointNode[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       6,   76, // properties
       1,  100, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       3,    1,   57,    2, 0x06 /* Public */,
       4,    1,   60,    2, 0x06 /* Public */,
       5,    1,   63,    2, 0x06 /* Public */,
       6,    1,   66,    2, 0x06 /* Public */,
       7,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   72,    2, 0x08 /* Private */,
      11,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::QString, 0x00495103,
      13, QMetaType::Int, 0x00495103,
      14, QMetaType::Int, 0x00495103,
      15, QMetaType::QString, 0x00495103,
      16, QMetaType::QString, 0x00495103,
      17, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       2,
       1,
       0,
       3,
       4,
       5,

 // enums: name, alias, flags, count, data
      18,   18, 0x0,    5,  105,

 // enum data: key, value
      19, uint(model::EndpointNode::GET),
      20, uint(model::EndpointNode::POST),
      21, uint(model::EndpointNode::PUT),
      22, uint(model::EndpointNode::PATCH),
      23, uint(model::EndpointNode::DEL),

       0        // eod
};

void model::EndpointNode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EndpointNode *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->httpMethodChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->timeoutChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->urlChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->contentTypeChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->consoleLogChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->userAgentChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->processResponse((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 7: _t->onTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EndpointNode::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EndpointNode::httpMethodChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EndpointNode::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EndpointNode::timeoutChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (EndpointNode::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EndpointNode::urlChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (EndpointNode::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EndpointNode::contentTypeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (EndpointNode::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EndpointNode::consoleLogChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (EndpointNode::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EndpointNode::userAgentChanged)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<EndpointNode *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getUrl(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getTimeout(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->getHttpMethod(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getContentType(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getConsoleLog(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getUserAgent(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<EndpointNode *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setUrl(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setTimeout(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setHttpMethod(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setContentType(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setConsoleLog(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setUserAgent(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject model::EndpointNode::staticMetaObject = { {
    QMetaObject::SuperData::link<model::Node::staticMetaObject>(),
    qt_meta_stringdata_model__EndpointNode.data,
    qt_meta_data_model__EndpointNode,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *model::EndpointNode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *model::EndpointNode::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_model__EndpointNode.stringdata0))
        return static_cast<void*>(this);
    return model::Node::qt_metacast(_clname);
}

int model::EndpointNode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = model::Node::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void model::EndpointNode::httpMethodChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void model::EndpointNode::timeoutChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void model::EndpointNode::urlChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void model::EndpointNode::contentTypeChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void model::EndpointNode::consoleLogChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void model::EndpointNode::userAgentChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
struct qt_meta_stringdata_logic__EndpointNode_t {
    QByteArrayData data[5];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_logic__EndpointNode_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_logic__EndpointNode_t qt_meta_stringdata_logic__EndpointNode = {
    {
QT_MOC_LITERAL(0, 0, 19), // "logic::EndpointNode"
QT_MOC_LITERAL(1, 20, 12), // "EndpointNode"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 20), // "model::EndpointNode*"
QT_MOC_LITERAL(4, 55, 9) // "modelNode"

    },
    "logic::EndpointNode\0EndpointNode\0\0"
    "model::EndpointNode*\0modelNode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_logic__EndpointNode[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       1,   17, // constructors
       0,       // flags
       0,       // signalCount

 // constructors: parameters
    0x80000000 | 2, 0x80000000 | 3,    4,

 // constructors: name, argc, parameters, tag, flags
       1,    1,   14,    2, 0x0e /* Public */,

       0        // eod
};

void logic::EndpointNode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { EndpointNode *_r = new EndpointNode((*reinterpret_cast< model::EndpointNode*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    }
    Q_UNUSED(_o);
}

QT_INIT_METAOBJECT const QMetaObject logic::EndpointNode::staticMetaObject = { {
    QMetaObject::SuperData::link<view::Node::staticMetaObject>(),
    qt_meta_stringdata_logic__EndpointNode.data,
    qt_meta_data_logic__EndpointNode,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *logic::EndpointNode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *logic::EndpointNode::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_logic__EndpointNode.stringdata0))
        return static_cast<void*>(this);
    return view::Node::qt_metacast(_clname);
}

int logic::EndpointNode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = view::Node::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
