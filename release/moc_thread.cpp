/****************************************************************************
** Meta object code from reading C++ file 'thread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../thread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Thread_t {
    QByteArrayData data[16];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Thread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Thread_t qt_meta_stringdata_Thread = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Thread"
QT_MOC_LITERAL(1, 7, 12), // "send_message"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 8), // "Message&"
QT_MOC_LITERAL(4, 30, 7), // "message"
QT_MOC_LITERAL(5, 38, 9), // "send_opts"
QT_MOC_LITERAL(6, 48, 4), // "Opt&"
QT_MOC_LITERAL(7, 53, 3), // "opt"
QT_MOC_LITERAL(8, 57, 17), // "send_progress_max"
QT_MOC_LITERAL(9, 75, 4), // "size"
QT_MOC_LITERAL(10, 80, 18), // "send_process_value"
QT_MOC_LITERAL(11, 99, 8), // "filesize"
QT_MOC_LITERAL(12, 108, 13), // "send_speed_up"
QT_MOC_LITERAL(13, 122, 5), // "speed"
QT_MOC_LITERAL(14, 128, 15), // "send_speed_down"
QT_MOC_LITERAL(15, 144, 10) // "send_speed"

    },
    "Thread\0send_message\0\0Message&\0message\0"
    "send_opts\0Opt&\0opt\0send_progress_max\0"
    "size\0send_process_value\0filesize\0"
    "send_speed_up\0speed\0send_speed_down\0"
    "send_speed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Thread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,
       8,    1,   55,    2, 0x06 /* Public */,
      10,    1,   58,    2, 0x06 /* Public */,
      12,    1,   61,    2, 0x06 /* Public */,
      14,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Thread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Thread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_message((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 1: _t->send_opts((*reinterpret_cast< Opt(*)>(_a[1]))); break;
        case 2: _t->send_progress_max((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->send_process_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->send_speed_up((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->send_speed_down((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->send_speed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Thread::*)(Message & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thread::send_message)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Thread::*)(Opt & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thread::send_opts)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Thread::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thread::send_progress_max)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Thread::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thread::send_process_value)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Thread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thread::send_speed_up)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Thread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Thread::send_speed_down)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Thread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_Thread.data,
    qt_meta_data_Thread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Thread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Thread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Thread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Thread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Thread::send_message(Message & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Thread::send_opts(Opt & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Thread::send_progress_max(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Thread::send_process_value(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Thread::send_speed_up(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Thread::send_speed_down(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
