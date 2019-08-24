#ifndef SYSCORE_H
#define SYSCORE_H

#include <QObject>
#include <QErrorMessage>
#include <QFile>
#include "formsize.h"
#include "commandqueue.h"
#include "chipinfo.h"


class SysCore:public QObject
{
    Q_OBJECT

public:
    SysCore(QObject *parent = nullptr);
    ~SysCore();
    bool needWash;
    bool DisUndo() const {return CurrentStep == 0;}
    bool DisNext() const {return CurrentStep == TotalStep && Fin;}
    ChipInfo & DisplayCurrent();
    void WashCur();

signals:
    void PlayFinish();
    void ErrorOccur(QString);

public slots:
    void SysCoreStart(FormSize *SZ, CommandQueue *CMDQ, bool _nW);
    void reset();
    void Finish();
    void Undo();
    void PlayNext();

private:
    QList<ChipInfo> *CQ;
    CommandQueue *CMDQ;
    int TotalStep, CurrentStep;
    bool Fin;
    void CMDInput(Command);
    void CMDOutput(Command);
    void CMDMove(Command);
    void CMDSplit0(Command);
    void CMDSplit1(Command);
    void CMDMerge0(Command);
    void CMDMerge1(Command);
};

#endif // SYSCORE_H
