#include "cipherdialog.h"
#include "ui_cipherdialog.h"
#include "mainwindow.h"
#include <QDebug>

cipherDialog::cipherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cipherDialog)
{
    ui->setupUi(this);
    if (single_r_on) ui->single_reduction->setChecked(true);
    if (francis_on) ui->francis_bacon->setChecked(true);
    if (satanic_on) ui->satanic->setChecked(true);
    if (jewish_on) ui->jewish->setChecked(true);
}

cipherDialog::~cipherDialog()
{
    delete ui;
}

void cipherDialog::on_buttonBox_accepted()
{
    if (ui->single_reduction->isChecked()) single_r_on = true;
    else single_r_on = false;
    if (ui->francis_bacon->isChecked()) francis_on = true;
    else francis_on = false;
    if (ui->satanic->isChecked()) satanic_on = true;
    else satanic_on = false;
    if (ui->jewish->isChecked()) jewish_on = true;
    else jewish_on = false;
    //if (primeson) qDebug() << "Prime is on";
}
