#ifndef INPUTSETTING_H
#define INPUTSETTING_H

#include <QDialog>

namespace Ui {
class InputSetting;
}

class InputSetting : public QDialog
{
    Q_OBJECT

public:
    explicit InputSetting(QWidget *parent = nullptr);
    ~InputSetting();

private slots:
    void on_buttonBox_accepted();

signals:
    void InputAdded(int, int);

private:
    Ui::InputSetting *ui;
};

#endif // INPUTSETTING_H
