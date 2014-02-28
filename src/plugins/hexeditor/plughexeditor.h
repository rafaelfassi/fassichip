#ifndef PLUGHEXEDITOR_H
#define PLUGHEXEDITOR_H

#include <QObject>

#include "fplugininterface.h"
#include "fmainwindow.h"
#include "fhexeditor.h"
#include "version.h"

class PlugHexEditor : public QObject, public FPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(FPluginInterface)
    Q_PLUGIN_METADATA(IID "com.fassi.fassichip.hexeditor" FILE "hexeditor.json")

public:
    QString Name() { return "HexEditor"; }
    QString Author() { return "Rafael Fassi Lobão"; }
    QString AuthorContact() { return "rafael@fassi.com.br"; }
    QString Version() { return FassiChip_Version; }
    QString Description() { return tr("Hexadecimal Editor"); }
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

public slots:
    void on_ActNewHexEditor_triggered();

private:
    void NewHexEditor(FDataPackage *_DataPackage, QString _Label);
    FMainWindow *MainWindow;
    QString PluginFileName;

    QAction *ActNewHexEditor;

};

#endif // PLUGHEXEDITOR_H
