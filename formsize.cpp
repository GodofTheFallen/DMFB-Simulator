#include "formsize.h"

void FormSize::SafetyCheck()
{
    bool _Val=_SafetyCheck();
    if (_Val!=Validity) emit ValidityChanged(Validity=_Val);
}

bool FormSize::_SafetyCheck()
{
    if (Row <= 0 || Row > MAX_SIZE) {emit SendErrMsg("Invalid size."); return false;}
    if (Col <= 0 || Col > MAX_SIZE) {emit SendErrMsg("Invalid size."); return false;}
    if (Input.first != 1 && Input.first != Row && Input.second != 1 && Input.second != Col) {emit SendErrMsg("Invalid Input place."); return false;}
    if (Output.first != 1 && Output.first != Row && Output.second != 1 && Output.second != Col) {emit SendErrMsg("Invalid Output place."); return false;}
    if (Input == Output) {emit SendErrMsg("Input & Output must be in different places."); return false;}
    if (Wash == Input || Wash == Output) {emit SendErrMsg("Invalid Wash place."); return false;}
    if (Waste == Input || Waste == Output) {emit SendErrMsg("Invalid Waste place."); return false;}
    return true;
}

bool FormSize::isValid() const
{
    return Validity;
}

void FormSize::ChangeFormSize(int _r ,int _c,POS _I,POS _O, bool NeedWash)
{
    Row=_r;
    Col=_c;
    Input=_I;
    Output=_O;
    Wash = NeedWash?std::make_pair(1,1):std::make_pair(0,0);
    Waste = NeedWash?std::make_pair(Row,Col):std::make_pair(0,0);
    SafetyCheck();
}

bool FormSize::SetWashStatus(int NeedWash)
{
    Wash = NeedWash?std::make_pair(1,1):std::make_pair(0,0);
    Waste = NeedWash?std::make_pair(Row,Col):std::make_pair(0,0);
    SafetyCheck();
    return Validity;
}

