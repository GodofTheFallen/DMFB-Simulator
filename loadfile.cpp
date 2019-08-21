#include "loadfile.h"
#include "ui_loadfile.h"

LoadFile::LoadFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadFile)
{
    ui->setupUi(this);
}

LoadFile::~LoadFile()
{
    delete ui;
}
