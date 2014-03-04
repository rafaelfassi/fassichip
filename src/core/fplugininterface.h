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

#ifndef FPLUGININTERFACE_H
#define FPLUGININTERFACE_H

#include <QtPlugin>
#include <QList>

class QWidget;
class QString;
class QStringList;
class FMainWindow;
class FDataPackage;

class FPluginInterface
{
public:
    virtual ~FPluginInterface() {}

    virtual QString Name()=0;
    virtual QString Author()=0;
    virtual QString AuthorContact()=0;
    virtual QString Version()=0;
    virtual QString Description()=0;
    virtual QString License()=0;
    virtual QStringList Dependencies()=0;
    virtual QString FileName()=0;
    virtual void SetFileName(QString _FileName)=0;
    virtual QWidget *SettingsWidget()=0;
    virtual void SetMainWindow(FMainWindow *_MainWindow)=0;
    virtual void SetPlugins(QList<FPluginInterface*> *_Plugins)=0;
    virtual void OpenedFile(int _Type)=0;
    virtual bool AcceptCloseFile(int _Type)=0;
    virtual bool AcceptCloseApp()=0;
    virtual bool AcceptSaveFile()=0;
    virtual QStringList KeyFunctions() const = 0;
    virtual void ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage) = 0;

};

Q_DECLARE_INTERFACE(FPluginInterface, "com.Fassi.FassiChip.FPluginInterface/1.0")

#endif // FPLUGININTERFACE_H
