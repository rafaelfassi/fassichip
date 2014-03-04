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

#ifndef FDRIVERELEMENT_H
#define FDRIVERELEMENT_H

#include <QObject>

#include "fxmldriver.h"

class FDriverElement : public QObject
{
    Q_OBJECT

public:
    explicit FDriverElement(QString _Name, QObject *parent = 0);

    void SetDocument(QDomDocument *_Doc) { DriverDocument = _Doc; ReadDocument(); }
    QString GetName() { return Name; }
    QString GetGroup();
    QStringList GetGroups();
    bool AddNewGroup(QString _Group);
    virtual void Edit()=0;
    virtual void Execute() {}

signals:
    void EditingFinished();

public slots:

protected:
    virtual void ReadDocument()=0;
    virtual void WriteDocument()=0;
    QDomDocument *DriverDocument;
    FXmlDriver XmlDriver;

private:
    QString Name;

};

#endif // FDRIVERELEMENT_H
