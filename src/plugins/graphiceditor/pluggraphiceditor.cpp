#include "pluggraphiceditor.h"


void PlugGraphicEditor::SetMainWindow(FMainWindow *_MainWindow)
{
    MainWindow = _MainWindow;

    ActNewGraphicEditor = new QAction(QIcon(":/graphiceditor/images/GraphicEditor.png"), tr("Graphic Editor"), MainWindow);
    ActNewGraphicEditor->setIconVisibleInMenu(true);
    connect(ActNewGraphicEditor, SIGNAL(triggered()), this, SLOT(on_ActNewGraphicEditor_triggered()));

    MainWindow->MenuEdit->addAction(ActNewGraphicEditor);
    MainWindow->ToolBarEdit->addAction(ActNewGraphicEditor);

    connect(MainWindow, SIGNAL(UpdatePluginsSettings()), this, SLOT(ReadSettingsWidget()));

}

void PlugGraphicEditor::on_ActNewGraphicEditor_triggered()
{
    NewGraphicEditor(0, "");
}

QStringList PlugGraphicEditor::KeyFunctions() const
{
    return QStringList() << "Show";
}

void PlugGraphicEditor::ExecFunction(const QString _KeyFunction, FDataPackage *_DataPackage)
{
    QString _Function;
    QString _Label;

    int _Index = _KeyFunction.indexOf(QChar(':'));

    if(_Index > 0)
    {
        _Function = _KeyFunction.mid(0, _Index);
        _Label = _KeyFunction.mid(_Index+1);
    }
    else
    {
        _Function = _KeyFunction;
        _Label = "";
    }


    if(_Function == "Show") NewGraphicEditor(_DataPackage, _Label);
}

void PlugGraphicEditor::NewGraphicEditor(FDataPackage *_DataPackage, QString _Label)
{
    if(!_DataPackage) _DataPackage = new FDataPackage(MainWindow->Data);

    FGraphicEditor *GraphicEditor = new FGraphicEditor(MainWindow);
    GraphicEditor->Chart2d->SetEnableRefresh(false);

    if(_Label == "") _Label = tr("Graphic Editor");

    GraphicEditor->SetData(_DataPackage);
    GraphicEditor->SetTabLabel(_Label);
    MainWindow->AddNewTab(GraphicEditor);
    MainWindow->RefreshDataTabs();

    if(_DataPackage->GetStartAddress() < _DataPackage->GetEndAddress())
    {
        GraphicEditor->Chart2d->SetXMinMax(_DataPackage->GetStartAddress(),
                                           _DataPackage->GetEndAddress());

        GraphicEditor->Chart2d->SetPointer(_DataPackage->GetStartAddress());
    }

    if( (_DataPackage->GetSelX() >= 2) && (_DataPackage->GetSelY() >= 2) )
    {
        GraphicEditor->Chart2d->SetSelection(_DataPackage->GetStartAddress(),
                                             _DataPackage->GetSelX(),
                                             _DataPackage->GetSelY());
    }

    GraphicEditor->Chart2d->SetEnableRefresh(true);
    GraphicEditor->Chart2d->RefreshScreen();

}

QWidget *PlugGraphicEditor::SettingsWidget()
{
    Config = new FGraphicEditorCfg(this->Name());
    return Config;
}

void PlugGraphicEditor::ReadSettingsWidget()
{
    Config->WriteSettings();
}

//Q_EXPORT_PLUGIN2(pluggraphiceditor, PlugGraphicEditor)
