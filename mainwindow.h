#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QGraphicsView>
#include <QMediaPlayer>
#include "syscore.h"
#include "sizesetting.h"
#include "chippaintbox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

    QErrorMessage *ErrDisplay;

    bool IsReady();

    bool Playing;

    SysCore* SysC;

    ChipPaintBox *PaintBox;

    QMediaPlayer *SoundPlayer;

    CommandQueue *CMDList;

    FormSize *SZ;

    QFile *CMDDir;

    void ResetWash(Qt::CheckState);

    void SetFormSize(int , int , QList<POS>, POS, int);

    static const int IdleTime = 500;

    static const int WashT1 = 200;

    static const int WashT2 = 200;

    QTimer *Timer,*WTimer1,*WTimer2;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool needWash()const;

public slots:
    void SetCMDDir();
    void ResetUndoNext();
    void PaintNext();
    void StopPlay();
    void ShowCurWash();
    void ShowCurOri();
    void SetBan(POS);
    void RepaintBan();

signals:
    void WashStatusChanged(Qt::CheckState);

    void SysStart(FormSize*,CommandQueue*,int);

    void PaintNow(const ChipInfo&, bool);

    void ChangeFailed(Qt::CheckState);

private slots:

    void SetBasicFormSize(int,int,QList<POS>,POS);

    void ResetStartEnable();

    void DisableUndo(Qt::CheckState);

    void on_pushButton_SetSize_clicked();

    void on_pushButton_Load_clicked();

    void on_checkBox_Wash_stateChanged(int);

    void on_pushButton_Start_clicked();

    void on_pushButton_Undo_clicked();

    void on_pushButton_Next_clicked();

    void on_pushButton_Play_clicked();

    void on_pushButton_Reload_clicked();
};

#endif // MAINWINDOW_H
