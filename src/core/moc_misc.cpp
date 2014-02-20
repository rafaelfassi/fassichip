/****************************************************************************
** Meta object code from reading C++ file 'misc.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "misc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'misc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FHexSpinBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   13,   12,   12, 0x05,
      41,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   12,   12,   12, 0x08,
      90,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FHexSpinBox[] = {
    "FHexSpinBox\0\0_Value\0ValueChanged(qint64)\0"
    "EditingFinished()\0on_EdtDisplayEditingFinished()\0"
    "on_BtnClicked()\0"
};

void FHexSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FHexSpinBox *_t = static_cast<FHexSpinBox *>(_o);
        switch (_id) {
        case 0: _t->ValueChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: _t->EditingFinished(); break;
        case 2: _t->on_EdtDisplayEditingFinished(); break;
        case 3: _t->on_BtnClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FHexSpinBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FHexSpinBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FHexSpinBox,
      qt_meta_data_FHexSpinBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FHexSpinBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FHexSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FHexSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FHexSpinBox))
        return static_cast<void*>(const_cast< FHexSpinBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int FHexSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FHexSpinBox::ValueChanged(qint64 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FHexSpinBox::EditingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
