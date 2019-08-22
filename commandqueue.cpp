#include "commandqueue.h"


void CommandQueue::Add_Command(const Command &CMD)
{
    Q->push(CMD);
}

void CommandQueue::LoadFromFile(QFile *CMDDir)
{
    if (Q) delete Q;
    Q = new std::priority_queue <Command,std::vector<Command>,std::greater<Command>>;
    if (!CMDDir->open(QIODevice::ReadOnly)) throw QString("Fail to open file.");
    QString AllIn = CMDDir->readAll();
    AllIn.replace("\n","");
    AllIn.replace("\r","");
    AllIn.replace(";"," ");
    AllIn.replace(','," ");
    QStringList L = AllIn.split(' ');
    int i=0;
#define ERR throw(QString("Command format error"))
    auto NX=[=](int& i){if (i+1>=L.size()) ERR; return ++i;};
    auto getint=[&](){bool ok;int tmp = L[NX(i)].toInt(&ok);if (!ok) ERR;return tmp;};
    while (i<L.size()) {
        if (L[i] == "") {++i;continue;}
        if (L[i] == "Input") {
            int t = getint();
            int x = getint();
            int y = getint();
            Add_Command(Command(CMDType::INPUT,t,std::make_pair(x,y),std::make_pair(0,0)));
        }
        else if (L[i] == "Move") {
            int t = getint();
            int x1 = getint();
            int y1 = getint();
            int x2 = getint();
            int y2 = getint();
            //if (abs(x1-x2)+abs(y1-y2)!=1) ERR;
            Add_Command(Command(CMDType::MOVE,t,std::make_pair(x1,y1),std::make_pair(x2,y2)));
        }
        else if (L[i] == "Mix") {
            int t = getint();
            int x1 = getint();
            int y1 = getint();
            while (1) {
                if (i+1>=L.size()) ERR;
                bool ok; L[i+1].toInt(&ok); if (!ok) break;
                int x2 = getint();
                int y2 = getint();
                //if (abs(x1-x2)+abs(y1-y2)!=1) ERR;
                Add_Command(Command(CMDType::MOVE,t,std::make_pair(x1,y1),std::make_pair(x2,y2)));
                ++t;x1=x2;y1=y2;
            }
        }
        else if (L[i] == "Split") {
            int t = getint();
            int x1 = getint();
            int y1 = getint();
            int x2 = getint();
            int y2 = getint();
            int x3 = getint();
            int y3 = getint();
            if (x1*2!=x2+x3 || y1*2!=y2+y3)
                QEM->showMessage("There's a Split command with wrong (x1,y1), but may not cause fatal error.");
            //if (!(abs(x2-x3)==2 && (x2+x3)/2==x1 && y1==y2 && y1==y3) &&
            //        !(abs(y2-y3)==2 && (y2+y3)/2==y1 && x1==x2 && x1==x3)) ERR;
            Add_Command(Command(CMDType::SPLIT0,t,std::make_pair(x2,y2),std::make_pair(x3,y3)));
            Add_Command(Command(CMDType::SPLIT1,t+1,std::make_pair(x2,y2),std::make_pair(x3,y3)));
        }
        else if (L[i] == "Merge") {
            int t = getint();
            int x1 = getint();
            int y1 = getint();
            int x2 = getint();
            int y2 = getint();
            //if (!(abs(x1-x2)==2 && y1==y2) && !(abs(y1-y2)==2 && x1==x2)) ERR;
            Add_Command(Command(CMDType::MERGE0,t,std::make_pair(x1,y1),std::make_pair(x2,y2)));
            Add_Command(Command(CMDType::MERGE1,t+1,std::make_pair(x1,y1),std::make_pair(x2,y2)));
        }
        else if (L[i] == "Output") {
            int t = getint();
            int x = getint();
            int y = getint();
            Add_Command(Command(CMDType::OUTPUT,t,std::make_pair(x,y),std::make_pair(0,0)));
        }
        else ERR;
        ++i;
    }
#undef NX
#undef ERR
}

CommandQueue::CommandQueue(QObject *parent) : QObject (parent)
{
    Validity = false;
    Q = nullptr;
    QEM =new QErrorMessage;
    QEM->setWindowTitle("Error!");
}

CommandQueue::~CommandQueue()
{
    delete QEM;
}

bool CommandQueue::isValid()
{
    return Validity;
}

void CommandQueue::LoadCMDDir(QFile *CMDDir)
{
    bool _Val = true;
    if (!CMDDir)
    try {
        LoadFromFile(CMDDir);
    }
    catch (QString& ErrMsg){
        QEM->showMessage(ErrMsg);
        _Val = false;
    }
    if (_Val != Validity) emit ValidityChanged(Validity=_Val);
}
