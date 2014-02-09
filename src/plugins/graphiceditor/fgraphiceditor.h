#ifndef FGRAPHICEDITOR_H
#define FGRAPHICEDITOR_H

#include <QtGui>

#include "fwindowtab.h"
#include "fdatapackage.h"
#include "fchart2d.h"
#include "fchart3d.h"
#include "fgraphicedttool.h"

class FGraphicEditor : public FWindowTab
{
    Q_OBJECT

public:
    FGraphicEditor(QWidget *parent = 0);

    void SetData(FDataPackage *_DataPackage);
    FDataPackage* GetData() { return DataPackage; }
    void RefreshData();
    void UpdateSettings();
    void DefineByteOffsetByAddress(int _Address);
    FGraphicEdtTool *GraphicEdtTool;
    FChart2d *Chart2d;
    FChart3d *Chart3d;

public slots:
    void on_CursorMode_Triggered();
    void on_Chart2d_CursorModeChange(int _CursorMode);
    void on_Chart2d_PointerChange(int _Pointer);
    void on_Chart2d_SelectionChange(int _SelIni, int _SelNPos, int _SelMult);
    void on_GraphicEdtTool_SelectionEdited(int _SelIni, int _SelNPos, int _SelMult);
    void on_GraphicEdtTool_PointerEdited(int _Pointer);
    void on_SpinBoxByteOffset_Change(int _Value);
    void on_ViewChart3D_Triggered();
    void on_ChangeModeBits();
    void on_ChangeActiveComp();

private:
    void CreateActions();
    void CreateToolBar();
    void CreateConnections();
    void SetByteOffset(unsigned int _ByteOffset);
    void DefinePointerChart3d();
    void EnableComparisons();
    FDataPackage *DataPackage;
    FData *Data;
    FDataMode DataMode;
    FDataMap3d DataMap3d;
    QAction *ActCursorSel;
    QAction *ActCursorHand;
    QAction *ActCursorZoomArea;
    QAction *ActCursorZoomReset;
    QAction *ActViewChart3D;
    QActionGroup *ActGroupCursor;
    QAction *ActModeBits8;
    QAction *ActModeBits16;
    QAction *ActModeBits32;
    QAction *ActModeBitsF;
    QActionGroup *ActGroupModeBits;
    QAction *ActModeBitsSignal;
    QAction *ActModeBitsSwap;
    QAction *ActComp1;
    QAction *ActComp2;
    QAction *ActComp3;
    QActionGroup *ActGroupComp;

    QMenuBar *MenuBar;
    QMenu *MenuView;


    QSpinBox *SpinBoxByteOffset;

    QToolBar *ToolBar;

    QSplitter *Splitter;

};

#endif // FGRAPHICEDITOR_H
