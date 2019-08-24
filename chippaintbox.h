#ifndef CHIPPAINTBOX_H
#define CHIPPAINTBOX_H

#include <QWidget>
#include <QPainter>
#include "formsize.h"
#include "element.h"
#include "chipinfo.h"

class ChipPaintBox : public QWidget
{
    Q_OBJECT
public:
    explicit ChipPaintBox(QWidget *parent = nullptr);
    ~ChipPaintBox();
    static const int GridWidth = 50;
    static const QPointF O_POS;
    static const int WasteAlpha = 64;
    static QColor InputColor,OutputColor,WaterColor,WasteColor;

signals:
    void MouseClick(POS);

public slots:
    void PaintNow(const ChipInfo&, bool);

private:
    bool Washing;
    ChipInfo *CI;
    QPen TableEdges;

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *event);
};

#endif // CHIPPAINTBOX_H
