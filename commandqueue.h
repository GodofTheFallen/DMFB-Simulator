#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "command.h"
#include <queue>

class CommandQueue
{
private:
    std::priority_queue <Command,std::vector<Command>,std::greater<Command>> Q;
public:
    CommandQueue();
    void Add_Command(const Command&);
};

#endif // COMMANDQUEUE_H
