#ifndef FMAINWINDOW_H
#define FMAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class FDataPackage;
class FWindowTab;
class FData;
class FPluginInterface;

class FMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FMainWindow(QWidget *parent = 0);

    void AddNewTab(FWindowTab *_WindowTab);
    FData *Data;
    QMenu *MenuFile;
    QMenu *MenuEdit;
    QMenu *MenuTools;
    QMenu *MenuHelp;
    QToolBar *ToolBarFile;
    QToolBar *ToolBarEdit;


signals:
    void UpdatePluginsSettings();

public slots:
    void RefreshDataTabs();
    void UpdateDataTabs();
    void UpdateSettingsTabs();
    void ExecPluginsFunctions(FDataPackage *_DataPackage);


protected:
    QList<FPluginInterface*> Plugins;
    QTabWidget *TabWidgetMain;

};

#endif // FMAINWINDOW_H
