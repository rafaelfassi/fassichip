#ifndef FCHART3D_H
#define FCHART3D_H

#include <QtWidgets>
#include "fdatamap3d.h"
#include "qwt3d_surfaceplot.h"
#include "qwt3d_parametricsurface.h"
#include "qwt3d_enrichment.h"

using namespace Qwt3D;

class FSurface3d : public ParametricSurface
{
public:

  FSurface3d(SurfacePlot& pw);

  Triple operator()(double u, double v);
  void SetData(FDataMap3d *_DataMap3d) { DataMap3d = _DataMap3d; }

private:
  FDataMap3d *DataMap3d;
};


class FDrawPointer : public VertexEnrichment
{
public:
  FDrawPointer(Triple *_Pointer);
  Enrichment* clone() const {return new FDrawPointer(*this);}
  void draw(Qwt3D::Triple const&);
  Triple *Pointer;
};


class FChart3d : public SurfacePlot
{
    Q_OBJECT

public:
    FChart3d();

    void SetData(FDataMap3d *_DataMap3d);
    void Update();
    QColor GetColorData(double z);
    void SetPointer(double x, double y);

private slots:
    void on_SetPlotStyle();
    void on_SetCoordStyle();
    void on_SetFloorStyle();

private:
    void CreateContextMenu();
    double GetMaxZ();
    FSurface3d *Surface3d;
    FDrawPointer *DrawPointer;
    FDataMap3d *DataMap3d;
    Triple Pointer;
    QAction *ActCoordStyleNoCoord;
    QAction *ActCoordStyleBox;
    QAction *ActCoordStyleFrame;
    QActionGroup *ActGroupCoordStyle;

    QAction *ActPlotStyleNoPlot;
    QAction *ActPlotStyleFilled;
    QAction *ActPlotStyleFilledMesh;
    QAction *ActPlotStyleWireFrame;
    QAction *ActPlotStyleHiddenLine;
    QAction *ActPlotStylePoints;
    QActionGroup *ActGroupPlotStyle;

    QAction *ActFloorStyleNoFloor;
    QAction *ActFloorStyleFloorData;
    QAction *ActFloorStyleFloorIso;
    QActionGroup *ActGroupStyleFloor;

};

#endif // FCHART3D_H
