#ifndef FSCRIPTEDITOR_H
#define FSCRIPTEDITOR_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include "fscriptedit.h"
#include "../fdriverelement.h"

class FScriptEditor : public QWidget
{
Q_OBJECT

public:
    explicit FScriptEditor(QWidget *parent = 0, FDriverElement *ObjParent = 0);

    QString GetScript() { return ScriptEdit->text(); }
    QString GetGroup();
    QString GetUiFileName();



signals:
    void ExecuteScript(QString _Script, QString _UiFileName, bool _Debug);
    void AbortEvaluation();
    void EditingFinished(bool _Ok);

public slots:
    void SetCursorPosition(int _Line, int _Col);
    void SetScriptRunningState(bool _State);
    void SetUiFileName(QString _UiFileName) { UiFileName = _UiFileName; }
    void SetUiPath(QDir _UiPath);
    void SetScript(QString _Script) { ScriptEdit->setText(_Script); }
    void FindGroups();


private slots:
    void on_Script_Clicked();
    void on_ScriptEdit_CursorPositionChanged(int _Line, int _Col);
    void on_Ok_Cancel_Clicked();
    void NewGroup();
    void FindUi();

private:
    void CreateForm();
    QPushButton *BtnRunScript;
    QPushButton *BtnRunInDebug;
    QPushButton *BtnAbortEvaluation;
    QComboBox *CmbGroups;
    QPushButton *BtnNewGroup;
    QComboBox *CmbUiFileName;
    QPushButton *BtnRefreshUiLst;
    FScriptEdit *ScriptEdit;
    QLabel *LabLineCol;
    QPushButton *BtnOk;
    QPushButton *BtnCancel;

    QDir UiPath; // Pasta onde procurar os arquivos Ui;
    QString UiFileName;

    FDriverElement *ScriptObj;

protected:
    void closeEvent(QCloseEvent * event);

};

#endif // FSCRIPTEDITOR_H
