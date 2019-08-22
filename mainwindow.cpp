#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    SZ = new FormSize(this);
    CMDDir = nullptr;
    CMDList = new CommandQueue(this);
    SysC = new SysCore(this);


    PaintBox = new QGraphicsView(this);
    PaintBox->setGeometry(20,80,650,650);
    PaintBox->setFixedSize(650,650);
    PaintBox->layout();


    ErrDisplay = new QErrorMessage(this);
    ErrDisplay->setWindowTitle("Error!");
    connect(SZ, SIGNAL(SendErrMsg(const QString&)) ,ErrDisplay, SLOT(showMessage(const QString &)));
    //connect(SysC->SZ, &FormSize::SendErrMsg ,ErrDisplay, &QErrorMessage::showMessage);

    connect(this,&MainWindow::WashStatusChanged,this,&MainWindow::ResetWash);
    connect(this,&MainWindow::WashStatusChanged,this,&MainWindow::DisableUndo);
    connect(this,&MainWindow::ChangeFailed,ui->checkBox_Wash,&QCheckBox::setCheckState);


    //ui->groupBox_Control->setEnabled(false); //has set in UI
    connect(SZ, &FormSize::ValidityChanged, this, &MainWindow::ResetStartEnable);
    connect(CMDList, &CommandQueue::ValidityChanged, this, &MainWindow::ResetStartEnable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_Wash_stateChanged(int NowStatus)
{
    //qDebug() << NowStatus;
    emit WashStatusChanged(Qt::CheckState(NowStatus));
}

void MainWindow::on_pushButton_SetSize_clicked()
{
    SizeSetting *SizeSettingDiag = new SizeSetting(this);
    connect(SizeSettingDiag,&SizeSetting::MsgAccepted,this,&MainWindow::SetBasicFormSize);
    SizeSettingDiag->exec();
    delete SizeSettingDiag;
}

void MainWindow::SetBasicFormSize(int TR, int TC, QList<POS> I, POS O)
{
    ui->checkBox_Wash->setCheckState(Qt::Unchecked);
    SetFormSize(TR,TC,I,O,0);
}

void MainWindow::ResetStartEnable()
{
    bool EAB = IsReady();
    ui->pushButton_Start->setEnabled(EAB);
}

void MainWindow::on_pushButton_Load_clicked()
{
    if (SZ->isValid()) SetCMDDir();
    else ErrDisplay->showMessage("Please set size first");
}

void MainWindow::DisableUndo(Qt::CheckState NowState)
{
    ui->pushButton_Undo->setEnabled(NowState==Qt::Unchecked);
}

void MainWindow::on_pushButton_Start_clicked()
{
    ui->groupBox_Setting->setEnabled(false);
    ui->groupBox_Control->setEnabled(true);
    ui->pushButton_Start->setEnabled(false);
    emit SysStart(SZ,CMDList,ui->checkBox_Wash->checkState());
}

bool MainWindow::IsReady()
{
    return SZ->isValid() && CMDList->isValid();
}

void MainWindow::ResetWash(Qt::CheckState NowStatus)
{
    if (!SZ->SetWashStatus(NowStatus)) emit ChangeFailed(Qt::Unchecked);
}

void MainWindow::SetFormSize(int _r, int _c, QList<POS> _I, POS _O,int _NW)
{
    SZ->ChangeFormSize(_r,_c,_I,_O,_NW);
}

void MainWindow::SetCMDDir()
{
    if (CMDDir) delete CMDDir;
    CMDDir = new QFile(QFileDialog::getOpenFileName());
    CMDList->LoadCMDDir(CMDDir);
}

void MainWindow::on_pushButton_Undo_clicked()
{

}

void MainWindow::on_pushButton_Next_clicked()
{

}

void MainWindow::on_pushButton_Play_clicked()
{

}

void MainWindow::on_pushButton_Reload_clicked()
{
    //SysC->Clear();
    ui->groupBox_Setting->setEnabled(true);
    ui->groupBox_Control->setEnabled(false);
    ui->pushButton_Start->setEnabled(true);
    CMDList->LoadCMDDir(CMDDir);
}
