#include "calwindow.h"
#include "ui_calwindow.h"

CalWindow::CalWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalWindow)
{
    ui->setupUi(this);
}

CalWindow::~CalWindow()
{
    delete ui;
}
