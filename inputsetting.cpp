#include "inputsetting.h"
#include "ui_inputsetting.h"

InputSetting::InputSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputSetting)
{
    ui->setupUi(this);
}

InputSetting::~InputSetting()
{
    delete ui;
}

void InputSetting::on_buttonBox_accepted()
{
    emit InputAdded(ui->plainTextEdit_IR->toPlainText().toInt(),ui->plainTextEdit_IC->toPlainText().toInt());
}
