#ifndef SYSCORE_H
#define SYSCORE_H

#include <QObject>
#include "commandqueue.h"

class SysCore:QObject
{
    Q_OBJECT
private:
    FormSize SZ;
    CommandQueue CMDList;
public:
    SysCore(QObject *parent = nullptr):QObject(parent){}
signals:
    void DisplayTable();
public slots:
    void SetFormSize(int,int,int,int);
};

#endif // SYSCORE_H
