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
}

rankDialog::~rankDialog()
{
    delete ui;
}

void rankDialog::on_buttonBox_accepted()
{
    phrase = ui->phrases->text();
    ns = ui->min->value();
    if (ui->checkBox->isChecked()) ns += 100;
}

void rankDialog::on_buttonBox_rejected()
{
    if (phrase != "<none>") phrase = "<none>";
}
