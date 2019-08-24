#include "chipinfo.h"

#include <QQueue>

ChipInfo::ChipInfo(const FormSize & SZ,bool _nW)
{
    Row = SZ.getRow();
    Col = SZ.getCol();

    needWash = _nW;

    Wash = SZ.getWash();
    Waste = SZ.getWaste();

    for (int i = 1; i <= Row; ++i)
        for (int j = 1; j <= Col; ++j)
            CT.append(ChipCell());

    for (auto P : SZ.Input)
        getCell(P)->setInput();

    getCell(SZ.Output)->setOutput();
}

int ChipInfo::FindDrop(const POS &_P)
{
    for (int i = 0; i < EQ.size(); ++i) {
        if (EQ.at(i).getP()==_P) return i;
    }
    return -1;
}

ChipCell* ChipInfo::getCell(const POS & _P)
{
    if (!(1 <= _P.first && _P.first <= Row && 1 <= _P.second && _P.second <= Col)) return nullptr;
    return &CT[(_P.first-1)*Col+(_P.second-1)];

}

void ChipInfo::cleanAfterCopy()
{
    soundMv = soundS0 = soundS1 = soundM0 = soundM1 = false;

    for (auto &i : CT) i.cantWash = i.shouldWash = false;

}

void ChipInfo::getCanWash()
{
    if (!needWash) return;

    for (int i = 1; i <= Row; ++i)
        for (int j = 1; j <= Col; ++j) {
            ChipCell *cc = getCell(std::make_pair(i,j));
            if (cc->banWash()) cc->cantWash = true;
        }
    // ban ban

    for (int i = 0; i < EQ.size(); ++i) {
        int U, V;
        switch (EQ[i].getType()) {
        case DropType::Dot:
            U = V = 1; break;
        case DropType::Hor:
            U = 2; V = 1; break;
        case DropType::Per:
            U = 1; V = 2; break;
        }
        POS cur = EQ[i].getP();
        for (int u = -U; u <= V; ++u)
            for (int v = -V; v <= V; ++v)
            {
                ChipCell *cc = getCell(std::make_pair(cur.first + u, cur.second + v));
                if (cc) cc->cantWash = true;
            }
    }
    // ban drop

    QQueue<POS> WQ;

    WQ.push_back(Wash);
    while (!WQ.empty()) {
        POS cur = WQ.front();
        WQ.pop_front();
        POS P[4];
        P[0] = std::make_pair(cur.first,cur.second + 1);
        P[1] = std::make_pair(cur.first,cur.second - 1);
        P[2] = std::make_pair(cur.first - 1,cur.second);
        P[3] = std::make_pair(cur.first + 1,cur.second);
        for (int i = 0; i < 4; ++i) {
            ChipCell *cc = getCell(P[i]);
            if (!cc) continue;
            if (!cc->cantWash && !cc->shouldWash) {
                cc->shouldWash = true;
                WQ.push_back(P[i]);
            }
        }
    }
    if (!getCell(std::make_pair(Row,Col))->shouldWash) {
        for (int i = 1; i <= Row; ++i)
            for (int j = 1; j <= Col; ++j) {
                ChipCell *cc = getCell(std::make_pair(i,j));
                cc->shouldWash = false;
            }
    }

    for (int i = 1; i <= Row; ++i)
        for (int j = 1; j <= Col; ++j) {
            ChipCell *cc = getCell(std::make_pair(i,j));
            if (cc->shouldWash) cc->Pollution.clear();
        }
}

bool ChipInfo::CheckStatic()
{
    for (int i = 0; i < EQ.size(); ++i) {
        bool pol[FormSize::MAX_SIZE+2][FormSize::MAX_SIZE+2];
        memset(pol,0,sizeof(pol));
        for (int j = 0; j < EQ.size(); ++j) {
            auto CV = [=](int x, int y) {return (1 <= x && x <= Row && 1 <= y && y <= Col);};
            if (j == i) continue;
            int U, V;
            switch (EQ[j].getType()) {
            case DropType::Dot:
                U = V = 1; break;
            case DropType::Hor:
                U = 2; V = 1; break;
            case DropType::Per:
                U = 1; V = 2; break;
            }
            POS cur = EQ[j].getP();
            for (int u = -U; u <= U; ++u)
                for (int v = -V; v <= V; ++v)
                    if (CV(cur.first+u,cur.second+v)) pol[cur.first+u][cur.second+v] = true;
        }
        if (pol[EQ[i].getP().first][EQ[i].getP().second]) return false;
        if (EQ[i].getType() == DropType::Hor) {
            if (pol[EQ[i].getP().first - 1][EQ[i].getP().second]) return false;
            if (pol[EQ[i].getP().first + 1][EQ[i].getP().second]) return false;
        }
        if (EQ[i].getType() == DropType::Per) {
            if (pol[EQ[i].getP().first][EQ[i].getP().second - 1]) return false;
            if (pol[EQ[i].getP().first][EQ[i].getP().second + 1]) return false;
        }
    }
    return true;
}

bool ChipInfo::CheckPollution()
{
    for (auto cur : EQ) {
        ChipCell *cc = getCell(cur.getP());
        if (cc->Pollution.size() > 1) return 1;
    }
    return 0;
}
