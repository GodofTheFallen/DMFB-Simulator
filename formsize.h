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
    QList<POS> Input;
    POS Output;
    POS Wash,Waste;
    void SafetyCheck(); //Will emit signal ValidityChanged
    bool _SafetyCheck();

signals:
    void SendErrMsg(const QString &); //If setting is invalid, emit error signal
    void ValidityChanged(bool);

public:
    static const int MAX_SIZE = 12; //Max Size
    FormSize(QObject *parent = nullptr):QObject (parent),Row(0),Col(0) {Validity = false;}
    bool isValid() const; //if not valid, refuse to paint

public slots:
    void ChangeFormSize(int _r, int _c, QList<POS> _I, POS _O, bool NeedWash);
    bool SetWashStatus(Qt::CheckState);
};

#endif // FORMSIZE_H
