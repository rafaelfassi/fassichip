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

#include "fgraphiceditorcfg.h"

#include <QCheckBox>
#include <QLayout>
#include <QSettings>
#include <QApplication>

FGraphicEditorCfg::FGraphicEditorCfg(QString _Key, QWidget *parent)
    : QWidget(parent)
{
    Key = _Key;

    ChkViewXScale = new QCheckBox(tr("Show X scale"), this);
    ChkViewYScale = new QCheckBox(tr("Show Y scale"), this);

    QVBoxLayout *VLayout = new QVBoxLayout(this);
    VLayout->addWidget(ChkViewXScale);
    VLayout->addWidget(ChkViewYScale);
    setLayout(VLayout);


    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup(Key);
        ChkViewXScale->setChecked(_Settings.value("ViewXScale", true).toBool());
        ChkViewYScale->setChecked(_Settings.value("ViewYScale", false).toBool());
    _Settings.endGroup();

}

void FGraphicEditorCfg::WriteSettings()
{

    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup(Key);
        _Settings.setValue("ViewXScale", ChkViewXScale->isChecked());
        _Settings.setValue("ViewYScale", ChkViewYScale->isChecked());
    _Settings.endGroup();

}
