#ifndef FVIEWER3D_H
#define FVIEWER3D_H

#include <QtGui>

#include "fwindowtab.h"
#include "fdatapackage.h"
#include "fchart3d.h"

class FViewer3d : public FWindowTab
{  
    Q_OBJECT

public:
    FViewer3d(QWidget *parent = 0);

    void SetData(FDataPackage *_DataPackage);
    FDataPackage* GetData() { return DataPackage; }
    FChart3d *Chart3d;
    QTableWidget *TableChart3d;

public slots:
    void on_Refresh_Triggered();
    void on_TableChart3d_ItemSelectionChanged(int y0, int x0, int y1, int x1);

private:
    void CreateActions();
    void CreateToolBar();
    void CreateConnections();
    FDataPackage *DataPackage;
    FDataMap3d *DataMap3d;

    QAction *ActRefresh;
    QToolBar *ToolBar;

    QLineEdit TesteLineEdit;
};


#endif // FVIEWER3D_H
