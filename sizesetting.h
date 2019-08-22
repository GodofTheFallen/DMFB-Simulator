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
    int TR,TC,OR,OC;
    QList<POS> I;
public:
    explicit SizeSetting(QWidget *parent = nullptr);
    ~SizeSetting();
signals:
    void MsgAccepted(int TR, int TC, QList<POS> I, POS O);

public slots:
private slots:
    void on_buttonBox_accepted();
    void on_pushButton_addI_clicked();
    void on_pushButton_delI_clicked();
    void Add_Inport(int, int);

};

#endif // SIZESETTING_H
