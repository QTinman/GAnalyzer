#include "headdialog.h"
#include "ui_headdialog.h"
#include "mainwindow.h"
#include "gcalc.h"
#include "tools.h"
#include <QIntValidator>
#include <QStringList>


headDialog::headDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::headDialog)
{
    List = getheadlines(filesource,20);
    ui->setupUi(this);
    ui->lineEdit->setValidator( new QIntValidator(0, 9999, this) );
    //ui->lineEdit->setInputMask("0000");
    ui->lineEdit->clear();
    //model = new QStringListModel(this);
    //model->setStringList(List);
    for ( const auto& i : List  )
    {
        ui->textBrowser->append(i);
    }

}

headDialog::~headDialog()
{
    delete ui;
}

void headDialog::on_lineEdit_returnPressed()
{
    QStringList filterlist,displaylist;
    QString nstype;
    int nslist = ui->lineEdit->text().toUInt(),sum[20],count,startw,endw,red=0,rev=0;
    if (ui->englisordinal->isChecked()) nstype = "English Ordinal";
    if (ui->fullreduction->isChecked()) {
        red=1;
        nstype = "Full Reduction";
    }
    if (ui->reverseordinal->isChecked()) {
        rev=1;
        nstype = "Reverse Ordinal";
    }
    if (ui->reversefull->isChecked()) {
        red=1;
        rev=1;
        nstype = "Reverse Full Reduction";
    }
    for ( const auto& i : List  )
    {


        filterlist = i.split(" ");
        count = -1;
        for ( const auto& l : filterlist  ){
            count ++;
            sum[count] = getwordnumericvalue(l.toUtf8().constData(),red,rev,0);
        }
        bool found = false;
        int result=0,c=0,loops=0;
        while (!found && loops<count) {
            result += sum[c];
            if (result==nslist) {
                found = true;
                startw=loops;
                endw=c;
            }

            if (count == c || result>nslist) {
                result=0;
                loops++;
                c=loops;
            } else if (!found) c++;
        }
        if (!found) displaylist << i;
        if (found) {
            count = 0;
            QString formated="";
            for ( const auto& l : filterlist  ){

                if (count >= startw && count <= endw && sum[count] !=0) formated += Qformattext(l.toUtf8().constData(),2,1)+" ";
                else formated += l+" ";
                count ++;
            }
            //displaylist << "Words from "+QString::number(startw+1)+" to "+QString::number(endw+1)+" calculate to "+QString::number(nslist) ;
            displaylist << formated+" - "+nstype+" number "+Qformattext(QString::number(nslist).toUtf8().constData(),1,1);
        }
    }
    ui->textBrowser->clear();
    for ( const auto& i : displaylist  )
    {
        ui->textBrowser->append(i);
    }

}
