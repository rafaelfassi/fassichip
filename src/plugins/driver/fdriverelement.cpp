#include "fdriverelement.h"

FDriverElement::FDriverElement(QString _Name, QObject *parent) :
    QObject(parent)
{
    Name = _Name;
}

QStringList FDriverElement::GetGroups()
{
    return XmlDriver.GetGroups(DriverDocument);
}

bool FDriverElement::AddNewGroup(QString _Group)
{
    return XmlDriver.AddGroup(DriverDocument, _Group);
}

QString FDriverElement::GetGroup()
{
    return XmlDriver.GetGroupByElementName(DriverDocument, GetName());
}
