#include "command.h"

Command::Command(CMDType _t,POS _p1,POS _p2):type(_t),P1(_p1),P2(_p2)
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
