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

#include "plugdriver.h"


void PlugDriver::SetMainWindow(FMainWindow *_MainWindow)
{
    MainWindow = _MainWindow;

    QDockWidget *DockWidgetDriver = new QDockWidget(tr("Driver"), MainWindow);
    DockWidgetDriver->setObjectName("DockDrivers");

    Driver = new FDriver(DockWidgetDriver);
    Driver->SetData(MainWindow->Data);

    DockWidgetDriver->setWidget(Driver);

    MainWindow->addDockWidget(Qt::LeftDockWidgetArea, DockWidgetDriver);

    connect(Driver, SIGNAL(ReturnFunctions(FDataPackage*)), MainWindow, SLOT(ExecPluginsFunctions(FDataPackage*)));

}

QStringList PlugDriver::KeyFunctions() const
{
    return QStringList();
}

void PlugDriver::ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage)
{
    Q_UNUSED(_KeyFunction)
    Q_UNUSED(_DataPackage)
}

bool PlugDriver::AcceptCloseApp()
{
    return Driver->CloseDriver();
}

//Q_EXPORT_PLUGIN2(plugdriver, PlugDriver)
