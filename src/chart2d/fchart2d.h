#ifndef FCHART2D_H
#define FCHART2D_H

#include <QPixmap>
#include <QWidget>
#include "fdata.h"

enum {CURSOR_DEFAULT, CURSOR_HAND, CURSOR_ZOOM, CURSOR_RESET_ZOOM};

class QScrollBar;
//class PlotSettings;

class FChart2d : public QWidget
{
    Q_OBJECT

public:
    FChart2d(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void SetXMinMax(int _Min, int _Max) { _SetXMinMax(AddressImp(_Min), AddressImp(_Max)); }
    void SetYMinMax(double _Min, double _Max);
    void SetExtremesY(double _Min, double _Max);
    void SetDefaultPageX(int _Points);
    void SetMinX(int _Min) { _SetMinX(AddressImp(_Min)); }
    void SetMaxX(int _Max) { _SetMaxX(AddressImp(_Max)); }
    void SetMinY(double _Min);
    void SetMaxY(double _Max);
    void SetByteOffset(unsigned int _ByteOffset) { ByteOffset = _ByteOffset; }
    unsigned int GetByteOffset() { return ByteOffset; }
    void SetLockSelectionOnPage(bool _LockSelectionOnPage) { LockSelectionOnPage = _LockSelectionOnPage; }
    bool GetLockSelectionOnPage() { return LockSelectionOnPage; }
    void SetCursorMode(int _Cursor);
    //void SetSelectionMult(int _SelMult) { SelectionMult = _SelMult; }
    void RefreshScreen();
    void UpdateSettings();
    int GetPointer() { return AddressExp(PosPointer); }
    int GetSelectionIni() { return AddressExp(SelectionIni); }
    int GetSelectionNPos() { return SelectionNPos; }
    int GetSelectionMult() { return SelectionMult; }
    int GetMinX() { return AddressExp(MinX); }
    int GetMaxX() { return AddressExp(MaxX); }
    void EmitSignals();
    void ResetZoom();
    void SetBackgroundColor(QColor _Color);
    void SetScaleColor(QColor _Color) { ScaleColor = _Color; }
    void SetSelectionColor(QColor _Color) {SelectionColor = _Color; }
    void SetPointerColor(QColor _Color) { PointerColor = _Color; }
    void SetData(FData *_Data) { Data = _Data; }
    void SetDataMode(FDataMode *_DataMode) { DataMode = _DataMode; }
    void SetShowScaleY(bool _IsShow) { ShowScaleY = _IsShow; }
    void SetShowScaleX(bool _IsShow) { ShowScaleX = _IsShow; }
    void SetEnableRefresh(bool _Enable) { EnableRefresh = _Enable; }
    void SetAutoSizeMargLeft(bool _Enable) { AutoSizeMargLeft = _Enable; }
    void SelectionOnPage();
    void GotoPointer();
    double SpanX() const { return MaxX - MinX; }
    double SpanY() const { return MaxY - MinY; }
    int MarginHoriz() const{ return MarginLeft + MarginRight; }
    int MarginVert() const { return MarginTop + MarginButton; }

public slots:
    void SetPointer(int _Pointer);
    void SetSelection(int _SelIni, int _SelNPos, int _SelMult);
    void SetPage(int _PageIni, int _NPoints);

private slots:
    void on_HorScrollBar_Change(int);

signals:
    void PointerChange(int _Pointer);
    void SelectionChange(int _SelIni, int _SelNPos, int _SelMult);
    void CursorModeChange(int _CursorMode);
    void PageChange(int _PageIni, int _NPoints);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void DrawScaleY(QPainter *_Painter);
    void DrawScaleX(QPainter *_Painter, int *_PointsX);
    void DrawPoints(QPainter *_Painter, int *_PointsX, int *_PointsY, int _MaxY, int _Id);
    void DrawSelection(QPainter *_Painter);
    void DrawCursor(QPainter *_Painter);
    void Scroll(int dx, int dy);
    int ScreenToPosX(int _ScreenX);
    int PosToScreenX(int _PosX);
    double ScreenToValY(int _ScreenY);
    int ValToScreenY(double _ValY);
    void _SetMinX(int _Min);
    void _SetMaxX(int _Max);
    void _SetXMinMax(int _Min, int _Max);
    int AddressImp(int _AddressByte) { return Data->GetPosRelative(_AddressByte, *DataMode); }
    int AddressExp(int _AddressRelative) { return Data->GetPosByte(_AddressRelative, *DataMode) + ByteOffset; }

    QScrollBar *HorScrollBar;
    FData *Data;
    FDataMode *DataMode;
    unsigned int ByteOffset;
    int MinX;
    int MaxX;
    int NumXTicks;
    double MinY;
    double MaxY;
    int NumYTicks;
    double ExtremeMinY;
    double ExtremeMaxY;
    int CursorMod;
    //int Data->Pointer;
    //int SelectionIni;   // Endereço inicial da seleção
    int SelectionStart; // Posição inicial que se clica para fazer uma seleção
   // int SelectionNPos;
   // int SelectionMult;

    int PosPointer;      // Posição do ponteiro no arquivo
    int SelectionIni;    // Posição inicial da seleção
    int SelectionNPos;   // Número de posições selecionadas
    int SelectionMult;   // Multiplicativo da seleção
    int PageIni;         // Início da página

    int MarginTop;
    int MarginButton;
    int MarginLeft;
    int MarginRight;
    int MinimumMarginH;
    int HandStartPosX;
    double HandStartValY;
    int FinPosX;
    int DefaultPageX;
    int SuggestionPageX;
    bool HandIsActive;
    bool ZoomAreaIsShown;
    bool ShowScaleY;
    bool ShowScaleX;
    bool EnableRefresh;
    bool AutoSizeMargLeft;
    bool ScaleYIsHex;
    bool ScaleXIsHex;
    bool LockSelectionOnPage;
    QPalette BackgroundColor;
    QColor ScaleColor;
    QColor SelectionColor;
    QColor PointerColor;
    QRect ZoomAreaRect;
    QPixmap Pixmap;
};

/*
        QMessageBox msgBox;
        msgBox.setText(QString::number(spanX()));
        msgBox.exec();
*/

#endif
