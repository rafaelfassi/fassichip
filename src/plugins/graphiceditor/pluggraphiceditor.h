/****************************************************************************
**
**Copyright (C) 2010-2014 - Rafael Fassi Lobão
**Contact: www.fassi.com.br - rafael@fassi.com.br
**
**This file is part of the Fassichip.
**Fassichip is free software; you can redistribute it and/or modify
**it under the terms of the GNU General Public License as published by
**the Free Software Foundation; either version 2 of the License, or
**(at your option) any later version.
**
**This program is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**GNU General Public License for more details.
**
****************************************************************************/

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
    Q_PLUGIN_METADATA(IID "com.fassi.fassichip.graphiceditor" FILE "graphiceditor.json")

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
