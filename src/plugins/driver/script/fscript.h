/****************************************************************************
**
**Copyright (C) 2010-2014 - Rafael Fassi Lobão
**Contact: www.fassi.com.br - rafael@fassi.com.br
**
**This file is part of the Fassichip.
**Fassichip is free software; you can redistribute it and/or modify
**it under the terms of the GNU General Public License as published by
**the Free Software Foundation; either version 2 of the License, or
**(at your option) any later version.
**
**This program is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**GNU General Public License for more details.
**
****************************************************************************/

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
