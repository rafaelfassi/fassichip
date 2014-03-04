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

#ifndef FDRIVNEWELEMDIALOG_H
#define FDRIVNEWELEMDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QRadioButton;
class QDialogButtonBox;

class FDrivNewElemDialog : public QDialog
{
    Q_OBJECT

public:
    FDrivNewElemDialog(const QMap<QString, QIcon> &_Options, QWidget *parent = 0);

    QString GetName();
    QString GetCheckedItemName();
    int GetCheckedItemId();

private:
    QLabel *LabName;
    QLineEdit *EdtName;
    QDialogButtonBox *ButtonBox;
    QList<QRadioButton *> LstRadBot;

};

#endif // FDRIVNEWELEMDIALOG_H
