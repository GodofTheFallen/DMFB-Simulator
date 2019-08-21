#include "syscore.h"


SysCore::SysCore(QObject *parent):QObject(parent){
    SZ = new FormSize(this);
    CMDList = new CommandQueue(this);
}

SysCore::~SysCore()
{

}

bool SysCore::IsReady()
{
    return SZ->isValid() && CMDList->isValid();
}

void SysCore::ResetWash(int NowStatus)
{
    if (!SZ->SetWashStatus(NowStatus)) emit ChangeFailed(Qt::Unchecked);
}

void SysCore::SetFormSize(int _r, int _c, int _IR, int _IC, int _OR,int _OC,int _NW)
{
    SZ->ChangeFormSize(_r,_c,std::make_pair(_IR,_IC),std::make_pair(_OR,_OC),_NW);
}
