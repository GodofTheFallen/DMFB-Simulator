#include "commandqueue.h"
#include <QErrorMessage>

bool CommandQueue::_SafetyCheck()
{
    if (!CMDDir) return false;
    return true;
}

void CommandQueue::Add_Command(const Command &CMD)
{
    Q.push(CMD);
}

void CommandQueue::LoadFromFile()
{

}

CommandQueue::CommandQueue(QObject *parent) : QObject (parent)
{
    Validity = false;
    CMDDir = nullptr;
}

bool CommandQueue::isValid()
{
    return Validity;
}

void CommandQueue::CheckValidity()
{
    bool _Val = _SafetyCheck();
    if (_Val != Validity) emit ValidityChanged(Validity=_Val);
}

void CommandQueue::SetCMDDir()
{
    if (CMDDir) delete CMDDir;
    CMDDir = new QFile(QFileDialog::getOpenFileName());
    try {
        LoadFromFile();

    }
    catch (QString& ErrMsg){
        QErrorMessage().showMessage(ErrMsg);
        CMDDir = nullptr;
    }
    CheckValidity();
}
