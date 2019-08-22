#ifndef CHIPPAINTBOX_H
#define CHIPPAINTBOX_H

#include <QWidget>
#include <QGraphicsView>
#include "formsize.h"

class ChipPaintBox : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChipPaintBox(QWidget *parent = nullptr);

signals:

public slots:
    //void PaintNow(const FormSize& _SZ);


private:
    FormSize SZ;
    //QList<ChipElement> List;
    bool Washing;
    int NeedWash[FormSize::MAX_SIZE+1][FormSize::MAX_SIZE+1];

protected:
    void paintEvent(QPaintEvent *ev);
};

#endif // CHIPPAINTBOX_H
