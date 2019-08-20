#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
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

signals:
    void WashStatusChanged(int);

private slots:
    void on_checkBox_Wash_stateChanged(int);

    void on_pushButton_SetSize_clicked();

private:
    Ui::MainWindow *ui;
    SysCore* SysC;
};

#endif // MAINWINDOW_H
