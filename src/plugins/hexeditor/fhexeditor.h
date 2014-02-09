#ifndef FHEXEDITOR_H
#define FHEXEDITOR_H

#include <QtGui>

#include "fwindowtab.h"
#include "fdatapackage.h"
#include "fhexview.h"

class FHexEditor : public FWindowTab
{
    Q_OBJECT

public:
    FHexEditor(QWidget *parent = 0);

    void SetData(FDataPackage *_DataPackage);
    FDataPackage* GetData() { return DataPackage; }
    void RefreshData();

    FHexView *HexView;

private slots:
    void on_HexView_PointerChange(int _Pointer);
    void on_TreeWidDecode_ItemClicked(QTreeWidgetItem*,int);

private:
    void CreateForm();
    void CreateConnections();
    FDataPackage *DataPackage;
    FData *Data;
    FDataMode DataMode;

    QTreeWidget *TreeWidDecode;
    QTreeWidgetItem *ValBinary8b;
    QTreeWidgetItem *ValOctanal8b;
    QTreeWidgetItem *ValHex8b;
    QTreeWidgetItem *ValHex16b;
    QTreeWidgetItem *ValHex32b;
    QTreeWidgetItem *ValDec8b;
    QTreeWidgetItem *ValDec8bS;
    QTreeWidgetItem *ValDec16b;
    QTreeWidgetItem *ValDec16bS;
    QTreeWidgetItem *ValDec32b;
    QTreeWidgetItem *ValDec32bS;
    QTreeWidgetItem *ValFP32b;
    QTreeWidgetItem *ValChar8b;
    QTreeWidgetItem *ValUtf8;



};

#endif // FHEXEDITOR_H
