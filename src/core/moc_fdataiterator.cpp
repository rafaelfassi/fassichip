/****************************************************************************
** Meta object code from reading C++ file 'fdataiterator.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "data/fdataiterator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fdataiterator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FDataIterator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       4,   24, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      27,   22,   15,   14, 0x02,
      49,   42,   15,   14, 0x02,

 // properties: name, type, flags
      71,   67, 0x02095003,
      88,   80, 0x0a095003,
     102,   97, 0x01095003,
     109,   97, 0x01095003,

       0        // eod
};

static const char qt_meta_stringdata_FDataIterator[] = {
    "FDataIterator\0\0double\0_Pos\0ReadValue(int)\0"
    "_Value\0To_16Bits(double)\0int\0FileType\0"
    "QString\0ModeBits\0bool\0Signal\0Swap\0"
};

void FDataIterator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FDataIterator *_t = static_cast<FDataIterator *>(_o);
        switch (_id) {
        case 0: { double _r = _t->ReadValue((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 1: { double _r = _t->To_16Bits((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FDataIterator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FDataIterator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FDataIterator,
      qt_meta_data_FDataIterator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FDataIterator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FDataIterator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FDataIterator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FDataIterator))
        return static_cast<void*>(const_cast< FDataIterator*>(this));
    return QObject::qt_metacast(_clname);
}

int FDataIterator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = GetFileType(); break;
        case 1: *reinterpret_cast< QString*>(_v) = GetModeBits(); break;
        case 2: *reinterpret_cast< bool*>(_v) = GetSignal(); break;
        case 3: *reinterpret_cast< bool*>(_v) = GetSwap(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: SetFileType(*reinterpret_cast< int*>(_v)); break;
        case 1: SetModeBits(*reinterpret_cast< QString*>(_v)); break;
        case 2: SetSignal(*reinterpret_cast< bool*>(_v)); break;
        case 3: SetSwap(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
