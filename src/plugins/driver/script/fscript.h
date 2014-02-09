#ifndef FSCRIPT_H
#define FSCRIPT_H

#include <QMainWindow>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QAction>
#include <QApplication>
#include <QMessageBox>

#include "../fdriverelement.h"
#include "fscripteditor.h"
#include "fdatapackage.h"


// Objeto inserido no script que invoca as funções dos plugins -------------------------------------------
class FPlugtinExec : public QObject
{
    Q_OBJECT

public:
    FPlugtinExec(FData *_Data, QObject *parent = 0) :
            QObject(parent), Data(_Data) {}

    Q_INVOKABLE void Exec(QString _PlugCommand, FDataPackage *_DataPackage)
    {
        if(_PlugCommand != "")
        {
            FDataPackage *DatPac = qobject_cast<FDataPackage*>(_DataPackage);

            if(DatPac)
            {
                    DatPac->SetCommand(_PlugCommand);
                    DatPac->SetData(Data);
                    DatPac->Data3d->SetData(Data);
                    emit ScriptReturn(DatPac);
            }
            else
            {
                QMessageBox::warning(qobject_cast<QWidget *>(this), tr("Error"),
                                     tr("The data sent to the plugin are invalid!"));
            }

        }
    }

signals:
    void ScriptReturn(FDataPackage *_DataPackage);

private:
    FData *Data;

};

// Diálogo de script em processo ----------------------------------------------------------------------------
class FScriptProcessDialog : public QDialog
{
    Q_OBJECT

public:
    FScriptProcessDialog(QObject *parent = 0) :
            QDialog(qobject_cast<QWidget*>(parent->parent()))
    {
        setWindowFlags(Qt::SplashScreen);
        QLabel *Label = new QLabel(tr("Processing the script..."), this);
        QPushButton *BtnCancel = new QPushButton(tr("Cancel"), this);
        QVBoxLayout *VLayout = new QVBoxLayout(this);
        VLayout->addWidget(Label);
        VLayout->addWidget(BtnCancel);
        setLayout(VLayout);
        connect(BtnCancel, SIGNAL(clicked()), this, SIGNAL(CancelScript()));
    }

signals:
    void CancelScript();
};

//-----------------------------------------------------------------------------------------------------------
class FScript : public FDriverElement
{
    Q_OBJECT

public:
    explicit FScript(QString _Name, FData *_Data = 0, QObject *parent = 0);
    ~FScript();
    void SetData(FData *_Data);

    void SetScript(QString _Script) { Script = _Script; }
    QString GetScript() { return Script; }
    void Edit();
    void Execute();
    void SetUiPath(QDir _UiPath) { UiPath = _UiPath; }
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void ScriptReturn(FDataPackage *_DataPackage);
    void ScriptRunningState(bool _State);

public slots:
    void RunScript(QString _Script, QString _UiFileName, bool _Debug);
    void AbortEvaluation();
    void on_Editor_Finished(bool _Ok);

private:
    void ReportScriptError(const QScriptValue &error);
    void ReadDocument();
    void WriteDocument();

    FData *Data;
    FDataIterator *DataIterator;

    QString Script;
    bool UseDialog;
    QString UiFileName;
    QDir UiPath;

    FScriptEditor *ScriptEditor;

    FPlugtinExec *PlugtinExec;

    QScriptValue SvDataObjConstructor;
    QScriptValue SvMetaDataObj;
    QScriptValue SvPlugtinExec;
    QScriptValue SvDataIterator;
    QScriptValue SvMsgBox;

    QScriptEngine *ScriptEngine;
    QScriptEngineDebugger *ScriptEngineDebugger;
    QMainWindow *DebugWindow;
    QWidget *UiDialog;


};

#endif // FSCRIPT_H
