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

#include "fxmldriver.h"


bool FXmlDriver::AddGroup(QDomDocument *_Doc, QString _GroupName)
{

    QDomElement _Groups = _Doc->elementsByTagName("Groups").item(0).toElement();

    if(_Groups.isNull()) // Se ainda não existir a tag <Groups> no documento.
    {
        _Groups = _Doc->createElement("Groups");

        QDomElement root = _Doc->firstChildElement("Driver");
        root.appendChild(_Groups);
    }

    if(!FindItemText(_Groups, "Item", _GroupName).isNull()) return false; // Ja existe um grupo com esse nome.

    QDomElement _Item;
    QDomText _DomGroupName;

        _DomGroupName = _Doc->createTextNode(_GroupName);
        _Item = _Doc->createElement("Item");
        _Item.appendChild(_DomGroupName);
        _Groups.appendChild(_Item);

        return true;

}

QStringList FXmlDriver::GetGroups(QDomDocument *_Doc)
{
    QDomNode _DocNode = _Doc->elementsByTagName("Groups").item(0);
    QDomElement _Item = _DocNode.firstChildElement("Item");

    QStringList _GroupsNames;

       while (!_Item.isNull())
       {
           _GroupsNames.append(_Item.text());

           _Item = _Item.nextSiblingElement("Item");
       }

       return _GroupsNames;
}

bool FXmlDriver::MoveGroup(QDomDocument *_Doc, QString _GroupName, bool _Up)
{
    QStringList _GroupsName = GetGroups(_Doc);
    if(_GroupsName.isEmpty()) return false;

    QDomElement _Groups = _Doc->elementsByTagName("Groups").item(0).toElement();

    QDomElement _Item = FindItemText(_Groups, "Item", _GroupName);

    int _Index = _GroupsName.indexOf(_GroupName);

    if(_Up)
    {
        if(_GroupsName.first() == _GroupName) return false; // Ja é o primeiro grupo

        QDomElement _ItemBefore = FindItemText(_Groups, "Item", _GroupsName.value(_Index - 1));

        _Groups.insertBefore(_Item, _ItemBefore);

    }
    else
    {
        if(_GroupsName.last() == _GroupName) return false; // Ja é o último grupo

        QDomElement _ItemAfter = FindItemText(_Groups, "Item", _GroupsName.value(_Index + 1));

        _Groups.insertAfter(_Item, _ItemAfter);
    }

    return true;
}

bool FXmlDriver::RenameGroup(QDomDocument *_Doc, QString _GroupName, QString _NewGroupName)
{
    QDomElement _Groups = _Doc->elementsByTagName("Groups").item(0).toElement();

    if(!FindItemText(_Groups, "Item", _NewGroupName).isNull()) return false; // Ja existe um grupo com esse nome.

    ReplaceElementsGroup(_Doc, _GroupName, _NewGroupName);

    QDomElement _ItemOld = FindItemText(_Groups, "Item", _GroupName);
    QDomElement _ItemNew = _Doc->createElement("Item");
    QDomText NewGroupText = _Doc->createTextNode(_NewGroupName);

    _ItemNew.appendChild(NewGroupText);
    _Groups.replaceChild(_ItemNew, _ItemOld);

    return true;
}

bool FXmlDriver::DeleteGroup(QDomDocument *_Doc, QString _GroupName)
{
   QDomElement _Groups = _Doc->elementsByTagName("Groups").item(0).toElement();

   QDomElement _Item = FindItemText(_Groups, "Item", _GroupName);
   if(_Item.isNull()) return false;

   ReplaceElementsGroup(_Doc, _GroupName, "");

   _Groups.removeChild(_Item);

   return true;
}

void FXmlDriver::ReplaceElementsGroup(QDomDocument *_Doc, QString _GroupName, QString _NewGroupName)
{
    QStringList _ElementsName = GetElementsNameByGroup(_Doc, _GroupName);
    QDomElement _Element;

    foreach(QString _ElementName, _ElementsName)
    {
        _Element = FindElementByAttribute(_Doc, "Name", _ElementName);
        _Element.setAttribute("Group", _NewGroupName);
    }
}

bool FXmlDriver::AddNewElement(QDomDocument *_Doc, QString _TypeName, QString _Name, QString _Group)
{

    if(!FindElementByAttribute(_Doc, "Name", _Name).isNull()) return false;

        QDomElement _Type = _Doc->createElement(_TypeName);
        _Type.setAttribute("Name", _Name);
        _Type.setAttribute("Group", _Group);

        QDomElement root = _Doc->firstChildElement("Driver");
        root.appendChild(_Type);

        return true;

}

bool FXmlDriver::RenameElement(QDomDocument *_Doc, QString _ElementName, QString _NewElementName)
{
    if(!FindElementByAttribute(_Doc, "Name", _NewElementName).isNull()) return false;

    QDomElement _Element = FindElementByAttribute(_Doc, "Name", _ElementName);
    if(_Element.isNull()) return false;

    _Element.setAttribute("Name", _NewElementName);
    return true;
}

bool FXmlDriver::DeleteElement(QDomDocument *_Doc, QString _ElementName)
{
    QDomElement _Element = FindElementByAttribute(_Doc, "Name", _ElementName);
    if(_Element.isNull()) return false;

    if(_Doc->documentElement().removeChild(_Element).isNull()) return false;

    return true;
}

bool FXmlDriver::SetElementGroup(QDomDocument *_Doc, QString _ElementName, QString _GroupName)
{
    QDomElement _Element;

   _Element = FindElementByAttribute(_Doc, "Name", _ElementName);
   if(_Element.isNull()) return false;

   QDomElement _Groups = _Doc->elementsByTagName("Groups").item(0).toElement();

   if(FindItemText(_Groups, "Item", _GroupName).isNull() && (_GroupName != "")) // Se o grupo não existir, será criado.
   {
       if(!AddGroup(_Doc, _GroupName)) return false;
   }

   _Element.setAttribute("Group", _GroupName);

   return true;
}

bool FXmlDriver::AddElementItem(QDomDocument *_Doc, QString _ElementName, QString _ItemName, QString _ItemValue)
{
     QDomElement _Element = FindElementByAttribute(_Doc, "Name",  _ElementName);

     if(_Element.isNull()) return false;

     QDomElement _Item;
     QDomText _DomItemName;

         _DomItemName = _Doc->createTextNode(_ItemValue);
         _Item = _Doc->createElement(_ItemName);
         _Item.appendChild(_DomItemName);
         _Element.appendChild(_Item);

         return true;
}

bool FXmlDriver::ClearElement(QDomDocument *_Doc, QString _ElementName)
{
    QDomElement _Element = FindElementByAttribute(_Doc, "Name",  _ElementName);
    if(_Element.isNull()) return false;

    QDomNode _Item = _Element.firstChild();

    while(!_Item.isNull())
    {
        _Element.removeChild(_Item);
        _Item = _Element.firstChild();
    }

    return true;


    /*QDomElement _Groups = _Doc->elementsByTagName("Groups").item(0).toElement();

     QDomElement _Item = FindItemText(_Groups, "Item", _GroupName);
     if(_Item.isNull()) return false;

     _Groups.removeChild(_Item);

     return true;*/
}

 QStringList FXmlDriver::GetItemsTextFromElementName(QDomDocument *_Doc, QString _ElementName, QString _ItemName)
 {
     QDomElement _DocNode = FindElementByAttribute(_Doc, "Name", _ElementName);// _Doc->elementsByTagName(_ElementName).item(0);
     QDomElement _Item = _DocNode.firstChildElement(_ItemName);

     QStringList _ItemsText;

        while (!_Item.isNull())
        {
            _ItemsText.append(_Item.text());
            _Item = _Item.nextSiblingElement(_ItemName);
        }

        return _ItemsText;
 }

QDomElement FXmlDriver::FindElementByAttribute(QDomDocument *_Doc, QString _Attribute, QString _Value)
{
    QDomElement _DocElement = _Doc->documentElement();
    QDomElement _Element;

    for(QDomNode n = _DocElement.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        _Element = n.toElement();
        if(_Element.attribute(_Attribute, "null") == _Value) return _Element;
    }

    _Element.clear();
    return _Element;
}

QDomElement FXmlDriver::FindItemText(QDomElement _Element, QString _ItemName, QString _Text)
{
    _Element = _Element.firstChildElement(_ItemName);

    while (!_Element.isNull())
    {
        if(_Element.text() == _Text) return _Element;

        _Element = _Element.nextSiblingElement(_ItemName);
    }

    _Element.clear();
    return _Element;
}

QStringList FXmlDriver::GetElementsNameByGroup(QDomDocument *_Doc, QString _Group)
{
    QDomElement _DocElement = _Doc->documentElement();
    QStringList _NameLst;

    for(QDomNode n = _DocElement.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        if(n.toElement().attribute("Group", "null") == _Group)
            _NameLst.append(n.toElement().attribute("Name", "null"));
    }

    return _NameLst;
}

QString FXmlDriver::GetGroupByElementName(QDomDocument *_Doc, QString _ElementName)
{
    QDomElement _Element = FindElementByAttribute(_Doc, "Name", _ElementName);
    return _Element.attribute("Group", "null");
    return "";
}

QString FXmlDriver::GetTagByElementName(QDomDocument *_Doc, QString _ElementName)
{
    return FindElementByAttribute(_Doc, "Name", _ElementName).tagName();
}

bool FXmlDriver::MoveElement(QDomDocument *_Doc, QString _ElementName, bool _Up)
{
    QDomElement _Element = FindElementByAttribute(_Doc, "Name", _ElementName);
    QString _Group = _Element.attribute("Group", "null");
    QStringList _ElementsName = GetElementsNameByGroup(_Doc, _Group);

    if(_ElementsName.isEmpty()) return false;

    int _Index = _ElementsName.indexOf(_ElementName);

    if(_Up)
    {
        if(_ElementsName.first() == _ElementName) return false; // O elemento ja é o prieiro do grupo.

        QDomElement _ElementBefore = FindElementByAttribute(_Doc, "Name", _ElementsName.value(_Index - 1));

         _Doc->documentElement().insertBefore(_Element, _ElementBefore);
    }
    else
    {
        if(_ElementsName.last() == _ElementName) return false; // O elemento ja é o último do grupo.

        QDomElement _ElementAfter = FindElementByAttribute(_Doc, "Name", _ElementsName.value(_Index + 1));

        _Doc->documentElement().insertAfter(_Element, _ElementAfter);
    }

    return true;

}
