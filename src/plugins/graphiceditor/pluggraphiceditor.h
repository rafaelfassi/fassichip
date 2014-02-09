#ifndef PLUGGRAPHICEDITOR_H
#define PLUGGRAPHICEDITOR_H

#include <QObject>

#include "fplugininterface.h"
#include "fmainwindow.h"
#include "fgraphiceditor.h"
#include "fgraphiceditorcfg.h"
#include "version.h"

class PlugGraphicEditor : public QObject, public FPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(FPluginInterface)

public:
    QString Name() { return "GraphicEditor"; }
    QString Author() { return "Rafael Fassi Lobão"; }
    QString AuthorContact() { return "rafael@fassi.com.br"; }
    QString Version() { return FassiChip_Version; }
    QString Description() { return tr("Graphic Editor"); }
    QString License() { return tr("GPL"); }
    QStringList Dependencies() { return QStringList(); }
    QWidget *SettingsWidget();
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
    void on_ActNewGraphicEditor_triggered();
    void ReadSettingsWidget();

private:
    void NewGraphicEditor(FDataPackage *_DataPackage, QString _Label);
    FMainWindow *MainWindow;
    QString PluginFileName;
    FGraphicEditorCfg *Config;

    QAction *ActNewGraphicEditor;

};

#endif // PLUGGRAPHICEDITOR_H
