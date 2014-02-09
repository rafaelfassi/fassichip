#include <QtGui>
#include <cmath>
#include "fchart2d.h"
#include "misc.h"


FChart2d::FChart2d(QWidget *parent)
    : QWidget(parent)
{
    Data = 0;
    ByteOffset = 0;
    NumXTicks = 5;
    NumYTicks = 5;
    CursorMod = CURSOR_DEFAULT;
    HandIsActive = false;
    ShowScaleY = false;
    ShowScaleX = true;
    EnableRefresh = true;
    AutoSizeMargLeft = false;    //Auto-margin
    ScaleYIsHex = true;        //Scale Hex
    ScaleXIsHex = true;
    LockSelectionOnPage = false;
    MinX = 0;
    MaxX = 0;
    FinPosX = 0;
    ExtremeMinY = 0.0;
    ExtremeMaxY = 255.0;
    MinY = ExtremeMinY;
    MaxY = ExtremeMaxY;

    PosPointer = 0;      // Posição do ponteiro no arquivo
    SelectionIni = 0;    // Posição inicial da seleção
    SelectionNPos = 1;   // Número de posições selecionadas
    SelectionMult = 1;   // Multiplicativo da seleção
    //PageIni;         // Início da página

    MarginTop = 10;
    MarginButton = 50;  //  30 + altura da HorScrollBar
    MarginLeft = 10;
    MarginRight = 10;
    MinimumMarginH = 10;
    SuggestionPageX = 100;
    DefaultPageX = 0;

    HorScrollBar = new QScrollBar(Qt::Horizontal, this);
    HorScrollBar->setMinimum(0);
    HorScrollBar->setMaximum(0);
    HorScrollBar->setCursor(Qt::ArrowCursor);
    HorScrollBar->show();

    setBackgroundRole(QPalette::Base);
    ScaleColor = Qt::black;
    SelectionColor = Qt::lightGray;
    PointerColor = Qt::yellow;

    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    ZoomAreaIsShown = false;

    connect(HorScrollBar, SIGNAL(valueChanged(int)), this, SLOT(on_HorScrollBar_Change(int)));

    //refreshPixmap();
}

void FChart2d::on_HorScrollBar_Change(int _Position)
{  
    int _Span = MaxX - MinX;

    MinX = _Position;
    MaxX = _Position + _Span;

    emit PageChange(AddressExp(MinX), SpanX()+1);
    //Data->SetPageIni(Data->GetPosByte(MinX));

   if(LockSelectionOnPage) SelectionOnPage();

    RefreshScreen();
}

void FChart2d::UpdateSettings()
{
    if(Data->IsEmply())
    {
        PosPointer = 0;
        SelectionIni = 0;
        SelectionNPos = 1;
        SelectionMult = 1;
        //Data->SetPageIni(0);
    }
    else
    {
        FinPosX = Data->GetSizeRelative(Data->GetSerieMain(), ByteOffset, *DataMode) - 1;

        //int _min = Data->GetPosReletive(Data->GetPageIni());
        SetDefaultPageX(SuggestionPageX);
        //SetMinX(_min);


        /*
            Mesmo sem visualizar a escala Y as margens esquerda e direira não podem ser zero,
            pois metade do endereço final e inicial da escala X ocupam as margens horizontais.
        */
        QFontMetrics fm(this->font());
        MinimumMarginH = fm.width(DoubleToHexS(FinPosX, 1))/2 + 2;
        MarginLeft = MinimumMarginH;
        MarginRight = MinimumMarginH;
    }
}

QSize FChart2d::minimumSizeHint() const
{
    /*
       Reimplementação da classe virtual de QWidget.
       Define o tamanho mínimo recomendado para o widget.
    */

    return QSize(6 * MarginLeft, 4 * MarginButton);
}

QSize FChart2d::sizeHint() const
{
    /*
       Reimplementação da classe virtual de QWidget.
       Define o tamanho recomendado para o widget.
    */

    return QSize(12 * MarginLeft, 8 * MarginButton);
}

void FChart2d::paintEvent(QPaintEvent * /* event */)
{
    /*
        Reimplementação da classe virtual de QWidget.
        Recebe o evento de solicitação de redesenho da tela e aplica o
        desenho definido em pixmap. Também desenha a seleção do zoom caso
    */

    QStylePainter painter(this);
    painter.drawPixmap(0, 0, Pixmap);

    if (ZoomAreaIsShown) {
        QPen pen(SelectionColor, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawRect(ZoomAreaRect.normalized().adjusted(0, 0, -1, -1));
    }

   /* if (hasFocus()) {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }*/
}

void FChart2d::resizeEvent(QResizeEvent * /* event */)
{
    HorScrollBar->setGeometry(0, this->height() - 20, this->width(), 20);
    RefreshScreen();
}

void FChart2d::mousePressEvent(QMouseEvent *event)
{
    if(Data->IsEmply()) return;
   // QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    QRect rect(MarginLeft, MarginTop, width() - MarginHoriz(), height() - MarginVert());

    switch(CursorMod)
    {
    case CURSOR_DEFAULT:
        if(event->button() == Qt::LeftButton)
        {
            PosPointer = ScreenToPosX(event->x());
            if(PosPointer > MaxX) PosPointer = MaxX;
            if(PosPointer < MinX) PosPointer = MinX;

            int _PosPointerAb = AddressExp(PosPointer);
            emit PointerChange(_PosPointerAb);

            if(SelectionMult == 1 && SelectionNPos == 1)
            {
              SelectionIni = PosPointer;
              emit SelectionChange(_PosPointerAb, 1, 1);
            }

            RefreshScreen();
        }
        else if(event->button() == Qt::RightButton)
        {
            SelectionIni = ScreenToPosX(event->x());
            SelectionStart = SelectionIni;
            if(SelectionIni > MaxX) SelectionIni = MaxX;
            if(SelectionIni < MinX) SelectionIni = MinX;       
            SelectionNPos = 1;
            SelectionMult = 1;

            emit SelectionChange(AddressExp(SelectionIni), 1, 1);
            RefreshScreen();
        }
        break;
    case CURSOR_ZOOM:
        if(event->button() == Qt::LeftButton)
        {
            if (rect.contains(event->pos()))
            {
                ZoomAreaIsShown = true;
                ZoomAreaRect.setTopLeft(event->pos());
                ZoomAreaRect.setBottomRight(event->pos());
                update();
            }
        }
        else
        {
            SetCursorMode(CURSOR_DEFAULT);
            ZoomAreaIsShown = false;
            update();
        }
        break;
    case CURSOR_HAND:
        if(event->button() == Qt::LeftButton)
        {
            if (rect.contains(event->pos()))
            {
                setCursor(Qt::ClosedHandCursor);
                HandStartPosX = ScreenToPosX(event->x());
                HandStartValY = ScreenToValY(event->y());
                HandIsActive = true;
            }
        }
        else
        {
            SetCursorMode(CURSOR_DEFAULT);
            HandIsActive = false;
        }
        break;
    }

}

void FChart2d::mouseMoveEvent(QMouseEvent *event)
{

 QRect rect(MarginLeft, MarginTop, width() - MarginHoriz(), height() - MarginVert());

    if(Data->IsEmply()) return;

    switch(CursorMod)
    {
    case CURSOR_DEFAULT:
        if(event->buttons() == Qt::LeftButton)
        {
            PosPointer = ScreenToPosX(event->x());
            if(PosPointer > MaxX) PosPointer = MaxX;
            if(PosPointer < MinX) PosPointer = MinX;

            int _PosPointerAb = AddressExp(PosPointer);
            emit PointerChange(_PosPointerAb);

            if(SelectionMult == 1 && SelectionNPos == 1)
            {
              SelectionIni =  PosPointer;
              emit SelectionChange(_PosPointerAb, 1, 1);
            }

            RefreshScreen();
        }
        else if(event->buttons() == Qt::RightButton)
        {
            int _PosAt = ScreenToPosX(event->x());
            if(_PosAt < 0) _PosAt = 0;
            if(_PosAt > FinPosX) _PosAt = FinPosX;
            if(_PosAt > MaxX) _SetMaxX(_PosAt);
            if(_PosAt < MinX) _SetMinX(_PosAt);

            SelectionNPos = abs(_PosAt - SelectionStart) + 1;
            //Data->SetSelectionNPos(abs(_PosAt - SelectionStart) + 1);

            if(_PosAt >= SelectionStart) SelectionIni = SelectionStart;// Data->SetSelectionIni(Data->GetPosByte(SelectionStart) + ByteOffset);
            else SelectionIni = _PosAt;// Data->SetSelectionIni(Data->GetPosByte(_PosAt) + ByteOffset);

            emit SelectionChange(AddressExp(SelectionIni), SelectionNPos, SelectionMult);
            RefreshScreen();
        }
        break;
    case CURSOR_ZOOM:
        if(event->buttons() == Qt::LeftButton)
        {
            if (rect.contains(event->pos()))
            {
                if(ZoomAreaIsShown)
                {
                    ZoomAreaRect.setBottomRight(event->pos());
                    update();
                }
            }
        }
        break;
    case CURSOR_HAND:
        if(event->buttons() == Qt::LeftButton)
        {
            if(HandIsActive)
            {
                int _Pos = ScreenToPosX(event->x());
                double _Val = ScreenToValY(event->y());
                _SetMinX(MinX + HandStartPosX - _Pos);
                SetMinY(MinY + HandStartValY - _Val);
            }
        }
        break;
    }

}

void FChart2d::mouseReleaseEvent(QMouseEvent *event)
{
    if(Data->IsEmply()) return;

    if ((event->button() == Qt::LeftButton) && ZoomAreaIsShown)
    {
        ZoomAreaIsShown = false;
        update();
        //unsetCursor();

        QRect rect = ZoomAreaRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return;

        rect.translate(-MarginLeft, -MarginTop);

        double tmp1, tmp2;
        double dx = SpanX() / (width() - MarginHoriz());
        double dy = SpanY() / (height() - MarginVert());

        tmp1 = ApproximateInt(MinX + dx * rect.left());
        tmp2 = ApproximateInt(MinX + dx * rect.right());

        if((tmp2 - tmp1) >= 1)
        {
          MinX = tmp1;
          MaxX = tmp2;
          emit PageChange(AddressExp(MinX), SpanX()+1);
          //Data->SetPageIni(Data->GetPosByte(MinX));
        }

        tmp1 = MaxY;
        MinY = MaxY - dy * rect.bottom();
        MaxY = tmp1 - dy * rect.top();

        HorScrollBar->setMaximum(FinPosX - SpanX());
        HorScrollBar->setValue(MinX);
        RefreshScreen();
    }
    else if((event->button() == Qt::LeftButton) && HandIsActive)
    {
        HandIsActive = false;
        setCursor(Qt::OpenHandCursor);
    }
}

void FChart2d::keyPressEvent(QKeyEvent *event)
{
    if(Data->IsEmply()) return;

    switch (event->key()) {
    case Qt::Key_Plus:
        //zoomIn();
        break;
    case Qt::Key_Minus:
        //zoomOut();
        break;
    case Qt::Key_Left:
        Scroll(-1, 0);
        RefreshScreen();
        break;
    case Qt::Key_Right:
        Scroll(+1, 0);
        RefreshScreen();
        break;
    case Qt::Key_Down:
        Scroll(0, -1);
        RefreshScreen();
        break;
    case Qt::Key_Up:
        Scroll(0, +1);
        RefreshScreen();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void FChart2d::wheelEvent(QWheelEvent *event)
{
    if(Data->IsEmply()) return;

    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal)
    {
        Scroll(numTicks, 0);
    } else {
        Scroll(0, numTicks);
    }
    RefreshScreen();
}

void FChart2d::SetBackgroundColor(QColor _Color)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, _Color);
    setPalette(pal);
    setBackgroundRole(pal.Background);
}

void FChart2d::RefreshScreen()
{



    Pixmap = QPixmap(size());
    if(Pixmap.isNull()) return;
    Pixmap.fill(this, 0, 0);
    QPainter Painter(&Pixmap);
    Painter.initFrom(this);

    if(Data->IsEmply())
    {
       repaint();
       return;
    }
    else if(!EnableRefresh) return;

    // Desenha a escala Y------------------------------------------------------------------------------------
    // A escala y deve ser desenhada neste ponto pois a depender das opções ela pode redefinir a margem esquerda
    if(ShowScaleY) DrawScaleY(&Painter);
    else MarginLeft = MinimumMarginH;

    QRect rect(MarginLeft, MarginTop, width() - MarginHoriz(), height() - MarginVert());
    if (!rect.isValid()) return;

    bool ScaleDraw = false; // Serve para verificcar se a escala já foi desenhada.
    int MaxScreenY = height()*10;
    int *PointsX = new int[(int)SpanX() + 1];
    int *PointsY = new int[(int)SpanX() + 1];


    for(int Type = Data->GetMaxFileType(); Type >=0; Type--) // Procura as séries existentes
    {
         if(!Data->ExistFileType(Type)) continue;
         int Id = Data->GetId(Type);

            for (int j = MinX; j <= MaxX; ++j) // Obtem os pontos x e y da série
            {
                double dx = j - MinX;
                int x = ApproximateInt(rect.left() + (dx * (rect.width() - 1) / SpanX()));
                int Pj = AddressExp(j);
                double dy = Data->GetValue(Id, Pj, *DataMode);
                double y;

                if(!IsNan(dy))
                {
                    dy -= MinY;
                    y = rect.bottom() - (dy * (rect.height() - 1) / SpanY());

                    // Limita y a um valor máximo, pois um valor de screen muito grande, e assim muito fora
                    // da tela, costuma dar erro na plotagem do gráfico
                    if(y > MaxScreenY) y = MaxScreenY;
                    else if(y < -MaxScreenY) y = -MaxScreenY;
                }
                else y = MaxScreenY + 1; // Se for um valor numérico inválido (colocar y acima de MaxScreenY
                                         // vai avisar a função que plota os pontos para não plotar este ponto)
                PointsX[j - MinX] = x;
                PointsY[j - MinX] = ApproximateInt(y);

            }

            // Desenha a escala X----------------------------------------------------------------------------
            if(!ScaleDraw && ShowScaleX) DrawScaleX(&Painter, PointsX);
            ScaleDraw = true; // A escala ja foi plotada e não precisa ser redesenhada para as outras séries

            // Plota os pontos da série----------------------------------------------------------------------
            DrawPoints(&Painter, PointsX, PointsY, MaxScreenY, Id);

    }

    // Desenha Seleção---------------------------------------------------------------------------------------
    DrawSelection(&Painter);

    // Desenha cursor----------------------------------------------------------------------------------------
    DrawCursor(&Painter);

    delete [] PointsX;
    delete [] PointsY;
    repaint();
}

void FChart2d::DrawScaleY(QPainter *_Painter)
{
    QString StMax;
    QString StMin;

    _Painter->setPen(ScaleColor);

    if(ScaleYIsHex && DataMode->Mode != BITSF)
    {
        int Dig = QString::number((qint64)ExtremeMaxY, 16).length();

        StMax = DoubleToHexS(MaxY, Dig);
        StMin = DoubleToHexS(MinY, Dig);
    }
    else
    {
        StMax = StMax.number(MaxY);
        StMin = StMin.number(MinY);
    }

   /* int LenScalMax = StMax.length();
    int LenScalMin = StMin.length();
    int WidthLabY = LenScalMax > LenScalMin ? LenScalMax: LenScalMin;*/


    int WidthStMin, WidthStMax, WidthLabY, HeightLabY;
    QFontMetrics fm(_Painter->font());
    HeightLabY = fm.height();
    WidthStMax = fm.width(StMax);
    WidthStMin = fm.width(StMin);
    WidthLabY = WidthStMax > WidthStMin ? WidthStMax: WidthStMin;
    WidthLabY+= 6;


    //WidthLabY = WidthLabY*6 + 6;

    if(MinimumMarginH < WidthLabY) MinimumMarginH = WidthLabY;

    MarginLeft = MinimumMarginH;

    //if(MarginLeft < WidthLabY) MarginLeft = WidthLabY;
    if(AutoSizeMargLeft && (WidthLabY > MinimumMarginH)) MarginLeft = WidthLabY;



    _Painter->drawLine(MarginLeft-1, MarginTop+1, MarginLeft-1, height() - MarginButton + 2);




    _Painter->drawText(MarginLeft-WidthLabY-2, MarginTop - 10, WidthLabY, HeightLabY,
                      Qt::AlignRight | Qt::AlignVCenter,
                      StMax);

    _Painter->drawText(MarginLeft-WidthLabY-2, height() - MarginButton - 10, WidthLabY, HeightLabY,
                      Qt::AlignRight | Qt::AlignVCenter,
                      StMin);
}

void FChart2d::DrawScaleX(QPainter *_Painter, int *_PointsX)
{

    int NLab = 0;
    int WidthLabX, HeightLabX;
    int Pos;
    int Bottom = height() - MarginButton;
    QString Str1;
    int Dig = QString::number(FinPosX, 16).length();

    QFontMetrics fm(_Painter->font());

    if(ScaleXIsHex) WidthLabX = fm.width(DoubleToHexS(MaxX, Dig)) + 30;
    else WidthLabX = fm.width(QString::number(MaxX)) + 30;

    HeightLabX = fm.height();

  //  if(ScaleXIsHex) WidthLabX = DoubleToHexS(MaxX, Dig).length() * 6 + 30;
  //  else WidthLabX = QString::number(MaxX).length() * 6 + 30;

    _Painter->setPen(ScaleColor);

    _Painter->drawLine(MarginLeft, Bottom + 2, width() - MarginHoriz() + MarginLeft, Bottom + 2);

    for (int j = MinX; j <= MaxX; ++j)
    {
        Pos = j - MinX;

        if(ScaleXIsHex) Str1 = DoubleToHexS(AddressExp(j), Dig);
        else Str1 = QString::number(AddressExp(j));

          if(((_PointsX[Pos]-MarginLeft)/WidthLabX) >= NLab)
          {
              _Painter->drawText(_PointsX[Pos] - (WidthLabX/2) + 1,  Bottom + 10, WidthLabX, HeightLabX,
                                Qt::AlignHCenter | Qt::AlignTop,
                                Str1);

              _Painter->drawLine(_PointsX[Pos], Bottom + 2, _PointsX[Pos], Bottom + 7);
              NLab++;
          }
          else _Painter->drawLine(_PointsX[Pos], Bottom + 2, _PointsX[Pos], Bottom + 5);
   }
}

void FChart2d::DrawPoints(QPainter *_Painter, int *_PointsX, int *_PointsY, int _MaxY, int _Id)
{
    int _spanX = SpanX();
    int x_ant = -1;
    int y_ant = -1;

    // Limita o desenho na área especificada pelo retângulo. (Evita que o gráfico apareça nas margens e em cima da escala)
    _Painter->setClipRect(MarginLeft, MarginTop, width() - MarginHoriz(), height() - MarginVert());
    _Painter->setPen(Data->GetSerieColor(_Id));

    for(int k=0; k<=_spanX; k++)
    {
        if(_PointsY[k] <= _MaxY) // Se o ponto y está acima do valor máximo é porque não deve ser plotado
        {
          if(x_ant >= 0) _Painter->drawLine(x_ant, y_ant, _PointsX[k], _PointsY[k]);
          x_ant = _PointsX[k];
          y_ant = _PointsY[k];
        }
        else x_ant = -1;
    }

    // Volta ao ClipRect anterior. (Seria mais interessante gravar o ClipRect no início da função e restaura-lo aqui)
    _Painter->setClipRect(rect());
}

void FChart2d::DrawSelection(QPainter *_Painter)
{
    int _IniSel = SelectionIni;
    int _NPos = SelectionNPos;
    int _Mult = SelectionMult;
    int _FimSel = _IniSel + _NPos*_Mult - 1;

    if(!(((_IniSel < MinX) && (_FimSel < MinX)) || ((_FimSel > MaxX) && (_IniSel > MaxX))))
    {
        //Painter.setBrush(SelectionColor);
        //Painter.setPen(Qt::NoPen);
        _Painter->setOpacity(0.3);


        _Painter->setClipRect( MarginLeft, 0, width() - MarginLeft - MarginRight, height());


        for(int w = 0; w < _Mult; w++)
        {

            if(_NPos > 1)
            {
                _Painter->setBrush(SelectionColor);
                _Painter->setPen(Qt::NoPen);
                _Painter->drawRect(PosToScreenX(_IniSel),
                                   MarginTop,
                                   PosToScreenX(_IniSel + _NPos - 1) - PosToScreenX(_IniSel) + 1,
                                   height() - MarginVert());
            }
            else
            {
               _Painter->setPen(SelectionColor);
               _Painter->drawLine(PosToScreenX(_IniSel),
                                  MarginTop,
                                  PosToScreenX(_IniSel),
                                  height() - MarginButton);
            }
                _IniSel += _NPos;
        }
    }

}

void FChart2d::DrawCursor(QPainter *_Painter)
{
    if(PosPointer >= MinX && PosPointer <= MaxX)
    {
        _Painter->setOpacity(1);
        _Painter->setPen(PointerColor);
        _Painter->drawLine(PosToScreenX(PosPointer), MarginTop, PosToScreenX(PosPointer), height() - MarginButton);
    }
}

void FChart2d::Scroll(int dx, int dy)
{
    /*
        Rola a tela em dx e dy informados
    */

    if(Data->IsEmply()) return;
    //if(!Data->GetSeriesCount()) return;

    double stepX = (SpanX() / NumXTicks) * dx;

    if(fabs(stepX) < 1) stepX = dx;
    if(MinX + stepX < 0) stepX = -MinX;
    if(MaxX + stepX > FinPosX) stepX = FinPosX - MaxX;


    MinX += ApproximateInt(stepX);
    MaxX += ApproximateInt(stepX);

    emit PageChange(AddressExp(MinX), SpanX()+1);
    //Data->SetPageIni(Data->GetPosByte(MinX));

    HorScrollBar->setValue(MinX);

    double stepY = (SpanY() / NumYTicks) * dy;

    if((MinY + stepY) < ExtremeMinY) stepY = ExtremeMinY - MinY;
    if((MaxY + stepY) > ExtremeMaxY) stepY = ExtremeMaxY - MaxY;

    MinY += stepY;
    MaxY += stepY;
}

int FChart2d::ScreenToPosX(int _ScreenX)
{
    double dx = SpanX() / (width() - MarginHoriz() - 1);
    return ApproximateInt(MinX + dx * (_ScreenX - MarginLeft));
}

int FChart2d::PosToScreenX(int _PosX)
{
    double dx = SpanX() / (width() - MarginHoriz() - 1);
    return ApproximateInt(((_PosX - MinX) / dx) + MarginLeft);
}

double FChart2d::ScreenToValY(int _ScreenY)
{
    double dy = SpanY() / (height() - MarginVert() - 1);
    return MaxY - dy * (_ScreenY - MarginTop);
}

int FChart2d::ValToScreenY(double _ValY)
{
    double dy = SpanY() / (height() - MarginVert() - 1);
    return ApproximateInt(((MaxY - _ValY)/dy) + MarginTop);
}

void FChart2d::SetDefaultPageX(int _Points)
{   
    SuggestionPageX = _Points;
    _Points--;

    if(_Points <= FinPosX)
    {
        DefaultPageX = _Points;

        if(MinX < 1 && MaxX < _Points) // Caso ainda não se tenha definido a página ( primeiro arquivo aberto )
        {
            MinX = 0;
            MaxX = _Points;
        }
        else if(MaxX > FinPosX) // Se visualização atual da página estiver acima do tamanho do arquivo
        {
            MaxX = FinPosX;
            MinX = FinPosX - _Points;
        }

    }
    else // Se o arquivo aberto for menor que o tamanho da página sugerido.
    {
        DefaultPageX = FinPosX + 1;
        MinX = 0;
        MaxX = FinPosX;
    }

    //Data->SetPageIni(Data->GetPosByte(MinX));
    HorScrollBar->setMaximum(FinPosX - SpanX());
    emit PageChange(AddressExp(MinX), SpanX()+1);
}

void FChart2d::SetExtremesY(double _Min, double _Max)
{
    ExtremeMinY = _Min;
    ExtremeMaxY = _Max;
    MinY = ExtremeMinY;
    MaxY = ExtremeMaxY;
}

void FChart2d::_SetXMinMax(int _Min, int _Max)
{
    if(_Min < 0) _Min = 0;
    if(_Max > FinPosX) _Max = FinPosX;

    MinX = _Min;
    MaxX = _Max;

    emit PageChange(AddressExp(MinX), SpanX()+1);
    //Data->SetPageIni(Data->GetPosByte(MinX));
    HorScrollBar->setMaximum(FinPosX - SpanX());
    HorScrollBar->setValue(MinX);
    RefreshScreen();
}

void FChart2d::SetYMinMax(double _Min, double _Max)
{
    if(_Min < ExtremeMinY) _Min = ExtremeMinY;
    if(_Max > ExtremeMaxY) _Max = ExtremeMaxY;
    MinY = _Min;
    MaxY = _Max;

    RefreshScreen();
}

void FChart2d::_SetMinX(int _Min)
{
    int tela = SpanX();

    if(_Min < 0) _Min = 0;
    if(_Min + tela > FinPosX) _Min = FinPosX - tela;

    MinX = _Min;
    MaxX = _Min + tela;

    emit PageChange(AddressExp(MinX), SpanX()+1);
    //Data->SetPageIni(Data->GetPosByte(MinX));
   // HorScrollBar->setMaximum(FinPosX - SpanX());
    HorScrollBar->setValue(MinX);
    RefreshScreen();
}

void FChart2d::_SetMaxX(int _Max)
{
    int tela = SpanX();

    if(_Max > FinPosX) _Max = FinPosX;
    if(_Max - tela < 0) _Max = tela;

    MaxX = _Max;
    MinX = _Max - tela;

    emit PageChange(AddressExp(MinX), SpanX()+1);
    //Data->SetPageIni(Data->GetPosByte(MinX));
    //HorScrollBar->setMaximum(EndPosX - spanX());
    HorScrollBar->setValue(MinX);
    RefreshScreen();
}

void FChart2d::SetMinY(double _Min)
{
   double tela = SpanY();

   if(_Min < ExtremeMinY) _Min = ExtremeMinY;
   if(_Min + tela > ExtremeMaxY) _Min = ExtremeMaxY - tela;

   MinY = _Min;
   MaxY = _Min + tela;

   RefreshScreen();

}

void FChart2d::SetMaxY(double _Max)
{
   double tela = SpanY();

   if(_Max > ExtremeMaxY) _Max = ExtremeMaxY;
   if(_Max - tela < ExtremeMinY) _Max = tela;

   MaxY = _Max;
   MinY = _Max - tela;

   RefreshScreen();
}

void FChart2d::SetCursorMode(int _Cursor)
{

    switch(_Cursor)
    {
    case CURSOR_DEFAULT:
        setCursor(Qt::ArrowCursor);
        CursorMod = _Cursor;
        break;
    case CURSOR_HAND:
        setCursor(Qt::OpenHandCursor);
        CursorMod = _Cursor;
        break;
    case CURSOR_ZOOM:
        setCursor(Qt::CrossCursor);
        CursorMod = _Cursor;
        break;
    case CURSOR_RESET_ZOOM:
        ResetZoom();
    }

    emit CursorModeChange(CursorMod);
}

void FChart2d::EmitSignals()
{
    emit PointerChange(AddressExp(PosPointer));
    emit SelectionChange(AddressExp(SelectionIni), SelectionNPos, SelectionMult);
    emit CursorModeChange(CursorMod);
}

void FChart2d::ResetZoom()
{

    if(MinX + DefaultPageX > FinPosX) MinX = FinPosX - DefaultPageX;

    _SetXMinMax(MinX, MinX + DefaultPageX);

    SetYMinMax(ExtremeMinY, ExtremeMaxY);
}

void FChart2d::SelectionOnPage()
{

    if((SelectionNPos > 1) && (SelectionMult > 1))
    {
        //int _Min = Data->GetPosByte(MinX) + ByteOffset;

        SelectionIni = MinX;
        MaxX = MinX + SelectionNPos*SelectionMult - 1;
        HorScrollBar->setMaximum(FinPosX - SpanX());
        emit SelectionChange(AddressExp(SelectionIni), SelectionNPos, SelectionMult);
    }

}

void FChart2d::GotoPointer()
{
    if((PosPointer < MinX) || (PosPointer > MaxX))
    {
        _SetMinX(PosPointer - (SpanX())/2);
    }
}

void FChart2d::SetPointer(int _Pointer)
{
    PosPointer = AddressImp(_Pointer);

    emit PointerChange(_Pointer);

    if((SelectionNPos < 2) && (SelectionMult < 2))
    {
        SelectionIni = PosPointer;
        emit SelectionChange(AddressExp(SelectionIni), SelectionNPos, SelectionMult);
    }

    RefreshScreen();
}

void FChart2d::SetSelection(int _SelIni, int _SelNPos, int _SelMult)
{
    SelectionIni = AddressImp(_SelIni);
    SelectionNPos = _SelNPos;
    SelectionMult = _SelMult;

    emit SelectionChange(_SelIni, _SelNPos, _SelMult);

    if(LockSelectionOnPage) SelectionOnPage();

    RefreshScreen();
}

void FChart2d::SetPage(int _PageIni, int _NPoints)
{
    _NPoints--;
    _PageIni = AddressImp(_PageIni);

    if(MinX + _NPoints > FinPosX) MinX = FinPosX - _NPoints;

    _SetXMinMax(_PageIni, _PageIni + _NPoints);
}
