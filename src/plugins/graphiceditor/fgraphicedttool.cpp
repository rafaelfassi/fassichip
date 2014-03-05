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

#include "fgraphicedttool.h"

FGraphicEdtTool::FGraphicEdtTool(QWidget *parent)
    : QWidget(parent)
{
    ByteOffset = 0;
    ActiveComp = 2;

    CreateForm();
    CreateConnections();

    HexValidatorEnd = new FHexValidator(this);
    EdtSelFin->setValidator(HexValidatorEnd);
}

FGraphicEdtTool::~FGraphicEdtTool()
{

}

void FGraphicEdtTool::SetPointer(int _Pointer)
{

    if(Data->IsEmply()) SpinBoxPointer->SetValue(0);
    else SpinBoxPointer->SetValue(_Pointer);

    double ValOri, ValEd, ValComp;

    if(Data->ExistFileType(ORI))
    {
       ValOri = Data->GetValueForType(ORI, _Pointer, *DataMode);
       if(DataMode->Mode == BITSF) EdtValOrHex->setText("-");
       else EdtValOrHex->setText(DoubleToHexS(ValOri, 1));
       EdtValOrDeci->setText(QString::number(ValOri));

       int pointerOffset = _Pointer - Data->GetPosByte(SpinBoxOffsetOr->value(), *DataMode);
       EdtPointerOr->setText(DoubleToHexS(pointerOffset));
    }
    else
    {
       EdtValOrHex->setText("-");
       EdtValOrDeci->setText("-");
       EdtPointerOr->setText("-");
    }

    if(Data->ExistFileType(EDIT))
    {
        ValEd = Data->GetValueForType(EDIT, _Pointer, *DataMode);
        if(DataMode->Mode == BITSF) EdtValEdHex->setText("-");
        else EdtValEdHex->setText(DoubleToHexS(ValEd, 1));
        EdtValEdDeci->setText(QString::number(ValEd));

         if(Data->ExistFileType(ORI))
         {
            EdtDifAbEd->setText(QString::number(ValEd - ValOri));
            if(ValOri == 0) EdtDifPercEd->setText("0");
            else EdtDifPercEd->setText(QString::number(ApproximateInt(((ValEd - ValOri)/ValOri)*100)));
         }
         else
         {
             EdtDifAbEd->setText("-");
             EdtDifPercEd->setText("-");
         }
    }
    else
    {
       EdtValEdHex->setText("-");
       EdtValEdDeci->setText("-");
       EdtDifAbEd->setText("-");
       EdtDifPercEd->setText("-");
    }

    if(Data->ExistFileType(ActiveComp))
    {
        ValComp = Data->GetValueForType(ActiveComp, _Pointer, *DataMode);
        if(DataMode->Mode == BITSF) EdtValCompHex->setText("-");
        else EdtValCompHex->setText(DoubleToHexS(ValComp, 1));
        EdtValCompDeci->setText(QString::number(ValComp));

        int pointerComp = _Pointer - Data->GetPosByte(SpinBoxOffsetComp->value(), *DataMode);
        EdtPointerComp->setText(DoubleToHexS(pointerComp));

        if(Data->ExistFileType(ORI))
        {
           EdtDifAbComp->setText(QString::number(ValComp - ValOri));
           if(ValOri == 0) EdtDifPercComp->setText("0");
           else EdtDifPercComp->setText(QString::number(ApproximateInt(((ValComp - ValOri)/ValOri)*100)));
        }
        else
        {
            EdtDifAbComp->setText("-");
            EdtDifPercComp->setText("-");
        }
    }
    else
    {
       EdtValCompHex->setText("-");
       EdtValCompDeci->setText("-");
       EdtDifAbComp->setText("-");
       EdtDifPercComp->setText("-");
       EdtPointerComp->setText("-");
    }
}

void FGraphicEdtTool::SetSelection(int _SelIni, int _SelNPos, int _SelMult)
{
    if(Data->IsEmply())
    {
        SpinBoxSelIni->SetValue(0);
        EdtSelFin->setText("-");
        EdtNPos->setText("-");
        EdtMultip->setText("-");
    }
    else
    {
        int _FinSel = _SelIni + Data->GetPosByte(_SelNPos, *DataMode)*_SelMult - Data->GetPosByte(1, *DataMode);

        SpinBoxSelIni->SetValue(_SelIni);
        EdtSelFin->setText(DoubleToHexS(_FinSel, 1));
        EdtNPos->setText(QString::number(_SelNPos));
        EdtMultip->setText(QString::number(_SelMult));
    }
}

void FGraphicEdtTool::SetPage(int _PageIni, int _NPoints)
{
    int xAddr = Data->GetPosByte(1, *DataMode);

    if(_NPoints < 1) _NPoints = 1;

    EdtIniPag->setText(DoubleToHexS(_PageIni, 1));
    EdtFinPag->setText(DoubleToHexS(_PageIni + (_NPoints-1)*xAddr, 1));
    EdtPtsPag->setText(QString::number(_NPoints));

    if(_NPoints < ScrollBarPtsPag->minimum()) ScrollBarPtsPag->setMinimum(_NPoints);
    if(_NPoints > ScrollBarPtsPag->maximum()) ScrollBarPtsPag->setMaximum(_NPoints);

    ScrollBarPtsPag->setValue(_NPoints);

}

void FGraphicEdtTool::on_ScrollBarPtsPag_Change(int _Pos)
{
    int _PageIni = HexToInt(EdtIniPag->text());

    SetPage(_PageIni, _Pos);

    emit PageEdited(_PageIni, _Pos);
}

void FGraphicEdtTool::on_PageIniPts_EditingFinished()
{
    int _PageIni = HexToInt(EdtIniPag->text());
    int _PtsPage = EdtPtsPag->text().toInt();

    SetPage(_PageIni, _PtsPage);

    emit PageEdited(_PageIni, _PtsPage);
}

void FGraphicEdtTool::on_PageFin_EditingFinished()
{
    int _PageIni = HexToInt(EdtIniPag->text());
    int _PageFin = HexToInt(EdtFinPag->text());

    int _PtsPage = _PageFin - _PageIni + 1;

    if(_PtsPage > 0)
    {
        SetPage(_PageIni, _PtsPage);
        emit PageEdited(_PageIni, _PtsPage);
    }
    else EdtFinPag->undo();

}

void FGraphicEdtTool::on_SpinBoxPointer_EditingFinished()
{
    if(Data->IsEmply()) return;

    int _Pointer = SpinBoxPointer->GetValue(); //Ja tem o offset pois é a posição exibida no campo.

    int _SizeFile = Data->GetSizeFile(Data->GetSerieMain()) - Data->GetPosByte(1, *DataMode);

    if(_Pointer > _SizeFile) _Pointer = _SizeFile;

    /*
        Filtra a posição, para por exemplo, não aparecer 5 no campo posição quando a visualização
        for de 16 bits sem offset. Se _IniSel ja contém o offset, ele será removido.
    */
    _Pointer = Data->GetPosByte(Data->GetPosRelative(_Pointer - ByteOffset, *DataMode), *DataMode) + ByteOffset;

    SpinBoxPointer->SetValue(_Pointer);
    //Data->SetPointer(_Pointer);
    emit PointerEdited(_Pointer);
}

void FGraphicEdtTool::on_SelectionChanged()
{
    if(Data->IsEmply()) return;

    int _IniSel = SpinBoxSelIni->GetValue(); //Ja tem o offset pois é a posição exibida no campo.
    int _NPos = EdtNPos->text().toInt();
    int _Mult = EdtMultip->text().toInt();
    int _nMode = Data->GetPosByte(1, *DataMode);

    if(_NPos < 1) _NPos = 1;
    if(_Mult < 1) _Mult = 1;

    int _SizeFile = Data->GetSizeFile(Data->GetSerieMain()) - Data->GetPosByte(1, *DataMode);

    if(_IniSel > _SizeFile) _IniSel = _SizeFile;

    /*
        Filtra a posição, para por exemplo, não aparecer 5 no campo posição quando a visualização
        for de 16 bits sem offset. Se _IniSel ja contém o offset, ele será removido.
    */
    _IniSel = Data->GetPosByte(Data->GetPosRelative(_IniSel - ByteOffset, *DataMode), *DataMode) + ByteOffset;

    /*
      Se _IniSel + _NPos ou _NPos*_Mult superar o tamanho do arquivo.
    */
    if((_IniSel + _NPos*_nMode - _nMode) > _SizeFile) _NPos = (_SizeFile - _IniSel + _nMode)/_nMode;  
    if((_IniSel + _NPos*_nMode*_Mult - _nMode) > _SizeFile) _Mult = (_SizeFile - _IniSel + _nMode)/(_NPos*_nMode);

    int _FinSel = (_IniSel + Data->GetPosByte(_NPos, *DataMode)*_Mult - Data->GetPosByte(1, *DataMode));

    SpinBoxSelIni->SetValue(_IniSel);
    EdtSelFin->setText(DoubleToHexS(_FinSel, 1));
    EdtNPos->setText(QString::number(_NPos));
    EdtMultip->setText(QString::number(_Mult));

    //Data->SetSelectionIni(_IniSel);
    //Data->SetSelectionNPos(_NPos);
    //Data->SetSelectionMult(_Mult);

    emit SelectionEdited(_IniSel, _NPos, _Mult);
}

void FGraphicEdtTool::on_BtnNPosClicked()
{
    int inc(1);
    if(sender() == BtnNPosDec) inc = -1;

    bool ok;
    int val = EdtNPos->text().toInt(&ok);

    if(ok)
    {
        val += inc;

        if(val > 0)
        {
            EdtNPos->setText(QString::number(val));
            on_SelectionChanged();
        }
    }
}

void FGraphicEdtTool::on_BtnMultipClicked()
{
    int inc(1);
    if(sender() == BtnMultipDec) inc = -1;

    bool ok;
    int val = EdtMultip->text().toInt(&ok);

    if(ok)
    {
        val += inc;

        if(val > 0)
        {
            EdtMultip->setText(QString::number(val));
            on_SelectionChanged();
        }
    }
}

void FGraphicEdtTool::on_SpinBoxOffsetValueChanged(int _value)
{
    QSpinBox *spinBox = dynamic_cast<QSpinBox*>(sender());
    QLineEdit *edtPointer(0);

    int fileId(-1);

    if(spinBox == SpinBoxOffsetOr && Data->ExistFileType(ORI))
    {
        fileId = Data->GetId(ORI);
        edtPointer = EdtPointerOr;
    }
    else if(spinBox == SpinBoxOffsetComp && Data->ExistFileType(ActiveComp))
    {
        fileId = Data->GetId(ActiveComp);
        edtPointer = EdtPointerComp;
    }

    int offset = Data->GetPosByte(_value, *DataMode);

    if(fileId >= 0 && edtPointer)
    {
        int pointerOffset = SpinBoxPointer->GetValue() - offset;
        edtPointer->setText(DoubleToHexS(pointerOffset));

        Data->SetAttribute(fileId, "FGraphicEdtTool/Offset", _value);
        Data->SetOffsetFile(fileId, offset);
        emit OffsetChanged();
    }

}

void FGraphicEdtTool::UpdateSettings()
{
    if(Data->IsEmply())
    {
        HexValidatorEnd->setRange(0, 0);
        SpinBoxPointer->SetRange(0, 0);
        SpinBoxSelIni->SetRange(0, 0);
        this->setEnabled(false);
        return;
    }

    this->setEnabled(true);

    int _Size = Data->GetSizeFile(Data->GetSerieMain()) - 1;
    int _RelSize = Data->GetPosRelative(_Size, *DataMode);
    int _Step = Data->GetPosByte(1, *DataMode);

    int oriId = Data->GetId(ORI);
    int compId = Data->GetId(ActiveComp);

    HexValidatorEnd->setRange(0, _Size);
    SpinBoxPointer->SetRange(0, _Size);
    SpinBoxSelIni->SetRange(0, _Size);

    SpinBoxPointer->SetSingleStep(_Step);
    SpinBoxSelIni->SetSingleStep(_Step);

    QPalette pal = palette();

    if(Data->ExistFileType(EDIT))
    {
        pal.setColor(QPalette::Text, Data->GetSerieColor(Data->GetId(EDIT)));
        EdtValEdHex->setPalette(pal);
        EdtValEdDeci->setPalette(pal);
        EdtDifAbEd->setPalette(pal);
        EdtDifPercEd->setPalette(pal);
    }

    if(Data->ExistFileType(ORI))
    {
        pal.setColor(QPalette::Text, Data->GetSerieColor(oriId));
        EdtValOrHex->setPalette(pal);
        EdtValOrDeci->setPalette(pal);
        EdtPointerOr->setPalette(pal);

        int _RelSizeOri = Data->GetPosRelative(Data->GetSizeFile(oriId)-1, *DataMode);
        SpinBoxOffsetOr->setRange(-_RelSizeOri, _RelSize);
        SpinBoxOffsetOr->setEnabled(true);
    }
    else
    {
        SpinBoxOffsetOr->setEnabled(false);
        SpinBoxOffsetOr->setValue(0);
    }

    if(Data->ExistFileType(ActiveComp))
    {
        pal.setColor(QPalette::Text, Data->GetSerieColor(compId));
        EdtValCompHex->setPalette(pal);
        EdtValCompDeci->setPalette(pal);
        EdtDifAbComp->setPalette(pal);
        EdtDifPercComp->setPalette(pal);
        EdtPointerComp->setPalette(pal);

        int _RelSizeComp = Data->GetPosRelative(Data->GetSizeFile(compId)-1, *DataMode);
        SpinBoxOffsetComp->setRange(-_RelSizeComp, _RelSize);
        SpinBoxOffsetComp->setEnabled(true);
    }
    else
    {
        SpinBoxOffsetComp->setEnabled(false);
        SpinBoxOffsetComp->setValue(0);
    }

    for(int fileId = 0; fileId < Data->Count(); fileId++)
    {
        int offset(0);
        if(Data->ContainsAttribute(fileId, "FGraphicEdtTool/Offset"))
        {
            offset = Data->GetAttribute(fileId, "FGraphicEdtTool/Offset").toInt();
            Data->SetOffsetFile(fileId, Data->GetPosByte(offset, *DataMode));
        }

        if(fileId == oriId) SpinBoxOffsetOr->setValue(offset);
        else if(fileId == compId) SpinBoxOffsetComp->setValue(offset);
    }

}

void FGraphicEdtTool::Update()
{
    if(!Data->IsEmply())
    {
        SetSelection(0, 1, 1);
        SetPointer(0);
    }
}

void FGraphicEdtTool::CreateForm()
{
    //this->setMaximumHeight(150);
    //this->setGeometry(0, 0, 960, 50);
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);


    // Grupo PosCur -------------------------------------------------------------------------------
    QLabel *LabPosCur = new QLabel("Pos.", this);
    LabPosCur->setMinimumSize(30, 20);

    SpinBoxPointer = new FHexSpinBox(this);
    SpinBoxPointer->setFixedSize(131, 20);

    QHBoxLayout *HLayCursor = new QHBoxLayout;
    HLayCursor->setSpacing(0);
    HLayCursor->addWidget(LabPosCur);
    HLayCursor->addWidget(SpinBoxPointer);

   // HLayCursor->setMargin(4);
    HLayCursor->setContentsMargins(9, 9, 9, 0);

   // EdtPosCur->setText(QString::number(HLayCursor->contentsMargins().top()));

    GrpPointer = new QGroupBox(tr("Pointer"),this);
    GrpPointer->setLayout(HLayCursor);
    GrpPointer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);



    // Grupo Selection ----------------------------------------------------------------------------
    QLabel *LabSelIni = new QLabel(tr("Ini."), this);
    LabSelIni->setFixedSize(30, 20);

    SpinBoxSelIni = new FHexSpinBox(this);
    SpinBoxSelIni->setFixedSize(131, 20);

    QLabel *LabSelFin = new QLabel(tr("Fin."), this);
    LabSelFin->setFixedSize(30, 20);

    EdtSelFin = new QLineEdit(this);
    EdtSelFin->setFixedSize(90, 20);
    EdtSelFin->setReadOnly(true);
    EdtSelFin->setFocusPolicy(Qt::NoFocus);

    QLabel *LabNPos = new QLabel(tr("X"), this);
    LabNPos->setFixedSize(15, 20);

    EdtNPos = new QLineEdit(this);
    EdtNPos->setFixedSize(50, 20);

    BtnNPosDec = new QPushButton("-", this);
    BtnNPosDec->setFixedSize(20, 20);
    BtnNPosDec->setAutoRepeat(true);

    BtnNPosInc = new QPushButton("+", this);
    BtnNPosInc->setFixedSize(20, 20);
    BtnNPosInc->setAutoRepeat(true);

    QLabel *LabMultip = new QLabel(tr("Y"), this);
    LabMultip->setFixedSize(15, 20);

    EdtMultip = new QLineEdit(this);
    EdtMultip->setFixedSize(50, 20);

    BtnMultipDec = new QPushButton("-", this);
    BtnMultipDec->setFixedSize(20, 20);
    BtnMultipDec->setAutoRepeat(true);

    BtnMultipInc = new QPushButton("+", this);
    BtnMultipInc->setFixedSize(20, 20);
    BtnMultipInc->setAutoRepeat(true);

    QSpacerItem *SpacerSelection = new QSpacerItem(20, 20, QSizePolicy::Expanding,  QSizePolicy::Fixed);

    QGridLayout *GridLaySelection = new QGridLayout;
    GridLaySelection->setHorizontalSpacing(1);
    GridLaySelection->addWidget(LabSelIni,      0, 0);
    GridLaySelection->addWidget(SpinBoxSelIni,  0, 1);
    GridLaySelection->addItem(SpacerSelection,  0, 2);
    GridLaySelection->addWidget(LabNPos,        0, 3);
    GridLaySelection->addWidget(EdtNPos,        0, 4);
    GridLaySelection->addWidget(BtnNPosDec,     0, 5);
    GridLaySelection->addWidget(BtnNPosInc,     0, 6);
    GridLaySelection->addWidget(LabSelFin,      1, 0);
    GridLaySelection->addWidget(EdtSelFin,      1, 1);
    GridLaySelection->addWidget(LabMultip,      1, 3);
    GridLaySelection->addWidget(EdtMultip,      1, 4);
    GridLaySelection->addWidget(BtnMultipDec,   1, 5);
    GridLaySelection->addWidget(BtnMultipInc,   1, 6);

    GridLaySelection->setContentsMargins(9, 8, 9, 0);


    GrpSelection = new QGroupBox(tr("Selection"),this);
    GrpSelection->setLayout(GridLaySelection);
    GrpSelection->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    // Grupo Values -------------------------------------------------------------------------------
    QLabel *LabValHex = new QLabel(tr("Hexadecimal"), this);
    LabValHex->setFixedSize(100, 20);

    QLabel *LabValDeci = new QLabel(tr("Decimal"), this);
    LabValDeci->setFixedSize(100, 20);

    EdtValOrHex = new QLineEdit(this);
    EdtValOrHex->setFixedSize(100, 20);
    EdtValOrHex->setReadOnly(true);
    EdtValOrHex->setFocusPolicy(Qt::NoFocus);

    EdtValOrDeci = new QLineEdit(this);
    EdtValOrDeci->setFixedSize(100, 20);
    EdtValOrDeci->setReadOnly(true);
    EdtValOrDeci->setFocusPolicy(Qt::NoFocus);

    QLabel *LabDifAb = new QLabel(tr("Absolute diff"), this);
    LabDifAb->setFixedSize(100, 20);

    QLabel *LabDifPerc = new QLabel(tr("Diff. (%)"), this);
    LabDifPerc->setFixedSize(60, 20);

    EdtValEdHex = new QLineEdit(this);
    EdtValEdHex->setFixedSize(100, 20);

    EdtValEdDeci = new QLineEdit(this);
    EdtValEdDeci->setFixedSize(100, 20);

    EdtDifAbEd = new QLineEdit(this);
    EdtDifAbEd->setFixedSize(100, 20);

    EdtDifPercEd = new QLineEdit(this);
    EdtDifPercEd->setFixedSize(60, 20);

    EdtValCompHex = new QLineEdit(this);
    EdtValCompHex->setFixedSize(100, 20);
    EdtValCompHex->setReadOnly(true);
    EdtValCompHex->setFocusPolicy(Qt::NoFocus);

    EdtValCompDeci = new QLineEdit(this);
    EdtValCompDeci->setFixedSize(100, 20);
    EdtValCompDeci->setReadOnly(true);
    EdtValCompDeci->setFocusPolicy(Qt::NoFocus);

    EdtDifAbComp = new QLineEdit(this);
    EdtDifAbComp->setFixedSize(100, 20);
    EdtDifAbComp->setReadOnly(true);
    EdtDifAbComp->setFocusPolicy(Qt::NoFocus);

    EdtDifPercComp = new QLineEdit(this);
    EdtDifPercComp->setFixedSize(60, 20);
    EdtDifPercComp->setReadOnly(true);
    EdtDifPercComp->setFocusPolicy(Qt::NoFocus);

    QGridLayout *GridLayValues = new QGridLayout;
    GridLayValues->setHorizontalSpacing(1);
    GridLayValues->addWidget(LabValHex,         0, 0);
    GridLayValues->addWidget(LabValDeci,        0, 1);
    GridLayValues->addWidget(EdtValOrHex,       1, 0);
    GridLayValues->addWidget(EdtValOrDeci,      1, 1);
    GridLayValues->addWidget(LabDifAb,          1, 2);
    GridLayValues->addWidget(LabDifPerc,        1, 3);
    GridLayValues->addWidget(EdtValEdHex,       2, 0);
    GridLayValues->addWidget(EdtValEdDeci,      2, 1);
    GridLayValues->addWidget(EdtDifAbEd,        2, 2);
    GridLayValues->addWidget(EdtDifPercEd,      2, 3);
    GridLayValues->addWidget(EdtValCompHex,     3, 0);
    GridLayValues->addWidget(EdtValCompDeci,    3, 1);
    GridLayValues->addWidget(EdtDifAbComp,      3, 2);
    GridLayValues->addWidget(EdtDifPercComp,    3, 3);

    GridLayValues->setContentsMargins(9, 9, 9, 0);

    GrpValues = new QGroupBox(tr("Values"),this);
    GrpValues->setLayout(GridLayValues);
    GrpValues->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    // TabPágina-----------------------------------------------------------------------------------
    QLabel *LabIniPag = new QLabel(tr("Start"), this);
    LabIniPag->setFixedSize(90, 20);

    EdtIniPag = new QLineEdit(this);
    EdtIniPag->setFixedSize(90, 20);

    QLabel *LabFinPag = new QLabel(tr("End"), this);
    LabFinPag->setFixedSize(90, 20);

    EdtFinPag = new QLineEdit(this);
    EdtFinPag->setFixedSize(90, 20);

    QLabel *LabPtsPag = new QLabel(tr("Points"), this);
    LabPtsPag->setFixedSize(50, 20);

    EdtPtsPag = new QLineEdit(this);
    EdtPtsPag->setFixedSize(50, 20);

    ScrollBarPtsPag = new QScrollBar(Qt::Horizontal, this);
    ScrollBarPtsPag->setMinimum(2);
    ScrollBarPtsPag->setMaximum(400);

    QSpacerItem *SpacerPage = new QSpacerItem(20, 20, QSizePolicy::Expanding,  QSizePolicy::Fixed);

    QGridLayout *GridLayPage = new QGridLayout;
    GridLayPage->setHorizontalSpacing(1);
    GridLayPage->addWidget(LabIniPag, 0, 0);
    GridLayPage->addWidget(LabFinPag, 0, 1);
    GridLayPage->addWidget(LabPtsPag, 0, 2);
    GridLayPage->addWidget(EdtIniPag, 1, 0);
    GridLayPage->addWidget(EdtFinPag, 1, 1);
    GridLayPage->addWidget(EdtPtsPag, 1, 2);
    GridLayPage->addItem(SpacerPage,  0, 3);

    QVBoxLayout *VLayPage = new QVBoxLayout;
    VLayPage->addLayout(GridLayPage);
    VLayPage->addWidget(ScrollBarPtsPag);

    WidTabPage = new QWidget(this);
    WidTabPage->setLayout(VLayPage);
    //WidTabPage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    // TabOffset-----------------------------------------------------------------------------------
    QLabel *LabOffsetPointer = new QLabel(tr("Pointer"), this);
    LabOffsetPointer->setFixedSize(90, 20);

    QLabel *LabOffsetOffset = new QLabel(tr("Offset"), this);
    LabOffsetOffset->setFixedSize(70, 20);

    QLabel *LabOffsetOr = new QLabel(tr("Original"), this);
    LabOffsetOr->setFixedSize(60, 20);

    EdtPointerOr = new QLineEdit(this);
    EdtPointerOr->setFixedSize(90, 20);
    EdtPointerOr->setReadOnly(true);
    EdtPointerOr->setFocusPolicy(Qt::NoFocus);

    SpinBoxOffsetOr = new QSpinBox(this);
    SpinBoxOffsetOr->setFixedSize(70, 20);  

    QLabel *LabOffsetComp = new QLabel(tr("Comparison"), this);
    LabOffsetComp->setFixedSize(60, 20);

    EdtPointerComp = new QLineEdit(this);
    EdtPointerComp->setFixedSize(90, 20);
    EdtPointerComp->setReadOnly(true);
    EdtPointerComp->setFocusPolicy(Qt::NoFocus);

    SpinBoxOffsetComp = new QSpinBox(this);
    SpinBoxOffsetComp->setFixedSize(70, 20);

    QSpacerItem *SpacerOffset = new QSpacerItem(20, 20, QSizePolicy::Expanding,  QSizePolicy::Fixed);

    QGridLayout *GridLayOffset = new QGridLayout;
    GridLayOffset->setHorizontalSpacing(1);
    GridLayOffset->addWidget(LabOffsetPointer,  0, 1);
    GridLayOffset->addWidget(LabOffsetOffset,   0, 2);
    GridLayOffset->addWidget(LabOffsetOr,       1, 0);
    GridLayOffset->addWidget(EdtPointerOr,      1, 1);
    GridLayOffset->addWidget(SpinBoxOffsetOr,   1, 2);
    GridLayOffset->addWidget(LabOffsetComp,     2, 0);
    GridLayOffset->addWidget(EdtPointerComp,    2, 1);
    GridLayOffset->addWidget(SpinBoxOffsetComp, 2, 2);
    GridLayOffset->addItem(SpacerOffset,        1, 3);

    WidTabOffset = new QWidget(this);
    WidTabOffset->setLayout(GridLayOffset);


    // Montagem------------------------------------------------------------------------------------
    TabWidMisc = new QTabWidget(this);
    TabWidMisc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    TabWidMisc->addTab(WidTabPage, tr("Page"));
    TabWidMisc->addTab(WidTabOffset, tr("Offset"));

    QVBoxLayout *HLayLeftForm = new QVBoxLayout;
    HLayLeftForm->addWidget(GrpPointer);
    HLayLeftForm->addWidget(GrpSelection);

    QHBoxLayout *HLayMain = new QHBoxLayout;
    HLayMain->addLayout(HLayLeftForm);
    HLayMain->addWidget(GrpValues);
    HLayMain->addWidget(TabWidMisc);
    HLayMain->setContentsMargins(0, 0, 0, 0);

    this->setLayout(HLayMain);
    //this->setFixedSize();


/*
    QVBoxLayout *vertLayout = new QVBoxLayout;
    //vertLayout->setContentsMargins(0,0,0,0);
    vertLayout->addWidget(&EdtPosCur);
    //vertLayout->addWidget(FrmEditChart2d);
    this->setLayout(vertLayout);
 */
}

void FGraphicEdtTool::CreateConnections()
{
    connect(SpinBoxPointer, SIGNAL(EditingFinished()), this, SLOT(on_SpinBoxPointer_EditingFinished()));

    connect(SpinBoxSelIni, SIGNAL(EditingFinished()), this, SLOT(on_SelectionChanged()));
    connect(EdtNPos, SIGNAL(editingFinished()), this, SLOT(on_SelectionChanged()));
    connect(EdtMultip, SIGNAL(editingFinished()), this, SLOT(on_SelectionChanged()));

    connect(ScrollBarPtsPag, SIGNAL(valueChanged(int)), this, SLOT(on_ScrollBarPtsPag_Change(int)));

    connect(EdtIniPag, SIGNAL(editingFinished()), this, SLOT(on_PageIniPts_EditingFinished()));
    connect(EdtPtsPag, SIGNAL(editingFinished()), this, SLOT(on_PageIniPts_EditingFinished()));
    connect(EdtFinPag, SIGNAL(editingFinished()), this, SLOT(on_PageFin_EditingFinished()));

    connect(BtnNPosDec, SIGNAL(clicked()), this, SLOT(on_BtnNPosClicked()));
    connect(BtnNPosInc, SIGNAL(clicked()), this, SLOT(on_BtnNPosClicked()));
    connect(BtnMultipDec, SIGNAL(clicked()), this, SLOT(on_BtnMultipClicked()));
    connect(BtnMultipInc, SIGNAL(clicked()), this, SLOT(on_BtnMultipClicked()));

    connect(SpinBoxOffsetOr, SIGNAL(valueChanged(int)), this, SLOT(on_SpinBoxOffsetValueChanged(int)));
    connect(SpinBoxOffsetComp, SIGNAL(valueChanged(int)), this, SLOT(on_SpinBoxOffsetValueChanged(int)));

}
