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

#ifndef FCONFIGDIALOG_H
#define FCONFIGDIALOG_H

#include <QDialog>

class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QRadioButton;
class QStackedLayout;
class FPluginInterface;

class FConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FConfigDialog(QList<FPluginInterface*> *_Plugins, QWidget *parent = 0);

    QComboBox *CmbBoxStyle;
    QComboBox *CmbBoxLanguages;

private slots:
    void on_CmbBoxLanguages_CurrentIndexChanged(QString);

private:
    void CreateGeralPage();
    QStackedLayout *StackedLayout;
    QListWidget *ListWidget;
    QDialogButtonBox *ButtonBox;
    QWidget *WdtGeneralPage;

};

#endif // FCONFIGDIALOG_H
