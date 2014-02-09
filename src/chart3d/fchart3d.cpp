#include "fchart3d.h"
#include <QMessageBox>

// FMap3d--------------------------------------------------------------------------------------------------------------
FSurface3d::FSurface3d(SurfacePlot& pw)
:ParametricSurface(pw)
{
    DataMap3d = 0;
}

Triple FSurface3d::operator()(double u, double v)
{
    if(DataMap3d->AddressZ.IsEmpty()) return Triple(0, 0, 0);

    int Pos;
    double z;

    //Pos = DataMap3d->Data->GetSelectionIni() + int(u) + int(v)*DataMap3d->Data->GetSelectionNPos();
    //z = DataMap3d->Data->GetData(DataMap3d->Data->GetId(1), Pos, &DataMode);
    //return Triple(int(u*10), int(v), z);

    Pos = DataMap3d->AddressZ.GetAddressZ(u, v);
    z = DataMap3d->Data->GetValue(DataMap3d->Data->GetSerieMain(), Pos, DataMap3d->ModeDataValZ);

     return Triple(DataMap3d->ScaleX[u], DataMap3d->ScaleY[v], z);
}

// FDrawPointer--------------------------------------------------------------------------------------------------------

FDrawPointer::FDrawPointer(Triple *_Pointer)
{
    Pointer = _Pointer;
}

void FDrawPointer::draw(Qwt3D::Triple const& pos)
{
    if(IsNan(Pointer->x) || IsNan(Pointer->y)) return;

    if(pos.x == Pointer->x && pos.y == Pointer->y)
    {

        double PtsSize = (((plot->hull().maxVertex - plot->hull().minVertex).length())/plot->hull().maxVertex.length())*10;

        //É preciso salvar o status de tamanho anterior pra não dar erro na projeção de curvas (Isoline projections).
        GLfloat _SizePrevious;
        glGetFloatv(GL_POINT_SIZE, &_SizePrevious);

        glPointSize(PtsSize);
        glEnable(GL_POINT_SMOOTH);
        glColor3d(1, 1, 0); // Yellow

        glBegin(GL_POINTS);
        glVertex3d(pos.x,pos.y,pos.z);
        glEnd();

        //Retomando o status anterior as mudanças.
        glDisable(GL_POINT_SMOOTH);
        glPointSize(_SizePrevious);

    }
}

// FChart3d------------------------------------------------------------------------------------------------------------
FChart3d::FChart3d()
{
    DataMap3d = 0;
    Pointer = Triple(0,0,0);

    Surface3d = new FSurface3d(*this);
    DrawPointer = new FDrawPointer(&Pointer);

    addEnrichment(*DrawPointer);

    CreateContextMenu();

    setCoordinateStyle(BOX);
}

void FChart3d::SetData(FDataMap3d *_DataMap3d)
{
    DataMap3d = _DataMap3d;
    Surface3d->SetData(_DataMap3d);
}

void FChart3d::Update()
{
    double MaxX = DataMap3d->AddressZ.GetSizeX();
    double MaxY = DataMap3d->AddressZ.GetSizeY();

    if(DataMap3d->Data->IsEmply() || DataMap3d->AddressZ.IsEmpty())
    {
        Surface3d->setMesh(3, 3);
        Surface3d->setDomain(0, 1, 0 , 1);
        Surface3d->create();
        updateData();
        updateGL();
        return;
    }

    double MaxZ = GetMaxZ();

    setTitle("A Simple SurfacePlot Demonstration");

    setRotation(30,0,15);
    setScale(MaxZ / MaxX, MaxZ / MaxY, 1);
    setShift(0,0,0); // Deslocamento
    setZoom(0.5);

    Surface3d->setMesh(MaxX+1, MaxY+1);
    Surface3d->setDomain(0, MaxX-1, 0 ,MaxY-1);
    Surface3d->create();


    for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
    {
      coordinates()->axes[i].setMajors(4);
      coordinates()->axes[i].setMinors(2);
    }

    coordinates()->setAutoScale(true);


    double Tic = (coordinates()->second() - coordinates()->first()).length() / MaxZ;
            Tic = Tic * 0.5;
            coordinates()->setTicLength(Tic,0.6*Tic);


    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
    //coordinates()->axes[Z1].setLabelString(QChar(0x38f)); // Omega - see http://www.unicode.org/charts/


    //setCoordinateStyle(BOX);

    updateData();
    updateGL();
}

double FChart3d::GetMaxZ()
{
    int AddrIni = DataMap3d->AddressZ.GetAddressZ(0, 0);
    int MaxX = AddrIni + DataMap3d->AddressZ.GetSizeX() * DataMap3d->AddressZ.GetSizeY();
    double MaxZ = 0;
    double Val;

    for(int x = AddrIni; x <= MaxX; x++)
    {        
        Val = DataMap3d->Data->GetValue(DataMap3d->Data->GetSerieMain(), x, DataMap3d->ModeDataValZ);
        if(Val > MaxZ) MaxZ = Val;
    }

    return MaxZ;
}

QColor FChart3d::GetColorData(double z)
{

    RGBA rgba = (*dataColor())(Triple(0, 0, z));
    return QColor::fromRgbF(rgba.r, rgba.g, rgba.b, rgba.a);
}

void FChart3d::SetPointer(double x, double y)
{
    Pointer.x = x;
    Pointer.y = y;

    updateData();
    updateGL();

    /*QMessageBox msgBox;
    msgBox.setText(QString::number(x) + " - " + QString::number(y) + " - " + QString::number(z));
    msgBox.exec();*/
}

void FChart3d::on_SetCoordStyle()
{
    QAction *clickedStyle = qobject_cast<QAction *>(sender());
    int _Style = clickedStyle->data().toInt();

    this->setCoordinateStyle(Qwt3D::COORDSTYLE(_Style));
}

void FChart3d::on_SetPlotStyle()
{
    QAction *clickedStyle = qobject_cast<QAction *>(sender());
    int _Style = clickedStyle->data().toInt();

    this->setPlotStyle(Qwt3D::PLOTSTYLE(_Style));

    updateData();
    updateGL();
}

void FChart3d::on_SetFloorStyle()
{
    QAction *clickedStyle = qobject_cast<QAction *>(sender());
    int _Style = clickedStyle->data().toInt();

    this->setFloorStyle(Qwt3D::FLOORSTYLE(_Style));

    updateData();
    updateGL();
}

void FChart3d::CreateContextMenu()
{

    // CoordinateStyle-----------------------------------------------------------------------------
    ActCoordStyleNoCoord = new QAction(QIcon(":/chart3d/images/NoCoord.png"), tr("No Coord"), this);
    ActCoordStyleNoCoord->setData(Qwt3D::NOCOORD);
    ActCoordStyleNoCoord->setIconVisibleInMenu(true);
    ActCoordStyleNoCoord->setCheckable(true);
    connect(ActCoordStyleNoCoord, SIGNAL(triggered()), this,  SLOT(on_SetCoordStyle()));

    ActCoordStyleBox = new QAction(QIcon(":/chart3d/images/Box.png"), tr("Box"), this);
    ActCoordStyleBox->setData(Qwt3D::BOX);
    ActCoordStyleBox->setIconVisibleInMenu(true);
    ActCoordStyleBox->setCheckable(true);
    ActCoordStyleBox->setChecked(true);
    connect(ActCoordStyleBox, SIGNAL(triggered()), this,  SLOT(on_SetCoordStyle()));

    ActCoordStyleFrame = new QAction(QIcon(":/chart3d/images/Frame.png"), tr("Frame"), this);
    ActCoordStyleFrame->setData(Qwt3D::FRAME);
    ActCoordStyleFrame->setIconVisibleInMenu(true);
    ActCoordStyleFrame->setCheckable(true);
    connect(ActCoordStyleFrame, SIGNAL(triggered()), this,  SLOT(on_SetCoordStyle()));

    ActGroupCoordStyle = new QActionGroup(this);
    ActGroupCoordStyle->addAction(ActCoordStyleNoCoord);
    ActGroupCoordStyle->addAction(ActCoordStyleBox);
    ActGroupCoordStyle->addAction(ActCoordStyleFrame);
    this->addActions(ActGroupCoordStyle->actions());


    QAction *SeparatorAction1 = new QAction(this);
    SeparatorAction1->setSeparator(true);
    this->addAction(SeparatorAction1);


    // PlotStyle-----------------------------------------------------------------------------------
    ActPlotStyleNoPlot = new QAction(QIcon(":/chart3d/images/NoPlot.png"), tr("No Plot"), this);
    ActPlotStyleNoPlot->setData(Qwt3D::NOPLOT);
    ActPlotStyleNoPlot->setIconVisibleInMenu(true);
    ActPlotStyleNoPlot->setCheckable(true);
    connect(ActPlotStyleNoPlot, SIGNAL(triggered()), this,  SLOT(on_SetPlotStyle()));

    ActPlotStyleFilled = new QAction(QIcon(":/chart3d/images/Filled.png"), tr("Filled"), this);
    ActPlotStyleFilled->setData(Qwt3D::FILLED);
    ActPlotStyleFilled->setIconVisibleInMenu(true);
    ActPlotStyleFilled->setCheckable(true);
    connect(ActPlotStyleFilled, SIGNAL(triggered()), this,  SLOT(on_SetPlotStyle()));

    ActPlotStyleFilledMesh = new QAction(QIcon(":/chart3d/images/FilledMesh.png"), tr("Filled Mesh"), this);
    ActPlotStyleFilledMesh->setData(Qwt3D::FILLEDMESH);
    ActPlotStyleFilledMesh->setIconVisibleInMenu(true);
    ActPlotStyleFilledMesh->setCheckable(true);
    ActPlotStyleFilledMesh->setChecked(true);
    connect(ActPlotStyleFilledMesh, SIGNAL(triggered()), this,  SLOT(on_SetPlotStyle()));

    ActPlotStyleWireFrame = new QAction(QIcon(":/chart3d/images/WireFrame.png"), tr("Wire Frame"), this);
    ActPlotStyleWireFrame->setData(Qwt3D::WIREFRAME);
    ActPlotStyleWireFrame->setIconVisibleInMenu(true);
    ActPlotStyleWireFrame->setCheckable(true);
    connect(ActPlotStyleWireFrame, SIGNAL(triggered()), this,  SLOT(on_SetPlotStyle()));

    ActPlotStyleHiddenLine = new QAction(QIcon(":/chart3d/images/HiddenLine.png"), tr("Hidden Line"), this);
    ActPlotStyleHiddenLine->setData(Qwt3D::HIDDENLINE);
    ActPlotStyleHiddenLine->setIconVisibleInMenu(true);
    ActPlotStyleHiddenLine->setCheckable(true);
    connect(ActPlotStyleHiddenLine, SIGNAL(triggered()), this,  SLOT(on_SetPlotStyle()));

    ActPlotStylePoints = new QAction(QIcon(":/chart3d/images/Points.png"), tr("Points"), this);
    ActPlotStylePoints->setData(Qwt3D::POINTS);
    ActPlotStylePoints->setIconVisibleInMenu(true);
    ActPlotStylePoints->setCheckable(true);
    connect(ActPlotStylePoints, SIGNAL(triggered()), this,  SLOT(on_SetPlotStyle()));

    ActGroupPlotStyle = new QActionGroup(this);
    ActGroupPlotStyle->addAction(ActPlotStyleNoPlot);
    ActGroupPlotStyle->addAction(ActPlotStyleFilled);
    ActGroupPlotStyle->addAction(ActPlotStyleFilledMesh);
    ActGroupPlotStyle->addAction(ActPlotStyleWireFrame);
    ActGroupPlotStyle->addAction(ActPlotStyleHiddenLine);
    ActGroupPlotStyle->addAction(ActPlotStylePoints);
    this->addActions(ActGroupPlotStyle->actions());


    QAction *SeparatorAction2 = new QAction(this);
    SeparatorAction2->setSeparator(true);
    this->addAction(SeparatorAction2);


    // FloorStyle----------------------------------------------------------------------------------
    ActFloorStyleNoFloor = new QAction(QIcon(":/chart3d/images/NoFloor.png"), tr("No Floor"), this);
    ActFloorStyleNoFloor->setData(Qwt3D::NOFLOOR);
    ActFloorStyleNoFloor->setIconVisibleInMenu(true);
    ActFloorStyleNoFloor->setCheckable(true);
    ActFloorStyleNoFloor->setChecked(true);
    connect(ActFloorStyleNoFloor, SIGNAL(triggered()), this,  SLOT(on_SetFloorStyle()));

    ActFloorStyleFloorData = new QAction(QIcon(":/chart3d/images/FloorData.png"), tr("Floor Data"), this);
    ActFloorStyleFloorData->setData(Qwt3D::FLOORDATA);
    ActFloorStyleFloorData->setIconVisibleInMenu(true);
    ActFloorStyleFloorData->setCheckable(true);
    connect(ActFloorStyleFloorData, SIGNAL(triggered()), this,  SLOT(on_SetFloorStyle()));

    ActFloorStyleFloorIso = new QAction(QIcon(":/chart3d/images/FloorIso.png"), tr("Floor Iso"), this);
    ActFloorStyleFloorIso->setData(Qwt3D::FLOORISO);
    ActFloorStyleFloorIso->setIconVisibleInMenu(true);
    ActFloorStyleFloorIso->setCheckable(true);
    connect(ActFloorStyleFloorIso, SIGNAL(triggered()), this,  SLOT(on_SetFloorStyle()));

    ActGroupStyleFloor = new QActionGroup(this);
    ActGroupStyleFloor->addAction(ActFloorStyleNoFloor);
    ActGroupStyleFloor->addAction(ActFloorStyleFloorData);
    ActGroupStyleFloor->addAction(ActFloorStyleFloorIso);
    this->addActions(ActGroupStyleFloor->actions());


    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}
