#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SysC = new SysCore(this);
    connect(this,SIGNAL(WashStatusChanged(int)),SysC,SLOT(ResetWash(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_Wash_stateChanged(int NowStatus)
{
    emit WashStatusChanged(NowStatus);
}

void MainWindow::on_pushButton_SetSize_clicked()
{
    SizeSetting *SizeSettingDiag = new SizeSetting(this);
    SizeSettingDiag->exec();

}
