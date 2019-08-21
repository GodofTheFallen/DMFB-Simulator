#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SysC = new SysCore(this);
    ErrDisplay = new QErrorMessage(this);
    ErrDisplay->setWindowTitle("Error!");
    //connect(SysC->SZ, &FormSize::SendErrMsg ,ErrDisplay, &QErrorMessage::showMessage);
    connect(SysC->SZ, SIGNAL(SendErrMsg(const QString&)) ,ErrDisplay, SLOT(showMessage(const QString &)));
    connect(this,&MainWindow::WashStatusChanged,SysC,&SysCore::ResetWash);
    connect(SysC,&SysCore::ChangeFailed,ui->checkBox_Wash,&QCheckBox::setCheckState);
    ui->groupBox_Control->setEnabled(false);
    connect(SysC->SZ, &FormSize::ValidityChanged, this, &MainWindow::ResetControlEnable);
    connect(SysC->CMDList, &CommandQueue::ValidityChanged, this, &MainWindow::ResetControlEnable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_Wash_stateChanged(int NowStatus)
{
    qDebug() << NowStatus;
    emit WashStatusChanged(NowStatus);
}

void MainWindow::on_pushButton_SetSize_clicked()
{
    SizeSetting *SizeSettingDiag = new SizeSetting(this);
    connect(SizeSettingDiag,&SizeSetting::MsgAccepted,this,&MainWindow::SetBasicFormSize);
    SizeSettingDiag->exec();
    delete SizeSettingDiag;
}

void MainWindow::SetBasicFormSize(int TR, int TC, int IR, int IC, int OR, int OC)
{
    ui->checkBox_Wash->setCheckState(Qt::Unchecked);
    SysC->SetFormSize(TR,TC,IR,IC,OR,OC,0);
}

void MainWindow::ResetControlEnable()
{
    bool EAB = SysC->IsReady();
    ui->groupBox_Control->setEnabled(EAB);
}

void MainWindow::on_pushButton_Load_clicked()
{
    if (SysC->SZ->isValid()) SysC->CMDList->SetCMDDir();
    else ErrDisplay->showMessage("Please set size first");
}
