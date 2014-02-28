#include "plugviewer3d.h"


void PlugViewer3d::SetMainWindow(FMainWindow *_MainWindow)
{
    MainWindow = _MainWindow;
}

QStringList PlugViewer3d::KeyFunctions() const
{
    return QStringList() << "Show";
}

void PlugViewer3d::ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage)
{
    QString _Function;
    QString _Label;

    int _Index = _KeyFunction.indexOf(QChar(':'));

    if(_Index > 0)
    {
        _Function = _KeyFunction.mid(0, _Index);
        _Label = _KeyFunction.mid(_Index+1);
    }
    else
    {
        _Function = _KeyFunction;
        _Label = "";
    }


    if(_Function == "Show") NewViewer3d(_DataPackage, _Label);
}

void PlugViewer3d::NewViewer3d(FDataPackage *_DataPackage, QString _Label)
{
    if(!_DataPackage) _DataPackage = new FDataPackage(MainWindow->Data);

    FViewer3d *Viewer3d = new FViewer3d(MainWindow);

    if(_Label == "") _Label = tr("3D Viewer");

    Viewer3d->SetData(_DataPackage);
    Viewer3d->SetTabLabel(_Label);
    MainWindow->AddNewTab(Viewer3d);
    MainWindow->RefreshDataTabs();
}

//Q_EXPORT_PLUGIN2(plugviewer3d, PlugViewer3d)
