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

#ifndef FMAINWINDOW_H
#define FMAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class FDataPackage;
class FWindowTab;
class FData;
class FPluginInterface;

class FMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FMainWindow(QWidget *parent = 0);

    void AddNewTab(FWindowTab *_WindowTab);
    FData *Data;
    QMenu *MenuFile;
    QMenu *MenuEdit;
    QMenu *MenuTools;
    QMenu *MenuHelp;
    QToolBar *ToolBarFile;
    QToolBar *ToolBarEdit;


signals:
    void UpdatePluginsSettings();

public slots:
    void RefreshDataTabs();
    void UpdateDataTabs();
    void UpdateSettingsTabs();
    void ExecPluginsFunctions(FDataPackage *_DataPackage);


protected:
    QList<FPluginInterface*> Plugins;
    QTabWidget *TabWidgetMain;

};

#endif // FMAINWINDOW_H
