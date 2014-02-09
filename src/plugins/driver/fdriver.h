#ifndef FDRIVER_H
#define FDRIVER_H

#include <QtGui>

#include "fxmldriver.h"
#include "fdata.h"
#include "fdatapackage.h"

class FDriver : public QWidget
{
    Q_OBJECT

public:
    FDriver(QWidget *parent = 0);
    ~FDriver();

    void SetData(FData *_Data) { Data = _Data; }
    bool OpenDriver(QString _DriverFileName);
    bool CloseDriver();
    bool SaveDriver(QString _DriverFileName);
    void ExecElement(QString _ElementName, QString _ElementType, bool _Edit);

public slots:
    void on_BtnNew_Clicked();
    void on_BtnOpen_Clicked();
    void on_BtnClose_Clicked();
    void on_BtnSave_Clicked();
    void on_BtnSaveAs_Clicked();

    void on_ActTreeElementUpDown_Triggered();
    void on_ActTreeElementRename_Triggered();
    void on_ActTreeElementDelete_Triggered();
    void on_ActTreeElementNew_Triggered();
    void on_ActTreeElementExecOrEdit_Triggered();

    void on_ActTreeGroupUpDown_Triggered();
    void on_ActTreeGroupRename_Triggered();
    void on_ActTreeGroupDelete_Triggered();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_treeWidget_ContextMenuRequested(QPoint _Point);

    void on_Element_EditingFinished();

    void RefreshTreeDrivers();

signals:
    void ReturnFunctions(FDataPackage*);

private:
    void CreateForm();
    void CreateConnections();
    QIcon GetIconToElements(QString _Type);
    QFile DriverFile;

    QPushButton *BtnNew;
    QPushButton *BtnOpen;
    QPushButton *BtnClose;
    QPushButton *BtnSave;
    QPushButton *BtnSaveAs;
    QPushButton *BtnFindDriver;   
    QPushButton *BtnProperties;
    QTreeWidget *TreeWidDrivers;
    QLabel *LabDriverOpen;

    QAction *ActTreeElementUp;
    QAction *ActTreeElementDown;
    QAction *ActTreeElementNew;
    QAction *ActTreeElementRename;
    QAction *ActTreeElementDelete;
    QAction *ActTreeElementExec;
    QAction *ActTreeElementEdit;

    QAction *ActTreeGroupUp;
    QAction *ActTreeGroupDown;
    QAction *ActTreeGroupDelete;
    QAction *ActTreeGroupRename;

    QDomDocument *DriverDocument;
    FXmlDriver XmlDriver;
    FData *Data;
};

#endif // FDRIVER_H
