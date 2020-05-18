#include "headdialog.h"
#include "ui_headdialog.h"
#include "mainwindow.h"


headDialog::headDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::headDialog)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    model->setStringList(List);
    ui->listView->setModel(model);

}

headDialog::~headDialog()
{
    delete ui;
}
