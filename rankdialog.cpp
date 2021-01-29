#include "rankdialog.h"
#include "ui_rankdialog.h"
#include "mainwindow.h"

rankDialog::rankDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rankDialog)
{
    ui->setupUi(this);
    if (phrase != "<none>") ui->phrases->setText(phrase);
    ui->min->setValue(2);
    ui->Chipers->setChecked(true);
    ui->phrases->selectAll();
}

rankDialog::~rankDialog()
{
    delete ui;
}

void rankDialog::on_buttonBox_accepted()
{
    phrase = ui->phrases->text();
    ns = ui->min->value();
    if (ui->Solar->isChecked()) ns += 100;
    if (ui->Prime->isChecked()) ns += 1000;
    if (ui->Triangular->isChecked()) ns += 10000;
    if (ui->Dates->isChecked()) ns += 100000;
    if (ui->Chipers->isChecked()) ns += 1000000;
}

void rankDialog::on_buttonBox_rejected()
{
    if (phrase != "<none>") phrase = "<none>";
}
