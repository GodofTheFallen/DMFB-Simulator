#include "chippaintbox.h"

#include <QMouseEvent>
#include <QtDebug>

const QPointF ChipPaintBox::O_POS(25,625);
QColor ChipPaintBox::InputColor(250,187,50);
QColor ChipPaintBox::OutputColor(125,44,201);
QColor ChipPaintBox::WaterColor(81,209,232);
QColor ChipPaintBox::WasteColor(153,11,11);

ChipPaintBox::ChipPaintBox(QWidget *parent) : QWidget(parent),CI()
{
    CI = nullptr;
    TableEdges.setColor(Qt::black);
    TableEdges.setWidth(1);
    QPen TableEdges;
}

ChipPaintBox::~ChipPaintBox()
{
    if (CI) delete CI;
}

void ChipPaintBox::PaintNow(const ChipInfo &_CI, bool _nW)
{
    Washing = _nW;
    if (CI) delete CI;
    CI = new ChipInfo(_CI);
    repaint();
}

void ChipPaintBox::ClearPainting()
{
    Washing = false;
    if (CI) delete CI;
    CI = nullptr;
    repaint();
}

void ChipPaintBox::paintEvent(QPaintEvent *)
{
    if (!CI) return;
    QPainter Painter(this);
    Painter.translate(O_POS);
    QFont font = Painter.font();
    font.setPixelSize(12);
    Painter.setFont(font);
    auto ScaledPoint = [=](double x,double y) {return QPointF(x*GridWidth,y*-GridWidth);};
    Painter.setPen(TableEdges);
    for (int i = 0; i <= CI->Row; ++i)
        Painter.drawLine(ScaledPoint(i,0),ScaledPoint(i,CI->Col));
    for (int i = 0; i <= CI->Col; ++i)
        Painter.drawLine(ScaledPoint(0,i),ScaledPoint(CI->Row,i));

    if (CI->needWash) {
        Painter.setBrush(QBrush(WaterColor));
        QVector<QPointF> WaterTri;
        WaterTri << ScaledPoint(-0.5,0) << ScaledPoint(-0.5,1) << ScaledPoint(0,0.5);
        Painter.drawPolygon(QPolygonF(WaterTri));
        Painter.setBrush(QBrush(WasteColor));
        QVector<QPointF> WasteTri;
        WasteTri << ScaledPoint(CI->Row,CI->Col) << ScaledPoint(CI->Row,CI->Col-1) << ScaledPoint(CI->Row+0.5,CI->Col-0.5);
        Painter.drawPolygon(QPolygonF(WasteTri));
    }

    for (int i = 1; i <= CI->Row; ++i)
        for (int j = 1; j <= CI->Col; ++j) {
            ChipCell *cur = CI->getCell(std::make_pair(i,j));
            if (cur->isInput()) {
                Painter.setBrush(QBrush(InputColor));
                QVector<QPointF> InputTri;
                if (i == 1)
                    InputTri << ScaledPoint(-0.5,j-1) << ScaledPoint(-0.5,j) << ScaledPoint(0,j-0.5);
                else if (j == 1)
                    InputTri << ScaledPoint(i-1,-0.5) << ScaledPoint(i,-0.5) << ScaledPoint(i-0.5,0);
                else if (i == CI->Row) {
                    InputTri << ScaledPoint(CI->Row+0.5,j-1) << ScaledPoint(CI->Row+0.5,j) << ScaledPoint(CI->Row,j-0.5);
                }
                else if (j == CI->Col) {
                    InputTri << ScaledPoint(i-1,CI->Col+0.5) << ScaledPoint(i,CI->Col+0.5) << ScaledPoint(i-0.5,CI->Col);
                }
                Painter.drawPolygon(QPolygonF(InputTri));
            }
            if (cur->isOutput()) {
                Painter.setBrush(QBrush(OutputColor));
                QVector<QPointF> OutputTri;
                if (i == 1)
                    OutputTri << ScaledPoint(0,j-1) << ScaledPoint(0,j) << ScaledPoint(-0.5,j-0.5);
                else if (j == 1)
                    OutputTri << ScaledPoint(i-1,0) << ScaledPoint(i,0) << ScaledPoint(i-0.5,-0.5);
                else if (i == CI->Row) {
                    OutputTri << ScaledPoint(CI->Row,j-1) << ScaledPoint(CI->Row,j) << ScaledPoint(CI->Row+0.5,j-0.5);
                }
                else if (j == CI->Col) {
                    OutputTri << ScaledPoint(i-1,CI->Col) << ScaledPoint(i,CI->Col) << ScaledPoint(i-0.5,CI->Col+0.5);
                }
                Painter.drawPolygon(QPolygonF(OutputTri));
            }
        }

    Painter.setPen(Qt::transparent);
    for (int i = 1; i <= CI->Row; ++i)
        for (int j = 1; j <= CI->Col; ++j) {
            ChipCell *cur = CI->getCell(std::make_pair(i,j));
            Painter.setBrush(Qt::gray);
            if (CI->needWash && cur->banWash()) Painter.drawRect(QRectF(ScaledPoint(i-0.95,j-0.95),ScaledPoint(i-0.05,j-0.05)));
            for (auto wid : cur->Pollution) {
                QColor WC = Element::ColorMap[wid];
                WC.setAlpha(WasteAlpha);
                Painter.setBrush(QBrush(WC));
                Painter.drawEllipse(QRectF(ScaledPoint(i-0.75,j-0.75),ScaledPoint(i-0.25,j-0.25)));
            }
            Painter.setBrush(WaterColor);
            if (Washing && cur->shouldWash) Painter.drawRect(QRectF(ScaledPoint(i-1,j-1),ScaledPoint(i,j)));
            Painter.setPen(Qt::black);
            Painter.drawText(QRectF(ScaledPoint(i-1,j),ScaledPoint(i,j-1)),QString::number(cur->Pollution.size()),Qt::AlignRight|Qt::AlignBottom);
            Painter.setPen(Qt::transparent);
        }
    for (auto cur : CI->EQ) {
        int x=cur.getP().first, y=cur.getP().second;
        Painter.setPen(Qt::transparent);
        Painter.setBrush(cur.getColor());
        switch (cur.getType()) {
        case DropType::Dot:
            Painter.drawEllipse(QRectF(ScaledPoint(x-0.9,y-0.9),ScaledPoint(x-0.1,y-0.1)));
            break;
        case DropType::Hor:
            Painter.drawEllipse(QRectF(ScaledPoint(x-1.7,y-0.9),ScaledPoint(x+0.7,y-0.1)));
            break;
        case DropType::Per:
            Painter.drawEllipse(QRectF(ScaledPoint(x-0.9,y-1.8),ScaledPoint(x-0.1,y+0.7)));
            break;
        }
    }
}

void ChipPaintBox::mousePressEvent(QMouseEvent *event)
{
    if (!CI) return;
    if (!CI->needWash) return;
    if (event->button() != Qt::RightButton) return;
    int mx = event->x(), my = event->y();
    int x = (mx-O_POS.toPoint().x())/GridWidth + 1;
    int y = (O_POS.toPoint().y()-my)/GridWidth + 1;
    qDebug() << x << ' ' << y;
    if (1 <= x && x <= CI->Row && 1 <= y && y <= CI->Col) emit MouseClick(std::make_pair(x,y));
}
