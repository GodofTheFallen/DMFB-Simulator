#ifndef COMMAND_H
#define COMMAND_H

#include <algorithm>

//Simply record commands (by steps)
typedef enum{MOVE,SPLIT0,SPLIT1,MERGE0,MERGE1,INPUT,OUTPUT} CMDType;
//The 'MIX' command will be splited up into 'MOVE's
typedef std::pair<int,int> POS;

class Command
{
private:
    CMDType type;
    int tick;
    POS P0,P1;
    /*
     * For MOVE, P0 = before, P1 = after
     * For SPLICT & MERGE, P0, P1 = 2 side POS, check validity in commandqueue
    */
public:
    Command(CMDType, int, POS, POS);
    bool operator < (const Command& another) const{return this->tick<another.tick;}
    bool operator > (const Command& another) const{return this->tick>another.tick;}
    int GetTick() const;
    CMDType getType() const;
    POS GetP0() const;
    POS GetP1() const;
};

#endif // COMMAND_H
