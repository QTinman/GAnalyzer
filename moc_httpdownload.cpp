/****************************************************************************
** Meta object code from reading C++ file 'httpdownload.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.0.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "httpdownload.h"
#include <QtNetwork/QSslError>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httpdownload.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.0.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HttpDownload_t {
    const uint offsetsAndSize[20];
    char stringdata0[165];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_HttpDownload_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_HttpDownload_t qt_meta_stringdata_HttpDownload = {
    {
QT_MOC_LITERAL(0, 12), // "HttpDownload"
QT_MOC_LITERAL(13, 8), // "dl_ready"
QT_MOC_LITERAL(22, 0), // ""
QT_MOC_LITERAL(23, 25), // "on_downloadButton_clicked"
QT_MOC_LITERAL(49, 21), // "on_quitButton_clicked"
QT_MOC_LITERAL(71, 13), // "httpReadyRead"
QT_MOC_LITERAL(85, 20), // "httpDownloadFinished"
QT_MOC_LITERAL(106, 22), // "updateDownloadProgress"
QT_MOC_LITERAL(129, 20), // "enableDownloadButton"
QT_MOC_LITERAL(150, 14) // "cancelDownload"

    },
    "HttpDownload\0dl_ready\0\0on_downloadButton_clicked\0"
    "on_quitButton_clicked\0httpReadyRead\0"
    "httpDownloadFinished\0updateDownloadProgress\0"
    "enableDownloadButton\0cancelDownload"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HttpDownload[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    0 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   65,    2, 0x08,    2 /* Private */,
       4,    0,   66,    2, 0x08,    3 /* Private */,
       5,    0,   67,    2, 0x08,    4 /* Private */,
       6,    0,   68,    2, 0x08,    5 /* Private */,
       7,    2,   69,    2, 0x08,    6 /* Private */,
       8,    0,   74,    2, 0x08,    9 /* Private */,
       9,    0,   75,    2, 0x08,   10 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HttpDownload::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HttpDownload *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dl_ready((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->on_downloadButton_clicked(); break;
        case 2: _t->on_quitButton_clicked(); break;
        case 3: _t->httpReadyRead(); break;
        case 4: _t->httpDownloadFinished(); break;
        case 5: _t->updateDownloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 6: _t->enableDownloadButton(); break;
        case 7: _t->cancelDownload(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HttpDownload::*)(const QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HttpDownload::dl_ready)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject HttpDownload::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_HttpDownload.offsetsAndSize,
    qt_meta_data_HttpDownload,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_HttpDownload_t
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qint64, std::false_type>, QtPrivate::TypeAndForceComplete<qint64, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *HttpDownload::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HttpDownload::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HttpDownload.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int HttpDownload::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void HttpDownload::dl_ready(const QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
