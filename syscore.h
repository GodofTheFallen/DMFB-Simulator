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

signals:

public slots:
    //void SysCoreStart(FormSize *SZ,CommandQueue *CMDQ);
    //void reset();
};

#endif // SYSCORE_H
