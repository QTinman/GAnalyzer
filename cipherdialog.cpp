#include "cipherdialog.h"
#include "ui_cipherdialog.h"
#include "mainwindow.h"
#include <QDebug>

cipherDialog::cipherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cipherDialog)
{
    ui->setupUi(this);
    if (primeson) ui->primes->setChecked(true);
}

cipherDialog::~cipherDialog()
{
    delete ui;
}

void cipherDialog::on_buttonBox_accepted()
{
    if (ui->primes->isChecked()) primeson = true;
    else primeson = false;
    if (primeson) qDebug() << "Prime is on";
}
