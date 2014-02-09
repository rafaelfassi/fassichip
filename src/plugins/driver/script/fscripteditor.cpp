#include "fscripteditor.h"

#include <QDesktopWidget>

FScriptEditor::FScriptEditor(QWidget *parent, FDriverElement *ObjParent) :
    QWidget(parent, Qt::Window)
{
    setAttribute(Qt::WA_DeleteOnClose);

    ScriptObj = ObjParent;

    this->setWindowTitle(ScriptObj->GetName());
    this->setWindowIcon(QIcon(":/driver/images/Script.png"));
    this->resize(800, 600);

    QRect rect = QApplication::desktop()->availableGeometry(this);
    this->move(rect.center() - this->rect().center());

    CreateForm();
    FindGroups();
    CmbGroups->setCurrentIndex(CmbGroups->findText(ScriptObj->GetGroup()));
}

QString FScriptEditor::GetGroup()
{
    if( CmbGroups->currentIndex() >= 0 ) return CmbGroups->currentText();
    else return "";
}

QString FScriptEditor::GetUiFileName()
{
    if((CmbUiFileName->currentIndex() < 0)) return "";
    else return CmbUiFileName->currentText();
}

void FScriptEditor::on_Script_Clicked()
{
    QObject *ClickedButton = qobject_cast<QObject *>(sender());
    bool _Debug = ClickedButton->property("debug").toBool();

    QString _ui = "";
    if(CmbUiFileName->currentIndex() >= 0) _ui = CmbUiFileName->currentText();

    emit ExecuteScript(ScriptEdit->text(), _ui, _Debug);
}

void FScriptEditor::SetCursorPosition(int _Line, int _Col)
{
    ScriptEdit->setCursorPosition(_Line-1, _Col-1);
    ScriptEdit->setFocus();
}

void FScriptEditor::on_ScriptEdit_CursorPositionChanged(int _Line, int _Col)
{
    LabLineCol->setText(tr("Line: %0, Col: %1")
                        .arg(QString::number(_Line + 1))
                        .arg(QString::number(_Col + 1)));
}

void FScriptEditor::on_Ok_Cancel_Clicked()
{
    QObject *ClickedButton = qobject_cast<QObject *>(sender());
    bool _Ok = ClickedButton->property("Ok").toBool();

    emit EditingFinished(_Ok);
}

void FScriptEditor::closeEvent(QCloseEvent * event)
{
    if(BtnAbortEvaluation->isEnabled()) event->ignore();
    else
    {
        event->accept();
        emit EditingFinished(false);
    }
}

void FScriptEditor::SetScriptRunningState(bool _State)
{
    if(_State)
    {
        BtnRunScript->setEnabled(false);
        BtnRunInDebug->setEnabled(false);
        BtnCancel->setEnabled(false);
        BtnOk->setEnabled(false);
        BtnAbortEvaluation->setEnabled(true);
    }
    else
    {
        BtnRunScript->setEnabled(true);
        BtnRunInDebug->setEnabled(true);
        BtnCancel->setEnabled(true);
        BtnOk->setEnabled(true);
        BtnAbortEvaluation->setEnabled(false);
    }
}

void FScriptEditor::SetUiPath(QDir _UiPath)
{
    UiPath = _UiPath;
    FindUi();
}

void FScriptEditor::FindGroups()
{
    CmbGroups->clear();
    CmbGroups->addItem("");
    CmbGroups->addItems(ScriptObj->GetGroups());
}

void FScriptEditor::NewGroup()
{
    bool ok;
         QString _NewGroupName = QInputDialog::getText(this, tr("Driver"),
                                              tr("New group:"), QLineEdit::Normal,
                                              "", &ok);

         if(ok && !_NewGroupName.isEmpty())
         {
                 ScriptObj->AddNewGroup(_NewGroupName);
                 FindGroups();
                 CmbGroups->setCurrentIndex(CmbGroups->findText(_NewGroupName));
         }
}

void FScriptEditor::FindUi()
{
    if(UiPath.path() == ".") return; // Não foi especificada a pasta

    CmbUiFileName->clear();
    CmbUiFileName->addItem("");

    QStringList _UiFileNames = UiPath.entryList(QStringList("*.ui"), QDir::Files);

    foreach(QString _UiFileName, _UiFileNames)
        CmbUiFileName->addItem(_UiFileName);

     CmbUiFileName->setCurrentIndex(CmbUiFileName->findText(UiFileName));
}

void FScriptEditor::CreateForm()
{

    BtnRunScript = new QPushButton(QIcon(":/driver/images/Run.png"), tr("Run Script"), this);
    BtnRunScript->setProperty("debug", false);
    BtnRunScript->setFocusPolicy(Qt::NoFocus);
    connect(BtnRunScript, SIGNAL(clicked()), this, SLOT(on_Script_Clicked()));

    BtnRunInDebug = new QPushButton(QIcon(":/driver/images/DebuggerStart.png"), tr("Run in Debug"), this);
    BtnRunInDebug->setProperty("debug", true);
    BtnRunInDebug->setFocusPolicy(Qt::NoFocus);
    connect(BtnRunInDebug, SIGNAL(clicked()), this, SLOT(on_Script_Clicked()));

    BtnAbortEvaluation = new QPushButton(QIcon(":/driver/images/Stop.png"), tr("Abort"), this);
    BtnAbortEvaluation->setFocusPolicy(Qt::NoFocus);
    BtnAbortEvaluation->setEnabled(false);
    connect(BtnAbortEvaluation, SIGNAL(clicked()), this, SIGNAL(AbortEvaluation()));

    CmbGroups = new QComboBox(this);
    CmbGroups->setFocusPolicy(Qt::NoFocus);
    CmbGroups->setSizePolicy(QSizePolicy::Expanding,  QSizePolicy::Preferred);

    BtnNewGroup = new QPushButton(QIcon(":/driver/images/Add.png"), tr("New Group"), this);
    BtnNewGroup->setFocusPolicy(Qt::NoFocus);
    connect(BtnNewGroup, SIGNAL(clicked()), this, SLOT(NewGroup()));

    QHBoxLayout *HLayTop = new QHBoxLayout;
    HLayTop->addWidget(BtnRunScript);
    HLayTop->addWidget(BtnRunInDebug);
    HLayTop->addWidget(BtnAbortEvaluation);
    HLayTop->addWidget(CmbGroups);
    HLayTop->addWidget(BtnNewGroup);


    QLabel *LabUseDialog = new QLabel(tr("Form"), this);
    CmbUiFileName = new QComboBox(this);
    CmbUiFileName->setFocusPolicy(Qt::NoFocus);
    CmbUiFileName->setSizePolicy(QSizePolicy::Expanding,  QSizePolicy::Fixed);
    //connect(ChkUseDialog, SIGNAL(toggled(bool)), CmbUiFileName, SLOT(setEnabled(bool)));

    BtnRefreshUiLst = new QPushButton(QIcon(":/driver/images/Refresh_16.png"), tr("Refresh"), this);
    BtnRefreshUiLst->setIconSize(QSize(16,16));
    BtnRefreshUiLst->setFocusPolicy(Qt::NoFocus);
    connect(BtnRefreshUiLst, SIGNAL(clicked()), this, SLOT(FindUi()));

    QHBoxLayout *HLayUi = new QHBoxLayout;
    HLayUi->addWidget(LabUseDialog);
    HLayUi->addWidget(CmbUiFileName);
    HLayUi->addWidget(BtnRefreshUiLst);

    ScriptEdit = new FScriptEdit(this);

    LabLineCol = new QLabel(this);
    LabLineCol->setSizePolicy(QSizePolicy::Expanding,  QSizePolicy::Fixed);
    connect(ScriptEdit, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(on_ScriptEdit_CursorPositionChanged(int,int)));

    BtnOk = new QPushButton(QIcon(":/driver/images/Ok.png"), tr("Ok"), this);
    BtnOk->setProperty("Ok", true);
    BtnOk->setFocusPolicy(Qt::NoFocus);
    connect(BtnOk, SIGNAL(clicked()), this, SLOT(on_Ok_Cancel_Clicked()));

    BtnCancel = new QPushButton(QIcon(":/driver/images/Cancel.png"), tr("Cancel"), this);
    BtnCancel->setProperty("Ok", false);
    BtnCancel->setFocusPolicy(Qt::NoFocus);
    connect(BtnCancel, SIGNAL(clicked()), this, SLOT(on_Ok_Cancel_Clicked()));

    QHBoxLayout *HLayBottom = new QHBoxLayout;
    HLayBottom->addWidget(LabLineCol);
    HLayBottom->addWidget(BtnOk);
    HLayBottom->addWidget(BtnCancel);



    QVBoxLayout *VLayMain = new QVBoxLayout;
    VLayMain->addLayout(HLayTop);
    VLayMain->addLayout(HLayUi);
    VLayMain->addWidget(ScriptEdit);
    VLayMain->addLayout(HLayBottom);

    this->setLayout(VLayMain);
}
