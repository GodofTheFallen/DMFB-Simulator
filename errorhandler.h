#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QObject>

class ErrorHandler : public QObject
{
    Q_OBJECT

public:
    explicit ErrorHandler(QObject *parent = nullptr);

signals:
    void Err2Display(std::string);

public slots:
    int ErrorReceive(std::string);
};

#endif // ERRORHANDLER_H
