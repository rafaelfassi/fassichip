#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fmainwindow.h"

class QSplashScreen;
class QActionGroup;
class QRecentFilesMenu;


class MainWindow : public FMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSplashScreen *SplashScreen, QWidget *parent = 0);

public slots:
    bool OpenFile(QString _FileName, int _Type); // Este não tenta salvar o arquivo
    bool OpenFile(QString _FileName);
    bool CloseFile(int _Type); // Não tenta salvar o arquivo
    bool SaveFileAs(QString _FileName, int _Type);
    bool SaveFile();

private slots:
    void PeriodicalUpdate();
    void on_ActOpenFile_triggered();
    void on_ActNewFile_triggered();
    void on_ActCloseFile_triggered();
    void on_ActCloseAllFiles_triggered();
    void on_TabWidgetMain_CloseRequested(int _Id);
    void on_ActOptions_triggered();
    void on_ActInstalledPlugins_triggered();
    void on_ActUndoRedo_triggered();
    void on_ActAbout_triggered();
    void on_ActTeste_triggered();

protected:
    void closeEvent(QCloseEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);


private:
    void CreateActions();
    void CreateMenus();
    void CreateToolBars();
    void LoadPlugins();
    void ReadGeneralConfig();
    void WriteSettings();
    void ReadSettings();
    bool TrySaveFile();
    int PopupGetFileType(bool _Opened);

    QStringList NameTypeForIds;
    QActionGroup *ActGrTypeFiles;
    QActionGroup *ActGrOpenedFiles;
    QAction *ActOpenFile;
    QAction *ActNewFile;
    QAction *ActCloseFile;
    QAction *ActCloseAllFiles;
    QAction *ActExitApp;
    QAction *ActSave;
    QAction *ActUndo;
    QAction *ActRedo;
    QAction *ActOptions;
    QAction *ActInstalledPlugins;
    QAction *ActAbout;
    //QAction *ActAboutQt;
    QAction *ActTeste;
    QRecentFilesMenu *MenuRecentFiles;


};

#endif // MAINWINDOW_H
