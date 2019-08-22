#include "sizesetting.h"
#include "ui_sizesetting.h"
#include "inputsetting.h"

SizeSetting::SizeSetting(QWidget *parent) : QDialog(parent),ui(new Ui::SizeSetting)
{
    ui->setupUi(this);
    ui->label_TR->setText("Total Rows (1-"+QString::number(FormSize::MAX_SIZE)+")");
    ui->label_TC->setText("Total Columns (1-"+QString::number(FormSize::MAX_SIZE)+")");
    ui->label_ICount->setText(QString("Input Added: %1").arg(I.size()));
}

SizeSetting::~SizeSetting()
{
    delete ui;
}

void SizeSetting::on_buttonBox_accepted()
{
    TR=ui->plainTextEdit_TR->toPlainText().toInt();
    TC=ui->plainTextEdit_TC->toPlainText().toInt();
    OR=ui->plainTextEdit_OR->toPlainText().toInt();
    OC=ui->plainTextEdit_OC->toPlainText().toInt();
    emit MsgAccepted(TR,TC,I,std::make_pair(OR,OC));
}

void SizeSetting::on_pushButton_addI_clicked()
{
    InputSetting *IPS = new InputSetting(this);
    connect(IPS, &InputSetting::InputAdded, this, &SizeSetting::Add_Inport);
    IPS->exec();
    delete IPS;
}

void SizeSetting::on_pushButton_delI_clicked()
{
    I.clear();
    ui->label_ICount->setText(QString("Input Added: %1").arg(I.size()));
}

void SizeSetting::Add_Inport(int _IR, int _IC)
{
    I.append(std::make_pair(_IR,_IC));
    ui->label_ICount->setText(QString("Input Added: %1").arg(I.size()));
}
