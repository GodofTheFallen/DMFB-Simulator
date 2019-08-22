#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "command.h"
#include <QFileDialog>
#include <QErrorMessage>
#include <queue>
#include <QFile>
#include <QObject>

class CommandQueue:public QObject
{
    Q_OBJECT

private:
    std::priority_queue <Command,std::vector<Command>,std::greater<Command>> *Q;

    bool Validity;
    void Add_Command(const Command&);
    void LoadFromFile(QFile *CMDDir);
    QErrorMessage *QEM;

public:
    CommandQueue(QObject* parent = nullptr);
    ~CommandQueue();
    bool isValid();

signals:
    void ValidityChanged(bool);

public slots:
    void LoadCMDDir(QFile *CMDDir);
};

#endif // COMMANDQUEUE_H
