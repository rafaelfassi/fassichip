/****************************************************************************
** Meta object code from reading C++ file 'fdatamap3d.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "data/fdatamap3d.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fdatamap3d.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FDataMap3d[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x02,
      47,   33,   11,   11, 0x02,
      86,   64,   11,   11, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_FDataMap3d[] = {
    "FDataMap3d\0\0CreateDefaultScale()\0"
    "_SizeX,_SizeY\0SetSize(int,int)\0"
    "_PosX,_PosY,_AddressZ\0SetAddressZ(int,int,int)\0"
};

void FDataMap3d::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FDataMap3d *_t = static_cast<FDataMap3d *>(_o);
        switch (_id) {
        case 0: _t->CreateDefaultScale(); break;
        case 1: _t->SetSize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->SetAddressZ((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FDataMap3d::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FDataMap3d::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FDataMap3d,
      qt_meta_data_FDataMap3d, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FDataMap3d::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FDataMap3d::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FDataMap3d::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FDataMap3d))
        return static_cast<void*>(const_cast< FDataMap3d*>(this));
    return QObject::qt_metacast(_clname);
}

int FDataMap3d::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
