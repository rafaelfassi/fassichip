#ifndef PLUGDRIVER_H
#define PLUGDRIVER_H

#include "fplugininterface.h"
#include "fmainwindow.h"
#include "fdriver.h"
#include "version.h"

class PlugDriver : public QObject, public FPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(FPluginInterface)

public:
    QString Name() { return tr("Driver"); }
    QString Author() { return "Rafael Fassi Lobão"; }
    QString AuthorContact() { return "rafael@fassi.com.br"; }
    QString Version() { return FassiChip_Version; }
    QString Description() { return tr("Adds the feature of drivers at FassiChip."); }
    QString License() { return tr("GPL"); }
    QStringList Dependencies() { return QStringList() << "GraphicEditor" << "Viewer3d"; }
    QWidget *SettingsWidget() { return 0; }
    void SetMainWindow(FMainWindow *_MainWindow);
    void SetPlugins(QList<FPluginInterface*> *_Plugins) { Q_UNUSED(_Plugins) }
    void SetFileName(QString _FileName) { PluginFileName = _FileName; }
    QString FileName() { return PluginFileName; }
    void OpenedFile(int _Type) { Q_UNUSED(_Type) }
    bool AcceptCloseFile(int _Type) { Q_UNUSED(_Type) return true; }
    bool AcceptCloseApp();
    bool AcceptSaveFile() { return true; }
    QStringList KeyFunctions() const;
    void ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage = 0);

private:
    FMainWindow *MainWindow;
    QString PluginFileName;

    FDriver *Driver;

};

#endif // PLUGDRIVER_H
