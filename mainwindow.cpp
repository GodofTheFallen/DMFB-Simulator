#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSound>
#include <QTimer>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(690,750);
    SZ = new FormSize(this);
    CMDDir = nullptr;
    CMDList = new CommandQueue(this);

    SysC = new SysCore(this);
    connect(this,&MainWindow::SysStart,SysC,&SysCore::SysCoreStart);
    connect(SysC,&SysCore::PlayFinish,this,&MainWindow::StopPlay);

    Timer = new QTimer(this);
    connect(Timer,&QTimer::timeout,this,&MainWindow::PaintNext);

    WTimer1 = new QTimer(this);
    WTimer2 = new QTimer(this);

    Playing = false;

    PaintBox = new ChipPaintBox(this);
    PaintBox->setGeometry(20,80,650,650);
    PaintBox->setFixedSize(650,650);
    PaintBox->layout();
    connect(this, &MainWindow::PaintNow, PaintBox, &ChipPaintBox::PaintNow);

    connect(PaintBox,&ChipPaintBox::MouseClick,this,&MainWindow::SetBan);
    connect(SysC,&SysCore::BanSet,this,&MainWindow::RepaintBan);

    ErrDisplay = new QErrorMessage(this);
    ErrDisplay->setWindowTitle("Error!");
    ErrDisplay->setWindowIcon(QIcon(":/assert/error.ico"));
    connect(SZ, SIGNAL(SendErrMsg(const QString&)) ,ErrDisplay, SLOT(showMessage(const QString &)));

    connect(this,&MainWindow::WashStatusChanged,this,&MainWindow::ResetWash);
    connect(this,&MainWindow::WashStatusChanged,this,&MainWindow::DisableUndo);
    connect(this,&MainWindow::ChangeFailed,ui->checkBox_Wash,&QCheckBox::setCheckState);


    //ui->groupBox_Control->setEnabled(false); //has set in UI
    connect(SZ, &FormSize::ValidityChanged, this, &MainWindow::ResetStartEnable);
    connect(CMDList, &CommandQueue::ValidityChanged, this, &MainWindow::ResetStartEnable);

    QPalette pal;
    pal.setColor(QPalette::Foreground, Qt::red);
    ui->label_Pol->setPalette(pal);
    ui->label_Pol->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::needWash() const
{
    return ui->checkBox_Wash->checkState()==Qt::Checked;
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
    if (SZ->isValid()) emit PaintNow(ChipInfo(*SZ,needWash()),false);
    else PaintBox->ClearPainting();
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
    ResetUndoNext();
}

bool MainWindow::IsReady()
{
    return SZ->isValid() && CMDList->isValid();
}

void MainWindow::ResetWash(Qt::CheckState NowStatus)
{
    if (!SZ->SetWashStatus(NowStatus)) emit ChangeFailed(Qt::Unchecked);
    if (SZ->isValid()) emit PaintNow(ChipInfo(*SZ,needWash()),false);
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

void MainWindow::ResetUndoNext()
{
    ui->pushButton_Undo->setEnabled(!SysC->DisUndo() && !needWash());
    ui->pushButton_Next->setEnabled(!SysC->DisNext());
    ui->pushButton_Play->setEnabled(!SysC->DisNext());
    ui->pushButton_Reload->setEnabled(true);
}

void MainWindow::PaintNext()
{
    try {
        SysC->PlayNext();
        PaintNow(SysC->DisplayCurrent(),false);

        if (SysC->DisplayCurrent().soundI) QSound::play(":/assert/INPUT.wav");
        if (SysC->DisplayCurrent().soundO) QSound::play(":/assert/OUTPUT.wav");
        if (SysC->DisplayCurrent().soundMv) QSound::play(":/assert/MOVE.wav");
        if (SysC->DisplayCurrent().soundS0) QSound::play(":/assert/SPLIT0.wav");
        if (SysC->DisplayCurrent().soundS1) QSound::play(":/assert/SPLIT1.wav");
        if (SysC->DisplayCurrent().soundM0) QSound::play(":/assert/MERGE0.wav");
        if (SysC->DisplayCurrent().soundM1) QSound::play(":/assert/MERGE1.wav");

        if (needWash()) ui->label_Pol->setVisible(SysC->DisplayCurrent().CheckPollution());
        if (needWash()) WTimer1->singleShot(WashT1,this,&MainWindow::ShowCurWash);
        else if (!Playing) ResetUndoNext();
    }
    catch (QString &ErrMsg) {
        if (Timer->isActive()) StopPlay();
        ErrDisplay->showMessage(ErrMsg);
        ui->pushButton_Next->setEnabled(false);
        ui->pushButton_Undo->setEnabled(false);
        ui->pushButton_Reload->setEnabled(true);
        return;
    }
}

void MainWindow::StopPlay()
{
    Timer->stop();
    Playing = false;
    ResetUndoNext();
}

void MainWindow::ShowCurWash()
{
    SysC->WashCur();
    PaintNow(SysC->DisplayCurrent(),true);
    WTimer2->singleShot(WashT2,this,&MainWindow::ShowCurOri);
}

void MainWindow::ShowCurOri()
{
    PaintNow(SysC->DisplayCurrent(),false);
    if (!Playing) ResetUndoNext();
}

void MainWindow::SetBan(POS P)
{
    if (Timer->isActive()) return;
    if (WTimer1->isActive()) return;
    if (WTimer2->isActive()) return;
    SysC->SetBan(P);
}

void MainWindow::RepaintBan()
{
    PaintNow(SysC->DisplayCurrent(),false);
}

void MainWindow::on_pushButton_Undo_clicked()
{
    ui->pushButton_Undo->setEnabled(false);
    ui->pushButton_Next->setEnabled(false);
    ui->pushButton_Play->setEnabled(false);
    ui->pushButton_Reload->setEnabled(false);
    SysC->Undo();
    emit PaintNow(SysC->DisplayCurrent(),false);
    ResetUndoNext();
}

void MainWindow::on_pushButton_Next_clicked()
{
    ui->pushButton_Undo->setEnabled(false);
    ui->pushButton_Next->setEnabled(false);
    ui->pushButton_Play->setEnabled(false);
    ui->pushButton_Reload->setEnabled(false);
    PaintNext();
}

void MainWindow::on_pushButton_Play_clicked()
{
    ui->pushButton_Undo->setEnabled(false);
    ui->pushButton_Next->setEnabled(false);
    ui->pushButton_Play->setEnabled(false);
    ui->pushButton_Reload->setEnabled(false);
    Playing = true;
    Timer->start(IdleTime);
}

void MainWindow::on_pushButton_Reload_clicked()
{
    SysC->reset();
    ui->groupBox_Setting->setEnabled(true);
    ui->groupBox_Control->setEnabled(false);
    ui->pushButton_Start->setEnabled(true);
    CMDList->LoadCMDDir(CMDDir);
    ui->label_Pol->setVisible(false);
    emit PaintNow(ChipInfo(*SZ,needWash()),false);
}
