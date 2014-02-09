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
