#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>
#include <QColor>
#include <QMap>
#include "basetypedef.h"

class Element
{
public:
    Element(POS,DropType);
    static int __Total;
    static QMap<int,QColor> ColorMap;
    int getID()const {return ID;}
    POS getP()const {return P;}
    void SetP(POS _P) {P=_P;}
    void SetType(DropType _T) {Type=_T;}
    DropType getType()const {return Type;}
    QColor getColor()const {return Color;}

private:
    int ID;
    DropType Type;
    POS P;
    QColor Color;
};

typedef QList<Element> ElementQueue;

#endif // ELEMENT_H
