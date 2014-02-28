#include "plugdriver.h"


void PlugDriver::SetMainWindow(FMainWindow *_MainWindow)
{
    MainWindow = _MainWindow;

    QDockWidget *DockWidgetDriver = new QDockWidget(tr("Driver"), MainWindow);
    DockWidgetDriver->setObjectName("DockDrivers");

    Driver = new FDriver(DockWidgetDriver);
    Driver->SetData(MainWindow->Data);

    DockWidgetDriver->setWidget(Driver);

    MainWindow->addDockWidget(Qt::LeftDockWidgetArea, DockWidgetDriver);

    connect(Driver, SIGNAL(ReturnFunctions(FDataPackage*)), MainWindow, SLOT(ExecPluginsFunctions(FDataPackage*)));

}

QStringList PlugDriver::KeyFunctions() const
{
    return QStringList();
}

void PlugDriver::ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage)
{
    Q_UNUSED(_KeyFunction)
    Q_UNUSED(_DataPackage)
}

bool PlugDriver::AcceptCloseApp()
{
    return Driver->CloseDriver();
}

//Q_EXPORT_PLUGIN2(plugdriver, PlugDriver)
