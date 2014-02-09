#ifndef PLUGVIEWER3D_H
#define PLUGVIEWER3D_H

#include <QObject>

#include "fplugininterface.h"
#include "fmainwindow.h"
#include "fviewer3d.h"
#include "version.h"

class PlugViewer3d : public QObject, public FPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(FPluginInterface)

public:
    QString Name() { return "Viewer3d"; }
    QString Author() { return "Rafael Fassi Lobão"; }
    QString AuthorContact() { return "rafael@fassi.com.br"; }
    QString Version() { return FassiChip_Version; }
    QString Description() { return tr("3D viewer"); }
    QString License() { return tr("GPL"); }
    QStringList Dependencies() { return QStringList(); }
    QWidget *SettingsWidget() { return 0; }
    void SetMainWindow(FMainWindow *_MainWindow);
    void SetPlugins(QList<FPluginInterface*> *_Plugins) { Q_UNUSED(_Plugins); }
    void SetFileName(QString _FileName) { PluginFileName = _FileName; }
    QString FileName() { return PluginFileName; }
    void OpenedFile(int _Type) { Q_UNUSED(_Type) }
    bool AcceptCloseFile(int _Type) { Q_UNUSED(_Type) return true; }
    bool AcceptCloseApp() { return true; }
    bool AcceptSaveFile() { return true; }
    QStringList KeyFunctions() const;
    void ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage = 0);

private:
    void NewViewer3d(FDataPackage *_DataPackage, QString _Label);
    FMainWindow *MainWindow;
    QString PluginFileName;
};

#endif // PLUGVIEWER3D_H
