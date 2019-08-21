#ifndef SYSCORE_H
#define SYSCORE_H

#include <QObject>
#include <QErrorMessage>
#include <QFile>
#include "formsize.h"
#include "commandqueue.h"

class SysCore:public QObject
{
    Q_OBJECT

public:
    SysCore(QObject *parent = nullptr);
    ~SysCore();
    bool IsReady();
    CommandQueue *CMDList;
    FormSize *SZ;

signals:
    void DisplayTable();
    void ChangeFailed(Qt::CheckState);
public slots:
    void ResetWash(int);
    void SetFormSize(int , int , int, int, int, int, int);
};

#endif // SYSCORE_H
