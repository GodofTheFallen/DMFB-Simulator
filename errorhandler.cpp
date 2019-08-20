#include "errorhandler.h"

ErrorHandler::ErrorHandler(QObject *parent) : QObject(parent)
{

}

int ErrorHandler::ErrorReceive(std::string ErrMsg)
{
    emit Err2Display(ErrMsg);
    return 1;
}
