#ifndef CHIPINFO_H
#define CHIPINFO_H

#include <QObject>
#include "command.h"
#include "chipcell.h"
#include "formsize.h"
#include "element.h"

class ChipInfo
{
public:
    explicit ChipInfo(const FormSize &, bool);

    ElementQueue EQ;
    QList<ChipCell> CT;

    int Row,Col;
    bool needWash;
    POS Wash,Waste;

    bool soundMv,soundS0,soundS1,soundM0,soundM1,soundI,soundO;

    int FindDrop(const POS&);
    ChipCell* getCell(const POS&);

    void cleanAfterCopy();

    void getCanWash();

    bool CheckStatic();

    bool CheckPollution();
};

#endif // CHIPINFO_H
