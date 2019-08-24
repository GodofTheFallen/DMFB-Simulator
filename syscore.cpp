#include "syscore.h"


SysCore::SysCore(QObject *parent):QObject(parent){
    CQ = nullptr;
    CMDQ = nullptr;
    needWash = false;
    TotalStep = CurrentStep =0;
    Fin = false;
}

SysCore::~SysCore()
{

}

ChipInfo &SysCore::DisplayCurrent()
{
    return CQ->operator[](CurrentStep);
}

void SysCore::WashCur()
{
    CQ->operator[](CurrentStep).getCanWash();
}

void SysCore::SysCoreStart(FormSize *SZ, CommandQueue *_CMDQ, bool _nW)
{
    needWash = _nW;
    Fin = false;
    CQ = new QList<ChipInfo>;
    CQ->append(ChipInfo(*SZ,_nW));
    CMDQ = _CMDQ;
    TotalStep = 0;
    CurrentStep = 0;
    connect(CMDQ, &CommandQueue::CommandFinish, this, &SysCore::Finish);
}

void SysCore::reset()
{
    delete CQ;
    CMDQ = nullptr;
    needWash = false;
    TotalStep = 0;
    CurrentStep = 0;
    Fin = false;
}

void SysCore::Finish()
{
    Fin = true;
    emit PlayFinish();
}

void SysCore::Undo()
{
    if (!needWash && CurrentStep) --CurrentStep;
}

void SysCore::PlayNext()
{
    ++CurrentStep;
    if (CurrentStep <= TotalStep) {
        if (Fin && CurrentStep == TotalStep) emit PlayFinish();
        return;
    }
    CQ->append(CQ->at(TotalStep));
    ++TotalStep;
    CQ->operator[](TotalStep).cleanAfterCopy();
    while (!CMDQ->empty() && CMDQ->top().GetTick() == CurrentStep) {
        Command curCMD = CMDQ->top();
        CMDType Type = curCMD.getType();
        switch (Type) {
        case CMDType::INPUT:
            CMDInput(curCMD); break;
        case CMDType::OUTPUT:
            CMDOutput(curCMD); break;
        case CMDType::MOVE:
            CMDMove(curCMD); break;
        case CMDType::SPLIT0:
            CMDSplit0(curCMD); break;
        case CMDType::SPLIT1:
            CMDSplit1(curCMD); break;
        case CMDType::MERGE0:
            CMDMerge0(curCMD); break;
        case CMDType::MERGE1:
            CMDMerge1(curCMD); break;
        }
        CMDQ->pop();
    }
    bool Sta = CQ->operator[](CurrentStep).CheckStatic();
    if (!Sta) throw QString("Static check failed!");
}

void SysCore::SetBan(POS P)
{
    if (!needWash) return;
    if (!CQ) return;
    CQ->operator[](CurrentStep).getCell(P)->setBan();
    emit BanSet();
}

void SysCore::CMDInput(Command curCMD)
{
    ChipCell *p0 = CQ->operator[](CurrentStep).getCell(curCMD.GetP0());
    if (!p0 || !p0->isInput()) throw QString("Invalid Input: No Input here!");

    bool pol[FormSize::MAX_SIZE+2][FormSize::MAX_SIZE+2];
    memset(pol,0,sizeof(pol));
    ChipInfo *CI = &CQ->operator[](CurrentStep -1);
    for (auto ele : CI->EQ) {
        auto CV = [=](int x, int y) {return (1 <= x && x <= CI->Row && 1 <= y && y <= CI->Col);};
        int U, V;
        switch (ele.getType()) {
        case DropType::Dot:
            U = V = 1; break;
        case DropType::Hor:
            U = 2; V = 1; break;
        case DropType::Per:
            U = 1; V = 2; break;
        }
        POS cur = ele.getP();
        for (int u = -U; u <= V; ++u)
            for (int v = -V; v <= V; ++v)
                if (CV(cur.first+u,cur.second+v)) pol[cur.first+u][cur.second+v] = true;
    }
    if (pol[curCMD.GetP0().first][curCMD.GetP0().second]) throw QString("Invalid Input: Dynamic check failed!");

    CQ->operator[](CurrentStep).EQ.append(Element(curCMD.GetP0(),DropType::Dot));
    p0->Pollution.insert(Element::__Total);
    CQ->operator[](CurrentStep).soundI = true;
}

void SysCore::CMDOutput(Command curCMD)
{
    ChipCell *p0 = CQ->operator[](CurrentStep).getCell(curCMD.GetP0());
    if (!p0 || !p0->isOutput()) throw QString("Invalid Output: No Output here!");

    ChipInfo *CI = &CQ->operator[](CurrentStep -1);
    int i;
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getP() == curCMD.GetP0()) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Output: No Drop here!");
    int iid = CI->EQ[i].getID();
    CI = &CQ->operator[](CurrentStep);
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getID() == iid) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Output: No Drop here!");
    if (CI->EQ[i].getP()!=curCMD.GetP0()) throw QString("Invalid Output: No Drop here!");

    CQ->operator[](CurrentStep).EQ.removeAt(i);
    CQ->operator[](CurrentStep).soundO = true;
}

void SysCore::CMDMove(Command curCMD)
{
    ChipCell *p0 = CQ->operator[](CurrentStep).getCell(curCMD.GetP0());
    ChipCell *p1 = CQ->operator[](CurrentStep).getCell(curCMD.GetP1());
    if (!p0 || !p1) throw QString("Invalid Move: Move from/to invalid position!");

    ChipInfo *CI = &CQ->operator[](CurrentStep -1);
    int i;
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getP() == curCMD.GetP0()) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Move: No Drop here!");
    int iid = CI->EQ[i].getID();
    CI = &CQ->operator[](CurrentStep);
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getID() == iid) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Move: No Drop here!");
    if (CI->EQ[i].getP()!=curCMD.GetP0()) throw QString("Invalid Move: No Drop here!");

    Element *curele = &CQ->operator[](CurrentStep).EQ[i];
    if (curele->getType()!=DropType::Dot) throw QString("Invalid Move: The Drop is Splitting/Merging here!");

    CI = &CQ->operator[](CurrentStep -1);
    bool pol[FormSize::MAX_SIZE+2][FormSize::MAX_SIZE+2];
    memset(pol,0,sizeof(pol));
    for (auto ele : CI->EQ) {
        auto CV = [=](int x, int y) {return (1 <= x && x <= CI->Row && 1 <= y && y <= CI->Col);};
        if (ele.getID() == curele->getID()) continue;
        int U, V;
        switch (ele.getType()) {
        case DropType::Dot:
            U = V = 1; break;
        case DropType::Hor:
            U = 2; V = 1; break;
        case DropType::Per:
            U = 1; V = 2; break;
        }
        POS cur = ele.getP();
        for (int u = -U; u <= V; ++u)
            for (int v = -V; v <= V; ++v)
                if (CV(cur.first+u,cur.second+v)) pol[cur.first+u][cur.second+v] = true;
    }
    if (pol[curCMD.GetP1().first][curCMD.GetP1().second]) throw QString("Invalid Move: Dynamic check failed!");

    curele->SetP(curCMD.GetP1());
    p1->Pollution.insert(curele->getID());
    CQ->operator[](CurrentStep).soundMv = true;
}

void SysCore::CMDSplit0(Command curCMD)
{
    POS P2 = std::make_pair((curCMD.GetP0().first+curCMD.GetP1().first)/2,(curCMD.GetP0().second+curCMD.GetP1().second)/2);
    DropType DT;
    if (curCMD.GetP0().first != curCMD.GetP1().first) DT = Hor;
    else DT = Per;

    ChipCell *p = CQ->operator[](CurrentStep).getCell(P2);
    if (!p) throw QString("Invalid Split: Split at invalid position");
    p = CQ->operator[](CurrentStep).getCell(curCMD.GetP0());
    if (!p) throw QString("Invalid Split: Split at invalid position");
    p = CQ->operator[](CurrentStep).getCell(curCMD.GetP1());
    if (!p) throw QString("Invalid Split: Split at invalid position");

    ChipInfo *CI = &CQ->operator[](CurrentStep -1);
    int i;
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getP() == P2) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Split: No Drop here!");
    int iid = CI->EQ[i].getID();
    CI = &CQ->operator[](CurrentStep);
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getID() == iid) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Split: No Drop here!");
    if (CI->EQ[i].getP()!=P2) throw QString("Invalid Split: No Drop here!");

    Element *curele = &CQ->operator[](CurrentStep).EQ[i];

    bool pol[FormSize::MAX_SIZE+2][FormSize::MAX_SIZE+2];
    memset(pol,0,sizeof(pol));
    CI = &CQ->operator[](CurrentStep -1);
    for (auto ele : CI->EQ) {
        auto CV = [=](int x, int y) {return (1 <= x && x <= CI->Row && 1 <= y && y <= CI->Col);};
        if (ele.getID() == curele->getID()) continue;
        int U, V;
        switch (ele.getType()) {
        case DropType::Dot:
            U = V = 1; break;
        case DropType::Hor:
            U = 2; V = 1; break;
        case DropType::Per:
            U = 1; V = 2; break;
        }
        POS cur = ele.getP();
        for (int u = -U; u <= V; ++u)
            for (int v = -V; v <= V; ++v)
                if (CV(cur.first+u,cur.second+v)) pol[cur.first+u][cur.second+v] = true;
    }
    if (pol[curCMD.GetP0().first][curCMD.GetP0().second]) throw QString("Invalid Split: Dynamic check failed!");
    if (pol[curCMD.GetP1().first][curCMD.GetP1().second]) throw QString("Invalid Split: Dynamic check failed!");

    curele->SetType(DT);
    CQ->operator[](CurrentStep).soundS0 = true;
}

void SysCore::CMDSplit1(Command curCMD)
{
    POS P2 = std::make_pair((curCMD.GetP0().first+curCMD.GetP1().first)/2,(curCMD.GetP0().second+curCMD.GetP1().second)/2);
    DropType DT;
    if (curCMD.GetP0().first != curCMD.GetP1().first) DT = Hor;
    else DT = Per;

    ChipCell *p0 = CQ->operator[](CurrentStep).getCell(curCMD.GetP0());
    if (!p0) throw QString("Invalid Split: Split at invalid position");
    ChipCell *p1 = CQ->operator[](CurrentStep).getCell(curCMD.GetP1());
    if (!p1) throw QString("Invalid Split: Split at invalid position");

    ChipInfo *CI = &CQ->operator[](CurrentStep -1);
    int i;
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getP() == P2) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Split: No Drop here!");
    int iid = CI->EQ[i].getID();
    CI = &CQ->operator[](CurrentStep);
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getID() == iid) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Split: No Drop here!");
    if (CI->EQ[i].getType()!=DT || CI->EQ[i].getP()!=P2) throw QString("Invalid Split: No Drop here!");

    CQ->operator[](CurrentStep).EQ.append(Element(curCMD.GetP0(),DropType::Dot));
    p0->Pollution.insert(Element::__Total);
    CQ->operator[](CurrentStep).EQ.append(Element(curCMD.GetP1(),DropType::Dot));
    p1->Pollution.insert(Element::__Total);
    CI->EQ.removeAt(i);

    CQ->operator[](CurrentStep).soundS1 = true;
}

void SysCore::CMDMerge0(Command curCMD)
{
    POS P2 = std::make_pair((curCMD.GetP0().first+curCMD.GetP1().first)/2,(curCMD.GetP0().second+curCMD.GetP1().second)/2);
    DropType DT;
    if (curCMD.GetP0().first != curCMD.GetP1().first) DT = Hor;
    else DT = Per;

    ChipCell *p0 = CQ->operator[](CurrentStep).getCell(curCMD.GetP0());
    if (!p0) throw QString("Invalid Merge: Merge at invalid position");
    ChipCell *p1 = CQ->operator[](CurrentStep).getCell(curCMD.GetP1());
    if (!p1) throw QString("Invalid Merge: Merge at invalid position");


    ChipInfo *CI = &CQ->operator[](CurrentStep -1);
    int i0;
    for (i0 = 0; i0 < CI->EQ.size(); ++i0)
        if (CI->EQ[i0].getP() == curCMD.GetP0()) break;
    if (i0 >= CI->EQ.size()) throw QString("Invalid Merge: No Drop here!");
    int iid0 = CI->EQ[i0].getID();
    CI = &CQ->operator[](CurrentStep);
    for (i0 = 0; i0 < CI->EQ.size(); ++i0)
        if (CI->EQ[i0].getID() == iid0) break;
    if (i0 >= CI->EQ.size()) throw QString("Invalid Merge: No Drop here!");
    if (CI->EQ[i0].getP()!=curCMD.GetP0()) throw QString("Invalid Merge: No Drop here!");
    CI->EQ.removeAt(i0);

    CI = &CQ->operator[](CurrentStep -1);
    int i1;
    for (i1 = 0; i1 < CI->EQ.size(); ++i1)
        if (CI->EQ[i1].getP() == curCMD.GetP1()) break;
    if (i1 >= CI->EQ.size()) throw QString("Invalid Merge: No Drop here!");
    int iid1 = CI->EQ[i1].getID();
    CI = &CQ->operator[](CurrentStep);
    for (i1 = 0; i1 < CI->EQ.size(); ++i1)
        if (CI->EQ[i1].getID() == iid1) break;
    if (i1 >= CI->EQ.size()) throw QString("Invalid Merge: No Drop here!");
    if (CI->EQ[i1].getP()!=curCMD.GetP1()) throw QString("Invalid Merge: No Drop here!");
    CI->EQ.removeAt(i1);

    ChipCell *p = CQ->operator[](CurrentStep).getCell(P2);
    CI->EQ.append(Element(P2,DT));
    p->Pollution.insert(Element::__Total);
    CQ->operator[](CurrentStep).soundM0 = true;
}

void SysCore::CMDMerge1(Command curCMD)
{
    POS P2 = std::make_pair((curCMD.GetP0().first+curCMD.GetP1().first)/2,(curCMD.GetP0().second+curCMD.GetP1().second)/2);
    DropType DT;
    if (curCMD.GetP0().first != curCMD.GetP1().first) DT = Hor;
    else DT = Per;

    ChipInfo *CI = &CQ->operator[](CurrentStep -1);
    int i;
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getP() == P2) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Merge: No Drop here!");
    int iid = CI->EQ[i].getID();
    CI = &CQ->operator[](CurrentStep);
    for (i = 0; i < CI->EQ.size(); ++i)
        if (CI->EQ[i].getID() == iid) break;
    if (i >= CI->EQ.size()) throw QString("Invalid Merge: No Drop here!");
    if (CI->EQ[i].getType()!=DT || CI->EQ[i].getP()!=P2) throw QString("Invalid Merge: No Drop here!");

    Element *curele = &CQ->operator[](CurrentStep).EQ[i];
    curele->SetType(DropType::Dot);

    CQ->operator[](CurrentStep).soundM1 = true;
}


