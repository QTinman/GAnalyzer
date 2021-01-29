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
    ui->current_url->setText(tmpstring);
    ui->lineEdit->setValidator( new QIntValidator(0, 9999, this) );
    ui->lineEdit->setFocus();
    //ui->lineEdit->setInputMask("0000");
    ui->lineEdit->clear();
    //model = new QStringListModel(this);
    //model->setStringList(List);
    for ( const auto& i : List  )
    {
        ui->textBrowser->append(i);
    }
    ui->SaveNewsFilter->hide();
    ui->ajustinput->setDisabled(true);
    ui->label_2->hide();
    ui->numberofchar->hide();
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
            displaylist << formated+" - "+nstype+": "+Qformattext(QString::number(nslist).toUtf8().constData(),1,1);
        }
    }
    ui->textBrowser->clear();
    for ( const auto& i : displaylist  )
    {
        ui->textBrowser->append(i);
    }

}

void headDialog::on_englisordinal_toggled(bool checked)
{
    if (checked) emit headDialog::on_lineEdit_returnPressed();
}

void headDialog::on_fullreduction_toggled(bool checked)
{
    if (checked) emit headDialog::on_lineEdit_returnPressed();
}

void headDialog::on_reverseordinal_toggled(bool checked)
{
    if (checked) emit headDialog::on_lineEdit_returnPressed();
}

void headDialog::on_reversefull_toggled(bool checked)
{
    if (checked) emit headDialog::on_lineEdit_returnPressed();
}

void headDialog::on_Adjust_clicked()
{
    if (ui->ajustinput->isEnabled()) {
        QString url = ui->current_url->text();
        int numberofchar = ui->numberofchar->text().toInt();
        if (numberofchar == 0) numberofchar=20;
        QString newsstring = ui->ajustinput->text();
        QStringList lstring = newsstring.split(" ");
        QString content,pat1,pat2;
        bool found=true;
        int pos1,pos2;
        QFile infile;
        if (filesource == "") infile.setFileName("headnews.html");
        else infile.setFileName(filesource);
      if (infile.open(QIODevice::ReadOnly)) {
            QTextStream in(&infile);
            content = in.readAll();
            //qDebug() << content.size();
            infile.close();
                //qDebug() << infile.size() << in.readAll();
        }

      for ( const auto& i : lstring  )
      {
          pos1 = content.indexOf(i);
          if (pos1 == -1) found=false;
          //qDebug() << i;
      }
      if (found) {
          content = clean_news_content(content,"");
          pos1 = content.indexOf(lstring.at(0));
          pos2 = content.indexOf(lstring.at(lstring.size()-1),pos1);
          pat1 = content.mid(pos1-numberofchar,numberofchar).trimmed();
          pat2 = content.mid(pos2+lstring.at(lstring.size()-1).length(),numberofchar).trimmed();
          tmpstring = url+"|"+pat1+"|"+pat2;
          qDebug() << url+"|"+pat1+"|"+pat2;
        List = getheadlines(filesource,20);
        ui->textBrowser->clear();
        for ( const auto& i : List  )
        {
            ui->textBrowser->append(i);
        }
        ui->SaveNewsFilter->show();
      }
    } else {
        ui->ajustinput->setDisabled(false);
        ui->label_2->show();
        ui->numberofchar->show();
        ui->textBrowser->clear();
        ui->textBrowser->append("To adjust newsread, paste newsstring into Adjustment string");
        ui->textBrowser->append("Adjust the number of characters used to find headlines and press Adjust news button again.");
        ui->textBrowser->append("Push Save if enabled and desired result is achieved");
        ui->textBrowser->append("If needed the file newssources.txt can be edited.");
    }

}

void headDialog::on_SaveNewsFilter_clicked()
{
    QFile newsources("newssources.txt");
    bool ns_found=false;
    QString url = ui->current_url->text();
    QString line;

    if (newsources.open(QIODevice::ReadWrite)) {
        QByteArray fileData;
        while(!newsources.atEnd()) {
             line = newsources.readLine();

             line = line.trimmed();
             int pos1 = line.indexOf(url);
             if (pos1 != -1) {
                ns_found = true;
                break;
             }
        }

            newsources.seek(0);
            fileData = newsources.readAll();
            QString text(fileData);
            if (ns_found) text.replace(QString(line), QString(tmpstring)); // replace text in string
            else text.append(tmpstring+"\n");
            newsources.seek(0);
            newsources.write(text.toUtf8()); // write the new text back to the file


        newsources.close();

    } //Priest who gave exorcisms to root out voter fraud leaves diocese
     ui->SaveNewsFilter->hide();
}
