#include "mainwindow.h"
#include "fwindowtab.h"
#include "fdatapackage.h"
#include "fplugininterface.h"
#include "fconfigdialog.h"
#include "finstalledpluginsdialog.h"
#include "fabout.h"
#include "QRecentFilesMenu.h"

#include <QPluginLoader>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QSettings>
#include <QCloseEvent>
#include <QUrl>
#include <QSplashScreen>
#include <QComboBox>
#include <QTimer>


MainWindow::MainWindow(QSplashScreen *SplashScreen, QWidget *parent) :
    FMainWindow(parent)
{
    SplashScreen->showMessage(tr("Creating environment..."));
    qApp->processEvents();
    //sleep(1);

    setWindowIcon(QIcon(":/images/Chip1.png"));
    setWindowTitle("FassiChip");


    NameTypeForIds << tr("Edition") << tr("Original") << tr("Comparison 1")
                   << tr("Comparison 2") << tr("Comparison 3");

    setAcceptDrops(true);

    CreateActions();
    CreateMenus();
    CreateToolBars();

    connect(TabWidgetMain, SIGNAL(tabCloseRequested(int)),
            this, SLOT(on_TabWidgetMain_CloseRequested(int)));

    QTimer *_Timer = new QTimer(this);
    connect(_Timer, SIGNAL(timeout()), this, SLOT(PeriodicalUpdate()));
    _Timer->start(500);

    SplashScreen->showMessage(tr("Loading plugins..."));
    qApp->processEvents();
    //sleep(1);

    LoadPlugins();

    ReadGeneralConfig();
    ReadSettings();
}

bool MainWindow::OpenFile(QString _FileName, int _Type)
{

    static const QColor colorForIds[6] = {
            Qt::red, Qt::blue, Qt::green, Qt::cyan, Qt::magenta, Qt::yellow
    };

    if(Data->ExistFileType(_Type))
    {
        if(!CloseFile(_Type)) return false;
    }

    QFile _File(_FileName);

    if (_File.open(QFile::ReadOnly))
    {
       Data->NewDataFile(_Type, _File.size());
       _File.read(Data->AddressBuffer(Data->GetId(_Type)), _File.size());
       _File.close();

       QString _ShortFileName = QFileInfo(_FileName).fileName();
       Data->SetFileName(Data->GetId(_Type), _FileName);
       Data->SetColorFile(Data->GetId(_Type), colorForIds[_Type]);
       ActGrOpenedFiles->actions()[_Type]->setText(_ShortFileName);
       ActGrOpenedFiles->actions()[_Type]->setVisible(true);
       ActGrTypeFiles->actions()[_Type]->setText(NameTypeForIds[_Type] + " - ( " + _ShortFileName + " )");
       ActNewFile->setEnabled(true);
       ActCloseFile->setEnabled(true);
       MenuRecentFiles->addRecentFile(_FileName);

       foreach(FPluginInterface* _Plugin, Plugins) _Plugin->OpenedFile(_Type);
       return true;
    }
    else
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Cannot open file: \"%0\"\n%1")
                              .arg(_FileName)
                              .arg(_File.errorString()));
        return false;
    }

}

bool MainWindow::OpenFile(QString _FileName)
{
    int _Type = PopupGetFileType(false);

    if(_Type < 0) return false;

    if(_Type == EDIT)
    {
        if(!TrySaveFile()) return false;
    }

    if(OpenFile(_FileName, _Type)) UpdateDataTabs();

    return true;
}

bool MainWindow::CloseFile(int _Type)
{
    if(!Data->ExistFileType(_Type)) return true;

    foreach(FPluginInterface* _Plugin, Plugins)
    {
        if(!_Plugin->AcceptCloseFile(_Type)) return false;
    }

    Data->DeleteDataFile(_Type);
    ActGrOpenedFiles->actions()[_Type]->setVisible(false);
    ActGrTypeFiles->actions()[_Type]->setText(NameTypeForIds[_Type]);
    ActNewFile->setEnabled(!Data->IsEmply());
    ActCloseFile->setEnabled(ActNewFile->isEnabled());
    return true;
}

bool MainWindow::SaveFileAs(QString _FileName, int _Type)
{
    int _Id = Data->GetId(_Type);
    QFile _File(_FileName);

    if(_File.open(QFile::WriteOnly))
    {
       _File.write(Data->AddressBuffer(_Id), Data->GetSizeFile(_Id));
       _File.close();
       return true;
    }
    else
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Cannot save file: \"%0\"\n%1")
                              .arg(_FileName)
                              .arg(_File.errorString()));
        return false;
    }
}

bool MainWindow::SaveFile()
{
    if(!Data->ExistFileType(EDIT)) return true;

    QString _FileName = Data->GetFileName(Data->GetId(EDIT));

    if(SaveFileAs(_FileName, 0))
    {
        Data->UndoClear();
        return true;
    }

    return false;
}

bool MainWindow::TrySaveFile()
{
    if(!Data->UndoIsEmply())
    {
        int _MsgRet = QMessageBox::question(this, tr("Save File"),
                                            tr("Save changes to \"%0\"?")
                                            .arg(QFileInfo(Data->GetFileName(Data->GetId(0))).fileName()),
                                            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                            QMessageBox::Save);

        switch (_MsgRet)
        {
           case QMessageBox::Save:
               return SaveFile();
           case QMessageBox::Cancel:
               return false;
         }
    }
    Data->UndoClear();
    return true;
}

void MainWindow::PeriodicalUpdate()
{
    ActUndo->setEnabled(!Data->UndoIsEmply());
    ActRedo->setEnabled(!Data->RedoIsEmply());

    ActSave->setEnabled(ActUndo->isEnabled());
}

void MainWindow::on_ActOpenFile_triggered()
{
    int _Type = PopupGetFileType(false);

    if(_Type < 0) return;

    if(_Type == EDIT)
    {
        if(!TrySaveFile()) return;
    }

    QString _FileName = QFileDialog::getOpenFileName(this);
    if(_FileName.isEmpty()) return;

    if(OpenFile(_FileName, _Type)) UpdateDataTabs();
}

void MainWindow::on_ActNewFile_triggered()
{
    int _Type = PopupGetFileType(true);

    if(_Type < 0) return;

    QString _FileName = QFileDialog::getSaveFileName(this);
    if(_FileName.isEmpty()) return;

    if(SaveFileAs(_FileName, _Type))
    {
        if(OpenFile(_FileName, 0)) UpdateDataTabs();
    }
}

void MainWindow::on_ActCloseFile_triggered()
{
    int _Type = PopupGetFileType(true);

    if(_Type < 0) return;

    if(_Type == EDIT)
    {
        if(!TrySaveFile()) return;
    }

    if(CloseFile(_Type)) UpdateDataTabs();
}

void MainWindow::on_ActCloseAllFiles_triggered()
{
    if(!TrySaveFile()) return;

    for(int i = 0; i < 5; i++)
    {
        if(!CloseFile(i)) return;
    }  
    UpdateDataTabs();
}

void MainWindow::on_TabWidgetMain_CloseRequested(int _Id)
{
    FWindowTab* _WindowTab = static_cast<FWindowTab *>(TabWidgetMain->widget(_Id));
    FDataPackage *_DataPackage = _WindowTab->GetData();

    TabWidgetMain->removeTab(_Id);
    delete _WindowTab;

    // O objeto não possuí dados.
    if(!_DataPackage) return;

    // Os dados não podem ser deletados, pois devem estar sendo usados por outro módulo.
    if(_DataPackage->IsPersistent()) return;

    // Faz a limpeza.
    // Verifica se alguma outra visualização está usando o DataPackage da tab fechada.
    // Se nenhuma outra estiver usando, os dados são deletados.
    bool _DataUsed = false;

    for(int id=0; id<TabWidgetMain->count(); id++)
    {
        //_WindowTab = (FWindowTab*)TabWidgetMain->widget(id);
        //_WindowTab = qobject_cast<FWindowTab *>(TabWidgetMain->widget(id));
        //_WindowTab = dynamic_cast<FWindowTab *>(TabWidgetMain->widget(id));
        _WindowTab = static_cast<FWindowTab *>(TabWidgetMain->widget(id));

        if(_WindowTab->GetData() == _DataPackage) { _DataUsed = true; break; }
    }
    if(!_DataUsed) delete _DataPackage;
}

void MainWindow::on_ActOptions_triggered()
{
    FConfigDialog *ConfigDialog = new FConfigDialog(&Plugins, this);

    if(ConfigDialog->exec())
    {
        QString _Lang = ""; // System
        if(ConfigDialog->CmbBoxLanguages->currentIndex() > 0) _Lang = ConfigDialog->CmbBoxLanguages->currentText();

        QSettings settings("Fassi", "FassiChip");
        settings.beginGroup("General");
        settings.setValue("Language", _Lang);
        settings.setValue("Style", ConfigDialog->CmbBoxStyle->currentText());
        settings.endGroup();

        ReadGeneralConfig();
        emit UpdatePluginsSettings();
        UpdateSettingsTabs();
        UpdateDataTabs();
    }
    delete ConfigDialog;
}

void MainWindow::on_ActInstalledPlugins_triggered()
{
    FInstalledPluginsDialog *InstalledPluginsDialog = new FInstalledPluginsDialog(&Plugins, this);
    InstalledPluginsDialog->exec();
    delete InstalledPluginsDialog;
}

void MainWindow::on_ActUndoRedo_triggered()
{
    QAction *_ActClicked = qobject_cast<QAction *>(sender());
    bool _Undo = _ActClicked->property("Undo").toBool();

    if(_Undo) Data->Undo();
    else Data->Redo();

    ActUndo->setEnabled(!Data->UndoIsEmply());
    ActRedo->setEnabled(!Data->RedoIsEmply());

    UpdateDataTabs();
}

void MainWindow::on_ActAbout_triggered()
{
    FAbout About(this);
    About.exec();
}

void MainWindow::on_ActTeste_triggered()
{
    /*static int _Pos;
    Data->SetValue(_Pos, _Pos*10, FDataMode(BITS8, false, false));
    _Pos++;*/

    static double x = 0;

    Data->AddUndoPoint();

    FDataMode _DataMode(BITS8, false, false);

    int _Inc = Data->GetPosByte(1, _DataMode);
    double _Max = Data->GetMaxY(_DataMode);
    int _Size = Data->GetSizeFile(Data->GetId(0));


    int p = 0;

    for(; (p*p < _Max) && (x+p < _Size); p+=_Inc)
    {
        Data->SetValue(x+p, p*p, _DataMode, ADD_UNDO);
    }

    x+=p;

    UpdateDataTabs();
}

int MainWindow::PopupGetFileType(bool _Opened)
{
    QMenu _Menu;
    //QAction *at = ActGrTypeFiles->actions()[0]; // Assumes actions is not empty

    if(_Opened)
    {
        foreach (QAction *a, ActGrOpenedFiles->actions()) _Menu.addAction(a);
        if(_Menu.isEmpty()) return -1; // Não existe elemento visível na popup (nenhum arquivo aberto)
    }
    else
    {
        foreach (QAction *a, ActGrTypeFiles->actions()) _Menu.addAction(a);
        _Menu.setActiveAction(ActGrTypeFiles->actions()[0]); // Seleciona o primeiro elemento da popup
    }

    QAction *_act = _Menu.exec(cursor().pos());

    if(_act) return _act->property("FileType").toInt();
    else return -1;
}

void MainWindow::LoadPlugins()
{

#if defined(Q_OS_LINUX)
    QString _Filter = "*.so";
#else
    QString _Filter = "*.dll";
#endif

    QDir PluginPath(QApplication::applicationDirPath() + "/plugins");

    QStringList _PluginFileNames = PluginPath.entryList(QStringList(_Filter), QDir::Files);

    foreach(QString _PluginFileName, _PluginFileNames)
    {
        QPluginLoader *PluginLoader = new QPluginLoader;
        FPluginInterface *Plugin;

        PluginLoader->setFileName(PluginPath.absoluteFilePath(_PluginFileName));

        if(PluginLoader->load())
        {
                if( (Plugin = qobject_cast<FPluginInterface *>(PluginLoader->instance())) )
                {
                    Plugins.append(Plugin);
                    Plugins.last()->SetFileName(_PluginFileName);
                    Plugins.last()->SetMainWindow(this);
                    Plugins.last()->SetPlugins(&Plugins);
                }
                else
                {
                    QMessageBox::warning(this, tr("Error"),
                                         tr("An error occurred while instantiating the plugin: %0")
                                         .arg(_PluginFileName));
                }
        }
        else
        {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Error loading the plugin: %0")
                                 .arg(_PluginFileName));
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool _Close = true;

    _Close = TrySaveFile();

    foreach(FPluginInterface* _Plugin, Plugins)
    {
        if(!_Plugin->AcceptCloseApp())
        {
            _Close = false;
            break;
        }
    }

    if(_Close)
    {
        WriteSettings();
        event->accept();
    }
    else event->ignore();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        OpenFile(event->mimeData()->urls()[0].toLocalFile());
        UpdateDataTabs();
    }
}

void MainWindow::CreateActions()
{

    ActGrTypeFiles = new QActionGroup(this);
    for (int i = 0; i < 5; ++i)
    {
        ActGrTypeFiles->addAction(NameTypeForIds[i]);
        ActGrTypeFiles->actions()[i]->setProperty("FileType", i);
        ActGrTypeFiles->actions()[i]->setIcon(QIcon(":/images/Arq" + QString::number(i) + ".png"));
        ActGrTypeFiles->actions()[i]->setIconVisibleInMenu(true);
    }


    ActGrOpenedFiles = new QActionGroup(this);
    for (int i = 0; i < 5; ++i)
    {
        ActGrOpenedFiles->addAction("");
        ActGrOpenedFiles->actions()[i]->setProperty("FileType", i);
        ActGrOpenedFiles->actions()[i]->setIcon(QIcon(":/images/Arq" + QString::number(i) + ".png"));
        ActGrOpenedFiles->actions()[i]->setIconVisibleInMenu(true);
        ActGrOpenedFiles->actions()[i]->setVisible(false);
    }

    ActOpenFile = new QAction(QIcon(":/images/OpenFile.png"), tr("&Open..."), this);
    ActOpenFile->setShortcut(QKeySequence::Open);
    ActOpenFile->setIconVisibleInMenu(true);
    connect(ActOpenFile, SIGNAL(triggered()), this, SLOT(on_ActOpenFile_triggered()));

    ActNewFile = new QAction(QIcon(":/images/NewFile_16.png"), tr("&New Based On..."), this);
    ActNewFile->setShortcut(QKeySequence::New);
    ActNewFile->setIconVisibleInMenu(true);
    ActNewFile->setEnabled(false);
    connect(ActNewFile, SIGNAL(triggered()), this, SLOT(on_ActNewFile_triggered()));

    ActSave = new QAction(QIcon(":/images/Save.png"), tr("&Save"), this);
    ActSave->setShortcut(QKeySequence::Save);
    ActSave->setIconVisibleInMenu(true);
    ActSave->setEnabled(false);
    connect(ActSave, SIGNAL(triggered()), this, SLOT(SaveFile()));

    ActCloseFile = new QAction(QIcon(":/images/CloseFile_16.png"), tr("&Close..."), this);
    ActCloseFile->setShortcut(QKeySequence::Close);
    ActCloseFile->setIconVisibleInMenu(true);
    ActCloseFile->setEnabled(false);
    connect(ActCloseFile, SIGNAL(triggered()), this, SLOT(on_ActCloseFile_triggered()));

    ActCloseAllFiles = new QAction(tr("Close All"), this);
    ActCloseAllFiles->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_W);
    connect(ActCloseAllFiles, SIGNAL(triggered()), this, SLOT(on_ActCloseAllFiles_triggered()));

    ActExitApp = new QAction(QIcon(":/images/Exit_16.png"), tr("E&xit"), this);
    ActExitApp->setShortcut(QKeySequence::Quit);
    ActExitApp->setIconVisibleInMenu(true);
    connect(ActExitApp, SIGNAL(triggered()), this, SLOT(close()));

    ActUndo = new QAction(QIcon(":/images/Undo.png"), tr("&Undo"), this);
    ActUndo->setShortcut(QKeySequence::Undo);
    ActUndo->setProperty("Undo", true);
    ActUndo->setIconVisibleInMenu(true);
    ActUndo->setEnabled(false);
    connect(ActUndo, SIGNAL(triggered()), this, SLOT(on_ActUndoRedo_triggered()));

    ActRedo = new QAction(QIcon(":/images/Redo.png"), tr("&Redo"), this);
    ActRedo->setShortcut(QKeySequence::Redo);
    ActRedo->setProperty("Undo", false);
    ActRedo->setIconVisibleInMenu(true);
    ActRedo->setEnabled(false);
    connect(ActRedo, SIGNAL(triggered()), this, SLOT(on_ActUndoRedo_triggered()));

    ActOptions = new QAction(QIcon(":/images/Configure_16.png"), tr("Options..."), this);
    ActOptions->setIconVisibleInMenu(true);
    connect(ActOptions, SIGNAL(triggered()), this, SLOT(on_ActOptions_triggered()));

    ActInstalledPlugins = new QAction(QIcon(":/images/Plugins_16.png"), tr("Installed plugins"), this);
    ActInstalledPlugins->setIconVisibleInMenu(true);
    connect(ActInstalledPlugins, SIGNAL(triggered()), this, SLOT(on_ActInstalledPlugins_triggered()));

    ActAbout = new QAction(QIcon(":/images/About_16.png"), tr("About"), this);
    ActAbout->setIconVisibleInMenu(true);
    connect(ActAbout, SIGNAL(triggered()), this, SLOT(on_ActAbout_triggered()));

    //ActAboutQt = new QAction(tr("About Qt"), this);
    //connect(ActAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    ActTeste = new QAction(tr("Teste"), this);
    connect(ActTeste, SIGNAL(triggered()), this, SLOT(on_ActTeste_triggered()));

}

void MainWindow::CreateMenus()
{

    MenuFile->addAction(ActOpenFile);

    MenuRecentFiles = new QRecentFilesMenu(tr("Open &Recent"), MenuFile);
    MenuFile->addMenu(MenuRecentFiles);
    connect(MenuRecentFiles, SIGNAL(recentFileTriggered(const QString &)), this, SLOT(OpenFile(QString)));

    MenuFile->addAction(ActNewFile);

    MenuFile->addSeparator();

    MenuFile->addAction(ActSave);

    MenuFile->addSeparator();

    MenuFile->addAction(ActCloseFile);
    MenuFile->addAction(ActCloseAllFiles);

    MenuFile->addSeparator();

    MenuFile->addAction(ActExitApp);


    MenuEdit->addAction(ActUndo);
    MenuEdit->addAction(ActRedo);

    MenuTools->addAction(ActOptions);

    //MenuHelp->addAction(ActAboutQt);
    MenuHelp->addAction(ActAbout);
    MenuHelp->addSeparator();
    MenuHelp->addAction(ActInstalledPlugins);
}

void MainWindow::CreateToolBars()
{

    ToolBarFile->addAction(ActOpenFile);
    ToolBarFile->addSeparator();
    ToolBarFile->addAction(ActSave);
    ToolBarFile->addSeparator();
    ToolBarFile->addAction(ActTeste);
    //ToolBarFile->addSeparator();

    ToolBarEdit->addAction(ActUndo);
    ToolBarEdit->addAction(ActRedo);
    ToolBarEdit->addSeparator();
}

void MainWindow::WriteSettings()
{
    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup("MainWindow");
    _Settings.setValue("Geometry", saveGeometry());
    _Settings.setValue("State", saveState());
    _Settings.setValue("RecentFiles", MenuRecentFiles->saveState());
    _Settings.endGroup();
}

void MainWindow::ReadSettings()
{
    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup("MainWindow");
    restoreGeometry(_Settings.value("Geometry").toByteArray());
    restoreState(_Settings.value("State").toByteArray());
    MenuRecentFiles->restoreState(_Settings.value("RecentFiles").toByteArray());
    _Settings.endGroup();
}

void MainWindow::ReadGeneralConfig()
{
    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup("General");
    qApp->setStyle(_Settings.value("Style", "Plastique").toString());
    _Settings.endGroup();
}
