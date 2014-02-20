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
