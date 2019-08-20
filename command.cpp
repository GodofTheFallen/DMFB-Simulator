#include "command.h"

Command::Command(CMDType _t,POS _p0,POS _p1):type(_t),P0(_p0),P1(_p1)
{

}

int Command::GetTick() const
{
    return tick;
}

POS Command::GetP1() const
{
    return P1;
}
