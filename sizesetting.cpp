#include "sizesetting.h"
#include "ui_sizesetting.h"

SizeSetting::SizeSetting(QWidget *parent) : QDialog(parent),ui(new Ui::SizeSetting)
{
    ui->setupUi(this);
    ui->label_TR->setText("Total Rows (1-"+QString::number(FormSize::MAX_SIZE)+")");
    ui->label_TC->setText("Total Columns (1-"+QString::number(FormSize::MAX_SIZE)+")");
}

SizeSetting::~SizeSetting()
{
    delete ui;
}

void SizeSetting::on_buttonBox_accepted()
{
    TR=ui->plainTextEdit_TR->toPlainText().toInt();
    TC=ui->plainTextEdit_TC->toPlainText().toInt();
    IR=ui->plainTextEdit_IR->toPlainText().toInt();
    IC=ui->plainTextEdit_IC->toPlainText().toInt();
    OR=ui->plainTextEdit_OR->toPlainText().toInt();
    OC=ui->plainTextEdit_OC->toPlainText().toInt();
    emit MsgAccepted(TR,TC,IR,IC,OR,OC);
}
