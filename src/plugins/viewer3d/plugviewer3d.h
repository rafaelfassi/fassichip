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
    Q_PLUGIN_METADATA(IID "com.fassi.fassichip.viewer3d" FILE "viewer3d.json")

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
