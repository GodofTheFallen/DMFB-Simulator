#include "formsize.h"

bool FormSize::SafetyCheck()
{
    if (Row <= 0 || Row > 500) {emit SendErrMsg("Invalid size."); return false;}
    if (Col <= 0 || Col > 500) {emit SendErrMsg("Invalid size."); return false;}
    if (Input.first != 1 && Input.first != Row) {emit SendErrMsg("Invalid Input place."); return false;}
    if (Input.second != 1 && Input.second != Col) {emit SendErrMsg("Invalid Input place."); return false;}
    if (Output.first != 1 && Output.first != Row) {emit SendErrMsg("Invalid Output place."); return false;}
    if (Output.second != 1 && Output.second != Col) {emit SendErrMsg("Invalid Output place."); return false;}
    if (Input == Output) {emit SendErrMsg("Input & Output must be in different places."); return false;}
    if (Wash == Input || Wash == Output) {emit SendErrMsg("Invalid Wash place."); return false;}
    if (Waste == Input || Waste == Output) {emit SendErrMsg("Invalid Waste place."); return false;}
    return true;
}

FormSize::FormSize(int _r ,int _c,POS _I,POS _O, bool NeedWash):Row(_r),Col(_c),Input(_I),Output(_O)
{
    Wash = NeedWash?std::make_pair(1,1):std::make_pair(0,0);
    Waste = NeedWash?std::make_pair(Row,Col):std::make_pair(0,0);
    Validity=SafetyCheck();
}

bool FormSize::isValid() const
{
    return Validity;
}
