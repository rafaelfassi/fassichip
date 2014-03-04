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

#ifndef FINSTALLEDPLUGINSDIALOG_H
#define FINSTALLEDPLUGINSDIALOG_H

#include <QDialog>

class FPluginInterface;
class QTreeWidget;
class QTreeWidgetItem;

class FInstalledPluginsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FInstalledPluginsDialog(QList<FPluginInterface*> *_Plugins, QWidget *parent = 0);

signals:

private slots:
    void on_TreeWidgetPlugins_itemDoubleClicked(QTreeWidgetItem *_Item, int _Col);

private:
    void ShowAboutPlugin(FPluginInterface *_PluginInterface);
    bool CheckDependencies(FPluginInterface *_PluginInterface);
    QList<FPluginInterface*> *Plugins;
    QTreeWidget *TreeWidgetPlugins;

};

#endif // FINSTALLEDPLUGINSDIALOG_H
