#ifndef SIZESETTING_H
#define SIZESETTING_H

#include <QDialog>

namespace Ui {
class SizeSetting;
}

class SizeSetting : public QDialog
{
    Q_OBJECT
private:
    Ui::SizeSetting *ui;
public:
    explicit SizeSetting(QWidget *parent = nullptr);
signals:

public slots:
};

#endif // SIZESETTING_H
