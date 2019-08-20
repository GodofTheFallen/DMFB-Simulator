#ifndef FORMSIZE_H
#define FORMSIZE_H

#include <QObject>
#include <algorithm>
typedef std::pair<int,int> POS;

class FormSize:public QObject
{
    Q_OBJECT
private:
    bool Validity;
    int Row,Col;
    POS Input,Output;
    POS Wash,Waste;
    bool SafetyCheck();
public:
    FormSize(int _r, int _c, POS _I, POS _O, bool NeedWash);
    bool isValid() const;
signals:
    void SendErrMsg(std::string);
};

#endif // FORMSIZE_H
