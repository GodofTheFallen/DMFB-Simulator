#ifndef COMMAND_H
#define COMMAND_H

#include <algorithm>

//Simply record commands (by steps)
typedef enum{MOVE,SPLIT0,SPLIT1,MERGE0,MERGE1} CMDType;
//The 'MIX' command will be splited up into 'MOVE's
typedef std::pair<int,int> POS;

class Command
{
private:
    CMDType type;
    int tick;
    POS P0,P1;
public:
    Command(CMDType,POS,POS);
    bool operator < (const Command& another) const{return this->tick<another.tick;}
    int GetTick() const;
    POS GetP1() const;
    POS GetP2() const;
};

#endif // COMMAND_H