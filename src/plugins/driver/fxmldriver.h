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
