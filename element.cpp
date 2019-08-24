#include "element.h"

int Element::__Total = 0;

QMap<int,QColor> Element::ColorMap;

Element::Element(POS _P, DropType _T = DropType::Dot)
{
    ID = ++__Total;
    P = _P;
    Type = _T;
    Color = QColor::fromHsl(qrand()%360,64+qrand()%128,64+qrand()%128);
    ColorMap[ID] = Color;    
}
