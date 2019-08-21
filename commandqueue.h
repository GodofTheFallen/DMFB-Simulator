#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "command.h"
#include <QFileDialog>
#include <queue>
#include <QFile>
#include <QObject>

class CommandQueue:public QObject
{
    Q_OBJECT

private:
    std::priority_queue <Command,std::vector<Command>,std::greater<Command>> Q;
    QFile *CMDDir;
    bool Validity;
    bool _SafetyCheck();
    void Add_Command(const Command&);
    void LoadFromFile();

public:
    CommandQueue(QObject* parent = nullptr);
    void CheckValidity();
    bool isValid();

signals:
    void ValidityChanged(bool);

public slots:
    void SetCMDDir();
};

#endif // COMMANDQUEUE_H
