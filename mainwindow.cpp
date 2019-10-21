#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_inputdialog.h"
#include "ui_selectdialog.h"
#include "cipherdialog.h"
#include <QtCore>
#include <QDate>
#include "string.h"

#include <QLabel>
#include <QDateTime>
#include "gcalc.h"
#include <QLocale>
#include <QMessageBox>

using namespace std;

int zerodays[8][250];
QString phrase = "<none>";
QString labeltext;
int year,dd,mm,ns,d2,m2,y2,filter;
bool eudate = true,single_r_on=false,francis_on=false,satanic_on=false,jewish_on=false;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QDate cd = QDate::currentDate();
    year = cd.year();
    dd = cd.day();
    mm = cd.month();
    QString scomstr = "Active phrase : " +phrase+ " - Current date : " + QString::number(dd) + "/" + QString::number(mm) + "/" + QString::number(year);
    ui->setupUi(this);
    //setCentralWidget(ui->centralwidget);
    //setCentralWidget(ui->label);
    setCentralWidget(ui->groupBox_3);
    ui->lineEdit->focusWidget();
    ui->statusbar->showMessage(scomstr);
    std::setlocale(LC_ALL,"");

            //int curr_locale = QLocale().language();
            //qDebug() << " String ="<< curr_locale << endl;
    emit welcome();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionDate_Search_triggered() //Ctrl-S
{
    labeltext = "Search number :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    QString html = loopYear(ns,year,1,eudate);
    ui->textBrowser->append("<br>Searching dates for whole year starts<br>");
    if (ns > 0) ui->textBrowser->append("<html>"+html+"</html>");
}

void MainWindow::updatestatusbar(QString str, int dd2, int mm2)
{
    QString scombstr;
    if (d2 == 0) {
    if (eudate) scombstr = "Active phrase : " +str+ " - Current date : " + QString::number(dd2) + "/" + QString::number(mm2) + "/" + QString::number(year);
    else scombstr = "Active phrase : " +str+ " - Current date : " + QString::number(mm2) + "/" + QString::number(dd2) + "/" + QString::number(year);
    } else {
    if (eudate) scombstr = "Active phrase : " +str+ " - Current date : " + QString::number(dd2) + "/" + QString::number(mm2) + "/" + QString::number(year) + " Second date : " + QString::number(d2) + "/" + QString::number(m2) + "/" + QString::number(y2);
    else scombstr = "Active phrase : " +str+ " - Current date : " + QString::number(mm2) + "/" + QString::number(dd2) + "/" + QString::number(year) + " Second date : " + QString::number(m2) + "/" + QString::number(d2) + "/" + QString::number(y2);
    }
    //ui->statusbar->setStyleSheet("color: red");
    ui->statusbar->showMessage(scombstr);
}

inputDialog::inputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDialog)
{

    ui->setupUi(this);
    if (labeltext == "New date :") {
        inputDialog::setWindowTitle("Enter new date");
        ui->lineEdit->setInputMask("00-00");
    }
    if (labeltext == "Enter second date :") {
        inputDialog::setWindowTitle("Enter second date for extended date details");
        ui->lineEdit->setInputMask("00-00-0000");
    }

    if (labeltext == "Search number :") {
        inputDialog::setWindowTitle("Search whole year for cipher");
        ui->lineEdit->setInputMask("000");
    }
    if (labeltext == "New Phrase :") inputDialog::setWindowTitle("Phrase needed for Analyzer to run");
    if (labeltext == "New phrase :") inputDialog::setWindowTitle("Enter new phrase");
    if (labeltext == "Enter Year:") {
        inputDialog::setWindowTitle("Change year");
        ui->lineEdit->setInputMask("0000");
    }
    if (labeltext == "Enter Cipher nr. :") {
        inputDialog::setWindowTitle("Search history.txt");
        ui->lineEdit->setInputMask("000");
    }
    ui->label->setText(labeltext);
}

inputDialog::~inputDialog()
{
    delete ui;
}

void inputDialog::displaydialog()
{

    if (labeltext == "New Phrase :") phrase = ui->lineEdit->text();
    if (labeltext == "New phrase :") phrase = ui->lineEdit->text();
    if (labeltext == "Search number :") ns = ui->lineEdit->text().mid(0,3).toInt();
    if (labeltext == "Compare to history :") ns = ui->lineEdit->text().mid(0,3).toInt();
    if (labeltext == "Enter Year:") year = ui->lineEdit->text().mid(0,4).toInt();
    if (labeltext == "Enter Cipher nr. :") ns = ui->lineEdit->text().mid(0,3).toInt();
    if (eudate) {
    if (labeltext == "New date :") {
        dd = ui->lineEdit->text().mid(0,2).toInt();
        mm = ui->lineEdit->text().mid(3,2).toInt();
        if (valid_date(dd,mm,year) == 0) {
            QDate cd = QDate::currentDate();
            year = cd.year();
            dd = cd.day();
            mm = cd.month();
        }

    }
    }else{
       if (labeltext == "New date :") {
            dd = ui->lineEdit->text().mid(3,2).toInt();
            mm = ui->lineEdit->text().mid(0,2).toInt();
            if (valid_date(mm,dd,year) == 0) {
                QDate cd = QDate::currentDate();
                year = cd.year();
                dd = cd.day();
                mm = cd.month();
            }
        }
    }
   if (eudate) {
    if (labeltext == "Enter second date :") {
        d2 = ui->lineEdit->text().mid(0,2).toInt();
        m2 = ui->lineEdit->text().mid(3,2).toInt();
        y2 = ui->lineEdit->text().mid(6,4).toInt();
        if (valid_date(d2,m2,y2) == 0) {
            d2 = 0;
            m2 = 0;
            y2 = 0;
        }
    }
    }else {
     if (labeltext == "Enter second date :") {
        d2 = ui->lineEdit->text().mid(3,2).toInt();
        m2 = ui->lineEdit->text().mid(0,2).toInt();
        y2 = ui->lineEdit->text().mid(6,4).toInt();
        if (valid_date(m2,d2,y2) == 0) {
            d2 = 0;
            m2 = 0;
            y2 = 0;
        }

    }
   }
}

void inputDialog::on_pushButton_clicked()
{
    emit displaydialog();
}

void inputDialog::on_lineEdit_returnPressed()
{
    emit displaydialog();
}

void MainWindow::on_actionNew_Phrase_triggered()
{
    labeltext = "New phrase :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar(phrase,dd,mm);
}

void MainWindow::on_actionNew_Date_triggered()
{
    labeltext = "New date :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar(phrase,dd,mm);
}

void MainWindow::on_actionSearch_History_txt_triggered() //Ctrl-H
{

    labeltext = "Enter Cipher nr. :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    ui->textBrowser->append("<br>Search history with number starts<br>");
    QString html = searchwords(ns,true);
    if (ns >0) ui->textBrowser->append("<html>"+html+"</html>");
}

void MainWindow::on_actionSet_Year_triggered()
{
    labeltext = "Enter Year:";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar(phrase,dd,mm);
}

void MainWindow::on_action_Analyze_triggered() //Ctrl-A
{
    if (phrase == "<none>") {
     labeltext = "New Phrase :";
     inputDialog datesearch;
     datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
     datesearch.exec();
     emit updatestatusbar(phrase,dd,mm);
    }
   if (phrase != "<none>") {
       ui->textBrowser->append("<br>Analyzing phrase starts");
       QString html = analyze(dd,mm,year,phrase,false,0,eudate);
       ui->textBrowser->append("<html>"+html+"</html>");
   }

}

void MainWindow::on_action_Eu_date_toggled(bool arg1)
{
    if (arg1) eudate = true;
    else eudate = false;
    emit updatestatusbar(phrase,dd,mm);
}

void MainWindow::on_actionDate_Details_triggered() //Ctrl-D
{
    QString html = gcalc(dd,mm,year,d2,m2,y2,eudate);
    ui->textBrowser->append("<html>"+html+"</html>");
    //#include <QDebug>//qDebug() << "\n" << html;
}

void MainWindow::on_lineEdit_returnPressed()
{

    QString tphrase = ui->lineEdit->text();
    std::string stdphrase = tphrase.toUtf8().constData();
    if (ui->checkBox->isChecked()) stdphrase = "/a" +stdphrase;
    if (stdphrase == "dd" || stdphrase == "DD") {
        QString html = printword(stdphrase,'D',true,false);
        ui->textBrowser->append("<html>"+html+"</html>");
    }
    else if (stdphrase [0] == '/') {
        //qDebug() << QString::fromStdString(stdphrase) << "\n";
        replaceAll(stdphrase,"/ ","/");
        replaceAll(stdphrase," /","/");
        replaceAll(stdphrase,"/a ","/a");
        replaceAll(stdphrase,"/c ","/c");
        replaceAll(stdphrase,"/n ","/n");
        replaceAll(stdphrase,"/s ","/s");
        replaceAll(stdphrase,"/h ","/h");
        replaceAll(stdphrase,"/w ","/w");
        replaceAll(stdphrase,"/e ","/e");
        replaceAll(stdphrase,"/o ","/o");
        //qDebug() << QString::fromStdString(stdphrase) << "\n";

        switch (stdphrase[1]) {
        case 'a':
        {
            if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) == "" && phrase == "<none>") {
             labeltext = "New Phrase :";
             inputDialog datesearch;
             datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
             datesearch.exec();
            } else if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) != "") phrase = QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2));
            QString html = analyze(dd,mm,year,phrase,false,0,eudate);
            ui->textBrowser->append("<html>"+html+"</html>");
            emit updatestatusbar(phrase,dd,mm);
            break;
        }
        case 'c':
        {
            ui->textBrowser->clear();
            break;
        }
        case 'n':
        {
            if (eudate) {
                dd = ui->lineEdit->text().mid(2,2).toInt();
                mm = ui->lineEdit->text().mid(5,2).toInt();
            } else {
                mm = ui->lineEdit->text().mid(2,2).toInt();
                dd = ui->lineEdit->text().mid(5,2).toInt();
            }
            if (ui->lineEdit->text().mid(8,4).toInt() > 0) year = ui->lineEdit->text().mid(8,4).toInt();
            if (valid_date(dd,mm,year) == 0) {
                //qDebug() << ui->lineEdit->text().mid(2,2) << " " << ui->lineEdit->text().mid(5,2) << " " << year;
                QDate cd = QDate::currentDate();
                dd = cd.day();
                mm = cd.month();
                year = cd.year();
            }
            emit updatestatusbar(phrase,dd,mm);
            break;
        }
        case 's':
        {
            if (eudate) {
                d2 = ui->lineEdit->text().mid(2,2).toInt();
                m2 = ui->lineEdit->text().mid(5,2).toInt();
                y2 = ui->lineEdit->text().mid(8,4).toInt();
            } else {
                m2 = ui->lineEdit->text().mid(2,2).toInt();
                d2 = ui->lineEdit->text().mid(5,2).toInt();
                y2 = ui->lineEdit->text().mid(8,4).toInt();
            }
            if (valid_date(d2,m2,y2) == 0) {
                //qDebug() << ui->lineEdit->text().mid(2,2) << " " << ui->lineEdit->text().mid(5,2) << " " << y2;
                d2 = 0;
                m2 = 0;
                y2 = 0;
            } else emit updatestatusbar(phrase,dd,mm);
            break;
        }
        case 'h':
            emit shorthelp();
            break;
        case 'w':
        {
            if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) == "" && phrase == "<none>") {
             labeltext = "New phrase :";
             inputDialog datesearch;
             datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
             datesearch.exec();

            } else if (QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2)) != "") phrase = QString::fromStdString(stdphrase.substr(2,stdphrase.length()-2));
            emit updatestatusbar(phrase,dd,mm);
            if (phrase != "<none>") {
                std::string stdphrase = phrase.toUtf8().constData();
                ui->textBrowser->append("<html>"+printallwords(stdphrase,'N',true,false)+"</html>");
            }
            break;
        case 'd':
            {
            QString html;
            if (eudate) {if (valid_date(ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(5,2).toInt(),year) == 1) {
                    if (ui->lineEdit->text().mid(8,4).toInt() > 0) html = gcalc(ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(8,4).toInt(),d2,m2,y2,eudate);
                    else html = gcalc(ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(5,2).toInt(),year,d2,m2,y2,eudate);
                }
                else html = gcalc(dd,mm,year,d2,m2,y2,eudate);
            }else { if (valid_date(ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(2,2).toInt(),year) == 1) {
                    if (ui->lineEdit->text().mid(8,4).toInt() > 0) html = gcalc(ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(2,2).toInt(),ui->lineEdit->text().mid(8,4).toInt(),d2,m2,y2,eudate);
                    else gcalc(ui->lineEdit->text().mid(5,2).toInt(),ui->lineEdit->text().mid(2,2).toInt(),year,d2,m2,y2,eudate);
                }
                else html = gcalc(dd,mm,year,d2,m2,y2,eudate);
             }
            ui->textBrowser->append("<html>"+html+"</html>");
            break;
            }
            case 'e':
                {
                QString html;
                int type=5;
                if (ui->lineEdit->text().mid(2,1) == "T") type = 1;
                if (ui->lineEdit->text().mid(2,1) == "A") type = 2;
                if (ui->lineEdit->text().mid(2,1) == "P") type = 3;
                if (ui->lineEdit->text().mid(2,1) == "H") type = 4;
                if (ui->lineEdit->text().mid(2,1) == "X") type = 5;
                //qDebug() << ui->lineEdit->text().mid(2,1) << " " << ui->lineEdit->text().mid(4,2) << " " << ui->lineEdit->text().mid(7,2) << " " << ui->lineEdit->text().mid(10,4);
                if (eudate) {if (valid_date(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year) == 1) {
                        if (ui->lineEdit->text().mid(10,4).toInt() > 0) html = solareclipe(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(10,4).toInt(),1,type,eudate);
                        else if (ui->lineEdit->text().mid(2,1) != "") html = solareclipe(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year,1,type,eudate);
                    }
                    else html = solareclipe(dd,mm,year, 1,type,eudate);
                }else { if (valid_date(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year) == 1) {
                        if (ui->lineEdit->text().mid(10,4).toInt() > 0) html = solareclipe(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(10,4).toInt(),1,type,eudate);
                        else solareclipe(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year,1,type,eudate);
                    }
                    else if (type > 0) html = solareclipe(dd,mm,year,1,type,eudate);
                 }
                ui->textBrowser->append("<html>"+html+"</html>");
                break;
                }

            case 'o':
                {
                QString html;
                if (ui->lineEdit->text().mid(2,1).toInt() > 0 && ui->lineEdit->text().mid(2,1).toInt() < 5) {
                if (eudate) {if (valid_date(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year) == 1) html = date2history(ui->lineEdit->text().mid(4,2).toInt(),ui->lineEdit->text().mid(7,2).toInt(),year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                    else html = date2history(dd,mm,year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                }else { if (valid_date(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year) == 1) html = date2history(ui->lineEdit->text().mid(7,2).toInt(),ui->lineEdit->text().mid(4,2).toInt(),year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                    else html = date2history(dd,mm,year,true,eudate,ui->lineEdit->text().mid(2,1).toInt());
                 }
                ui->textBrowser->append("<html>"+html+"</html>");
                }
                break;
                }
          }
        }

    }
    else if (stdphrase != ""){
        QString html = printword(stdphrase,'Y',true,false);
        ui->textBrowser->append("<html>"+html+"</html>");
    }
    ui->lineEdit->clear();
}

void MainWindow::shorthelp()
{
        ui->textBrowser->append("<br><b>All functions are available from the Menu</b>");
        ui->textBrowser->append("<b>By shortcut following functions are available</b>");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-S)</font> <b>Date search</b> connect number to dates spanning the active year.");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-H)</font> <b>Search history.txt</b> searches all words connected to entered number in history.txt.");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-A)</font> <b>Analyze</b> takes a phrase and compare it to current date displayed on the status bar.");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-D)</font> <b>Date details</b> displays calculations for current date. Second date will extend the information.");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-W)</font> <b>Word details</b> shows details about active phrase.");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-E)</font> <b>Compare Solar Eclipses to History.txt</b> for current date");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-O)</font> <b>Date compare to history</b> calculate current date and compares it to history.txt");
        ui->textBrowser->append("<font color=\"blue\">(Ctrl-T)</font> <b>Compare phrase to history.txt</b> takes one of the base ciphers from active phrase and compares it to history.txt<br>");


        ui->textBrowser->append("The input area takes phrases wich are displayed and saved to history.txt<br>");
        ui->textBrowser->append("<b>The input area also takes commands:</b>");

        ui->textBrowser->append("<font color=\"blue\">/a(phrase)</font> runs analyzer, (<b>Phrase</b> is optional)");
        ui->textBrowser->append("<font color=\"blue\">/w(phrase)</font> phrase details (<b>Phrase</b> is optional)");
        ui->textBrowser->append("<font color=\"blue\">/n##/##/####</font> gives new date, (Year is optional)");
        ui->textBrowser->append("<font color=\"blue\">/s##/##/####</font> gives new second date");
        ui->textBrowser->append("<font color=\"blue\">/d##/##/####</font> date details (date is optional, year is extra option)");
        ui->textBrowser->append("<font color=\"blue\">/o#/##/##</font> Date compare to history (first number is filter 1-4, date is optional)");
        ui->textBrowser->append("<font color=\"blue\">/e@/##/##/####</font> Last and next Solar eclipse relative to date. @ is type \"T A P H-X=for all\" (date is optional, year is extra option)");
        ui->textBrowser->append("<font color=\"blue\">dd</font> deletes last line from history.txt");
        ui->textBrowser->append("<font color=\"blue\">/h</font> shows this help");


        ui->textBrowser->append("<font color=\"blue\">/c</font> Clears output");
        ui->textBrowser->append("Enter <b>Word Phrase</b> shows details about that phrase and saves it to history.");
        ui->textBrowser->append("Change <b>Current Year</b> from The Edit menu");
        ui->textBrowser->append("Change <b>Date Style</b> from The Edit menu");
        ui->textBrowser->append("<br><b>© jonssofh@hotmail.com</b><br>");

}

void MainWindow::welcome()
{
        ui->textBrowser->append("<h1>Welcome to Gematria Analyzer!</h1><br>");
        ui->textBrowser->append("<h2>This program calculates Kabbalah ciphers from phrases and compares it to date numerology.</h2><br>");
        ui->textBrowser->append("<b>For details about ciphers select <font color=\"blue\">Tables->List ciphers</font></b>");
        ui->textBrowser->append("The program takes a phrase and date for comparison, also second date can be entered<br>");
        ui->textBrowser->append("Select <b>Help</b> from menu or type <font color=\"blue\">/h</font> in input area<br>");
}

void MainWindow::on_action_Word_details_triggered() //Ctrl-W
{
    if (phrase == "<none>") {
     labeltext = "New Phrase :";
     inputDialog datesearch;
     datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
     datesearch.exec();
     emit updatestatusbar(phrase,dd,mm);
    }
   if (phrase != "<none>") {
       std::string stdphrase = phrase.toUtf8().constData();
       ui->textBrowser->append("<html>"+printallwords(stdphrase,'N',true,false)+"</html>");
   }

}

void MainWindow::on_actionList_Ciphers_triggered()
{
    ui->textBrowser->append("English Ordinal");
    ui->textBrowser->append(listciphers(0,0,0));
    ui->textBrowser->append("Full Reduction");
    ui->textBrowser->append(listciphers(1,0,0));
    ui->textBrowser->append("Reverse Ordinal");
    ui->textBrowser->append(listciphers(0,1,0));
    ui->textBrowser->append("Reverse full Reduction");
    ui->textBrowser->append(listciphers(1,1,0));
    if (single_r_on) {
    ui->textBrowser->append("Single Reduction");
    ui->textBrowser->append(listciphers(0,0,1));
    }
    if (francis_on) {
    ui->textBrowser->append("Francis Bacon");
    ui->textBrowser->append(listciphers(0,0,2));
    }
    if (satanic_on) {
    ui->textBrowser->append("Satanic");
    ui->textBrowser->append(listciphers(0,0,3));
    }
    if (jewish_on) {
    ui->textBrowser->append("Jewish");
    ui->textBrowser->append(listciphers(0,0,4));
    }
}

void MainWindow::on_actionList_Primenumbers_triggered()
{
    ui->textBrowser->append(listnumbers('P'));
}

void MainWindow::on_actionList_Triangular_numbers_triggered()
{
    ui->textBrowser->append(listnumbers('T'));
}

void MainWindow::on_actionCompare_phrase_to_history_triggered() //Ctrl-T
{
    QString str2;
    if (phrase == "<none>") {
     labeltext = "New Phrase :";
     inputDialog datesearch;
     datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
     datesearch.exec();
     emit updatestatusbar(phrase,dd,mm);
    }
    if (phrase != "<none>") {
    labeltext = "New Phrase :";
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();
    ui->textBrowser->append("<br>Search history from phrase starts<br>");
    ui->textBrowser->append(searchhistory(ns,phrase.toUtf8().constData()));

    if (ns == 1) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("English ordinal",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 2) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("Full Reduction",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 3) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("Reverse Ordinal",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 4) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("Reverse full Reduction",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 5) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("Single Reduction",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 6) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("Francis Bacon",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 7) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("Satanic",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    if (ns == 8) ui->textBrowser->append( "Calculated from " +QString::fromStdString(formattext("Jewish",2,2)) +" from Phrase :"+QString::fromStdString(formattext(phrase.toUtf8().constData(),1,1))+"<br>");
    }
}

selectDialog::selectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectDialog)
{
    if (labeltext == "Date to history")selectDialog::setWindowTitle("Choose filter");
    ui->setupUi(this);
    if (labeltext == "solar") {
        ui->Jewish->show();
        ui->Francis->hide();
        ui->Satanic->hide();
        ui->SingleRed->hide();
        ui->radioButton1->setText("Total Solar Eclipse");
        ui->radioButton2->setText("Annular Solar Eclipse");
        ui->radioButton3->setText("Partial Solar Eclipse");
        ui->radioButton4->setText("Hybrid Solar Eclipse");
        ui->Jewish->setText("All Solar Eclipses");

    } else {
    if (!single_r_on) ui->SingleRed->hide();
    if (!francis_on) ui->Francis->hide();
    if (!satanic_on) ui->Satanic->hide();
    if (!jewish_on) ui->Jewish->hide();
    }

}

selectDialog::~selectDialog()
{
    delete ui;
}

void selectDialog::displaydialog()
{
    if (labeltext == "New Phrase :") {
    if (ui->radioButton1->isChecked()) ns = 1;
    if (ui->radioButton2->isChecked()) ns = 2;
    if (ui->radioButton3->isChecked()) ns = 3;
    if (ui->radioButton4->isChecked()) ns = 4;
    if (ui->SingleRed->isChecked()) ns = 5;
    if (ui->Francis->isChecked()) ns = 6;
    if (ui->Satanic->isChecked()) ns = 7;
    if (ui->Jewish->isChecked()) ns = 7;
    }
    if (labeltext == "Date to history") {
    if (ui->radioButton1->isChecked()) filter = 1;
    if (ui->radioButton2->isChecked()) filter = 2;
    if (ui->radioButton3->isChecked()) filter = 3;
    if (ui->radioButton4->isChecked()) filter = 4;
    if (ui->SingleRed->isChecked()) filter = 5;
    if (ui->Francis->isChecked()) filter = 6;
    if (ui->Satanic->isChecked()) filter = 7;
    if (ui->Jewish->isChecked()) filter = 7;
    }
    if (labeltext == "solar") {
    if (ui->radioButton1->isChecked()) filter = 1;
    if (ui->radioButton2->isChecked()) filter = 2;
    if (ui->radioButton3->isChecked()) filter = 3;
    if (ui->radioButton4->isChecked()) filter = 4;
    if (ui->Jewish->isChecked()) filter = 5;
    }
}


void selectDialog::on_pushButton_clicked()
{
    emit displaydialog();
}

void MainWindow::on_action_Second_date_triggered()
{
    labeltext = "Enter second date :";
    inputDialog datesearch;
    datesearch.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    datesearch.exec();
    emit updatestatusbar(phrase,dd,mm);
}

void MainWindow::on_actionC_ompare_date_to_History_txt_triggered() // Ctrl-O
{
    labeltext = "Date to history";
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();
    ui->textBrowser->append("<br>Search history connected to current date starts");
    ui->textBrowser->append(date2history(dd,mm,year,true,eudate,filter));
}

void MainWindow::on_actionHelp_triggered()
{
    emit MainWindow::shorthelp();
}

void MainWindow::on_actionSolar_Eclipses_triggered()
{
    emit ui->textBrowser->append(solareclipe(dd,mm,year,1,1,eudate)); // 1=print
}

void MainWindow::on_actionCompare_SolarE_to_history_triggered()
{
    labeltext = "solar";
    selectDialog sDialog;
    sDialog.setModal(true); // if nomodal is needed then create pointer inputdialog *datesearch; in mainwindow.h private section, then here use inputdialog = new datesearch(this); datesearch.show();
    sDialog.exec();


    ui->textBrowser->append(solar2history(dd,mm,year,filter));
}

void MainWindow::on_pushButton_clicked()
{
    dd ++;
    emit updatestatusbar(phrase,dd,mm);
}

void MainWindow::on_pushButton_2_clicked()
{
    dd --;
    emit updatestatusbar(phrase,dd,mm);
}

void MainWindow::on_actionAbout_triggered()
{
   // QMessageBox::about(this,"About Gematria Analyzer","Gematria Analyzer is a free software created for playing with the English language. Support is most appreciated.");
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Gematria Analyzer");
    msgBox.setText(tr("Gematria Analyzer is a free software created for playing with the English language"));
    QAbstractButton* pButtonYes = msgBox.addButton(tr("Donate!"), QMessageBox::YesRole);
    msgBox.addButton(tr("Ok"), QMessageBox::NoRole);

    msgBox.exec();

    if (msgBox.clickedButton()==pButtonYes) {
        QMessageBox::about(this,"Donate","Donations can be done by Paypal to jonssofh@hotmail.com or cryptocurrency BTC 3F8BM7RLiYp5j8k47JKSFqqpcru1pBbA9s , Support is most appreciated.");
    }
}

void MainWindow::on_action_Add_remove_ciphers_triggered()
{
    cipherDialog mDialog;
    mDialog.setModal(true);
    mDialog.exec();
}
