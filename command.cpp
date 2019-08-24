#include "command.h"

Command::Command(CMDType _t,int _ti, POS _p0,POS _p1):type(_t),tick(_ti + 1),P0(_p0),P1(_p1)
{

}

int Command::GetTick() const
{
    return tick;
}

CMDType Command::getType() const
{
    return type;
}

POS Command::GetP0() const
{
    return P0;
}

POS Command::GetP1() const
{
    return P1;
}
