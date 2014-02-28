#include "fscript.h"
#include <QtUiTools>


// Construtor de DataObj ------------------------------------------------------------------------------------
QScriptValue NewDataPackageFunc(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(context)

    FDataPackage *DataPack = new FDataPackage;
    DataPack->UseData3d();

    QScriptValue _ScriptDataPackage = engine->newQObject(DataPack, QScriptEngine::QtOwnership);
    QScriptValue _ScriptData3d = engine->newQObject(DataPack->Data3d, QScriptEngine::QtOwnership);

    _ScriptDataPackage.setProperty("Data3D", _ScriptData3d);

    return _ScriptDataPackage;
}
/*
  Obs.: "QScriptEngine::QtOwnership" serve para o objeto criado dentro do script não ser destruído após o
        término do script.
*/


//-----------------------------------------------------------------------------------------------------------
FScript::FScript(QString _Name, FData *_Data, QObject *parent) :
    FDriverElement(_Name, parent), Data(0), DataIterator(0),
    ScriptEditor(0), ScriptEngineDebugger(0), DebugWindow(0),
    UiDialog(0)
{
    ScriptEngine = new QScriptEngine(this);
    ScriptEngine->setProcessEventsInterval(100); // Deixa a interface processar os eventos a cada 100ms

    if(_Data) SetData(_Data);
}

FScript::~FScript()
{
    /*QMessageBox msg;
    msg.setText("Objeto Deletado");
    msg.exec();*/
}

void FScript::SetData(FData *_Data)
{
    Data = _Data;

    DataIterator = new FDataIterator(Data, this);

    PlugtinExec = new FPlugtinExec(Data, this);
    connect(PlugtinExec, SIGNAL(ScriptReturn(FDataPackage*)), this, SIGNAL(ScriptReturn(FDataPackage*)));

    SvDataObjConstructor = ScriptEngine->newFunction(NewDataPackageFunc); //NewDataPackageFunc = Construtor de DataObj
    SvMetaDataObj = ScriptEngine->newQMetaObject(&QObject::staticMetaObject, SvDataObjConstructor);

    SvDataIterator = ScriptEngine->newQObject(DataIterator);
    SvPlugtinExec = ScriptEngine->newQObject(PlugtinExec);
    SvMsgBox = ScriptEngine->newQObject(new QMessageBox);

    ScriptEngine->globalObject().setProperty("DataObj", SvMetaDataObj);
    ScriptEngine->globalObject().setProperty("Data", SvDataIterator);
    ScriptEngine->globalObject().setProperty("Plugin", SvPlugtinExec);
    ScriptEngine->globalObject().setProperty("MsgBox", SvMsgBox);
}

void FScript::Edit()
{
    ScriptEditor = new FScriptEditor(qobject_cast<QWidget *>(this->parent()), this);
    ScriptEditor->SetUiFileName(UiFileName);
    ScriptEditor->SetUiPath(UiPath);
    ScriptEditor->SetScript(Script);
    ScriptEditor->setVisible(true);

    connect(ScriptEditor, SIGNAL(ExecuteScript(QString,QString,bool)), this, SLOT(RunScript(QString,QString,bool)));
    connect(ScriptEditor, SIGNAL(AbortEvaluation()), this, SLOT(AbortEvaluation()));
    connect(this, SIGNAL(ScriptRunningState(bool)), ScriptEditor, SLOT(SetScriptRunningState(bool)));    
    connect(ScriptEditor, SIGNAL(EditingFinished(bool)), this, SLOT(on_Editor_Finished(bool)));
}

void FScript::Execute()
{
    FScriptProcessDialog *ScriptProcessDialog = new FScriptProcessDialog(this);
    connect(ScriptProcessDialog, SIGNAL(CancelScript()), this, SLOT(AbortEvaluation()));
    ScriptProcessDialog->show();

    RunScript(Script, UiFileName, false);
    delete ScriptProcessDialog;
    if(!UiDialog) deleteLater();
}

void FScript::RunScript(QString _Script, QString _UiFileName, bool _Debug)
{
    if(UiDialog)
    {
        delete UiDialog;
        UiDialog = 0;
    }

    if(_Script == "") _Script = Script;

    emit ScriptRunningState(true);

    if(_Debug)
    {
        if(!ScriptEngineDebugger)
        {
            ScriptEngineDebugger = new QScriptEngineDebugger(this);
            DebugWindow = ScriptEngineDebugger->standardWindow();
            DebugWindow->setWindowModality(Qt::ApplicationModal);
            DebugWindow->resize(1280, 704);
        }
        ScriptEngineDebugger->attachTo(ScriptEngine);
        ScriptEngineDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();       
    }


    QScriptValue MainScriptRet = ScriptEngine->evaluate(_Script);


    if(MainScriptRet.isObject() && !MainScriptRet.isError())
    {
            if(MainScriptRet.property("UseDialog").toBool() && (_UiFileName != ""))
            {
                    QFile UiFile(UiPath.absoluteFilePath(_UiFileName));
                    if(UiFile.open(QIODevice::ReadOnly))
                    {
                            QUiLoader UiLoader;
                            UiDialog = UiLoader.load(&UiFile, qobject_cast<QWidget*>(this->parent()));
                            UiFile.close();

                            if(UiDialog)
                            {
                                QScriptValue SvDialog = ScriptEngine->newQObject(UiDialog);
                                MainScriptRet.setProperty(UiDialog->objectName(), SvDialog);
                                UiDialog->installEventFilter(this);
                            }
                            else
                            {
                                QMessageBox::warning(qobject_cast<QWidget*>(this), tr("Error"),
                                                     tr("Error loading %1.")
                                                     .arg(_UiFileName));
                            }

                    }
                    else
                    {
                        QMessageBox::warning(qobject_cast<QWidget*>(this), tr("Error"),
                                             tr("Error opening file %1:\n%2.")
                                             .arg(_UiFileName)
                                             .arg(UiFile.errorString()));
                    }
            }

            MainScriptRet = MainScriptRet.property("Run").call(MainScriptRet);

    }

    emit ScriptRunningState(false);

    if(ScriptEngineDebugger)
    {
        ScriptEngineDebugger->detach();
        delete ScriptEngineDebugger;
        ScriptEngineDebugger = 0;
    }

    if(MainScriptRet.isError()) ReportScriptError(MainScriptRet);

}

void FScript::ReportScriptError(const QScriptValue &error)
{
    int _LineError = error.property("lineNumber").toInt32();

    QMessageBox::warning(qobject_cast<QWidget *>(this), tr("Error in script"), tr("Line %0: %1")
                         .arg(_LineError)
                         .arg(error.toString()));

    if(ScriptEditor) ScriptEditor->SetCursorPosition(_LineError, 1);
}

void FScript::AbortEvaluation()
{
    ScriptEngine->abortEvaluation();
    if(UiDialog) { delete UiDialog; UiDialog = 0; }
    emit ScriptRunningState(false);
}

void FScript::on_Editor_Finished(bool _Ok)
{
    if(_Ok)
    {
        WriteDocument();
        emit EditingFinished();
    }

    delete ScriptEditor;
    AbortEvaluation();
    deleteLater();
}

bool FScript::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == UiDialog && event->type() == QEvent::Close)
    {
        AbortEvaluation();
        if(!ScriptEditor) deleteLater();
        else ScriptEditor->raise();
        return true;
    }
    return false;
}

void FScript::ReadDocument()
{
    QStringList _LstItems;

    _LstItems = XmlDriver.GetItemsTextFromElementName(DriverDocument, GetName(), "UiFileName");
    if(!_LstItems.isEmpty()) UiFileName = _LstItems[0];

    _LstItems.clear();

    _LstItems = XmlDriver.GetItemsTextFromElementName(DriverDocument, GetName(), "StrScript");
    if(!_LstItems.isEmpty()) Script = _LstItems[0];
}

void FScript::WriteDocument()
{
    XmlDriver.ClearElement(DriverDocument, GetName());
    XmlDriver.SetElementGroup(DriverDocument, GetName(), ScriptEditor->GetGroup());
    XmlDriver.AddElementItem(DriverDocument, GetName(), "UiFileName", ScriptEditor->GetUiFileName());
    XmlDriver.AddElementItem(DriverDocument, GetName(), "StrScript", ScriptEditor->GetScript());
}
