#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QGraphicsView>
#include <QMediaPlayer>
#include "syscore.h"
#include "sizesetting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void SetCMDDir();

signals:
    void WashStatusChanged(Qt::CheckState);

    void SysStart(FormSize*,CommandQueue*,int);

    //void DisplayTable();

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

private:
    Ui::MainWindow *ui;

    QErrorMessage *ErrDisplay;

    bool IsReady();

    SysCore* SysC;

    QGraphicsView *PaintBox;

    QMediaPlayer *SoundPlayer;

    CommandQueue *CMDList;

    FormSize *SZ;

    QFile *CMDDir;

    void ResetWash(Qt::CheckState);

    void SetFormSize(int , int , QList<POS>, POS, int);

};

#endif // MAINWINDOW_H
