#include "chippaintbox.h"

ChipPaintBox::ChipPaintBox(QWidget *parent) : QGraphicsView(parent)
{

}

/*
void ChipPaintBox::PaintNow()
{

}
*/

void ChipPaintBox::paintEvent(QPaintEvent *)
{
    if (!SZ.isValid()) return;
    QPainter Painter(this);
    QPen TableEdges(Qt::black,2);
    Painter.setPen(TableEdges);


}
