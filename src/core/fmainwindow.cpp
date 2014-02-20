#include "fmainwindow.h"
#include "fwindowtab.h"
#include "fdatapackage.h"
#include "fplugininterface.h"

#include <QMessageBox>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>


FMainWindow::FMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    Data = new FData;

    MenuFile = menuBar()->addMenu(tr("&File"));
    MenuEdit = menuBar()->addMenu(tr("&Edit"));
    MenuTools = menuBar()->addMenu(tr("&Tools"));
    MenuHelp = menuBar()->addMenu(tr("&Help"));

    ToolBarFile = addToolBar(tr("&File"));
        ToolBarFile->setObjectName("fileToolBar");
    ToolBarEdit = addToolBar(tr("&Edit"));
        ToolBarEdit->setObjectName("editToolBar");

    TabWidgetMain = new QTabWidget(this);
    TabWidgetMain->setTabsClosable(true);
    TabWidgetMain->setMovable(true);

    setCentralWidget(TabWidgetMain);

}

void FMainWindow::AddNewTab(FWindowTab *_WindowTab)
{
    int IdTab = TabWidgetMain->addTab(_WindowTab, _WindowTab->GetTabLabel());
    TabWidgetMain->setCurrentIndex(IdTab);
}


void FMainWindow::RefreshDataTabs()
{
    for(int id=0; id<TabWidgetMain->count(); id++)
    {
        //FWindowTab *WindowTab = qobject_cast<FWindowTab *>(TabWidgetMain->widget(id));
        FWindowTab *WindowTab = static_cast<FWindowTab *>(TabWidgetMain->widget(id));
        WindowTab->RefreshData();
    }
}

void FMainWindow::UpdateDataTabs()
{
    for(int id=0; id<TabWidgetMain->count(); id++)
    {
        //FWindowTab *WindowTab = qobject_cast<FWindowTab *>(TabWidgetMain->widget(id));
        FWindowTab *WindowTab = static_cast<FWindowTab *>(TabWidgetMain->widget(id));
        WindowTab->UpdateData();
        WindowTab->RefreshData();
    }
}

void FMainWindow::UpdateSettingsTabs()
{
    for(int id=0; id<TabWidgetMain->count(); id++)
    {
        FWindowTab *WindowTab = static_cast<FWindowTab *>(TabWidgetMain->widget(id));
        WindowTab->UpdateSettings();
    }
}

void FMainWindow::ExecPluginsFunctions(FDataPackage *_DataPackage)
{
    QString _Command = _DataPackage->Command;
    QString _PluginName;
    QString _Function;

    // Separa o comando em nome do plugin e função enviada a ele----------------------------
    int _Index = _Command.indexOf(QChar('.'));

        if( (_Index > 0) && (_Index < _Command.size()-1))
        {
            _PluginName = _Command.mid(0, _Index);
            _Function = _Command.mid(_Index+1);
        }
        else
        {
            QMessageBox::warning(0, tr("Error"),
                                 tr("%0 is not a valid command!\n"
                                    "The name of the plugin and the command must be separated by \".\"")
                                 .arg(_Command));
            return;
        }

        // Executa o plugin apropriado passando a função e os dados a ele---------------------
        foreach(FPluginInterface* _Plugin, Plugins)
        {
            if(_Plugin->Name() == _PluginName)
            {
                _Plugin->ExecFunction(_Function, _DataPackage);
            }
        }


}
