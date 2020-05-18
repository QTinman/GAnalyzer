#include "calwindow.h"
#include "ui_calwindow.h"
#include "mainwindow.h"

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

void CalWindow::on_date_clicked()
{
    QDate cd(year,mm,dd);
    cd = ui->calendarWidget->selectedDate();
    year = cd.year();
    dd = cd.day();
    mm = cd.month();
    emit buttonpressed();
}

void CalWindow::on_seconddate_clicked()
{
    QDate cd(year,mm,dd);
    cd = ui->calendarWidget->selectedDate();
    y2 = cd.year();
    d2 = cd.day();
    m2 = cd.month();
    emit buttonpressed();
}
