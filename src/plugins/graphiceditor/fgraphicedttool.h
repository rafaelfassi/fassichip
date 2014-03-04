#ifndef FGRAPHICEDTTOOL_H
#define FGRAPHICEDTTOOL_H

#include <QtGui>
#include "fdata.h"
#include "misc.h"

class FGraphicEdtTool : public QWidget
{
    Q_OBJECT

public:
    FGraphicEdtTool(QWidget *parent = 0);
    ~FGraphicEdtTool();
    void SetData(FData *_Data) { Data = _Data; }
    void SetDataMode(FDataMode *_DataMode) { DataMode = _DataMode; }
    void UpdateSettings();
    void Update();
    void SetByteOffset(unsigned int _ByteOffset) { ByteOffset = _ByteOffset; }
    unsigned int GetByteOffset() { return ByteOffset; }
    void SetActiveComp(int _ActiveComp) { ActiveComp = _ActiveComp; }
    int GetActiveComp() { return ActiveComp; }

public slots:
    void SetPointer(int _Pointer);
    void SetSelection(int _SelIni, int _SelNPos, int _SelMult);
    void SetPage(int _PageIni, int _NPoints);
    void on_SpinBoxPointer_EditingFinished();
    void on_SelectionChanged();
    void on_BtnNPosClicked();
    void on_BtnMultipClicked();
    void on_SpinBoxOffsetValueChanged(int _value);

private slots:
    void on_ScrollBarPtsPag_Change(int);
    void on_PageIniPts_EditingFinished();
    void on_PageFin_EditingFinished();

signals:
    void PointerEdited(int _Pointer);
    void SelectionEdited(int _SelIni, int _SelNPos, int _SelMult);
    void PageEdited(int _PageIni, int _NPoints);
    void OffsetChanged();

private:
    void CreateForm();
    void CreateConnections();
    unsigned int ByteOffset;
    int ActiveComp;      // Comparação ativa
    FHexSpinBox *SpinBoxPointer;
    FHexSpinBox *SpinBoxSelIni;
    QLineEdit *EdtSelFin;
    QLineEdit *EdtNPos;
    QLineEdit *EdtMultip;
    QLineEdit *EdtValOrHex;
    QLineEdit *EdtValEdHex;
    QLineEdit *EdtValCompHex;
    QLineEdit *EdtValOrDeci;
    QLineEdit *EdtValEdDeci;
    QLineEdit *EdtValCompDeci;
    QLineEdit *EdtDifAbEd;
    QLineEdit *EdtDifAbComp;
    QLineEdit *EdtDifPercEd;
    QLineEdit *EdtDifPercComp;
    QLineEdit *EdtIniPag;
    QLineEdit *EdtFinPag;
    QLineEdit *EdtPtsPag;
    QScrollBar *ScrollBarPtsPag;
    QLineEdit *EdtPointerOr;
    QSpinBox *SpinBoxOffsetOr;
    QLineEdit *EdtPointerComp;
    QSpinBox *SpinBoxOffsetComp;

    QPushButton *BtnSelIniInc;
    QPushButton *BtnNPosDec;
    QPushButton *BtnNPosInc;
    QPushButton *BtnMultipDec;
    QPushButton *BtnMultipInc;

    QGroupBox *GrpPointer;
    QGroupBox *GrpSelection;
    QGroupBox *GrpValues;

    QWidget *WidTabPage;
    QWidget *WidTabOffset;

    QTabWidget *TabWidMisc;

    FData *Data;
    FDataMode *DataMode;
    FHexValidator *HexValidatorEnd;






};

#endif // FGRAPHICEDTTOOL_H
