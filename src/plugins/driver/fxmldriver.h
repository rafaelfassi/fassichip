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

#ifndef FXMLDRIVER_H
#define FXMLDRIVER_H

#include <QDomDocument>
#include <QtGui>

class FXmlDriver
{
public:
    bool AddGroup(QDomDocument *_Doc, QString _GroupName);
    QStringList GetGroups(QDomDocument *_Doc);
    bool MoveGroup(QDomDocument *_Doc, QString _GroupName, bool _Up);
    bool RenameGroup(QDomDocument *_Doc, QString _GroupName, QString _NewGroupName);
    bool DeleteGroup(QDomDocument *_Doc, QString _GroupName);
    void ReplaceElementsGroup(QDomDocument *_Doc, QString _GroupName, QString _NewGroupName);
    bool AddNewElement(QDomDocument *_Doc, QString _TypeName, QString _Name, QString _Group);
    bool RenameElement(QDomDocument *_Doc, QString _ElementName, QString _NewElementName);
    bool DeleteElement(QDomDocument *_Doc, QString _ElementName);
    bool SetElementGroup(QDomDocument *_Doc, QString _ElementName, QString _GroupName);
    bool AddElementItem(QDomDocument *_Doc, QString _ElementName, QString _ItemName, QString _ItemValue);
    bool ClearElement(QDomDocument *_Doc, QString _ElementName);
    QStringList GetItemsTextFromElementName(QDomDocument *_Doc, QString _ElementName, QString _ItemName);
    QDomElement FindElementByAttribute(QDomDocument *_Doc, QString _Attribute, QString _Value);
    QStringList GetElementsNameByGroup(QDomDocument *_Doc, QString _Group);
    QString GetGroupByElementName(QDomDocument *_Doc, QString _ElementName);
    QString GetTagByElementName(QDomDocument *_Doc, QString _ElementName);
    bool MoveElement(QDomDocument *_Doc, QString _ElementName, bool _Up);

private:
    QDomElement FindItemText(QDomElement _Element, QString _ItemName, QString _Text);



};

#endif // FXMLDRIVER_H
