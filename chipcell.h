#ifndef CHIPCELL_H
#define CHIPCELL_H

#include <QObject>
#include <set>

class ChipCell
{
public:
    ChipCell();

    bool cantWash;
    bool shouldWash;

    void setBan(bool _b) {ban = _b;}
    void setInput() {I = true;}
    void setOutput() {O = true;}

    bool banWash()const {return ban;}

    bool isInput()const {return I;}
    bool isOutput()const {return O;}

    std::set<int> Pollution;

private:
    bool ban;
    bool I,O;


};

#endif // CHIPCELL_H
