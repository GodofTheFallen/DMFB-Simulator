#include "syscore.h"


void SysCore::ResetWash(int NowStatus)
{
    SZ.SetWashStatus(NowStatus);
}

void SysCore::SetFormSize(int _r, int _c, int _IR, int _IC, int _OR,int _OC,int _NW)
{
    SZ.ChangeFormSize(_r,_c,std::make_pair(_IR,_IC),std::make_pair(_OR,_OC),_NW);
}
