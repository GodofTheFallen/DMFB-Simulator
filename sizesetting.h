#ifndef SIZESETTING_H
#define SIZESETTING_H

#include <QDialog>
#include "formsize.h"

namespace Ui {
class SizeSetting;
}

class SizeSetting : public QDialog
{
    Q_OBJECT
private:
    Ui::SizeSetting *ui;
    int TR,TC,IR,IC,OR,OC;
public:
    explicit SizeSetting(QWidget *parent = nullptr);
    ~SizeSetting();
signals:
    void MsgAccepted(int,int,int,int,int,int);
public slots:
private slots:
    void on_buttonBox_accepted();
};

#endif // SIZESETTING_H
