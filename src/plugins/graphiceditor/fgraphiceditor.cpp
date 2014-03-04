#include "fgraphiceditor.h"

FGraphicEditor::FGraphicEditor(QWidget *parent)
    : FWindowTab(parent)
{
    Chart2d = new FChart2d;
    Chart3d = new FChart3d;
    GraphicEdtTool = new FGraphicEdtTool(this);

    //Chart3d->setMinimumWidth(10);
    Chart3d->setVisible(false);

    DataMode.Mode = BITS8;
    DataMode.Signal = false;
    DataMode.Swap = false;


    Chart2d->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Chart3d->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    GraphicEdtTool->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    CreateActions();
    CreateToolBar();


    Splitter = new QSplitter;
    Splitter->setOrientation(Qt::Horizontal);
    Splitter->setContentsMargins(0,0,0,0);
    Splitter->addWidget(Chart2d);
    Splitter->addWidget(Chart3d);

    MenuBar = new QMenuBar(this);
    MenuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    MenuView = MenuBar->addMenu("View");
    MenuView->addActions(ActGroupModeBits->actions());

    QVBoxLayout *VertLayout1 = new QVBoxLayout;
    VertLayout1->setContentsMargins(0,0,0,0);
    VertLayout1->setSpacing(0);
    VertLayout1->addWidget(MenuBar);
    VertLayout1->addWidget(ToolBar);
    VertLayout1->addWidget(Splitter);
    VertLayout1->addWidget(GraphicEdtTool);
    this->setLayout(VertLayout1);

    CreateConnections();

    UpdateSettings();

}

void FGraphicEditor::SetData(FDataPackage *_DataPackage)
{
   DataPackage = _DataPackage;
   Data = _DataPackage->Data;
   DataMap3d.SetData(Data);
   Chart2d->SetData(Data);
   Chart2d->SetDataMode(&DataMode);
   GraphicEdtTool->SetData(Data);
   GraphicEdtTool->SetDataMode(&DataMode);
   Chart3d->SetData(&DataMap3d);
}

void FGraphicEditor::UpdateSettings()
{
    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup("GraphicEditor");
        Chart2d->SetShowScaleX(_Settings.value("ViewXScale", true).toBool());
        Chart2d->SetShowScaleY(_Settings.value("ViewYScale", false).toBool());
    _Settings.endGroup();
}

void FGraphicEditor::RefreshData()
{
    int _OffsetMax = SpinBoxByteOffset->maximum();

    switch(DataMode.Mode)
    {
    case BITS8:
        if(_OffsetMax != 0) SpinBoxByteOffset->setValue(0);
        SpinBoxByteOffset->setMaximum(0);
        break;
    case BITS16:
        if(_OffsetMax != 1) SpinBoxByteOffset->setValue(0);
        SpinBoxByteOffset->setMaximum(1);
        break;
    default:
        if(_OffsetMax != 3) SpinBoxByteOffset->setValue(0);
        SpinBoxByteOffset->setMaximum(3);
    }

    EnableComparisons();
    Chart2d->UpdateSettings();
    Chart2d->EmitSignals();
    GraphicEdtTool->UpdateSettings();
    Chart2d->RefreshScreen();

}

void FGraphicEditor::DefineByteOffsetByAddress(int _Address)
{
    int _Address2 = Data->GetPosByte(Data->GetPosRelative(_Address, DataMode), DataMode);
    int _Offset = _Address - _Address2;

    /*
       Se existe offset, com certeza o modo é de 16bits ou 32bits de pendendo do offset. Mas caso esta função
       seja chamada após o Modo ser mudado e antes da função Update(), quando Update() for chamada irá fazer o
       valor do SpinBoxByteOffset ser zerado, devido a mudança no valor máximo. A função abaixo ajusta o offset
       máximo agora, para que Update() não o zere pela mudança do máximo.
    */
    if(_Offset > 1)
    {
        if(DataMode.Mode == BITS16) SpinBoxByteOffset->setMaximum(1);
        else SpinBoxByteOffset->setMaximum(3);
    }

    SetByteOffset(_Offset);
}

void FGraphicEditor::SetByteOffset(unsigned int _ByteOffset)
{
    if(SpinBoxByteOffset->value() != (int)_ByteOffset) SpinBoxByteOffset->setValue(_ByteOffset);

    Chart2d->SetByteOffset(_ByteOffset);
    GraphicEdtTool->SetByteOffset(_ByteOffset);
}

void FGraphicEditor::on_CursorMode_Triggered()
{
    Chart2d->SetCursorMode(ActGroupCursor->checkedAction()->data().toInt());
}

void FGraphicEditor::on_Chart2d_CursorModeChange(int _CursorMode)
{
    switch(_CursorMode)
    {
    case CURSOR_DEFAULT:
        ActCursorSel->setChecked(true);
        break;
    case CURSOR_HAND:
        ActCursorHand->setChecked(true);
        break;
    case CURSOR_ZOOM:
        ActCursorZoomArea->setChecked(true);
    }
}

void FGraphicEditor::on_Chart2d_SelectionChange(int , int, int)
{
    if(ActViewChart3D->isChecked())
    {
        DataMap3d.ModeDataValZ = DataMode;
        DataMap3d.ReadMap(Chart2d->GetSelectionIni(), Chart2d->GetSelectionNPos(), Chart2d->GetSelectionMult());
        DataMap3d.CreateDefaultScale();
        Chart3d->Update();
        DefinePointerChart3d();
    }
}

void FGraphicEditor::on_Chart2d_PointerChange(int)
{
    if(ActViewChart3D->isChecked()) DefinePointerChart3d();
}

void FGraphicEditor::on_GraphicEdtTool_SelectionEdited(int, int, int)
{
    if(ActViewChart3D->isChecked())
    {
        DataMap3d.ModeDataValZ = DataMode;
        DataMap3d.ReadMap(Chart2d->GetSelectionIni(), Chart2d->GetSelectionNPos(), Chart2d->GetSelectionMult());
        DataMap3d.CreateDefaultScale();
        Chart3d->Update();
        DefinePointerChart3d();
    }
}

void FGraphicEditor::on_GraphicEdtTool_PointerEdited(int)
{
    if(ActViewChart3D->isChecked()) DefinePointerChart3d();
    Chart2d->GotoPointer();
}

void FGraphicEditor::DefinePointerChart3d()
{
    int _Pointer = Chart2d->GetPointer();

    int nx = Chart2d->GetSelectionNPos();
    int ny = Chart2d->GetSelectionMult();

    int ini = Chart2d->GetSelectionIni();
    int fin = ini + Data->GetPosByte(nx, DataMode)*ny - Data->GetPosByte(1, DataMode);


    if(_Pointer < ini || _Pointer > fin)
    {
        Chart3d->SetPointer(NanValue, 0);
    }
    else
    {
        _Pointer = Data->GetPosRelative(_Pointer - ini, DataMode);


        int x, y;

        for(y=1; y<=ny; y++)
            if(_Pointer < y*nx) break;

        // Pos = x + (y-1)*nx
        // y = (Pos - x + nx)/nx
        // x = Pos - (y-1)*nx

         x = _Pointer - (y-1)*nx;

         y--;

        Chart3d->SetPointer(x, y);
    }
}

void FGraphicEditor::on_ViewChart3D_Triggered()
{


   /* QMessageBox msgBox;
    msgBox.setText(QString::number(width()));
    msgBox.exec();*/

    Chart3d->setVisible(ActViewChart3D->isChecked());

    QList <int> SSizes;
    SSizes.append( width()*0.7 );
    SSizes.append( width()*0.3 );

    Splitter->setSizes(SSizes);

    DataMap3d.ModeDataValZ = DataMode;
    DataMap3d.ReadMap(Chart2d->GetSelectionIni(), Chart2d->GetSelectionNPos(), Chart2d->GetSelectionMult());
    DataMap3d.CreateDefaultScale();
    Chart3d->Update();
    DefinePointerChart3d();

}

void FGraphicEditor::on_SpinBoxByteOffset_Change(int _Value)
{
   SetByteOffset(_Value);

   //Data->SetPointer(Data->GetPosByte(Data->GetPosReletive(Data->GetPointer())) + _Value);
   //Data->SetSelectionIni(Data->GetPosByte(Data->GetPosReletive(Data->GetSelectionIni())) + _Value);

   RefreshData();
}

void FGraphicEditor::on_ChangeModeBits()
{
    Chart2d->SetEnableRefresh(false);

    // Grava os endereços atuais
    int _Min = Chart2d->GetMinX();
    int _Pointer = Chart2d->GetPointer();
    int _SelIni = Chart2d->GetSelectionIni();

    DataMode.Mode = ActGroupModeBits->checkedAction()->property("Mode").toInt();
    DataMode.Signal = ActModeBitsSignal->isChecked();
    DataMode.Swap = ActModeBitsSwap->isChecked();

    // Atualiza o tamanho relativo para SetMinX funcionar corretamente
    Chart2d->UpdateSettings();

    // Desabilita Swap quando em modo de 8 bits
    ActModeBitsSwap->setEnabled(!ActModeBits8->isChecked());

    // Desabilita Signal quando em modo de ponto flutuante
    ActModeBitsSignal->setEnabled(!ActModeBitsF->isChecked());

    // Restaura os endereços
    Chart2d->SetMinX(_Min);
    Chart2d->SetPointer(_Pointer);
    Chart2d->SetSelection(_SelIni, Chart2d->GetSelectionNPos(), Chart2d->GetSelectionMult());

    Chart2d->SetExtremesY(Data->GetMinY(DataMode), Data->GetMaxY(DataMode));
    Chart2d->SetEnableRefresh(true);

    RefreshData();
}

void FGraphicEditor::on_ChangeActiveComp()
{
    EnableComparisons();
    RefreshData();
}

void FGraphicEditor::EnableComparisons()
{
    if(ActComp1->isChecked()) GraphicEdtTool->SetActiveComp(2);
    else if(ActComp2->isChecked()) GraphicEdtTool->SetActiveComp(3);
    else if(ActComp3->isChecked()) GraphicEdtTool->SetActiveComp(4);

      char Comp = 5;

         for(int x = 2; x <= 4; x++) // As comparações iniciam em 2 pois, 0 = edição e 1 = original.
         {
              if(Data->ExistFileType(x))
              {
                if(x < Comp) Comp = x;

                   switch(x)
                   {
                     case 2:
                        ActComp1->setEnabled(true);
                        break;
                     case 3:
                        ActComp2->setEnabled(true);
                        break;
                     case 4:
                        ActComp3->setEnabled(true);
                        break;
                   }

              }
              else
                 {
                     switch(x)
                     {
                       case 2:
                          ActComp1->setEnabled(false);
                          break;
                       case 3:
                          ActComp2->setEnabled(false);
                          break;
                       case 4:
                          ActComp3->setEnabled(false);
                          break;
                     }

                 }
         }

              if(Comp >= 5) // Caso não exista nenhuma comparação aberta
              {
                ActComp1->setChecked(true);
                GraphicEdtTool->SetActiveComp(2);
                return;
              }

          // A comparação ativa ja está selecionada
          if(Data->ExistFileType(GraphicEdtTool->GetActiveComp())) Comp = GraphicEdtTool->GetActiveComp();

          // Se existir uma compração aberta mas a mesma não está selecionada, será usado o valor de Comp
          GraphicEdtTool->SetActiveComp(Comp);

             switch(Comp)
             {
               case 2:
                  ActComp1->setChecked(true);
                  break;
               case 3:
                  ActComp2->setChecked(true);
                  break;
               case 4:
                  ActComp3->setChecked(true);
                  break;
             }
}

void FGraphicEditor::CreateActions()
{
    ActCursorSel = new QAction(QIcon(":/graphiceditor/images/Cursor.png"), tr("Selection"), this);
    ActCursorSel->setData(CURSOR_DEFAULT);
    ActCursorSel->setCheckable(true);
    ActCursorSel->setChecked(true);

    ActCursorHand = new QAction(QIcon(":/graphiceditor/images/Hand.png"), tr("Hand"), this);
    ActCursorHand->setData(CURSOR_HAND);
    ActCursorHand->setCheckable(true);

    ActCursorZoomArea = new QAction(QIcon(":/graphiceditor/images/ZoomArea.png"), tr("Select zoom area"), this);
    ActCursorZoomArea->setData(CURSOR_ZOOM);
    ActCursorZoomArea->setCheckable(true);

    ActCursorZoomReset = new QAction(QIcon(":/graphiceditor/images/ZoomReset.png"), tr("Reset zoom"), this);
    ActCursorZoomReset->setData(CURSOR_RESET_ZOOM);
    ActCursorZoomReset->setCheckable(true);

    ActGroupCursor = new QActionGroup(this);
    ActGroupCursor->addAction(ActCursorSel);
    ActGroupCursor->addAction(ActCursorHand);
    ActGroupCursor->addAction(ActCursorZoomArea);
    ActGroupCursor->addAction(ActCursorZoomReset);



    // Modo -------------------------------------------------------------------------------
    ActModeBits8 = new QAction(QIcon(":/graphiceditor/images/Bits8.png"), tr("8 Bits"), this);
    ActModeBits8->setIconVisibleInMenu(true);
    ActModeBits8->setProperty("Mode", BITS8);
    ActModeBits8->setCheckable(true);
    ActModeBits8->setChecked(true);
    //connect(ActModeBits8, SIGNAL(triggered()), this, SLOT(ChangeModeBits()));

    ActModeBits16 = new QAction(QIcon(":/graphiceditor/images/Bits16.png"), tr("16 Bits"), this);
    ActModeBits16->setIconVisibleInMenu(true);
    ActModeBits16->setProperty("Mode", BITS16);
    ActModeBits16->setCheckable(true);
    //connect(ActModeBits16, SIGNAL(triggered()), this, SLOT(ChangeModeBits()));

    ActModeBits32 = new QAction(QIcon(":/graphiceditor/images/Bits32.png"), tr("32 Bits"), this);
    ActModeBits32->setIconVisibleInMenu(true);
    ActModeBits32->setProperty("Mode", BITS32);
    ActModeBits32->setCheckable(true);
    //connect(ActModeBits32, SIGNAL(triggered()), this, SLOT(ChangeModeBits()));

    ActModeBitsF = new QAction(QIcon(":/graphiceditor/images/BitsF.png"), tr("Floating Point"), this);
    ActModeBitsF->setIconVisibleInMenu(true);
    ActModeBitsF->setProperty("Mode", BITSF);
    ActModeBitsF->setCheckable(true);
    //connect(ActModeBitsF, SIGNAL(triggered()), this, SLOT(ChangeModeBits()));

    ActModeBitsSignal = new QAction(QIcon(":/graphiceditor/images/Signal.png"), tr("Signal"), this);
    ActModeBitsSignal->setIconVisibleInMenu(true);
    ActModeBitsSignal->setCheckable(true);
    //connect(ActModeBitsSignal, SIGNAL(triggered()), this, SLOT(ChangeModeBits()));

    ActModeBitsSwap = new QAction(QIcon(":/graphiceditor/images/Swap.png"), tr("Swap"), this);
    ActModeBitsSwap->setIconVisibleInMenu(true);
    ActModeBitsSwap->setCheckable(true);
    ActModeBitsSwap->setEnabled(false);
    //connect(ActModeBitsSwap, SIGNAL(triggered()), this, SLOT(ChangeModeBits()));

    ActGroupModeBits = new QActionGroup(this);
    ActGroupModeBits->addAction(ActModeBits8);
    ActGroupModeBits->addAction(ActModeBits16);
    ActGroupModeBits->addAction(ActModeBits32);
    ActGroupModeBits->addAction(ActModeBitsF);

    // Comparação --------------------------------------------------------------------
    ActComp1 = new QAction(QIcon(":/graphiceditor/images/Arq2.png"), tr("Comparison 1"), this);
    ActComp1->setIconVisibleInMenu(true);
    ActComp1->setCheckable(true);
    ActComp1->setChecked(true);
    ActComp1->setEnabled(false);
    //connect(ActComp1, SIGNAL(triggered()), this, SLOT(ChangeActiveComp()));

    ActComp2 = new QAction(QIcon(":/graphiceditor/images/Arq3.png"), tr("Comparison 2"), this);
    ActComp2->setIconVisibleInMenu(true);
    ActComp2->setCheckable(true);
    ActComp2->setEnabled(false);
    //connect(ActComp2, SIGNAL(triggered()), this, SLOT(ChangeActiveComp()));

    ActComp3 = new QAction(QIcon(":/graphiceditor/images/Arq4.png"), tr("Comparison 3"), this);
    ActComp3->setIconVisibleInMenu(true);
    ActComp3->setCheckable(true);
    ActComp3->setEnabled(false);
    //connect(ActComp3, SIGNAL(triggered()), this, SLOT(ChangeActiveComp()));

    ActGroupComp = new QActionGroup(this);
    ActGroupComp->addAction(ActComp1);
    ActGroupComp->addAction(ActComp2);
    ActGroupComp->addAction(ActComp3);




    ActViewChart3D = new QAction(QIcon(":/graphiceditor/images/Chart3d.png"), tr("3D"), this);
    ActViewChart3D->setCheckable(true);

}

void FGraphicEditor::CreateToolBar()
{
    SpinBoxByteOffset = new QSpinBox(this);
    SpinBoxByteOffset->setMinimum(0);
    SpinBoxByteOffset->setMaximum(0);

    ToolBar = new QToolBar();
    ToolBar->setIconSize(QSize(16,16));
    ToolBar->addActions(ActGroupCursor->actions());

    ToolBar->addSeparator();

    ToolBar->addActions(ActGroupModeBits->actions());

    ToolBar->addSeparator();

    ToolBar->addAction(ActModeBitsSignal);
    ToolBar->addAction(ActModeBitsSwap);
    ToolBar->addWidget(SpinBoxByteOffset);

    ToolBar->addSeparator();

    ToolBar->addActions(ActGroupComp->actions());

    ToolBar->addSeparator();

    ToolBar->addAction(ActViewChart3D);

}

void FGraphicEditor::CreateConnections()
{
    connect(Chart2d, SIGNAL(PointerChange(int)), GraphicEdtTool, SLOT(SetPointer(int)));
    connect(Chart2d, SIGNAL(PointerChange(int)), this, SLOT(on_Chart2d_PointerChange(int)));
    connect(Chart2d, SIGNAL(SelectionChange(int, int, int)), GraphicEdtTool, SLOT(SetSelection(int,int,int)));
    connect(Chart2d, SIGNAL(SelectionChange(int, int, int)), this, SLOT(on_Chart2d_SelectionChange(int, int, int)));
    connect(Chart2d, SIGNAL(CursorModeChange(int)), this, SLOT(on_Chart2d_CursorModeChange(int)));

    connect(Chart2d, SIGNAL(PageChange(int,int)), GraphicEdtTool, SLOT(SetPage(int,int)));

    // Conectar primeiro ao Chart2D
    connect(GraphicEdtTool, SIGNAL(SelectionEdited(int,int,int)), Chart2d, SLOT(SetSelection(int,int,int)));
    connect(GraphicEdtTool, SIGNAL(PointerEdited(int)), Chart2d, SLOT(SetPointer(int)));
    connect(GraphicEdtTool, SIGNAL(PageEdited(int,int)), Chart2d, SLOT(SetPage(int,int)));
    connect(GraphicEdtTool, SIGNAL(OffsetChanged()), Chart2d, SLOT(RefreshScreen()));

    // Depois ao form, pois os slots do Clart2D devem ser chamados primeiro
    connect(GraphicEdtTool, SIGNAL(SelectionEdited(int,int,int)),
            this, SLOT(on_GraphicEdtTool_SelectionEdited(int,int,int)));
    connect(GraphicEdtTool, SIGNAL(PointerEdited(int)),
            this, SLOT(on_GraphicEdtTool_PointerEdited(int)));

    connect(ActCursorSel, SIGNAL(triggered()), this, SLOT(on_CursorMode_Triggered()));
    connect(ActCursorHand, SIGNAL(triggered()), this, SLOT(on_CursorMode_Triggered()));
    connect(ActCursorZoomArea, SIGNAL(triggered()), this, SLOT(on_CursorMode_Triggered()));
    connect(ActCursorZoomReset, SIGNAL(triggered()), this, SLOT(on_CursorMode_Triggered()));
    connect(ActViewChart3D, SIGNAL(triggered()), this, SLOT(on_ViewChart3D_Triggered()));

    connect(ActModeBits8, SIGNAL(triggered()), this, SLOT(on_ChangeModeBits()));
    connect(ActModeBits16, SIGNAL(triggered()), this, SLOT(on_ChangeModeBits()));
    connect(ActModeBits32, SIGNAL(triggered()), this, SLOT(on_ChangeModeBits()));
    connect(ActModeBitsF, SIGNAL(triggered()), this, SLOT(on_ChangeModeBits()));
    connect(ActModeBitsSignal, SIGNAL(triggered()), this, SLOT(on_ChangeModeBits()));
    connect(ActModeBitsSwap, SIGNAL(triggered()), this, SLOT(on_ChangeModeBits()));

    connect(ActComp1, SIGNAL(triggered()), this, SLOT(on_ChangeActiveComp()));
    connect(ActComp2, SIGNAL(triggered()), this, SLOT(on_ChangeActiveComp()));
    connect(ActComp3, SIGNAL(triggered()), this, SLOT(on_ChangeActiveComp()));

    connect(SpinBoxByteOffset, SIGNAL(valueChanged(int)), this, SLOT(on_SpinBoxByteOffset_Change(int)));
}
