#include "gcalc.h"
#include "tools.h"
#include "mainwindow.h"
#include <QDebug>
#include <QDate>
#include <QFile>
#include <QtCore>
#include <QMessageBox>

//#include "downloadmanager.h"

#define BUFFERSIZE 256
using namespace std;
int pos;
/*
  QString::fromStdString(string)  <- from string to Qstring
  QString::number(int) <- from int to QString
  string = QString.toUtf8().constData() from QString to string
  QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,1))
  std::to_string(42) int to string
 */

QString loopYear(int ns,int dd, int mm, int year, int printcal, bool eudate)
{
      int days, mm2 = 12, dd2 = 31;
      int y1 = year/1000;
      int y2 = (year/100)-(y1*10);
      int y3 = (year-(y1*1000)-(y2*100))/10;
      int y4 = year-(y1*1000)-(y2*100)-(y3*10);
      QString Qdate,Qns,dd_mm,d_d_m_m,YY_yy,Y_Y_y_y,yy,y_y;
      QString buffer;

    // i --> Iterate through all the months
    // j --> Iterate through all the days of the
    //       month - i
    readsolarfile(dd,mm,year);

    for (int i = 0; i < mm2; i++)
    {
        //days = numberOfDays (i, year);
        QDate cd(year,i+1,1);
        days = cd.daysInMonth();
        int d1, d2, m1 ,m2;
        for (int j = 1; j <= days; j++)
        {
            if (j == dd2 && i == mm2) break;
              if (i > 8)
              {
                  m1 = 1;
                  m2 = i - 9;
              } else {
                  m1 = 0;
                  m2 = i + 1;
              }
            if (j > 29)
            {
                d2 = j-30;
                d1 = 3;
            } else if (j > 19)
            {
                d2 = j-20;
                d1 = 2;
            } else if (j > 9)
            {
                d2 = j-10;
                d1 = 1;
            } else
            {
                d2 = j;
                d1 = 0;
            }
            if (printcal == 1) {
          //      MainWindow::setui("hello");

            if (eudate) {
             Qdate = QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str() + " - ";
             d_d_m_m = QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0));
             dd_mm = "("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")" ;
            } else {
             Qdate = QString::fromStdString(getMonthName (i).c_str())+" "+QString::number(d1)+QString::number(d2) + " - ";
             d_d_m_m = QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0));
             dd_mm = "("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")";
            }
             Qns = QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1));

             YY_yy = "("+QString::fromStdString(formattext(QString::number((y1*10)+y2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
             Y_Y_y_y = QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));
             yy = "("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
             y_y = QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));
            buffer += Qtotable("",1,0,0,0);
            if (d1>0) {
             if (ns == (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4) buffer += Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110) + Qtotable(dd_mm + "+" + YY_yy + "= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4) buffer += Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110) + Qtotable(dd_mm+ "+" +Y_Y_y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2+y3+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110) +Qtotable(dd_mm+"+"+y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2+(y3*10)+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110) + Qtotable(dd_mm + "+" + yy+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == (d1*10)+d2+(m1*10)+m2) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110) + Qtotable(dd_mm +"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
            }
             if (ns == d1+d2+m1+m2+y1+y2+y3+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable(d_d_m_m+" + "+Y_Y_y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == d1+d2+m1+m2+y3+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable(d_d_m_m+" + "+y_y+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == d1+d2+m1+m2+(y1*10)+y2+(y3*10)+y4) buffer += Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable(d_d_m_m+ " + " +YY_yy+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == d1+d2+m1+m2+(y3*10)+y4) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable(d_d_m_m+" + "+yy+"= "+Qns,0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == daynr((d1*10)+d2,(m1*10)+m2,year)) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable(" It is the "+Qns+"th day of the year",0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == daynrleft((d1*10)+d2,(m1*10)+m2,year)) buffer +=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable("There are "+Qns+" days left of the year",0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == eu_amdate(2, d1, d2, m1, m2, ns)) buffer+=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable(Qns+" fits Europian style of date of dd/mm",0,0,1,300)+Qtotable("",0,2,0,0);
             if (ns == eu_amdate(1, d1, d2, m1, m2, ns)) buffer+=Qtotable("",0,1,0,0)+Qtotable(Qdate,0,0,1,110)+Qtotable(Qns+" fits MDY style of date of mm/dd",0,0,1,300)+Qtotable("",0,2,0,0);
            buffer += Qtotable("",2,0,0,0);
             if (searchzerodays(ns,1,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,1,"",eudate,false);
             }
             if (searchzerodays(ns,2,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,2,"",eudate,false);
             }
             if (searchzerodays(ns,3,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,3,"",eudate,false);
             }
             if (searchzerodays(ns,4,(d1*10)+d2,(m1*10)+m2,year) == ns) {
                 //qDebug() << ns;
                 buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,4,"",eudate,false);
             }
            }
        }
    }
    //qDebug() << buffer.length();

    return buffer;
}



QString gcalc(int dd, int mm, int year, int dd2, int mm2, int yy2,bool eudate) {
    int d1, d2, m1 ,m2, ns, daynumb, dayleft, wd1, wd2, nm1,nm2,days,daysbeetween;
    double w1;
    QString buffer;
    stringstream logline;
    string s1;
      int y1 = year/1000;
      int y2 = (year/100)-(y1*10);
      int y3 = (year-(y1*1000)-(y2*100))/10;
      int y4 = year-(y1*1000)-(y2*100)-(y3*10);
      readsolarfile(dd,mm,year);

    if (mm > 9)
          {
            m1 = 1;
            m2 = mm - 10;
          } else {
            m1 = 0;
            m2 = mm;
          }
    if (dd > 29)
          {
            d2 = dd-30;
            d1 = 3;
       } else if (dd > 19)
          {
            d2 = dd-20;
            d1 = 2;
       } else if (dd > 9)
          {
            d2 = dd-10;
            d1 = 1;
       } else
          {
            d2 = dd;
            d1 = 0;
          }
    daynumb = daynr(dd,mm,year);
    dayleft = daynrleft(dd,mm,year);
    //qDebug() << days << " " << QDate(year,mm,dd) << " " << QDate(yy2,mm2,dd2);
    if (QDate(year,mm,dd) > QDate(yy2,mm2,dd2)) {
        days = spanofdate(dd2,mm2,yy2,dd,mm,year);
        wd1 = spanofdate(dd2,mm2,yy2,dd,mm,year)/7;
        w1 = spanofdate(dd2,mm2,yy2,dd,mm,year);
        s1 = " - From ";
        //qDebug() << days << " " << QDate(dd,mm,year) << " " << QDate(dd2,mm2,yy2);
        if (dd2<=dd) {
         nm1 = mm - mm2 + (year - yy2) *12;
         nm2 = spanofdate(dd2,mm,year,dd,mm,year);
        } else {
         nm1 = mm - mm2-1 + (year - yy2) *12;
         nm2 = spanofdate(dd2,mm-1,year,dd,mm,year);
    }
    } else {
        days = spanofdate(dd,mm,year,dd2,mm2,yy2);
        wd1 = spanofdate(dd,mm,year,dd2,mm2,yy2)/7;
        w1 = spanofdate(dd,mm,year,dd2,mm2,yy2);
        s1 = " - To ";
        if (dd<=dd2) {
         nm1 = mm2 - mm + (yy2 - year) *12;
         nm2 = spanofdate(dd,mm2,yy2,dd2,mm2,yy2);
        } else {
         nm1 = mm2 - mm-1 + (yy2 - year) *12;
         nm2 = spanofdate(dd,mm2-1,yy2,dd2,mm2,yy2);
        }
    }
    w1 = w1/7-wd1;
    wd2 = round(w1*7);



    logtime();
    logline.clear();
    if (eudate) logline << "<br>Date of " << dd << "/" << mm << "/" << year << " is " << formattext(dayname(dayNumber(dd,mm,year)),1,2) << "<br>";
    else logline << "<br>Date of " << mm << "/" << dd << "/" << year << " is " << formattext(dayname(dayNumber(dd,mm,year)),1,2) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
        ns = (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);

        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1*10+d2),1,0) << ")+(" << formattext(std::to_string(m1*10+m2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1*10+m2),1,0) << ")+(" << formattext(std::to_string(d1*10+d2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        //buffer += formattext(QString::fromStdString(logline.str()),0,1);
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1*10+d2),1,0) << ")+(" << formattext(std::to_string(m1*10+m2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1*10+m2),1,0) << ")+(" << formattext(std::to_string(d1*10+d2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+y1+y2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1*10+d2),1,0) << ")+(" << formattext(std::to_string(m1*10+m2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1*10+m2),1,0) << ")+(" << formattext(std::to_string(d1*10+d2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1*10+d2),1,0) << ")+(" << formattext(std::to_string(m1*10+m2),1,0) << ")&emsp;&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1*10+m2),1,0) << ")+(" << formattext(std::to_string(d1*10+d2),1,0) << ")&emsp;&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+20+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1*10+d2),1,0) << ")+(" << formattext(std::to_string(m1*10+m2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1*10+m2),1,0) << ")+(" << formattext(std::to_string(d1*10+d2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate,true);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate,true);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate,true);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate,true);
        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        logline.str("");


        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year" << " Prime? " << isprime(daynumb) << " Triangular? " << istriangular(daynumb) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Is the " << formattext(std::to_string(daynumb),1,1) << "th day of the year - " << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        if (searchzerodays(daynumb,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,1,"",eudate,true);
        if (searchzerodays(daynumb,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,2,"",eudate,true);
        if (searchzerodays(daynumb,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,3,"",eudate,true);
        if (searchzerodays(daynumb,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,4,"",eudate,true);
        savelog(logline.str());

        logline.str("");
        if (eudate) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year" << " Prime? " << isprime(dayleft) << " Triangular? " << istriangular(dayleft) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;There is " << formattext(std::to_string(dayleft),1,1) << " days left in the year - " << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        if (searchzerodays(dayleft,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,1,"",eudate,true);
        if (searchzerodays(dayleft,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,2,"",eudate,true);
        if (searchzerodays(dayleft,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,3,"",eudate,true);
        if (searchzerodays(dayleft,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,4,"",eudate,true);
        savelog(logline.str());
        logline.str("");
        if (dd2 != 0){
          if (eudate) {
              logline << "<br>" << d1 << d2 << " " << getMonthName (mm-1).c_str() << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(days),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              if (wd2>0) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks +" << formattext(std::to_string(wd2),1,1) << " days<br>";
              else logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              if(nm2>0) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months +" << formattext(std::to_string(nm2),1,1) << " days<br>";
              else logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              } else {
              logline << "<br>" << getMonthName (mm-1).c_str() << " " << d1 << d2 << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(days),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              if (wd2>0) logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks +" << formattext(std::to_string(wd2),1,1) << " days<br>";
              else logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              if (nm2 >0) logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months +" << formattext(std::to_string(nm2),1,1) << " days<br>";
              else logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << s1 << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");}
                daynumb = daynr(dd2,mm2,year);
                dayleft = daynrleft(dd2,mm2,year);
                if (eudate) {
                    logline << dd2 << " " << getMonthName (mm2-1).c_str() << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    logline << dd2 << " " << getMonthName (mm2-1).c_str() << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    } else {
                    logline << getMonthName (mm2-1).c_str() << " " << dd2 << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    logline << getMonthName (mm2-1).c_str() << " " << dd2 << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");}
                dayleft = daynr(dd,mm,year);
                if (dayleft > daynumb) daysbeetween = dayleft - daynumb;
                else daysbeetween = daynumb-dayleft;
                wd1 = daysbeetween/7;
                w1 = daysbeetween;
                w1 = w1/7-wd1;
                wd2 = round(w1*7);
                if (dayleft > daynumb) {
                    if (dd2 <= dd) {
                     nm1 = mm - mm2;
                     nm2 = spanofdate(dd2,mm2+nm1,year,dd,mm,year);
                    }
                    else {
                     nm1 = mm - mm2-1;
                     nm2 = spanofdate(dd2,mm2+nm1,year,dd,mm,year);
                    }
                }
                else {
                    if (dd2 < dd) {
                     nm1 = mm2 - mm-1;
                     nm2 = spanofdate(dd,mm+nm1,year,dd2,mm2,year);
                    }
                    else {
                     nm1 = mm2 - mm;
                     nm2 = spanofdate(dd,mm+nm1,year,dd2,mm2,year);
                    }
                }
                if (eudate) {
                    if (dayleft > daynumb) logline << "From " << dd2 << " " << getMonthName (mm2-1).c_str() << " to " << dd << " " << getMonthName (mm-1).c_str() << " are "
                       << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1)
                       << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    else logline << "From " << dd << " " << getMonthName (mm-1).c_str() << " to " << dd2 << " " << getMonthName (mm2-1).c_str() << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    } else {
                    if (dayleft > daynumb) logline << "From " << getMonthName (mm2-1).c_str() << " " << dd2 << " to " << getMonthName (mm-1).c_str() << " " << dd << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days or " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    else logline << "From " << getMonthName (mm-1).c_str() << " " << dd << " to " << getMonthName (mm2-1).c_str() << " " << dd2 << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                }
        }
        //qDebug() << buffer;
    return buffer;
}

QString add_dots(QString str,int length)
{
    int i;

    QByteArray bytes = str.toUtf8();
    int lengt = bytes.size(); //Number of bytes
    for (i=0;i<length-lengt-1;i++) str += "-";
    str += ">";
    return str;
}

int weeks_months(QDate startDate, QDate endDate, bool months, bool days)
{
    int wd1,wd2,nm1,nm2=0,y,m,startmonth=startDate.month(),endmonth=endDate.month();
    double w1;
    QDateTime qstartDate(startDate);
    QDateTime qendDate(endDate);
    if (!qstartDate.isValid()) qDebug() << " invalid start date ";
    if (!qendDate.isValid()) qDebug() << " invalid end date ";
    nm1 = startDate.daysTo(endDate);
    for (y=startDate.year();y<=endDate.year();y++){
        if (y>startDate.year()) {
            startmonth = 1;
            endmonth = 12;
        }
        if (y<endDate.year()) endmonth = 12;
        if (y==endDate.year()) endmonth=endDate.month()-1;

        for (m=startmonth;m<=endmonth;m++)
        {
            if (nm1 > 30){
              nm1 -=numberOfDays(m-1,y);
              nm2++;
            }

        }
    }
    wd1 = startDate.daysTo(endDate)/7;
    w1 = startDate.daysTo(endDate);
    w1 = w1/7-wd1;
    wd2 = round(w1*7);
   // qDebug() << count2 << " " << counter;
    //qDebug() << " weeks " << wd1 << " weeks " << wd2 << " days, " << nm2 << " months " << nm1 << " days";
    if (months && !days) return nm2;  // number of months
    if (months && days) return nm1;   // number of days left from months
    if (!months && !days) return wd1; // number of weeks
    if (!months && days) return wd2;  // number of days left from weeks
    return 0;
}


void readsolarfile(int dd, int mm, int year)
{
    int counter=0,linelength=0;
    string line;
    ifstream myfile;
    stringstream ss;

    myfile.open("solareclipse.txt");
    if (myfile.is_open())
          {
          //while ( getline (myfile,line) )
          while ( myfile >>line )
            {
        //qDebug() << QString::fromStdString(line);
        linelength = line.length();
        if (linelength > 8) {
        if (QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))) > QDate(year, mm, dd)) {
            //if (weeks_months(QDate(year, mm, dd),QDate(fields[0].toInt(), fields[1].toInt(), fields[2].toInt()),false,true) == 0) {*/
                zerodays[0][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),false,false);
                zerodays[1][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),false,true);
                zerodays[2][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),true,false);
                zerodays[3][counter] = weeks_months(QDate(year, mm, dd),QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),true,true);
                zerodays[4][counter] =  stoi(line.substr(8,2)) ;
                zerodays[5][counter] = stoi(line.substr(5,2)) ;
                zerodays[6][counter] = stoi(line.substr(0,4)) ;
                if (line.substr(11,1) == "T") zerodays[7][counter] = 1;
                if (line.substr(11,1) == "A") zerodays[7][counter] = 2;
                if (line.substr(11,1) == "P") zerodays[7][counter] = 3;
                if (line.substr(11,1) == "H") zerodays[7][counter] = 4;
                counter ++;
           // qDebug() << zerodays[0][counter] << " " << zerodays[1][counter] << " " << zerodays[2][counter] << " " << zerodays[3][counter] << " " << zerodays[4][counter] << " " << zerodays[5][counter] << " " << counter2;
            //qDebug() << fields[2].toInt() << " " << fields[1].toInt() << " " << fields[0].toInt() << " " << fields[3] << " " << line;

           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 && startoffile) break;
           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 ) startoffile=true;



            //if (counter == 50) break;

        }
        if (QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))) < QDate(year, mm, dd)) {
            //if (weeks_months(QDate(year, mm, dd),QDate(fields[0].toInt(), fields[1].toInt(), fields[2].toInt()),false,true) == 0) {*/
                zerodays[0][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),false,false);
                zerodays[1][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),false,true);
                zerodays[2][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),true,false);
                zerodays[3][counter] = weeks_months(QDate(stoi(line.substr(0,4)), stoi(line.substr(5,2)), stoi(line.substr(8,2))),QDate(year, mm, dd),true,true);
                zerodays[4][counter] = stoi(line.substr(8,2)) ;
                zerodays[5][counter] = stoi(line.substr(5,2)) ;
                zerodays[6][counter] = stoi(line.substr(0,4)) ;
                if (line.substr(11,1) == "T") zerodays[7][counter] = 1;
                if (line.substr(11,1) == "A") zerodays[7][counter] = 2;
                if (line.substr(11,1) == "P") zerodays[7][counter] = 3;
                if (line.substr(11,1) == "H") zerodays[7][counter] = 4;
                counter ++;
           // qDebug() << zerodays[0][counter] << " " << zerodays[1][counter] << " " << zerodays[2][counter] << " " << zerodays[3][counter] << " " << zerodays[4][counter] << " " << zerodays[5][counter] << " " << counter2;
            //qDebug() << fields[2].toInt() << " " << fields[1].toInt() << " " << fields[0].toInt() << " " << fields[3] << " " << line;

           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 && startoffile) break;
           // if (fields[0].toInt()== 1950 && fields[1].toInt() == 03 && fields[2].toInt() == 18 ) startoffile=true;



            //if (counter == 50) break;

        }
       }
    }
   }
    myfile.close();
    for (counter=0;counter<250;counter++) {
    ss << "Counter " << counter << " - Weeks " << zerodays[0][counter] << " and days " << zerodays[1][counter] << " - months " << zerodays[2][counter] << " and days " << zerodays[3][counter]
          << " - date " << zerodays[4][counter] << "/" << zerodays[5][counter] << "/" << zerodays[6][counter] << " - type " << zerodays[7][counter] << "\n";
   savetotemp(ss.str());
   ss.str("");
}
}


QString solareclipe(int dd,int mm,int year, int output, int type,bool eudate)
{
    QString buffer,line,solartype,solartype2,solartypeweeksbefore,solartypemonthsbefore,solartypeweeksafter,solartypemonthsafter,typetmp;

    int sdd2=0,smm2=0,syear2=0,sdd1=0,smm1=0,syear1=0,counter=0,solartype2int,solartypeint=0,solartmp;
    int ddmonthsbeforezero=0,mmmonthsbeforezero=0,yearmonthsbeforezero=0,ddweeksbeforezero=0,mmweeksbeforezero=0,yearweeksbeforezero=0;
    int ddmonthsafterzero=0,mmmonthsafterzero=0,yearmonthsafterzero=0,ddweeksafterzero=0,mmweeksafterzero=0,yearweeksafterzero=0;
    int beforeweeks=0,beforeweekdays=0,afterweeks=0,afterweeksdays=0,beforemonths=0,beforemonthsdays=0,aftermonths=0,aftermonthsdays=0,solartypeweeksbeforeint=0,solartypemonthsbeforeint=0,solartypeweeksafterint=0,solartypemonthsafterint=0;
    int beforeweekszero=0,beforeweekdayszero=0,afterweekszero=0,afterweeksdayszero=0,beforemonthszero=0,beforemonthszerodays=0,aftermonthszero=0,aftermonthsdayszero=0;
    int eudd,eumm,eusdd1,eusmm1,eusdd2,eusmm2,euddweeksbeforezero,eummweeksbeforezero,euddweeksafterzero,eummweeksafterzero,euddmonthsbeforezero,eummmonthsbeforezero,euddmonthsafterzero,eummmonthsafterzero;
    bool foundafter=false,foundafterweek=false,foundaftermonths=false;
    solartype2int=type;
    readsolarfile(dd,mm,year);

    while (counter < 249) {

        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && !foundafter && (type == zerodays[7][counter] || type == 5)) {

                sdd1=zerodays[4][counter];smm1=zerodays[5][counter];syear1=zerodays[6][counter];solartypeint=zerodays[7][counter];
                 beforeweeks=zerodays[0][counter];beforeweekdays=zerodays[1][counter];
                 beforemonths=zerodays[2][counter];beforemonthsdays=zerodays[3][counter];

        }
        if (zerodays[1][counter] == 0 && !foundafterweek && QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && (type == zerodays[7][counter] || type == 5)) {
            ddweeksbeforezero=zerodays[4][counter];mmweeksbeforezero=zerodays[5][counter];yearweeksbeforezero=zerodays[6][counter];solartypeweeksbeforeint=zerodays[7][counter];
            beforeweekszero=zerodays[0][counter];
            beforeweekdayszero=zerodays[1][counter];
             //qDebug() << "Weeks before " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,false) << " days " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,true) << " " << ddweeksbeforezero << " " << mmweeksbeforezero << " " << yearweeksbeforezero;
        } //found weeks before with zero days
        if (zerodays[3][counter] == 0 && !foundaftermonths && QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && (type == zerodays[7][counter] || type == 5)) {
            ddmonthsbeforezero=zerodays[4][counter];mmmonthsbeforezero=zerodays[5][counter];yearmonthsbeforezero=zerodays[6][counter];solartypemonthsbeforeint=zerodays[7][counter];
            beforemonthszero=zerodays[2][counter];
            beforemonthszerodays=zerodays[3][counter];

            //qDebug() << "Months before " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,false) << " days " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,true) << " " << ddmonthsbeforezero << " " << mmmonthsbeforezero << " " << yearmonthsbeforezero;
        } //found months before with zero days
     counter ++;
     if (zerodays[6][counter] != 0) {
     solartmp = zerodays[7][counter];

    //qDebug() << counter;
    if (type == solartmp || type == 5) {
     if (!foundafter) {
         afterweeks=zerodays[0][counter];
         afterweeksdays =zerodays[1][counter];
         aftermonths=zerodays[2][counter];
         aftermonthsdays=zerodays[3][counter];
         sdd2 = zerodays[4][counter];
         smm2 = zerodays[5][counter];
         syear2 = zerodays[6][counter];
         solartype2int = zerodays[7][counter];
     }
     if (!foundafterweek) {
         afterweekszero=zerodays[0][counter];
         afterweeksdayszero =zerodays[1][counter];
         ddweeksafterzero = zerodays[4][counter];
         mmweeksafterzero = zerodays[5][counter];
         yearweeksafterzero = zerodays[6][counter];
         solartypeweeksafterint = zerodays[7][counter];
     }
     if (!foundaftermonths) {
         aftermonthszero=zerodays[2][counter];
         aftermonthsdayszero=zerodays[3][counter];
         ddmonthsafterzero = zerodays[4][counter];
         mmmonthsafterzero = zerodays[5][counter];
         yearmonthsafterzero = zerodays[6][counter];
         solartypemonthsafterint = zerodays[7][counter];
     }


    }
    if (QDate(syear2, smm2, sdd2) > QDate(year, mm, dd)) foundafter=true; // found date after

    if (zerodays[1][counter] == 0 && !foundafterweek && QDate(zerodays[6][counter],zerodays[5][counter],zerodays[4][counter]) > QDate(year, mm, dd) && (type == solartmp || type == 5))  //found weeks after with zero days
           foundafterweek=true;
    else if (!foundafterweek) ddweeksafterzero =0;

        //qDebug() << "Weeks after " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,false) << " days " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,true) << " " << ddweeksafterzero << " " << mmweeksafterzero << " " << yearweeksafterzero;
        //break;
    if (zerodays[3][counter] == 0 && !foundaftermonths && QDate(zerodays[6][counter],zerodays[5][counter],zerodays[4][counter]) > QDate(year, mm, dd) && (type == solartmp || type == 5))  //found months after with zero days
            foundaftermonths=true;
    else if (!foundaftermonths) ddmonthsafterzero = 0;

    }
}

   // qDebug() << "Weeks before " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,false) << " days " << weeks_months(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero),QDate(year, mm, dd),false,true) << " " << ddweeksbeforezero << " " << mmweeksbeforezero << " " << yearweeksbeforezero;
   // qDebug() << "Months before " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,false) << " days " << weeks_months(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero),QDate(year, mm, dd),true,true) << " " << ddmonthsbeforezero << " " << mmmonthsbeforezero << " " << yearmonthsbeforezero;
   // qDebug() << "Weeks after " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,false) << " days " << weeks_months(QDate(year, mm, dd),QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero),false,true) << " " << ddweeksafterzero << " " << mmweeksafterzero << " " << yearweeksafterzero;
   // qDebug() << "Months after" << weeks_months(QDate(year, mm, dd),QDate(yearmonthsafterzero, mmmonthsafterzero, ddmonthsafterzero),true,false) << " days " << weeks_months(QDate(year, mm, dd),QDate(yearmonthsafterzero, ddmonthsafterzero, ddmonthsafterzero),true,true) << " " << ddmonthsafterzero << " " << ddmonthsafterzero << " " << yearmonthsafterzero;
    if (solartypeint == 1) solartype = "Total";
    if (solartypeint == 2) solartype = "Annular";
    if (solartypeint == 3) solartype = "Partial";
    if (solartypeint == 4) solartype = "Hybrid";
    if (solartype2int == 1) solartype2 = "Total";
    if (solartype2int == 2) solartype2 = "Annular";
    if (solartype2int == 3) solartype2 = "Partial";
    if (solartype2int == 4) solartype2 = "Hybrid";
    if (solartypeweeksbeforeint == 1) solartypeweeksbefore = "Total";
    if (solartypeweeksbeforeint == 2) solartypeweeksbefore = "Annular";
    if (solartypeweeksbeforeint == 3) solartypeweeksbefore = "Partial";
    if (solartypeweeksbeforeint == 4) solartypeweeksbefore = "Hybrid";
    if (solartypemonthsbeforeint == 1) solartypemonthsbefore = "Total";
    if (solartypemonthsbeforeint == 2) solartypemonthsbefore = "Annular";
    if (solartypemonthsbeforeint == 3) solartypemonthsbefore = "Partial";
    if (solartypemonthsbeforeint == 4) solartypemonthsbefore = "Hybrid";
    if (solartypeweeksafterint == 1) solartypeweeksafter = "Total";
    if (solartypeweeksafterint == 2) solartypeweeksafter = "Annular";
    if (solartypeweeksafterint == 3) solartypeweeksafter = "Partial";
    if (solartypeweeksafterint == 4) solartypeweeksafter = "Hybrid";
    if (solartypemonthsafterint == 1) solartypemonthsafter = "Total";
    if (solartypemonthsafterint == 2) solartypemonthsafter = "Annular";
    if (solartypemonthsafterint == 3) solartypemonthsafter = "Partial";
    if (solartypemonthsafterint == 4) solartypemonthsafter = "Hybrid";
    if (type == 1) typetmp = "Total";
    if (type == 2) typetmp = "Annular";
    if (type == 3) typetmp = "Partial";
    if (type == 4) typetmp = "Hybrid";
    if (type == 5) typetmp = "Any";
    QDateTime startDate(QDate(syear1, smm1, sdd1));
    QDateTime currentDate(QDate(year, mm, dd));
    QDateTime endDate(QDate(syear2, smm2, sdd2));
    QDateTime startDateweeksbefore(QDate(yearweeksbeforezero, mmweeksbeforezero, ddweeksbeforezero));
    QDateTime startDateweeksafter(QDate(yearweeksafterzero, mmweeksafterzero, ddweeksafterzero));
    QDateTime startDatemonthsbefore(QDate(yearmonthsbeforezero, mmmonthsbeforezero, ddmonthsbeforezero));
    QDateTime startDatemonthsafter(QDate(yearmonthsafterzero, mmmonthsafterzero, ddmonthsafterzero));

    if (eudate) {
        eudd = dd;
        eumm = mm;
        eusdd1 = sdd1;
        eusmm1 = smm1;
        eusdd2 = sdd2;
        eusmm2 = smm2;
        euddweeksbeforezero = ddweeksbeforezero;
        eummweeksbeforezero = mmweeksbeforezero;
        euddweeksafterzero = ddweeksafterzero;
        eummweeksafterzero = mmweeksafterzero;
        euddmonthsbeforezero = ddmonthsbeforezero;
        eummmonthsbeforezero = mmmonthsbeforezero;
        euddmonthsafterzero = ddmonthsafterzero;
        eummmonthsafterzero = mmmonthsafterzero;
    } else {
        eudd = mm;
        eumm = dd;
        eusdd1 = smm1;
        eusmm1 = sdd1;
        eusdd2 = smm2;
        eusmm2 = sdd2;
        euddweeksbeforezero = mmweeksbeforezero;
        eummweeksbeforezero = ddweeksbeforezero;
        euddweeksafterzero = mmweeksafterzero;
        eummweeksafterzero = ddweeksafterzero;
        euddmonthsbeforezero = mmmonthsbeforezero;
        eummmonthsbeforezero = ddmonthsbeforezero;
        euddmonthsafterzero = mmmonthsafterzero;
        eummmonthsafterzero = ddmonthsafterzero;
    }

    switch (output) {
    case 1: {// print out to textbrowser

        buffer += "<br>Searching after " + typetmp +" Solar Eclipse<br>";
        buffer += "<br>Current working date : " + QString::fromStdString(formattext(QString::number(eudd).toUtf8().constData(),1,1)) + "/" + QString::fromStdString(formattext(QString::number(eumm).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(year).toUtf8().constData(),1,1)) + "<br>"; // display entered currrent date

        buffer += "<br>Last " + solartype + " Solar Eclipse : " + QString::fromStdString(formattext(QString::number(eusdd1).toUtf8().constData(),1,1)) + "/" + QString::fromStdString(formattext(QString::number(eusmm1).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(syear1).toUtf8().constData(),1,1)) + "<br>Next " + solartype2 + " Solar Eclipse : " + QString::fromStdString(formattext(QString::number(eusdd2).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eusmm2).toUtf8().constData(),1,1)) + "/" + QString::fromStdString(formattext(QString::number(syear2).toUtf8().constData(),1,1));
        buffer += "<br><br>From last " + solartype + " Solar eclipse to current date are: " + QString::fromStdString(formattext(QString::number(startDate.daysTo(currentDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(beforeweeks).toUtf8().constData(),1,1)) + " Weeks and "
                + QString::fromStdString(formattext(QString::number(beforeweekdays).toUtf8().constData(),1,1)) + " days or<br>" //weeks ends
                + QString::fromStdString(formattext(QString::number(beforemonths).toUtf8().constData(),1,1))  + " Months and "
                + QString::fromStdString(formattext(QString::number(beforemonthsdays).toUtf8().constData(),1,1)) + " days<br><br>"; //before ends
        buffer += "From current date to next " + solartype2 + " Solar eclipse are: " + QString::fromStdString(formattext(QString::number(currentDate.daysTo(endDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(afterweeks).toUtf8().constData(),1,1)) + " weeks and "
                + QString::fromStdString(formattext(QString::number(afterweeksdays).toUtf8().constData(),1,1)) + " days or<br>" //weeks ends
                + QString::fromStdString(formattext(QString::number(aftermonths).toUtf8().constData(),1,1)) + " months and "
                + QString::fromStdString(formattext(QString::number(aftermonthsdays).toUtf8().constData(),1,1)) + " days.<br><br>"; //after ends

        if (ddweeksbeforezero > 0)
        buffer += "<br>Last " + solartypeweeksbefore + " Solar Eclipse with zero days after week: " + QString::fromStdString(formattext(QString::number(euddweeksbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummweeksbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearweeksbeforezero).toUtf8().constData(),1,1));
        if (ddweeksafterzero > 0)
        buffer += "<br>Next " + solartypeweeksafter + " Solar Eclipse with zero days after week: " + QString::fromStdString(formattext(QString::number(euddweeksafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummweeksafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearweeksafterzero).toUtf8().constData(),1,1)) + "<br><br>";
        if (ddweeksbeforezero > 0)
        buffer += "From last " + solartypeweeksbefore + " Solar eclipse to current date with zero days after week are: " + QString::fromStdString(formattext(QString::number(startDateweeksbefore.daysTo(currentDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(beforeweekszero).toUtf8().constData(),1,1)) + " Weeks and "
                + QString::fromStdString(formattext(QString::number(beforeweekdayszero).toUtf8().constData(),1,1)) + " days<br>"; //weeks before with zero days
        if (ddweeksafterzero > 0)
        buffer += "From current date to next " + solartypeweeksafter + " Solar eclipse are: " + QString::fromStdString(formattext(QString::number(currentDate.daysTo(startDateweeksafter)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(afterweekszero).toUtf8().constData(),1,1)) + " weeks and "
                + QString::fromStdString(formattext(QString::number(afterweeksdayszero).toUtf8().constData(),1,1)) + " days<br><br>"; //weeks after with zero days

        if (ddmonthsbeforezero > 0)
        buffer += "<br>Last " + solartypemonthsbefore + " Solar Eclipse with zero days after month: " + QString::fromStdString(formattext(QString::number(euddmonthsbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummmonthsbeforezero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearmonthsbeforezero).toUtf8().constData(),1,1));
        if (ddmonthsafterzero > 0)
        buffer += "<br>Next " + solartypemonthsafter + " Solar Eclipse with zero days after month: " + QString::fromStdString(formattext(QString::number(euddmonthsafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(eummmonthsafterzero).toUtf8().constData(),1,1))
                + "/" + QString::fromStdString(formattext(QString::number(yearmonthsafterzero).toUtf8().constData(),1,1)) + "<br><br>";
        if (ddmonthsbeforezero > 0)
        buffer += "From last " + solartypemonthsbefore + " Solar eclipse to current date with zero days after month are: " + QString::fromStdString(formattext(QString::number(startDatemonthsbefore.daysTo(currentDate)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(beforemonthszero).toUtf8().constData(),1,1)) + " months and "
                + QString::fromStdString(formattext(QString::number(beforemonthszerodays).toUtf8().constData(),1,1)) + " days<br>"; //months before with zero days
        if (ddmonthsafterzero > 0)
        buffer += "From current date to next " + solartypemonthsafter + " Solar eclipse are: " + QString::fromStdString(formattext(QString::number(currentDate.daysTo(startDatemonthsafter)).toUtf8().constData(),1,1)) + " days or<br>"
                + QString::fromStdString(formattext(QString::number(aftermonthszero).toUtf8().constData(),1,1)) + " months and "
                + QString::fromStdString(formattext(QString::number(aftermonthsdayszero).toUtf8().constData(),1,1)) + " days<br><br>"; //months after with zero days
        break;
    }
  }
    return buffer;
}

int searchzerodays(int ns,int type,int dd,int mm, int year)
{
    int counter;

    for (counter=0;counter<250;counter++) {
        if (dd==0) {
        if (zerodays[0][counter] == ns && (zerodays[7][counter] == type || type == 5) && zerodays[1][counter] == 0) return 1;
        if (zerodays[2][counter] == ns && (zerodays[7][counter] == type || type == 5) && zerodays[3][counter] == 0) return 2;
        //qDebug() << type;
        //if (ns == 303) qDebug() << ns;
        //qDebug() << " Type " << zerodays[7][counter];
        //if (zerodays[0][counter] == 303) qDebug() << " zerodays " << zerodays[0][counter];
        } else {

        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) > QDate(year, mm, dd) && zerodays[7][counter] == type) {

                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,true) == 0) return ns;
                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,true) == 0) return ns;


        }
        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && zerodays[7][counter] == type) {
            if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,false) == ns &&
                             weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,true) == 0) return ns;
            if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,false) == ns &&
                             weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,true) == 0) return ns;
        }
        }
       }
    return 0;
}


QString solar2history(int dd, int mm, int year, int type, bool eudate)
{
    string line;
    //type =2;
    QString buffer,solartype;
    stringstream logline;
    int lines=0;
    ifstream myfile;
    readsolarfile(dd,mm,year);
    buffer += "Searching History.txt for match where Phrase is connected to Solar Eclipse and current date of " + QString::number(dd) + "/" +QString::number(mm) + "/" + QString::number(year) + "<br>";
    myfile.open("history.txt");
    if (myfile.is_open())
          {

          while ( getline (myfile,line) )
            {
                ns = getwordnumericvalue(line,0,0,0);
                if (searchzerodays(ns,type,0,0,0) > 0){
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - English Ordinal",eudate,true);
                }
                ns = getwordnumericvalue(line,1,0,0);
                if (searchzerodays(ns,type,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Full Reduction",eudate,true);
                }
                ns = getwordnumericvalue(line,0,1,0);
                if (searchzerodays(ns,type,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Reverse Ordinal",eudate,true);
                }
                ns = getwordnumericvalue(line,1,1,0);
                if (searchzerodays(ns,type,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Reverse full Reduction",eudate,true);
                }
                if (single_r_on) {
                    ns = getwordnumericvalue(line,0,0,1);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Single Reduction",eudate,true);
                    }
                }
                if (francis_on) {
                    ns = getwordnumericvalue(line,0,0,2);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Francis Bacon",eudate,true);
                    }
                }
                if (satanic_on) {
                    ns = getwordnumericvalue(line,0,0,3);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Satanic",eudate,true);
                    }
                }
                if (jewish_on) {
                    ns = getwordnumericvalue(line,0,0,4);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Jewish",eudate,true);
                    }
                }
                if (sumerian_on) {
                    ns = getwordnumericvalue(line,0,0,5);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Sumerian",eudate,true);
                    }
                }
                if (rev_sumerian_on) {
                    ns = getwordnumericvalue(line,0,1,5);
                    if (searchzerodays(ns,type,0,0,0) > 0) {
                        lines ++;
                        buffer += printzerodays(dd,mm,year,ns,type," - " + formattext(line,2,1) + " - Reverse Sumerian",eudate,true);
                    }
                }
            }

          if (type == 1) solartype = "Total";
          if (type == 2) solartype = "Annular";
          if (type == 3) solartype = "Partial";
          if (type == 4) solartype = "Hybrid";
          if (type == 5) solartype = "";
     buffer += "<br>" + QString::fromStdString(formattext(std::to_string(lines),1,1)) + " words found matching "+solartype+" Solar Eclipses <br><br>";
     // buffer += QString::fromStdString(logline.str());
      logline << buffer.toStdString();
      logtime();
      savelog(logline.str());

    myfile.close();
   }

  else buffer += "Unable to open file";
  return buffer;
}

QString printzerodays(int dd, int mm, int year, int ns, int type, string detail, bool eudate,bool read)
{
    int counter;
    bool printbuffer=false;
    stringstream logline;
    QString buffer,solartype, se1,se2,se3,se4,se5,se6,se7;
    eraseAllSubStr(detail,"th ");
    //if (eudate) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << " There was a Annular Solar Eclipse exacly " << formattext(std::to_string(ns),1,1) << " weeks ago - " << detail << "<br>";
    if (eudate) se7 = QString::number(dd) + " " +  getMonthName (mm-1).c_str() + " - ";
    else se7 = QString::fromStdString(getMonthName (mm-1).c_str()) + " " +  QString::number(dd) + " - ";
    for (counter=0;counter<250;counter++) {
        if (detail == "listsolareclipses" && read) {
            se1 = "";
            se2 = "";
            se3 = " on ";
            if (eudate) {
                se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
            } else {
                se5 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                se4 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
            }
            se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
            if (zerodays[7][counter] == 1) solartype = "Total";
            if (zerodays[7][counter] == 2) solartype = "Annular";
            if (zerodays[7][counter] == 3) solartype = "Partial";
            if (zerodays[7][counter] == 4) solartype = "Hybrid";
            if ((zerodays[7][counter] == type || type == 5) && zerodays[4][counter] != 0) buffer += solartype + " Solar Eclipse " + se3 + se2 + se4 + "/" + se5 + "/" + se6 +"<br>";
        }
        else if (((zerodays[0][counter] == ns && (zerodays[7][counter] == type || type == 5)) || (zerodays[2][counter] == ns && (zerodays[7][counter] == type || type == 5))) && read) {
            se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
            se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
            se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
            if (zerodays[7][counter] == 1) solartype = "Total";
            if (zerodays[7][counter] == 2) solartype = "Annular";
            if (zerodays[7][counter] == 3) solartype = "Partial";
            if (zerodays[7][counter] == 4) solartype = "Hybrid";
            if (QDate(zerodays[6][counter],zerodays[5][counter],zerodays[4][counter]) < QDate(year, mm, dd)) {
            if (zerodays[0][counter] == ns) {
                se1 = " - There was a ";
                se2 = " weeks ago on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[0][counter]),1,1));
                printbuffer = true;
           } else {
                se1 = " - There was a ";
                se2 = " months ago on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
                printbuffer = true;
            }
          } else {
            if (zerodays[0][counter] == ns) {
                se1 = " - There is a ";
                se2 = " weeks from now on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[0][counter]),1,1));
                printbuffer = true;
           } else {
                se1 = " - There is a ";
                se2 = " months from now on ";
                se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
                printbuffer = true;
                }
            }
        } else {

            if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) > QDate(year, mm, dd) && zerodays[7][counter] == type) {
                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,true) == 0) {
                    se1 = " - There is a ";
                   se2 = " weeks from now on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,true) == 0) {
                    se1 = " - There is a ";
                    se2 = " months from now on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
        }
            if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && zerodays[7][counter] == type) {
                if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,false) == ns &&
                                 weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,true) == 0) {
                    se1 = " - There was a ";
                    se2 = " weeks ago on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
                if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,false) == ns &&
                                 weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,true) == 0) {
                    se1 = " - There was a ";
                    se2 = " months ago on ";
                    se3 = QString::fromStdString(formattext(std::to_string(ns),1,1));
                    se4 = QString::fromStdString(formattext(QString::number(zerodays[4][counter]).toUtf8().constData(),1,1));
                    se5 = QString::fromStdString(formattext(QString::number(zerodays[5][counter]).toUtf8().constData(),1,1));
                    se6 = QString::fromStdString(formattext(QString::number(zerodays[6][counter]).toUtf8().constData(),1,1));
                    if (zerodays[7][counter] == 1) solartype = "Total";
                    if (zerodays[7][counter] == 2) solartype = "Annular";
                    if (zerodays[7][counter] == 3) solartype = "Partial";
                    if (zerodays[7][counter] == 4) solartype = "Hybrid";
                    printbuffer = true;
                }
          }
        }
    }
    if (printbuffer) {

        if (eudate) buffer += se7 + solartype + " Solar Eclipse " + se3 + se2 + se4 + "/" + se5 + "/" + se6 + QString::fromStdString(detail)+"<br>";
        else buffer += se7 + solartype + " Solar Eclipse " + se3 + se2 + se5 + "/" + se4 + "/" + se6 + QString::fromStdString(detail)+"<br>";

        /*buffer += Qtotable("",1,0,0,0);
        if (eudate) buffer += Qtotable("",0,1,0,0)+Qtotable(se7,0,0,1,110) + Qtotable(solartype + " Solar Eclipse " + se3 + se2 + se4 + "/" + se5 + "/" + se6 + QString::fromStdString(detail),0,0,1,1000)+Qtotable("",0,2,0,0);
        else buffer += Qtotable("",0,1,0,0)+Qtotable(se7,0,0,1,110) + Qtotable(solartype + " Solar Eclipse " + se3 + se2 + se5 + "/" + se4 + "/" + se6 + QString::fromStdString(detail),0,0,1,1000)+Qtotable("",0,2,0,0);
        buffer += Qtotable("",2,0,0,0);*/
    }
    return buffer;
}


int getns(string phrase, int out, int pt)
{
    int ns=0;
    switch (out) {
    case 1 :
        ns = getwordnumericvalue(phrase,0,0,0);
        break;
    case 2 :
        ns = getwordnumericvalue(phrase,1,0,0);
        break;
    case 3 :
        ns = getwordnumericvalue(phrase,0,1,0);
        break;
    case 4 :
        ns = getwordnumericvalue(phrase,1,1,0);
        break;
    case 5 :
        if (single_r_on && getwordnumericvalue(phrase,0,0,1) != getwordnumericvalue(phrase,1,0,0)) ns = getwordnumericvalue(phrase,0,0,1);
        break;
    case 6 :
        if (francis_on && getwordnumericvalue(phrase,0,0,2) != getwordnumericvalue(phrase,0,0,0)) ns = getwordnumericvalue(phrase,0,0,2);
        break;
    case 7 :
        if (satanic_on) ns = getwordnumericvalue(phrase,0,0,3);
        break;
    case 8 :
        if (jewish_on) ns = getwordnumericvalue(phrase,0,0,4);
        break;
    case 9 :
        if (sumerian_on) ns = getwordnumericvalue(phrase,0,0,5);
        break;
    case 10 :
        if (rev_sumerian_on) ns = getwordnumericvalue(phrase,0,1,5);
        break;
    }
    if (pt == 1) ns = getnprime(ns);
    if (pt == 2) ns = getntriangular(ns);
    return ns;
}


int counter(string phrase, int dd, int mm, int year,int minimum,bool runsolar, bool prime, bool triangular, bool dates, bool chipers)
{
    int rank=0, i, nrns,ns=0,for_start=1,for_end=22;
    if (dates) {
        for_start=10;
        for_end=14;
        if (!chipers & !prime & !triangular) chipers=true;
    }
    for(nrns=1;nrns<=10;nrns++){
        for(i=for_start;i<=for_end;i++){
           if (chipers) if (getns(phrase,nrns,0) > 0) if (phrasetodate(getns(phrase,nrns,0),dd,mm,year,i)) rank += 1;
           if (prime) if (getns(phrase,nrns,1) > 0) if (phrasetodate(getns(phrase,nrns,1),dd,mm,year,i)) rank += 1;
           if (triangular) if (getns(phrase,nrns,2) > 0) if (phrasetodate(getns(phrase,nrns,2),dd,mm,year,i)) rank += 1;
        }
        if (runsolar) {
            ns = searchzerodays(getns(phrase,nrns,0),1,dd,mm,year);
            if (rank >= minimum-1) if (ns > 0) {
                rank += 101;

        //qDebug() << dd << " " << mm << " " << ns;
        }
      }
        //qDebug() << getns(phrase,nrns,0);
    }

    return rank;
}


QString phraserank(string phrase, bool eudate, int minimum, bool prime, bool triangular, bool dates, bool chipers)
{
    QString buffer="";
    int year_rank[5][500];
    string s1="",s2="",solar="";
    bool runsolar=false;
    if (minimum > 100) {
        minimum -= 100;
        runsolar=true;
    }
    readsolarfile(dd,mm,year);
    size_t found = phrase.find(",");
    if (found < phrase.length()) {
        s1 = phrase.substr(0,found);
        s2 = phrase.substr(found+1,phrase.length());
    } else s1 = phrase;
    //cout << s1 << " " << s2;
    int rank, days, mm2 = 12,pos=0,top=0,dd2 = 31;
    for (int i = 0; i < mm2; i++)
    {
        //days = numberOfDays (i, year);
        QDate cd(year,i+1,1);
        days = cd.daysInMonth();

        for (int j = 1; j <= days; j++)
        {
            if (j == dd2 && i == mm2) break;
            rank = counter(s1,j,i+1,year,minimum,runsolar, prime, triangular,dates,chipers);

            if (found < phrase.length() && counter(s2,j,i+1,year,minimum,runsolar, prime, triangular,dates,chipers) > 0 && rank > 0) rank+=counter(s2,j,i+1,year, minimum,runsolar, prime, triangular,dates,chipers);
            if (rank >= 100) {
                rank -= 100;
                solar = "T";
            } else solar = "";
            if (rank >= 100) rank -= 100;
            if (rank >= minimum) {
                year_rank[0][pos] = rank;
                year_rank[1][pos] = j;
                year_rank[2][pos] = i+1;
                year_rank[3][pos] = year;
                if (solar == "T") {
                    year_rank[4][pos] = 1;
                    //qDebug() << QString::fromStdString(solar);
                } else year_rank[4][pos] = 0;
                if (rank > top) top = rank;

                pos ++;
                //qDebug() << j << "/" << i+1 << " " << rank << " " << pos << " " << buffer.length();
            }
        }
    }
    //for (int t=top;t>=minimum;t--){
    for (int t=minimum;t<=top;t++){
        int dfound=0;
        if (s2 =="") buffer += "<br>" + QString::fromStdString(formattext(QString::number(t).toUtf8().constData(),1,0)) + " Hits found on phrase " +QString::fromStdString(formattext(s1,1,0)) + "<br>";
        else buffer += "<br>" +  QString::fromStdString(formattext(QString::number(t).toUtf8().constData(),1,0)) + " Hits found on phrase " +QString::fromStdString(formattext(s1,1,0)) + " & " +QString::fromStdString(formattext(s2,1,0))+"<br>";
    for (int i=0;i<=pos;i++) {
        if (year_rank[0][i] == t) {
            dfound++;
            if (year_rank[4][i] == 1) solar = "T";
            else solar = "";

            if (eudate)
            buffer += QString::number(year_rank[1][i])+" "+QString::fromStdString(getMonthName (year_rank[2][i]-1).c_str()) + " " + QString::fromStdString(formattext(solar,1,1)) +"<br>";
            else buffer += QString::fromStdString(getMonthName (year_rank[2][i]-1).c_str()) + " " + QString::number(year_rank[1][i])+ "<br>";
        }
        //qDebug() << t << " " << i << " " << year_rank[0][i] << " " << buffer.length();
    }
    buffer += QString::fromStdString(formattext(QString::number(dfound).toUtf8().constData(),1,0)) + " dates found.<br>";
    dfound=0;
}
    return buffer;
}



bool phrasetodate(int ns, int dd, int mm, int year, int i) {
    int d1, d2, m1 ,m2,daynumb,dayleft,DMY,MDY;
    int y1 = year/1000;
    int y2 = (year/100)-(y1*10);
    int y3 = (year-(y1*1000)-(y2*100))/10;
    int y4 = year-(y1*1000)-(y2*100)-(y3*10);


    QString qns;

    if (mm > 9)
          {
            m1 = 1;
            m2 = mm - 10;
          } else {
            m1 = 0;
            m2 = mm;
          }
    if (dd > 29)
          {
            d2 = dd-30;
            d1 = 3;
       } else if (dd > 19)
          {
            d2 = dd-20;
            d1 = 2;
       } else if (dd > 9)
          {
            d2 = dd-10;
            d1 = 1;
       } else
          {
            d2 = dd;
            d1 = 0;
          }
    daynumb = daynr(dd,mm,year);
    dayleft = daynrleft(dd,mm,year);
    DMY = eu_amdate(2, d1, d2, m1, m2,ns);
    MDY = eu_amdate(1, d1, d2, m1, m2,ns);

    //cout << eudate << "-" << amdate << "<br>";
   switch(i) {
      case 1 :
        if (ns == (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4) return true;
        break;
      case 2 :
        if (ns == (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4) return true;
        break;
      case 3 :
        if (ns == d1+d2+m1+m2+y1+y2+y3+y4) return true;
        break;
      case 4 :
        if (ns == (d1*10)+d2+(m1*10)+m2+(y3*10)+y4) return true;
        break;
      case 5 :
        if (ns == d1+d2+m1+m2+y3+y4) return true;
        break;
      case 6 :
        if (ns == (d1*10)+d2+(m1*10)+m2) return true;
        break;
      case 7 :
        if (ns == d1+d2+m1+m2+(y1*10)+y2+(y3*10)+y4) return true;
        break;
      case 8 :
        if (ns == (d1*10)+d2+(m1*10)+m2+y3+y4) return true;
        break;
      case 9 :
        if (ns == d1+d2+m1+m2+(y3*10)+y4) return true;
        break;
      case 10 :
        if (ns == daynumb) return true;
        break;
      case 11 :
        if (ns == dayleft) return true;
        break;
      case 12 :
        if (ns == DMY) return true;
        break;
      case 13 :
        if (ns == MDY) return true;
        break;
      case 14 :
        if (ns == dd) return true;
        break;
      case 15 :
        if (ns == getnprime(dayleft)) return true;
        break;
      case 16 :
        if (ns == getnprime(DMY)) return true;
        break;
      case 17 :
        if (ns == getnprime(MDY)) return true;
        break;
      case 18 :
        if (ns == getnprime(daynumb)) return true;
        break;
      case 19 :
        if (ns == getntriangular(dayleft)) return true;
        break;
      case 20 :
        if (ns == getntriangular(DMY)) return true;
        break;
      case 21 :
        if (ns == getntriangular(MDY)) return true;
        break;
      case 22 :
        if (ns == getntriangular(daynumb)) return true;
        break;
   case 23 :
     if (searchzerodays(ns,1,0,0,0) > 0) return true;
     //if (searchzerodays(ns,1,dd,mm,year) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 24 :
     if (searchzerodays(ns,2,0,0,0) > 0) return true;
     //  if (searchzerodays(ns,2,dd,mm,year) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 25 :
     if (searchzerodays(ns,3,0,0,0) > 0) return true;
     //  if (searchzerodays(ns,3,dd,mm,year) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 26 :
     if (searchzerodays(ns,4,0,0,0) > 0) return true;
     //  if (searchzerodays(ns,4,dd,mm,year) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 27 :
       if (ns == a_seconddate("day_d_s")) return true;
       break;
   case 28 :
       if (ns == a_seconddate("day_s_d")) return true;
       break;
   case 29 :
       if (ns == a_seconddate("week_d_s")) return true;
       break;
   case 30 :
       if (ns == a_seconddate("week_s_d")) return true;
       break;
   case 31 :
       if (ns == a_seconddate("month_d_s")) return true;
       break;
   case 32 :
       if (ns == a_seconddate("month_s_d")) return true;
       break;
   case 33 :
       if (ns == a_seconddate("days_full")) return true;
       break;
   case 34 :
       if (ns == a_seconddate("week_full")) return true;
       break;
   case 35 :
       if (ns == a_seconddate("month_full")) return true;
       break;
   }
 return false;
}

QString print_p_to_d(int ns, int dd, int mm, int year, int i, string detail, bool eudate) {
    int d1, dd2, m1 ,mm2;
    QString buffer;
    QString Qdate,Qns,dd_mm,d_d_m_m,YY_yy,Y_Y_y_y,yy,y_y;
    int y1 = year/1000,md1,md2;
    int yy2 = (year/100)-(y1*10);
    int y3 = (year-(y1*1000)-(yy2*100))/10;
    int y4 = year-(y1*1000)-(yy2*100)-(y3*10);
    int days=0,wd1=0,wd2=0;
    double w1 = 0;
    stringstream logline;
    logline.str("");

    if (mm > 9)
          {
            m1 = 1;
            mm2 = mm - 10;
          } else {
            m1 = 0;
            mm2 = mm;
          }
    if (dd > 29)
          {
            dd2 = dd-30;
            d1 = 3;
       } else if (dd > 19)
          {
            dd2 = dd-20;
            d1 = 2;
       } else if (dd > 9)
          {
            dd2 = dd-10;
            d1 = 1;
       } else
          {
            dd2 = dd;
            d1 = 0;
          }

        if (eudate) {
            Qdate = QString::number(d1)+QString::number(dd2)+" "+getMonthName (mm-1).c_str() + " - ";
            d_d_m_m = QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(dd2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(mm2).toUtf8().constData(),1,0));
            dd_mm = "("+QString::fromStdString(formattext(QString::number(d1*10+dd2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1*10+mm2).toUtf8().constData(),1,0))+")" ;
           } else {
            Qdate = QString::fromStdString(getMonthName (mm-1).c_str())+" "+QString::number(d1)+QString::number(dd2) + " - ";
            d_d_m_m = QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(mm2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(dd2).toUtf8().constData(),1,0));
            dd_mm = "("+QString::fromStdString(formattext(QString::number(m1*10+mm2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1*10+dd2).toUtf8().constData(),1,0))+")";
           }
            Qns = QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1));

            YY_yy = "("+QString::fromStdString(formattext(QString::number((y1*10)+yy2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
            Y_Y_y_y = QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(yy2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));
            yy = "("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
            y_y = QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));
       switch(i) {
          case 1 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << YY_yy.toUtf8().constData() << ")&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 2 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << Y_Y_y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 3 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << Y_Y_y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 4 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << yy.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 5 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 6 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 7 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << YY_yy.toUtf8().constData() << ")&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 8 :
              logline << Qdate.toUtf8().constData() << dd_mm.toUtf8().constData() << " + " << y_y.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 9 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << d_d_m_m.toUtf8().constData() << " + " << yy.toUtf8().constData() << "&emsp;= " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 10 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Is the " << Qns.toUtf8().constData() << "th day of the year - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 11 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;There is " << Qns.toUtf8().constData() << " days left of the year - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 12 :
              logline << Qdate.toUtf8().constData() << " &emsp;" <<  "Matches DMY style date as " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 13 :
              logline << Qdate.toUtf8().constData() << " &emsp;" << "Matches MDY style date as " << Qns.toUtf8().constData() << " - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 14 :
           logline << Qdate.toUtf8().constData() << " &emsp;" <<  "Matches date as " << Qns.toUtf8().constData() << " - " << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
              break;
          case 15 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Days left in the year is " << Qns.toUtf8().constData() << "th prime number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 16 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(2, d1, dd2, m1, mm2,ns)),1,1) << " is " << Qns.toUtf8().constData() << "th prime number from DMY style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 17 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(1, d1, dd2, m1, mm2,ns)),1,1) << " is " << Qns.toUtf8().constData() << "th prime number from MDY style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 18 :
           eraseAllSubStr(detail,"th ");
           logline << Qdate.toUtf8().constData() << " &emsp;Day of the year is " << Qns.toUtf8().constData() << "th prime number - " << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
              break;
          case 19 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;Days left in the year is " << Qns.toUtf8().constData() << "th triangular number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 20 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(2, d1, dd2, m1, mm2,ns)),1,1) << " is " << Qns.toUtf8().constData() << "th triangular number from DMY style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 21 :
              eraseAllSubStr(detail,"th ");
              logline << Qdate.toUtf8().constData() << " &emsp;" << formattext(std::to_string(eu_amdate(1, d1, dd2, m1, mm2, ns)),1,1) << " is " << Qns.toUtf8().constData() << "th triangular number from MDY style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 22 :
           eraseAllSubStr(detail,"th ");
           logline << Qdate.toUtf8().constData() << " &emsp;Day of the year is " << Qns.toUtf8().constData() << "th triangular number - " << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
              break;
       case 23 :
           //logline << printzerodays(dd,mm,year,ns,1);
           buffer += printzerodays(dd,mm,year,ns,1," - " + detail,eudate,true); // printing of phrase compare to solar eclipse
           savelog(logline.str());
           break;
       case 24 :
           //logline << searchzerodays(ns,2);
           buffer += printzerodays(dd,mm,year,ns,2," - " + detail,eudate,true);
           savelog(logline.str());
           break;
       case 25 :
           //logline << searchzerodays(ns,3);
           buffer += printzerodays(dd,mm,year,ns,3," - " + detail,eudate,true);
           savelog(logline.str());
           break;
       case 26 :
           buffer += printzerodays(dd,mm,year,ns,4," - " + detail,eudate,true);
           savelog(logline.str());
           break;
       case 27 :
           logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(a_seconddate("day_d_s")).toUtf8().constData(),1,0) << " Days from date to second date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 28 :
           logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(a_seconddate("day_s_d")).toUtf8().constData(),1,0) << " Days from second date to date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 29 :
           days = a_seconddate("day_d_s");
           wd1 = days/7;
           w1 = days;
           w1 = w1/7-wd1;
           wd2 = round(w1*7);
           if (wd2 == 0) logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(wd1).toUtf8().constData(),1,0) << " Weeks from date to second date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           else logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(wd1).toUtf8().constData(),1,0) << " Weeks and "  << formattext(QString::number(wd2).toUtf8().constData(),1,0) << " days from date to second date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 30 :
           days = a_seconddate("day_s_d");
           wd1 = days/7;
           w1 = days;
           w1 = w1/7-wd1;
           wd2 = round(w1*7);
           if (wd2 == 0) logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(wd1).toUtf8().constData(),1,0) << " Weeks from second date to date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           else logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(wd1).toUtf8().constData(),1,0) << " Weeks and "  << formattext(QString::number(wd2).toUtf8().constData(),1,0) << " days from second date to date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 31 :
           md1 = monthbeetween(mm,m2,a_seconddate("day_d_s"),'M');
           md2 = monthbeetween(mm,m2,a_seconddate("day_d_s"),'D');
           if (md2 == 0) logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(md1).toUtf8().constData(),1,0) << " Months from date to second date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           else  logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(md1).toUtf8().constData(),1,0) << " Months and "  << formattext(QString::number(md2).toUtf8().constData(),1,0) << " days from date to second date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 32 :
           md1 = monthbeetween(m2,mm,a_seconddate("day_s_d"),'M');
           md2 = monthbeetween(m2,mm,a_seconddate("day_s_d"),'D');
           if (md2 == 0) logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(md1).toUtf8().constData(),1,0) << " Months from second date to date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           else logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(md1).toUtf8().constData(),1,0) << " Months and "  << formattext(QString::number(md2).toUtf8().constData(),1,0) << " days from second date to date within year - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 33 :
           logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(a_seconddate("days_full")).toUtf8().constData(),1,0) << " Days between dates - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 34 :
           days = a_seconddate("days_full");
           wd1 = days/7;
           w1 = days;
           w1 = w1/7-wd1;
           wd2 = round(w1*7);
           if (wd2 == 0) logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(wd1).toUtf8().constData(),1,0) << " Weeks between dates - " << Qns.toUtf8().constData() << detail << "<br>";
           else logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(wd1).toUtf8().constData(),1,0) << " Weeks and "  << formattext(QString::number(wd2).toUtf8().constData(),1,0) << " days between dates - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       case 35 :
           int mt1,mt2;
           if (QDate(year,mm,dd) > QDate(y2,m2,d2)) {
               mt1 = m2;
               mt2 = mm;
           } else {
               mt1 = mm;
               mt2 = m2;
           }
           md1 = monthbeetween(mt1,mt2,a_seconddate("days_full"),'M');
           md2 = monthbeetween(mt1,mt2,a_seconddate("days_full"),'D');
           if (md2 == 0) logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(md1).toUtf8().constData(),1,0) << " Months between dates - " << Qns.toUtf8().constData() << detail << "<br>";
           else logline << Qdate.toUtf8().constData() << " " << formattext(QString::number(md1).toUtf8().constData(),1,0) << " Months and "  << formattext(QString::number(md2).toUtf8().constData(),1,0) << " days between dates - " << Qns.toUtf8().constData() << detail << "<br>";
           buffer += QString::fromStdString(logline.str());
           savelog(logline.str());
           break;
       }
       return buffer;
}


QString runanalyze(int dd, int mm, int year, string phrase,bool hlist, int filter, bool eudate) {
    stringstream ex1, logline;
    QString buffer,qt1,qt2;
    bool found=false,used=true;
    int i,ns=0;
    ex1.str("");
    found = false;
    if (!hlist) {
        logtime();
        logline.str("");
        logline << "<br>Statistic for " << formattext(phrase,2,3) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());
    }
    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal - start of compare cifers to date
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==1||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," English Ordinal",eudate);
        found = true;
    }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==2||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Full Reduction",eudate);
        found = true;
    }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==3||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Reverse Ordinal",eudate);
        found = true;
    }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==4||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Reverse Full Reduction",eudate);
        found = true;
    }
    if (filter==5||!hlist) ns = getwordnumericvalue(phrase,0,0,1); //Single Reduction
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==5||!hlist) && single_r_on && ns != getwordnumericvalue(phrase,1,0,0)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Single Reduction",eudate);
        found = true;
    }
    if (filter==6||!hlist) ns = getwordnumericvalue(phrase,0,0,2); //Francis Bacon
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==6||!hlist) && francis_on && ns != getwordnumericvalue(phrase,0,0,0)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Francis Bacon",eudate);
        found = true;
    }
    if (filter==7||!hlist) ns = getwordnumericvalue(phrase,0,0,3); //Satanic
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==7||!hlist) && satanic_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Satanic",eudate);
        found = true;
    }
    if (filter==8||!hlist) ns = getwordnumericvalue(phrase,0,0,4); //Jewish
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==8||!hlist) && jewish_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Jewish",eudate);
        found = true;
    }
    if (filter==9||!hlist) ns = getwordnumericvalue(phrase,0,0,5); //Sumerian
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==9||!hlist) && sumerian_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Sumerian",eudate);
        found = true;
    }
    if (filter==10||!hlist) ns = getwordnumericvalue(phrase,0,1,5); //Reverse Sumerian
    for(i=1;i<=35;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==10||!hlist) && rev_sumerian_on) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Reverse Sumerian",eudate);
        found = true;
    }
    if (found) {
        used = false;
        //ex1 << ns << " " << phrase.size() ;
        ex1 << formattext(std::to_string(ns),1,1) ;
    }

    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal - start of compare prime numbers to date
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==1||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from English Ordinal",eudate);
            found = true;
        }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==2||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Full Reduction",eudate);
            found = true;
        }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==3||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Reverse Ordinal",eudate);
            found = true;
        }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==4||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Reverse Full Reduction",eudate);
            found = true;
        }
    if (filter==5||!hlist) ns = getwordnumericvalue(phrase,0,0,1); //Single Reduction
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==5||!hlist)) && getnprime(ns) != 0 && single_r_on && getnprime(ns) != getnprime(getwordnumericvalue(phrase,1,0,0))) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Single Reduction",eudate);
            found = true;
        }
    if (filter==6||!hlist) ns = getwordnumericvalue(phrase,0,0,2); //Francis Bacon
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==6||!hlist)) && getnprime(ns) != 0 && francis_on && getnprime(ns) != getnprime(getwordnumericvalue(phrase,0,0,0))) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Francis Bacon",eudate);
            found = true;
        }
    if (filter==7||!hlist) ns = getwordnumericvalue(phrase,0,0,3); //Satanic
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==7||!hlist)) && getnprime(ns) != 0 && satanic_on) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Satanic",eudate);
            found = true;
        }
    if (filter==8||!hlist) ns = getwordnumericvalue(phrase,0,0,4); //Jewish
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==8||!hlist)) && getnprime(ns) != 0 && jewish_on) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Jewish",eudate);
            found = true;
        }
    if (filter==9||!hlist) ns = getwordnumericvalue(phrase,0,0,5); //Sumerian
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==9||!hlist)) && getnprime(ns) != 0 && sumerian_on) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Sumerian",eudate);
            found = true;
        }
    if (filter==10||!hlist) ns = getwordnumericvalue(phrase,0,1,5); //Reverse Sumerian
    for(i=1;i<=35;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==10||!hlist)) && getnprime(ns) != 0 && rev_sumerian_on) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Reverse Sumerian",eudate);
            found = true;
        }
    if (found && used) {
        used = false;
        ex1 << formattext(std::to_string(getnprime(ns)),1,1) << formattext("th Prime",2,2);
    }
    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal - start of compare triangular numbers to date
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==1||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from English Ordinal",eudate);
            found = true;
        }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==2||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Full Reduction",eudate);
            found = true;
        }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==3||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Reverse Ordinal",eudate);
            found = true;
        }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==4||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Reverse Full Reduction",eudate);
            found = true;
        }
    if (filter==5||!hlist) ns = getwordnumericvalue(phrase,0,0,1); //Single Reduction
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==5||!hlist)) && getntriangular(ns) != 0 && single_r_on && getntriangular(ns) != getntriangular(getwordnumericvalue(phrase,1,0,0))) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Single Reduction",eudate);
            found = true;
        }
    if (filter==6||!hlist) ns = getwordnumericvalue(phrase,0,0,2); //Francis Bacon
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==6||!hlist)) && getntriangular(ns) != 0 && francis_on && getntriangular(ns) != getntriangular(getwordnumericvalue(phrase,0,0,0))) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Francis Bacon",eudate);
            found = true;
        }
    if (filter==7||!hlist) ns = getwordnumericvalue(phrase,0,0,3); //Satanic
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==7||!hlist)) && getntriangular(ns) != 0 && satanic_on) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Satanic",eudate);
            found = true;
        }
    if (filter==8||!hlist) ns = getwordnumericvalue(phrase,0,0,4); //Jewish
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==8||!hlist)) && getntriangular(ns) != 0 && jewish_on) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Jewish",eudate);
            found = true;
        }
    if (filter==9||!hlist) ns = getwordnumericvalue(phrase,0,0,5); //Sumerian
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==9||!hlist)) && getntriangular(ns) != 0 && sumerian_on) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Sumerian",eudate);
            found = true;
        }
    if (filter==10||!hlist) ns = getwordnumericvalue(phrase,0,1,5); //Reverse Sumerian
    for(i=1;i<=35;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==10||!hlist)) && getntriangular(ns) != 0 && rev_sumerian_on) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Reverse Sumerian",eudate);
            found = true;
        }
    if (found && used) {
        ex1 << formattext(std::to_string(getntriangular(ns)),1,1) << formattext("th Triangular",2,2) ;
        used = false;
    }

    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal - start of compare prime related to date
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==1||!hlist)) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to English Ordinal",eudate);
        found = true;
    }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==2||!hlist)) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Full Reduction",eudate);
        found = true;
    }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==3||!hlist)) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Reverse Ordinal",eudate);
        found = true;
    }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==4||!hlist)) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Reverse Full Reduction",eudate);
        found = true;
    }
    if (filter==5||!hlist) ns = getwordnumericvalue(phrase,0,0,1); //Single Reduction
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==5||!hlist) && single_r_on && primes[ns-1] != getwordnumericvalue(phrase,1,0,0)) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Single Reduction",eudate);
        found = true;
    }
    if (filter==6||!hlist) ns = getwordnumericvalue(phrase,0,0,2); //Francis Bacon
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==6||!hlist) && francis_on && primes[ns-1] != getwordnumericvalue(phrase,0,0,0)) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Francis Bacon",eudate);
        found = true;
    }
    if (filter==7||!hlist) ns = getwordnumericvalue(phrase,0,0,3); //Satanic
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==7||!hlist) && satanic_on) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Satanic",eudate);
        found = true;
    }
    if (filter==8||!hlist) ns = getwordnumericvalue(phrase,0,0,4); //Jewish
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==8||!hlist) && jewish_on) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Jewish",eudate);
        found = true;
    }
    if (filter==9||!hlist) ns = getwordnumericvalue(phrase,0,0,5); //Sumerian
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==9||!hlist) && sumerian_on) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Sumerian",eudate);
        found = true;
    }
    if (filter==10||!hlist) ns = getwordnumericvalue(phrase,0,1,5); //Reverse Sumerian
    for(i=1;i<=35;i++)
    if (phrasetodate(primes[ns-1],dd,mm,year,i) && (filter==10||!hlist) && rev_sumerian_on) {
        if (!hlist) buffer += print_p_to_d(primes[ns-1],dd,mm,year,i," Prime related to Reverse Sumerian",eudate);
        found = true;
    }
    if (found && used) {
        used = false;
        //ex1 << ns << " " << phrase.size() ;
        ex1 << formattext(std::to_string(primes[ns-1]),1,1) << formattext(" Prime related",2,2) ;
    }

    if (found && hlist){
        qt2 = "";
        buffer += Qtotable("",1,0,0,0);
        for(i=1;i<=35;i++) {
            qt2 = detail(ns,dd,mm,year,i,eudate);
            if (qt2 !="") break;
            qt2 = detail(getnprime(ns),dd,mm,year,i,eudate);
            if (qt2 !="") break;
            qt2 = detail(getntriangular(ns),dd,mm,year,i,eudate);
            if (qt2 !="") break;
            qt2 = detail(primes[ns-1],dd,mm,year,i,eudate);
            if (qt2 !="") break;
        }
        logline << totable("",0,1,0,0) << totable(QString::fromStdString(formattext(phrase,2,2)).toUtf8().constData(),0,0,1,150)
                << totable(ex1.str(),0,0,1,120) << totable(qt2.toStdString(),0,0,1,420) << totable("",0,2,0,0);

      buffer += QString::fromStdString(logline.str());
      buffer += Qtotable("",2,0,0,0);
      savelog(logline.str());
    }

    if (!found){
        if (!hlist) {
            logline.str("");
            logline << "<br>No pattern found between date and phrase!<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());

        }
    }
    return buffer;
}

QString detail(int ns, int dd, int mm, int year, int i,bool eudate) {
    int d1, dd2, m1 ,mm2,daynumb,dayleft,DMY,MDY;
    int y1 = year/1000,md1,md2;
    int yy2 = (year/100)-(y1*10);
    int y3 = (year-(y1*1000)-(yy2*100))/10;
    int y4 = year-(y1*1000)-(yy2*100)-(y3*10);
    int days=0,wd1=0,wd2=0;
    double w1 = 0;
    QString Qdate,Qns,dd_mm,d_d_m_m,YY_yy,Y_Y_y_y,yy,y_y;

    QString qns;

    if (mm > 9)
          {
            m1 = 1;
            mm2 = mm - 10;
          } else {
            m1 = 0;
            mm2 = mm;
          }
    if (dd > 29)
          {
            dd2 = dd-30;
            d1 = 3;
       } else if (dd > 19)
          {
            dd2 = dd-20;
            d1 = 2;
       } else if (dd > 9)
          {
            dd2 = dd-10;
            d1 = 1;
       } else
          {
            dd2 = dd;
            d1 = 0;
          }
    daynumb = daynr(dd,mm,year);
    dayleft = daynrleft(dd,mm,year);
    DMY = eu_amdate(2, d1, dd2, m1, mm2,ns);
    MDY = eu_amdate(1, d1, dd2, m1, mm2,ns);
    if (eudate) {
     Qdate = QString::number(d1)+QString::number(dd2)+" "+getMonthName (mm-1).c_str() + " - ";
     d_d_m_m = QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(dd2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(mm2).toUtf8().constData(),1,0));
     dd_mm = "("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(dd2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(mm2).toUtf8().constData(),1,0))+")" ;
    } else {
     Qdate = QString::fromStdString(getMonthName (mm-1).c_str())+" "+QString::number(d1)+QString::number(dd2) + " - ";
     d_d_m_m = QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(mm2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(dd2).toUtf8().constData(),1,0));
     dd_mm = "("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(mm2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(dd2).toUtf8().constData(),1,0))+")";
    }
     Qns = QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1));

     YY_yy = "("+QString::fromStdString(formattext(QString::number((y1*10)+yy2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
     Y_Y_y_y = QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(yy2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));
     yy = "("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")";
     y_y = QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0));

    //cout << eudate << "-" << amdate << "<br>";
     if (ns == 0) return "";
   switch(i) {
      case 1 :
        if (ns == (d1*10)+dd2+(m1*10)+mm2+(y1*10)+yy2+(y3*10)+y4) return dd_mm+" + "+YY_yy;
        break;
      case 2 :
        if (ns == (d1*10)+dd2+(m1*10)+mm2+y1+yy2+y3+y4) return dd_mm+" + "+Y_Y_y_y;
        break;
      case 3 :
        if (ns == d1+dd2+m1+mm2+y1+yy2+y3+y4) return d_d_m_m+" + "+Y_Y_y_y;
        break;
      case 4 :
        if (ns == (d1*10)+dd2+(m1*10)+mm2+(y3*10)+y4) return dd_mm+" + "+yy;
        break;
      case 5 :
        if (ns == d1+dd2+m1+mm2+y3+y4) return d_d_m_m+" + "+y_y;
        break;
      case 6 :
        if (ns == (d1*10)+dd2+(m1*10)+mm2) return dd_mm;
        break;
      case 7 :
        if (ns == d1+dd2+m1+mm2+(y1*10)+yy2+(y3*10)+y4) return d_d_m_m+" + "+YY_yy;
        break;
      case 8 :
        if (ns == (d1*10)+dd2+(m1*10)+mm2+y3+y4) return dd_mm+" + "+y_y;
        break;
      case 9 :
        if (ns == d1+dd2+m1+mm2+(y3*10)+y4) return d_d_m_m+" + "+yy;
        break;
      case 10 :
        if (ns == daynumb) return "Day of the year";
        break;
      case 11 :
        if (ns == dayleft) return "Days left";
        break;
      case 12 :
        if (ns == DMY) return "Fits DM";
        break;
      case 13 :
        if (ns == MDY) return "Fits MD";
        break;
      case 14 :
        if (ns == dd) return "Equal date";
        break;
      case 15 :
        if (ns == getnprime(dayleft)) return " prime from days left";
        break;
      case 16 :
        if (ns == getnprime(DMY)) return " prime from DM";
        break;
      case 17 :
        if (ns == getnprime(MDY)) return " prime from MD";
        break;
      case 18 :
        if (ns == getnprime(daynumb)) return " prime from day of the year";
        break;
      case 19 :
        if (ns == getntriangular(dayleft)) return " triangular from days left";
        break;
      case 20 :
        if (ns == getntriangular(DMY)) return " triangular from DM";
        break;
      case 21 :
        if (ns == getntriangular(MDY)) return " triangular from MD";
        break;
      case 22 :
        if (ns == getntriangular(daynumb)) return " triangular from day of the year";
        break;

   case 23 :
     if (searchzerodays(ns,1,0,0,0) > 0) return "Hit on Total Solar Eclipse";
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 24 :
     if (searchzerodays(ns,2,0,0,0) > 0) return "Hit on Partial Solar Eclipse";
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 25 :
     if (searchzerodays(ns,3,0,0,0) > 0) return "Hit on Annular Solar Eclipse";
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 26 :
     if (searchzerodays(ns,4,0,0,0) > 0) return "Hit on Hybrid Solar Eclipse";
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 27 :
     if (ns == a_seconddate("day_d_s")) return " " + Qformattext(QString::number(a_seconddate("day_d_s")),1,0) + " Days from date to second date within year";
     break;
   case 28 :
       if (ns == a_seconddate("day_s_d")) return " " + Qformattext(QString::number(a_seconddate("day_s_d")),1,0) + " Days from second date to date within year";
       break;
   case 29 :
       days = a_seconddate("day_d_s");
       wd1 = days/7;
       w1 = days;
       w1 = w1/7-wd1;
       wd2 = round(w1*7);
       if (ns == a_seconddate("week_d_s") && wd2 == 0) return " " + Qformattext(QString::number(wd1),1,0) + " Weeks from date to second date within year";
       if (ns == a_seconddate("week_d_s") && wd2 > 0) return " " + Qformattext(QString::number(wd1),1,0) + " Weeks and "  + Qformattext(QString::number(wd2),1,0) +" days from date to second date within year";
       break;
   case 30 :
       days = a_seconddate("day_s_d");
       wd1 = days/7;
       w1 = days;
       w1 = w1/7-wd1;
       wd2 = round(w1*7);
       if (ns == a_seconddate("week_s_d") && wd2 == 0) return " " + Qformattext(QString::number(wd1),1,0) + " Weeks from second date to date within year";
       if (ns == a_seconddate("week_s_d") && wd2 > 0) return " " + Qformattext(QString::number(wd1),1,0) + " Weeks and "  + Qformattext(QString::number(wd2),1,0) +" days from second date to date within year";
       break;
   case 31 :
       md1 = monthbeetween(mm,m2,a_seconddate("days_d_s"),'M');
       md2 = monthbeetween(mm,m2,a_seconddate("days_d_s"),'D');
       if (ns == a_seconddate("month_d_s") && md2 == 0) return " " + Qformattext(QString::number(md1),1,0) + " Months from date to second date within year";
       if (ns == a_seconddate("month_d_s") && md2 > 0) return " " + Qformattext(QString::number(md1),1,0) + " Months and "  + Qformattext(QString::number(md2),1,0) +" days from date to second date within year";
       break;
   case 32 :
       md1 = monthbeetween(mm,m2,a_seconddate("days_s_d"),'M');
       md2 = monthbeetween(mm,m2,a_seconddate("days_s_d"),'D');
       if (ns == a_seconddate("month_s_d") && md2 == 0) return " " + Qformattext(QString::number(md1),1,0) + " Months from second date to date within year";
       if (ns == a_seconddate("month_s_d") && md2 > 0) return " " + Qformattext(QString::number(md1),1,0) + " Months and "  + Qformattext(QString::number(md2),1,0) +" days from second date to date within year";
       break;
   case 33 :
     if (ns == a_seconddate("days_full")) return " " + Qformattext(QString::number(a_seconddate("days_full")),1,0) + " days between dates";
     break;
   case 34 :
       days = a_seconddate("days_full");
       wd1 = days/7;
       w1 = days;
       w1 = w1/7-wd1;
       wd2 = round(w1*7);
       if (ns == a_seconddate("week_full") && wd2 == 0) return " " + Qformattext(QString::number(wd1).toUtf8().constData(),1,0) + " Weeks between dates";
       if (ns == a_seconddate("week_full") && wd2 != 0) return " " + Qformattext(QString::number(wd1).toUtf8().constData(),1,0) + " Weeks and "  + Qformattext(QString::number(wd2).toUtf8().constData(),1,0) +" days between dates";
       break;
   case 35 :
       int mt1,mt2;
       if (QDate(year,mm,dd) > QDate(y2,m2,d2)) {
           mt1 = m2;
           mt2 = mm;
       } else {
           mt1 = mm;
           mt2 = m2;
       }
       md1 = monthbeetween(mt1,mt2,a_seconddate("days_full"),'M');
       md2 = monthbeetween(mt1,mt2,a_seconddate("days_full"),'D');
       if (ns == a_seconddate("month_full") && md2 == 0) return " " + Qformattext(QString::number(md1).toUtf8().constData(),1,0) + " Months between dates";
       if (ns == a_seconddate("month_full") && md2 > 0) return " " + Qformattext(QString::number(md1).toUtf8().constData(),1,0) + " Months and "  + Qformattext(QString::number(md2).toUtf8().constData(),1,0) +" days between dates";
       break;
   }
 return "";
}

QString analyze(int dd, int mm, int year, QString Qphrase,bool hlist, int filter, bool eudate) {


        bool dogoanalize = true;
        std::string phrase = Qphrase.toUtf8().constData();




        QString buffer;


        std::vector<std::string> v;

        std::istringstream is( phrase );

        std::string word;
        if (dogoanalize && getwordnumericvalue(phrase,0,0,0) > 0) buffer += runanalyze( dd, mm, year, phrase, hlist, filter,eudate);
        while ( is >> word && dogoanalize) {
        v.push_back( word );
        eraseAllSubStr(word,"<br>");
        eraseAllSubStr(phrase,"<br>");
        if (getwordnumericvalue(phrase,0,0,0) == getwordnumericvalue(word,0,0,0)) break;
        else if (getwordnumericvalue(word,0,0,0) > 0) buffer += runanalyze( dd, mm, year, word, hlist, filter,eudate);
        }
        dogoanalize = false;
        return buffer;
    }

QString date2history(int dd, int mm, int year,bool hlist, bool eudate,int filter)
{
    string line;
    stringstream logline;
    QString buffer;
    int d1,d2;
    if (eudate) {d1=dd;d2=mm;}
    else {d1=mm;d2=dd;}
    switch(filter) {
          case 1 :
              logline << "<br>English Ordinal - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
          case 2 :
              logline << "<br>Full Reduction - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
          case 3 :
              logline << "<br>Reverse Ordinal - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
          case 4 :
              logline << "<br>Reverse Full Reduction - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
            break;
        case 5 :
            //single_r_on = true;
            logline << "<br>Single Reduction - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
            buffer += QString::fromStdString(logline.str());
            savelog(logline.str());
          break;
        case 6 :
            //francis_on = true;
            logline << "<br>Francis Bacon - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
            buffer += QString::fromStdString(logline.str());
            savelog(logline.str());
          break;
        case 7 :
            //satanic_on = true;
            logline << "<br>Satanic - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
            buffer += QString::fromStdString(logline.str());
            savelog(logline.str());
          break;
    case 8 :
        //jewish_on = true;
        logline << "<br>Jewis - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());
      break;
    case 9 :
        //sumerian_on = true;
        logline << "<br>Sumerian - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());
      break;
    case 10 :
        //sumerian_on = true;
        logline << "<br>Reverse Sumerian - connected to date. " << formattext(std::to_string(d1),1,1) << "/" << formattext(std::to_string(d2),1,1) << "/" << formattext(std::to_string(year),1,1) <<"<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());
      break;
    }


    ifstream myfile;
    myfile.open("history.txt");
    if (myfile.is_open())
          {

            while ( getline (myfile,line) )
            {
                buffer += runanalyze(dd, mm, year, line, hlist,filter,eudate);

            }

    myfile.close();
    }
    return buffer;
}

string char2string(char c)
{

    std::string str(1,c);

    return str;
}

string totable(string str, int start, int endrow, int colum, int width)
{
    if (start == 1) str = "<table>";
    if (endrow == 1) str = "<tr>";
    if (colum == 1) str = "<td width=\"" + to_string(width) + "\">" + str + "</td>";
    if (endrow == 2) str = "</tr>";
    if (start == 2) str = "</table>";
    return str;
}

QString Qtotable(QString str, int start, int endrow, int colum, int width)
{
    //QString str;
    if (start == 1) str = "<table>";
    if (endrow == 1) str = "<tr>";
    if (colum == 1) str = "<td width=\"" + QString::number(width) + "\">" + str + "</td>";
    if (endrow == 2) str = "</tr>";
    if (start == 2) str = "</table>";
    return str;
}

QString listciphers(int reduced, int reversed, int type) {
    int i = 1;
    double t1;
    char l=0;
    int jcount=0;
    stringstream ss;
    //QString buffer;
    //ss << "<tbody><tr>";
    ss << totable("",1,1,0,70);

    for (char c = 'A'; c <= 'Z'; c++)
    {
        std::string s(1,c);
        t1 = i;
        if (type == 2) l= c+32;
        //if (type != 2 && type != 4) ss << "<td>" << c << " = " << getwordnumericvalue(tolowerCase(s),reduced,reversed,type) << "</td>";
        if (type != 2 && type != 4) ss << totable(formattext(char2string(c),1,1) + " = " + formattext(to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0),0,0,1,70);
        //if (type != 2 && type != 4) ss << totable(char2string(c) + " = " + to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0,1);
        if (type == 4 && (tolowerCase(s) != "j" ) && (tolowerCase(s) != "&" ) &&
                (tolowerCase(s) != "v" ) && (tolowerCase(s) != "w" )) {
            if (tolowerCase(s) == "z") jcount = 1;
            if (tolowerCase(s) == "i") i--;
            ss << totable(formattext(char2string(c),1,1) + " = " + formattext(to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0),0,0,1,70);
        }
        if (type == 2) ss << totable(formattext(char2string(l),1,1) + " = " + formattext(to_string(getwordnumericvalue(tolowerCase(s),reduced,reversed,type)),0,0),0,0,1,70);
        if (floor(i/9) == t1/9) ss << "</tr><tr>";

        if (type == 4 && jcount == 1) {
            ss << totable(formattext("J",1,1) + " = " + formattext(to_string(getwordnumericvalue("j",reduced,reversed,type)),0,0),0,0,1,70);
            jcount = 2;
        }
        if (type == 4 && jcount == 2) {
            ss << totable(formattext("V",1,1) + " = " + formattext(to_string(getwordnumericvalue("v",reduced,reversed,type)),0,0),0,0,1,70);
            jcount = 3;
        }
        if (type == 4 && jcount == 3) {
            ss << totable(formattext("&",1,1) + " = " + formattext(to_string(getwordnumericvalue("&",reduced,reversed,type)),0,0),0,0,1,70);
            jcount = 4;
        }
        if (type == 4 && jcount == 4) ss << totable(formattext("W",1,1) + " = " + formattext(to_string(getwordnumericvalue("w",reduced,reversed,type)),0,0),0,0,1,70);
        i++;
    }

    if (type == 2) {
        i = 1;
        ss << "</tr><br><tr>";
        for (char c = 'A'; c <= 'Z'; c++)
        {
            std::string s(1,c);
            t1 = i;
            ss << totable(formattext(char2string(c),1,1) + " = " + formattext(to_string(getwordnumericvalue(s,reduced,reversed,type)),0,0),0,0,1,70);
            if (floor(i/9) == t1/9) ss << "</tr><tr>";
            i++;
        }


    }
    //ss << "</tr></tbody>";
    ss << totable("",2,2,0,70);
    //qDebug() << QString::fromStdString(ss.str());
    return QString::fromStdString(ss.str());
    //return QString::fromStdString(formattext(ss.str(),1,1));
    //return "<html><head><style>thead {color:green;}tbody {color:blue;}tfoot {color:red;}table, th, td {border: 1px solid black;}</style></head><body><table>  <thead>    <tr>      <th width=\"300\">Month</th>      <th width=\"200\">Savings</th>    </tr>  </thead>  <tbody>    <tr>      <td>January</td>      <td>$100</td>    </tr>    <tr>      <td>February</td>      <td>$80</td>    </tr>  </tbody>  <tfoot>    <tr>      <td>Sum</td>      <td>$180</td>    </tr>  </tfoot></table><p><b>Tip:</b> The thead, tbody, and tfoot elements will not affect the layout of the table by default. However, you can use CSS to style these elements.</p></body></html>";
}

string charnumeric(int reduced, int reversed, string phrase, int type) {
    int i = 1;
    int lengt=phrase.length();
    stringstream ss,s1;
    for (i=0;i<lengt;i++)
    {
        s1 << phrase[i];
        if (i < lengt-1 && getwordnumericvalue(s1.str(),reduced,reversed,type) > 0) ss << getwordnumericvalue(s1.str(),reduced,reversed,type) << "+";
        else if (getwordnumericvalue(s1.str(),reduced,reversed,type) > 0)
        ss << getwordnumericvalue(s1.str(),reduced,reversed,type) << "=";
        s1.str("");
    }
    return ss.str();
}

QString printallwords(string line, char save, bool header, bool simpleprint)
{
    bool dogoanalize = true;
    //std::string phrase = Qphrase.toUtf8().constData();

    QString buffer;


        std::vector<std::string> v;

        std::istringstream is( line );

        std::string word;
        if (dogoanalize) buffer += printword(line,save,header,simpleprint) + "<br>";
        while ( is >> word && dogoanalize) {
        v.push_back( word );
        eraseAllSubStr(word,"<br>");
        eraseAllSubStr(line,"<br>");
        if (getwordnumericvalue(line,0,0,0) == getwordnumericvalue(word,0,0,0)) break;
        else buffer += printword(word,save,header,simpleprint) + "<br>";
        }
        dogoanalize = false;
        return buffer;

}

QString printword(string line, char save, bool header, bool simpleprint)
{
    int ns1,ns2,ns3,ns4,ns5,ns6,ns7,ns8,ns9,ns10;
    string tabs;
    QString buffer,qs1,qs2,qs3,qs4,qs5,qs6="",qs7="",qs8="",qs9="",qs10="",qs11="";
    ns1 = getwordnumericvalue(line,0,0,0); //English Ordinal
    ns2 = getwordnumericvalue(line,1,0,0); //Full Reduction
    ns3 = getwordnumericvalue(line,0,1,0); //Reverse Ordinal
    ns4 = getwordnumericvalue(line,1,1,0); //Reverse Full Reduction
    ns5 = getwordnumericvalue(line,0,0,1); //Single Reduction
    ns6 = getwordnumericvalue(line,0,0,2); //Francis Bacon
    ns7 = getwordnumericvalue(line,0,0,3); //Satanic
    ns8 = getwordnumericvalue(line,0,0,4); //Jewish
    ns9 = getwordnumericvalue(line,0,0,5); //Sumerian
    ns10 = getwordnumericvalue(line,0,1,5); //Reverse Sumerian
    eraseAllSubStr(line,"<br>");
    stringstream logline;
    if (header) logtime();
    logline.str("");
    if (line.size() <= 7) tabs = "&emsp;&emsp;&emsp;";
    if (line.size() < 16 && line.size() >= 8) tabs = "&emsp;&emsp;";
    if (line.size() < 24 && line.size() >= 16) tabs = "&emsp;";
    if (line.size() > 24) tabs = " ";
    if (save != 'D' && !simpleprint) {
    logline << "Statistic for phrase : " << formattext(line,2,1) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
    logline.str("");
    logline << "English Ordinal : &emsp;" << charnumeric(0,0,line,0) << formattext(std::to_string(ns1),1,1) << " Prime? "<< isprime(ns1) << " Triangular? " << istriangular(ns1) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
    logline.str("");
    logline << "Full Reduction : &emsp;" << charnumeric(1,0,line,0) << formattext(std::to_string(ns2),1,1) << " Prime? "<< isprime(ns2) << " Triangular? " << istriangular(ns2) << "<br>";
    //logline << "Prime number&emsp;&emsp;" << isprime(ns1) << "" << isprime(ns2) << "" << isprime(ns3) << "" << isprime(ns4) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      logline.str("");
    //logline << "Triangular number&emsp;&emsp;" << istriangular(ns1) << "" << istriangular(ns2) << "" << istriangular(ns3) << "" << istriangular(ns4) << "<br><br>";
      logline << "Reverse Ordinal :&emsp; " << charnumeric(0,1,line,0) << formattext(std::to_string(ns3),1,1) << " Prime? "<< isprime(ns3) << " Triangular? " << istriangular(ns3) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      logline.str("");
      logline << "Reverse Full Reduction : " << charnumeric(1,1,line,0) << formattext(std::to_string(ns4),1,1) << " Prime? "<< isprime(ns4) << " Triangular? " << istriangular(ns4) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      if (single_r_on) {
          logline.str("");
          logline << "Single Reduction : &emsp;" << charnumeric(0,0,line,1) << formattext(std::to_string(ns5),1,1) << " Prime? "<< isprime(ns5) << " Triangular? " << istriangular(ns5) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (francis_on) {
          logline.str("");
          logline << "Francis Bacon : &emsp;" << charnumeric(0,0,line,2) << formattext(std::to_string(ns6),1,1) << " Prime? "<< isprime(ns6) << " Triangular? " << istriangular(ns6) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (satanic_on) {
          logline.str("");
          logline << "Satanic : &emsp;" << charnumeric(0,0,line,3) << formattext(std::to_string(ns7),1,1) << " Prime? "<< isprime(ns7) << " Triangular? " << istriangular(ns7) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (jewish_on) {
          logline.str("");
          logline << "Jewish : &emsp;" << charnumeric(0,0,line,4) << formattext(std::to_string(ns8),1,1) << " Prime? "<< isprime(ns8) << " Triangular? " << istriangular(ns8) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (sumerian_on) {
          logline.str("");
          logline << "Sumerian : &emsp;" << charnumeric(0,0,line,5) << formattext(std::to_string(ns9),1,1) << " Prime? "<< isprime(ns9) << " Triangular? " << istriangular(ns9) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
      if (rev_sumerian_on) {
          logline.str("");
          logline << "Reverse Sumerian : &emsp;" << charnumeric(0,1,line,5) << formattext(std::to_string(ns10),1,1) << " Prime? "<< isprime(ns10) << " Triangular? " << istriangular(ns10) << "<br>";
          buffer += QString::fromStdString(logline.str());
          savelog(logline.str());
      }
    }
    if (simpleprint) {
     logline << "<br>" << line << tabs << ns1 << "&emsp;" << ns2 << "&emsp;" << ns3 << "&emsp;" << ns4 << "&emsp;";
     //qs1 = add_dots(QString::fromStdString(formattext(line,2,2)),50);
     qs1 = QString::fromStdString(formattext(line,2,2));
     //qs1 = add_dots(QString::fromStdString(line),25);

     qs2 = QString::fromStdString(formattext(std::to_string(ns1),1,1));
     qs3 = QString::fromStdString(formattext(std::to_string(ns2),1,1));
     qs4 = QString::fromStdString(formattext(std::to_string(ns3),1,1));
     qs5 = QString::fromStdString(formattext(std::to_string(ns4),1,1));
     if (single_r_on) qs6 = Qtotable("SR " + QString::fromStdString(formattext(std::to_string(ns5),1,1)),0,0,1,70);
     if (francis_on) qs7 = Qtotable("FB " + QString::fromStdString(formattext(std::to_string(ns6),1,1)),0,0,1,70);
     if (satanic_on) qs8 = Qtotable("Sat " + QString::fromStdString(formattext(std::to_string(ns7),1,1)),0,0,1,70);
     if (jewish_on) qs9 = Qtotable("Jew " + QString::fromStdString(formattext(std::to_string(ns8),1,1)),0,0,1,70);
     if (sumerian_on) qs10 = Qtotable("Sum " + QString::fromStdString(formattext(std::to_string(ns9),1,1)),0,0,1,70);
     if (rev_sumerian_on) qs11 = Qtotable("RS " + QString::fromStdString(formattext(std::to_string(ns10),1,1)),0,0,1,70);
     //buffer += "<br>" + qs1 + "&emsp;EO " + qs2 + "&emsp;FR " + qs3 + "&emsp;RO " + qs4 + "&emsp;RF " + qs5 + "&emsp;" + qs6 + "&emsp;" + qs7 + "&emsp;" + qs8 + "&emsp;" + qs9;
     buffer += Qtotable("",0,1,0,0)+Qtotable(qs1,0,0,1,150) + Qtotable("EO " + qs2,0,0,1,70) + Qtotable("FR " + qs3,0,0,1,70) + Qtotable("RO " + qs4,0,0,1,70) + Qtotable("RF " + qs5,0,0,1,70) + qs6+qs7+qs8+qs9+qs10+qs11+Qtotable("",0,2,0,0);
     buffer += Qtotable("",2,0,0,0);
     savelog(logline.str());
    }
    if (int(line[0]) <= 122 && int(line[0]) >= 97) line[0] -= 32;
    if (save == 'D' && ns1 > 0) buffer += deletelastline();
    if (save == 'Y' && ns1 > 0) buffer += savetext(line);
    return buffer;
}


string firstletterupper(string line)
{


    string buffer;

    std::vector<std::string> v;

    std::istringstream is( line );

    std::string word;
    size_t found;

    while ( is >> word) {
    v.push_back( word );
    word[0] -= 32;
    found = line.find(" ");
    if (found != string::npos) buffer = buffer + word + " ";
    else buffer = word;

    }

    return buffer + "\n";
}

QString searchwords(int pattern, bool simpleprint) //Ctrl-H
{
    string line;
    QString buffer1,buffer2,qs5="",qs6="",qs7="",qs8="",qs9="",qs10="";
    QStringList histfound;
    bool header=true;
    stringstream logline;
    int c1, lines = 0;
    ifstream myfile;
    if (single_r_on) qs5 = " - Single Reduction";
    if (francis_on) qs6 = " - Francis Bacon";
    if (satanic_on) qs7 = " - Satanic";
    if (jewish_on) qs8 = " - Jewish";
    if (sumerian_on) qs9 = " - Sumerian";
    if (rev_sumerian_on) qs10 = " - Reverse Sumerian";
    if (simpleprint) buffer1 += "&emsp;&emsp;&emsp;Englis Ordinal - Full Reduction - Reverse Ordinal - Reverse Full Reduction" + qs5 + qs6 + qs7 + qs8 + qs9 + qs10;
    myfile.open("history.txt");
    if (myfile.is_open())
          {
          c1 = pattern;
          while ( getline (myfile,line) )
            {
                  if (findword(c1,line)) {
                      histfound.append(printword(line,0,header,simpleprint));
                      lines++;
                      header = false;

                  }
                 // savetotemp(firstletterupper(line));
            }
    //histfound = buffer.split("\n");
    histfound.sort();
    //buffer1 = histfound.join("\n");
    logline << "<br>" << formattext(std::to_string(lines),1,1) << " words found matching number " << formattext(std::to_string(c1),1,1) << "<br>";
      buffer2 += QString::fromStdString(logline.str());
      savelog(logline.str());

    myfile.close();
    //buffer += "</table>";
  }

  else buffer1 += "Unable to open file";
  return buffer1+histfound.join("\n")+buffer2;
}


QString searchhistory(int i, string phrase) {
    int ns=0;
    QString buffer;
    stringstream ss;
    switch(i) {

    case 1 :
        ns = getwordnumericvalue(phrase,0,0,0); //English Ordinal

        break;
    case 2 :
        ns = getwordnumericvalue(phrase,1,0,0); //Full Reduction

        break;
    case 3 :
        ns = getwordnumericvalue(phrase,0,1,0); //Reverse Ordinal

        break;
    case 4 :
        ns = getwordnumericvalue(phrase,1,1,0); //Reverse Full Reduction

        break;
    case 5 :
        ns = getwordnumericvalue(phrase,0,0,1); //Single Reduction

        break;
    case 6 :
        ns = getwordnumericvalue(phrase,0,0,2); //Francis Bacon

        break;
    case 7 :
        ns = getwordnumericvalue(phrase,0,0,3); //Satanic

        break;
    case 8 :
        ns = getwordnumericvalue(phrase,0,0,4); //Jewish

        break;
    case 9 :
        ns = getwordnumericvalue(phrase,0,0,5); //Sumerian

        break;
    case 10 :
        ns = getwordnumericvalue(phrase,0,1,5); //Reverse Sumerian

        break;
    }
    buffer += searchwords(ns,true);
    if (getnprime(ns) != 0) {
        ss << "Now searching with prime number " << formattext(std::to_string(getnprime(ns)),1,1) << "<br>";
        buffer += QString::fromStdString(ss.str());
        buffer += searchwords(getnprime(ns),true);
    }
    if (getntriangular(ns) != 0) {
        ss << "Now searching with triangular number " << formattext(std::to_string(getntriangular(ns)),1,1) << "<br>";
        buffer += QString::fromStdString(ss.str());
        buffer += searchwords(getntriangular(ns),true);
    }
    return buffer;
}

void eraseAllQSubStr(QString & mainStr, const QString & toErase)
{
    //size_t pos = std::string::npos;
    int pos;

    // Search for the substd::string in std::string in a loop untill nothing is found
    while ((pos  = mainStr.indexOf(toErase) )!= -1)
    {
        // If found then erase it from std::string
        mainStr.remove(pos, toErase.length());
    }
}

void erasefromToQSubStr(QString & mainStr, const QString & fromErase, const QString & toErase)
{

    int pos,pos2;
    // Search for the substd::string in std::string in a loop untill nothing is found
    while ((pos  = mainStr.indexOf(fromErase) )!= -1)
    {
        // If found then erase it from std::string
        pos2 = mainStr.indexOf(toErase,pos+2);
        //qDebug() << mainStr.mid(pos,pos2-pos+1);
        mainStr.remove(pos, pos2-pos+1);
    }
}

QString wordnumbericlist(QString head)
{
    QString returnlist="",word;
    int ns1,tot=0;
    const QStringList pieces = head.split( " " );
    for ( const auto& i : pieces  )
    {
        //qDebug() << i;
        for (int n=1;n<i.length()/2+2;n++) returnlist += " ";
        ns1 = getwordnumericvalue(i.toUtf8().constData(),0,0,0);
        tot += ns1;
        returnlist += QString::number(ns1);
        //returnlist += QString::fromStdString(formattext(QString::number(ns1).toUtf8().constData(),1,1));
        for (int n=1;n<i.length()/2+2;n++) returnlist += " ";
    }
    returnlist += "   = " + QString::number(tot);
    return returnlist;



   /* while ( is >> word && dogoanalize) {
    v.push_back( word );
    eraseAllSubStr(word,"<br>");
    eraseAllSubStr(phrase,"<br>");
    if (getwordnumericvalue(phrase,0,0,0) == getwordnumericvalue(word,0,0,0)) break;
    else if (getwordnumericvalue(word,0,0,0) > 0) buffer += runanalyze( dd, mm, year, word, hlist, filter,eudate);
    }*/
}

QString clean_news_content(QString content, QString pattern)
{

    if (pattern.indexOf("<span>") == -1) eraseAllQSubStr(content,"<span>");
    if (pattern.indexOf("</span>") == -1) eraseAllQSubStr(content,"</span>");
    erasefromToQSubStr(content,"<a href",">");
    eraseAllQSubStr(content,"\n");
    eraseAllQSubStr(content,"  ");
    if (pattern == "") {
        eraseAllQSubStr(content,"<a>");
        eraseAllQSubStr(content,"</a>");
        erasefromToQSubStr(content,"<span",">");

    }
return content;
}

bool isheadlines(QString content, QString pattern)
{
    QString head;
    bool isok=false;
    int pos1,pos2=0,count=0,average=0;
    do {
     pos2 = content.indexOf(pattern,pos2);
     if (pos2 != -1){
       pos1 = content.lastIndexOf(">",pos2-3);
       head = content.mid(pos1+1,pos2-pos1-1);

     }
     average += head.length();
     pos2++;
     count++;
     if (count == 20) break;
   }while (head != "");
    if (average/20 > 15 && count == 20) isok = true;
  return isok;
}

QString headline(QString content, QString pattern)
{
    QString head,pattern2;
    int pos1,pos2=0;
    pattern.remove(QRegExp("[\\n\\t\\r]"));
    pos1 = pattern.indexOf("|");
    if (pos1 != -1) {
        pattern2 = pattern.mid(0,pos1);
        pattern = pattern.mid(pos1+1,pattern.length()-pos1-1);
    }
    if (content != "") {
        if (pattern2 == "") {
         pos2 = content.indexOf(pattern,pos);

         if (pos2 != -1){
           pos1 = content.lastIndexOf(">",pos2-3);
           head = content.mid(pos1+1,pos2-pos1-1).trimmed();
        }
       } else {
         pos2 = content.indexOf(pattern2,pos);

         if (pos2 != -1){
           pos1 = content.indexOf(pattern,pos2);
           head = content.mid(pos2+pattern2.length(),pos1-pos2-pattern2.length()).trimmed();
         }
       }
    }
    pos = pos2+1;
    head.replace("\n"," ");

   return head;
}

QStringList getheadlines(QString source, int numberof)
{
    QFile infile,outfile;
    QString content="",head,pattern="";
    QStringList list;
    int count=1;
    pos=1;
    infile.setFileName(source);
    outfile.setFileName("headlines.txt");
    if (infile.open(QIODevice::ReadOnly)) {
        QTextStream in(&infile);
        content = in.readAll();
        //qDebug() << content.size();
        infile.close();
            //qDebug() << infile.size() << in.readAll();
    }
    QFile sources;
    QString line;
    int pos1;
    sources.setFileName("newssources.txt");
    if (sources.open(QIODevice::ReadOnly)) {
        while(!sources.atEnd()) {
            line = sources.readLine().trimmed();
            //QStringList fields = line.split("|");
            pos1 = line.indexOf(tmpstring);
            if (pos1 != -1) break;

        }
        sources.close();

            //qDebug() << infile.size() << in.readAll();
    }
    if (tmpstring.indexOf("|") != -1) line = tmpstring;
    pos1 = line.indexOf("|");
    if (pos1 != -1) pattern = line.mid(pos1+1,line.length()-pos1).trimmed();

    content = clean_news_content(content,pattern);
    if (pattern == "") {
        if (isheadlines(content,"</h1>")) pattern = "</h1>";
        if (isheadlines(content,"</h2>")) pattern = "</h2>";
        if (isheadlines(content,"</h3>")) pattern = "</h3>";
    }
    //qDebug() << pattern;
    do {
    head = headline(content,pattern);

    if ((head != "" && head.length() < 100) && (tmpstring.toLower().indexOf(head.toLower()) == -1)){
        list << head;
        //list << wordnumbericlist(head);
    }
    //qDebug() << head << " " << count;
    count++;
    if (count == numberof) break;
   }while (head != "");
    return list;
}


