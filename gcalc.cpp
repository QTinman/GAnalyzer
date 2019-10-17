#include "gcalc.h"
#include "tools.h"
#include "mainwindow.h"
#include <QDebug>
#include <QDate>
#include <QFile>
#include <QMessageBox>

#define BUFFERSIZE 256
using namespace std;

/*
  QString::fromStdString(string)  <- from string to Qstring
  QString::number(int) <- from int to QString
  string = QString.toUtf8().constData() from QString to string
  QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,1))
  std::to_string(42) int to string
 */

QString loopYear(int ns, int year, int printcal, bool eutime)
{
      int days, mm2 = 12, dd2 = 31;
      int y1 = year/1000;
      int y2 = (year/100)-(y1*10);
      int y3 = (year-(y1*1000)-(y2*100))/10;
      int y4 = year-(y1*1000)-(y2*100)-(y3*10);
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



             if (ns == (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4) { if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y1*10)+y2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y1*10)+y2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4) { if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+"+QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+"+QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == d1+d2+m1+m2+y1+y2+y3+y4) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;"+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;"+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(y1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y2).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == (d1*10)+d2+(m1*10)+m2+(y3*10)+y4) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+") + ("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+") + ("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == d1+d2+m1+m2+y3+y4) { if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;"+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;"+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == (d1*10)+d2+(m1*10)+m2) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")&emsp;&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")&emsp;&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == d1+d2+m1+m2+(y1*10)+y2+(y3*10)+y4) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;"+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + ("+QString::fromStdString(formattext(QString::number((y1*10)+y2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;"+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + ("+QString::fromStdString(formattext(QString::number((y1*10)+y2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == (d1*10)+d2+(m1*10)+m2+y3+y4) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+ "+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;("+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+")+("+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+")+ "+QString::fromStdString(formattext(QString::number(y3).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(y4).toUtf8().constData(),1,0))+"&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}
             if (ns == d1+d2+m1+m2+(y3*10)+y4) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;"+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + ("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;"+QString::fromStdString(formattext(QString::number(m1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(m2).toUtf8().constData(),1,0))+" + "+QString::fromStdString(formattext(QString::number(d1).toUtf8().constData(),1,0))+"+"+QString::fromStdString(formattext(QString::number(d2).toUtf8().constData(),1,0))+" + ("+QString::fromStdString(formattext(QString::number((y3*10)+y4).toUtf8().constData(),1,0))+")&emsp;= "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"<br>";}

             if (ns == daynr((d1*10)+d2,(m1*10)+m2,year)) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;It is the "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"th day of the year<br>";
             else buffer = buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;It is the "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+"th day of the year<br>";}
             if (ns == daynrleft((d1*10)+d2,(m1*10)+m2,year)) {if (eutime) buffer = buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;There are "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+" days left of the year<br>";
             else buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;There are "+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+" days left of the year<br>";}

             if (ns == eu_amdate(2, d1, d2, m1, m2, ns)) {if (eutime) buffer=buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;"+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+" fits Europian style of date of dd/mm<br>";
             else buffer=buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;"+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+" fits Europian style of date of dd/mm<br>";}
             if (ns == eu_amdate(1, d1, d2, m1, m2, ns)) {if (eutime) buffer=buffer+QString::number(d1)+QString::number(d2)+" "+getMonthName (i).c_str()+" - &emsp;"+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+" fits American style of date of mm/dd<br>";
             else buffer=buffer+getMonthName (i).c_str()+" "+QString::number(d1)+QString::number(d2)+" - &emsp;"+QString::fromStdString(formattext(QString::number(ns).toUtf8().constData(),1,1))+" fits American style of date of mm/dd<br>";}
             if (searchzerodays(ns,1,(d1*10)+d2,(m1*10)+m2,year) > 0) buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,1,"",eudate);
             if (searchzerodays(ns,2,(d1*10)+d2,(m1*10)+m2,year) > 0) buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,2,"",eudate);
             if (searchzerodays(ns,3,(d1*10)+d2,(m1*10)+m2,year) > 0) buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,3,"",eudate);
             if (searchzerodays(ns,4,(d1*10)+d2,(m1*10)+m2,year) > 0) buffer += printzerodays((d1*10)+d2,(m1*10)+m2,year,ns,4,"",eudate);
            }
        }
    }

    return buffer;
}



QString gcalc(int dd, int mm, int year, int dd2, int mm2, int yy2,bool eutime) {
    int d1, d2, m1 ,m2, ns, daynumb, dayleft, wd1, wd2, nm1,nm2,days,daysbeetween;
    double w1;
    QString buffer;
    stringstream logline;
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
    days = spanofdate(dd2,mm2,yy2,dd,mm,year);
    wd1 = spanofdate(dd2,mm2,yy2,dd,mm,year)/7;
    w1 = spanofdate(dd2,mm2,yy2,dd,mm,year);
    w1 = w1/7-wd1;
    wd2 = round(w1*7);
    if (dd2<=dd) {
        nm1 = mm - mm2 + (year - yy2) *12;
        nm2 = spanofdate(dd2,mm,year,dd,mm,year);
    }
    else {
        nm1 = mm - mm2-1 + (year - yy2) *12;
        nm2 = spanofdate(dd2,mm-1,year,dd,mm,year);
    }

    logtime();
    logline.clear();
    if (eutime) logline << "<br>Date of " << dd << "/" << mm << "/" << year << " is " << formattext(dayname(dayNumber(dd,mm,year)),1,2) << "<br>";
    else logline << "<br>Date of " << mm << "/" << dd << "/" << year << " is " << formattext(dayname(dayNumber(dd,mm,year)),1,2) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
        ns = (d1*10)+d2+(m1*10)+m2+(y1*10)+y2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);

        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " " << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        //buffer += formattext(QString::fromStdString(logline.str()),0,1);
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+y1+y2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+y1+y2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")&emsp;&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")&emsp;&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+20+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = (d1*10)+d2+(m1*10)+m2+y3+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        ns = d1+d2+m1+m2+(y3*10)+y4;
        if (searchzerodays(ns,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,1,"",eudate);
        if (searchzerodays(ns,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,2,"",eudate);
        if (searchzerodays(ns,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,3,"",eudate);
        if (searchzerodays(ns,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,ns,4,"",eudate);
        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        savelog(logline.str());

        logline.str("");


        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year" << " Prime? " << isprime(daynumb) << " Triangular? " << istriangular(daynumb) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Is the " << formattext(std::to_string(daynumb),1,1) << "th day of the year - " << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        if (searchzerodays(daynumb,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,1,"",eudate);
        if (searchzerodays(daynumb,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,2,"",eudate);
        if (searchzerodays(daynumb,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,3,"",eudate);
        if (searchzerodays(daynumb,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,daynumb,4,"",eudate);
        savelog(logline.str());

        logline.str("");
        if (eutime) logline << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year" << " Prime? " << isprime(dayleft) << " Triangular? " << istriangular(dayleft) << "<br>";
        else logline << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;There is " << formattext(std::to_string(dayleft),1,1) << " days left in the year - " << " Prime? " << isprime(ns) << " Triangular? " << istriangular(ns) << "<br>";
        buffer += QString::fromStdString(logline.str());
        if (searchzerodays(dayleft,1,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,1,"",eudate);
        if (searchzerodays(dayleft,2,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,2,"",eudate);
        if (searchzerodays(dayleft,3,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,3,"",eudate);
        if (searchzerodays(dayleft,4,0,0,0) > 0) buffer += printzerodays(dd,mm,year,dayleft,4,"",eudate);
        savelog(logline.str());
        logline.str("");
        if (dd2 != 0){
          if (eutime) {
              logline << "<br>" << d1 << d2 << getMonthName (mm-1).c_str() << " - &emsp;From" << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(days),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << d1 << d2 << getMonthName (mm-1).c_str() << " - &emsp;From" << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks +" << formattext(std::to_string(wd2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << d1 << d2 << getMonthName (mm-1).c_str() << " - &emsp;From" << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months +" << formattext(std::to_string(nm2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              } else {
              logline << "<br>" << getMonthName (mm-1).c_str() << d1 << d2 << " - &emsp;From" << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(days),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << getMonthName (mm-1).c_str() << d1 << d2 << " - &emsp;From" << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(wd1),1,1) << " weeks +" << formattext(std::to_string(wd2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");
              logline << getMonthName (mm-1).c_str() << d1 << d2 << " - &emsp;From" << dd2 << "/" << mm2 << "/" << yy2 << " there are " << formattext(std::to_string(nm1),1,1) << " months +" << formattext(std::to_string(nm2),1,1) << " days<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              logline.str("");}
                daynumb = daynr(dd2,mm2,year);
                dayleft = daynrleft(dd2,mm2,year);
                if (eutime) {
                    logline << dd2 << getMonthName (mm2-1).c_str() << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    logline << dd2 << getMonthName (mm2-1).c_str() << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    } else {
                    logline << getMonthName (mm2-1).c_str() << dd2 << " - &emsp;It is the " << formattext(std::to_string(daynumb),1,1) << "th day in the year<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    logline << getMonthName (mm2-1).c_str() << dd2 << " - &emsp;There are " << formattext(std::to_string(dayleft),1,1) << " days left in the year<br>";
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
                if (eutime) {
                    if (dayleft > daynumb) logline << "From " << dd << " " << getMonthName (mm2-1).c_str() << " to " << dd2 << " " << getMonthName (mm-1).c_str() << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    else logline << "From " << dd << " " << getMonthName (mm-1).c_str() << " to " << dd2 << " " << getMonthName (mm2-1).c_str() << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days <b>or</b> " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
                    buffer += QString::fromStdString(logline.str());
                    savelog(logline.str());
                    logline.str("");
                    } else {
                    if (dayleft > daynumb) logline << "From " << getMonthName (mm2-1).c_str() << " " << dd << " to " << getMonthName (mm-1).c_str() << " " << dd2 << " are " << formattext(std::to_string(daysbeetween),1,1) << " days <b>or</b> " << formattext(std::to_string(wd1),1,1) << " weeks+" << formattext(std::to_string(wd2),1,1) << " days or " << formattext(std::to_string(nm1),1,1) << " months+" << formattext(std::to_string(nm2),1,1) << " days<br>";
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
    int counter=0;
    string line;
    ifstream myfile;
    stringstream ss;

    myfile.open("solareclipse.txt");
    if (myfile.is_open())
          {
          while ( getline (myfile,line) )
            {

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

    while (counter < 250) {

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
    if (type == 2) typetmp = "Partial";
    if (type == 3) typetmp = "Annular";
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
        if (zerodays[0][counter] == ns && zerodays[7][counter] == type && zerodays[1][counter] == 0) return 1;
        if (zerodays[2][counter] == ns && zerodays[7][counter] == type && zerodays[3][counter] == 0) return 2;
        //if (ns == 303) qDebug() << ns;
        //if (zerodays[0][counter] == 303) qDebug() << " zerodays " << zerodays[0][counter];
        } else {

        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) > QDate(year, mm, dd) && zerodays[7][counter] == type) {

                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),false,true) == 0) return true;
                if (weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,false) == ns &&
                                 weeks_months(QDate(year, mm, dd),QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),true,true) == 0) return true;


        }
        if (QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]) < QDate(year, mm, dd) && zerodays[7][counter] == type) {
            if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,false) == ns &&
                             weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),false,true) == 0) return true;
            if (weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,false) == ns &&
                             weeks_months(QDate(zerodays[6][counter], zerodays[5][counter], zerodays[4][counter]),QDate(year, mm, dd),true,true) == 0) return true;
        }
        }
       }
    return 0;
}


QString solar2history(int dd, int mm, int year)
{
    string line;
    QString buffer;
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
                ns = getwordnumericvalue(line,0,0);
                if (searchzerodays(ns,1,0,0,0) > 0){
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,1," - Phrase: " + formattext(line,1,1),eudate);
                }
                ns = getwordnumericvalue(line,1,0);
                if (searchzerodays(ns,2,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,2," - Phrase: " + formattext(line,1,1),eudate);
                }
                ns = getwordnumericvalue(line,0,1);
                if (searchzerodays(ns,3,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,3," - Phrase: " + formattext(line,1,1),eudate);
                }
                ns = getwordnumericvalue(line,1,1);
                if (searchzerodays(ns,4,0,0,0) > 0) {
                    lines ++;
                    buffer += printzerodays(dd,mm,year,ns,4," - Phrase: " + formattext(line,1,1),eudate);
                }
            }

     logline << "<br>" << formattext(std::to_string(lines),1,1) << " words found matching Solar Eclipses <br><br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());

    myfile.close();
   }

  else buffer += "Unable to open file";
  return buffer;
}

QString printzerodays(int dd, int mm, int year, int ns, int type, string detail, bool eudate)
{
    int counter;
    bool printbuffer=false;
    QString buffer,solartype, se1,se2,se3,se4,se5,se6;
    eraseAllSubStr(detail,"th ");
    //if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << " There was a Annular Solar Eclipse exacly " << formattext(std::to_string(ns),1,1) << " weeks ago - " << detail << "<br>";
    for (counter=0;counter<250;counter++) {
        //qDebug() << zerodays[0][counter] << " " << zerodays[1][counter] << " " << zerodays[2][counter] << " " << zerodays[3][counter] << " " << zerodays[4][counter] << " " << zerodays[5][counter];
        if ((zerodays[0][counter] == ns && zerodays[7][counter] == type) || (zerodays[2][counter] == ns && zerodays[7][counter] == type)) {
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
                    se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
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
                    se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
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
                    se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
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
                    se3 = QString::fromStdString(formattext(std::to_string(zerodays[2][counter]),1,1));
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
        if (eudate)
        buffer += QString::number(dd) + " " +  getMonthName (mm-1).c_str() + se1 + solartype + " Solar Eclipse exacly "
                + se3 + se2 + se4 + "/" + se5 + "/" + se6 + QString::fromStdString(detail) +"<br>";
        else buffer += QString::fromStdString(getMonthName (mm-1).c_str()) + " " +  QString::number(dd) + se1 + solartype + " Solar Eclipse exacly "
                + se3 + se2 + se5 + "/" + se4 + "/" + se6 + QString::fromStdString(detail) +"<br>";
    }

    return buffer;
}

bool phrasetodate(int ns, int dd, int mm, int year, int i) {
    int d1, d2, m1 ,m2,daynumb,dayleft,eudate,amdate;
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
    eudate = eu_amdate(2, d1, d2, m1, m2,ns);
    amdate = eu_amdate(1, d1, d2, m1, m2,ns);

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
        if (ns == eudate) return true;
        break;
      case 13 :
        if (ns == amdate) return true;
        break;
      case 14 :
        if (ns == getnprime(daynumb)) return true;
        break;
      case 15 :
        if (ns == getnprime(dayleft)) return true;
        break;
      case 16 :
        if (ns == getnprime(eudate)) return true;
        break;
      case 17 :
        if (ns == getnprime(amdate)) return true;
        break;
      case 18 :
        if (ns == getntriangular(daynumb)) return true;
        break;
      case 19 :
        if (ns == getntriangular(dayleft)) return true;
        break;
      case 20 :
        if (ns == getntriangular(eudate)) return true;
        break;
      case 21 :
        if (ns == getntriangular(amdate)) return true;
        break;
   case 22 :
     if (searchzerodays(ns,1,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 23 :
     if (searchzerodays(ns,2,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 24 :
     if (searchzerodays(ns,3,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;
   case 25 :
     if (searchzerodays(ns,4,0,0,0) > 0) return true;
     //if (ns == solareclipe(dd,mm,year,2,"T").toInt()) return true; //output whole weeks before of type Total
     break;

   }
 return false;
}

QString print_p_to_d(int ns, int dd, int mm, int year, int i, string detail, bool eutime) {
    int d1, d2, m1 ,m2;
    QString buffer;
    int y1 = year/1000;
    int y2 = (year/100)-(y1*10);
    int y3 = (year-(y1*1000)-(y2*100))/10;
    int y4 = year-(y1*1000)-(y2*100)-(y3*10);
    stringstream logline;
    logline.str("");

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
       switch(i) {
          case 1 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 2 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 3 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y1),1,0) << "+" << formattext(std::to_string(y2),1,0) <<"+" << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 4 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 5 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 6 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 7 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y1*10)+y2),1,0) << ")+(" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 8 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ")+(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;(" << formattext(std::to_string(m1),1,0) << formattext(std::to_string(m2),1,0) << ")+(" << formattext(std::to_string(d1),1,0) << formattext(std::to_string(d2),1,0) << ") + " << formattext(std::to_string(y3),1,0) << "+" << formattext(std::to_string(y4),1,0) << "&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 9 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + " << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << formattext(std::to_string(m1),1,0) << "+" << formattext(std::to_string(m2),1,0) << " + " << formattext(std::to_string(d1),1,0) << "+" << formattext(std::to_string(d2),1,0) << " + (" << formattext(std::to_string((y3*10)+y4),1,0) << ")&emsp;= " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 10 :
              eraseAllSubStr(detail,"th ");
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;Is the " << formattext(std::to_string(ns),1,1) << "th day of the year - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Is the " << formattext(std::to_string(ns),1,1) << "th day of the year - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 11 :
              eraseAllSubStr(detail,"th ");
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;There is " << formattext(std::to_string(ns),1,1) << " days left of the year - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;There is " << formattext(std::to_string(ns),1,1) << " days left of the year - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 12 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" <<  "Matches European style date as " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" <<  "Matches European style date as " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 13 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << "Matches American style date as " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << "Matches American style date as " << formattext(std::to_string(ns),1,1) << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 14 :
              eraseAllSubStr(detail,"th ");
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;Day of the year is " << formattext(std::to_string(ns),1,1) << "th prime number - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Day of the year is " << formattext(std::to_string(ns),1,1) << "th prime number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 15 :
              eraseAllSubStr(detail,"th ");
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;Days left in the year is " << formattext(std::to_string(ns),1,1) << "th prime number - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Days left in the year is " << formattext(std::to_string(ns),1,1) << "th prime number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 16 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << eu_amdate(2, d1, d2, m1, m2,ns) << " is " << formattext(std::to_string(ns),1,1) << "th prime number from European style date - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << eu_amdate(2, d1, d2, m1, m2, ns) << " is " << formattext(std::to_string(ns),1,1) << "th prime number from European style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 17 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << eu_amdate(1, d1, d2, m1, m2,ns) << " is " << formattext(std::to_string(ns),1,1) << "th prime number from American style date - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << eu_amdate(1, d1, d2, m1, m2,ns) << " is " << formattext(std::to_string(ns),1,1) << "th prime number from American style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 18 :
              eraseAllSubStr(detail,"th ");
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;Day of the year is " << formattext(std::to_string(ns),1,1) << "th triangular number - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Day of the year is " << formattext(std::to_string(ns),1,1) << "th triangular number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 19 :
              eraseAllSubStr(detail,"th ");
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;Days left in the year is " << formattext(std::to_string(ns),1,1) << "th triangular number - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;Days left in the year is " << formattext(std::to_string(ns),1,1) << "th triangular number - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 20 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << eu_amdate(2, d1, d2, m1, m2,ns) << " is " << formattext(std::to_string(ns),1,1) << "th triangular number from European style date - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << eu_amdate(2, d1, d2, m1, m2,ns) << " is " << formattext(std::to_string(ns),1,1) << "th triangular number from European style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
          case 21 :
              if (eutime) logline << "" << d1 << d2 << " " << getMonthName (mm-1).c_str() << " - &emsp;" << eu_amdate(1, d1, d2, m1, m2, ns) << " is " << formattext(std::to_string(ns),1,1) << "th triangular number from American style date - " << detail << "<br>";
              else logline << "" << getMonthName (mm-1).c_str() << " "  << d1 << d2 << " - &emsp;" << eu_amdate(1, d1, d2, m1, m2, ns) << " is " << formattext(std::to_string(ns),1,1) << "th triangular number from American style date - " << detail << "<br>";
              buffer += QString::fromStdString(logline.str());
              savelog(logline.str());
              break;
       case 22 :
           //logline << printzerodays(dd,mm,year,ns,1);
           buffer += printzerodays(dd,mm,year,ns,1," - " + detail,eudate);
           savelog(logline.str());
           break;
       case 23 :
           //logline << searchzerodays(ns,2);
           buffer += printzerodays(dd,mm,year,ns,2," - " +detail,eudate);
           savelog(logline.str());
           break;
       case 24 :
           //logline << searchzerodays(ns,3);
           buffer += printzerodays(dd,mm,year,ns,3," - " +detail,eudate);
           savelog(logline.str());
           break;
       case 25 :
           //logline << searchzerodays(ns,4);
           buffer += printzerodays(dd,mm,year,ns,4," - " +detail,eudate);
           savelog(logline.str());
           break;

       }
       return buffer;
}


QString runanalyze(int dd, int mm, int year, string phrase,bool hlist, int filter, bool eutime) {
    stringstream ex1, logline;
    QString buffer,qt1;
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
    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0); //English Ordinal - start of compare cifers to date
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==1||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," English Ordinal",eutime);
        found = true;
    }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0); //Full Reduction
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==2||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Full Reduction",eutime);
        found = true;
    }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1); //Reverse Ordinal
    for(i=1;i<=25;i++)
        if (phrasetodate(ns,dd,mm,year,i) && (filter==3||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Reverse Ordinal",eutime);
        found = true;
    }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1); //Reverse Full Reduction
    for(i=1;i<=25;i++)
    if (phrasetodate(ns,dd,mm,year,i) && (filter==4||!hlist)) {
        if (!hlist) buffer += print_p_to_d(ns,dd,mm,year,i," Reverse Full Reduction",eutime);
        found = true;
    }

    if (found) {
        used = false;
        //ex1 << ns << " " << phrase.size() ;
        ex1 << formattext(std::to_string(ns),1,1) ;
    }

    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0); //English Ordinal - start of compare prime numbers to date
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==1||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from English Ordinal",eutime);
            found = true;
        }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0); //Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==2||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Full Reduction",eutime);
            found = true;
        }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1); //Reverse Ordinal
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==3||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Reverse Ordinal",eutime);
            found = true;
        }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1); //Reverse Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getnprime(ns),dd,mm,year,i) && (filter==4||!hlist)) && getnprime(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getnprime(ns),dd,mm,year,i,"th Prime from Reverse Full Reduction",eutime);
            found = true;
        }
    if (found && used) {
        used = false;
        ex1 << formattext(std::to_string(getnprime(ns)),1,1) << "th Prime";
    }
    if (filter==1||!hlist) ns = getwordnumericvalue(phrase,0,0); //English Ordinal - start of compare triangular numbers to date
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==1||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from English Ordinal",eutime);
            found = true;
        }
    if (filter==2||!hlist) ns = getwordnumericvalue(phrase,1,0); //Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==2||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Full Reduction",eutime);
            found = true;
        }
    if (filter==3||!hlist) ns = getwordnumericvalue(phrase,0,1); //Reverse Ordinal
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==3||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Reverse Ordinal",eutime);
            found = true;
        }
    if (filter==4||!hlist) ns = getwordnumericvalue(phrase,1,1); //Reverse Full Reduction
    for(i=1;i<=25;i++)
        if ((phrasetodate(getntriangular(ns),dd,mm,year,i) && (filter==4||!hlist)) && getntriangular(ns) != 0) {
            if (!hlist) buffer += print_p_to_d(getntriangular(ns),dd,mm,year,i,"th Triangular from Reverse Full Reduction",eutime);
            found = true;
        }
    if (found && used) {
        ex1 << formattext(std::to_string(getntriangular(ns)),1,1) << "th Triangular" ;
        used = false;
    }

    if (phrase.size() > 24) if (found && hlist) logline << "<br> " << add_dots(QString::fromStdString(formattext(phrase,2,2)),50).toUtf8().constData() << "&emsp;" << ex1.str();
    if (phrase.size() > 18 && phrase.size() <= 24) if (found && hlist) logline << "<br> " << add_dots(QString::fromStdString(formattext(phrase,2,2)),50).toUtf8().constData() << "&emsp;&emsp;" << ex1.str();
    if (phrase.size() > 13 && phrase.size() <= 18) if (found && hlist) logline << "<br> " << add_dots(QString::fromStdString(formattext(phrase,2,2)),50).toUtf8().constData() << "&emsp;&emsp;" << ex1.str();
    if (phrase.size() > 6 && phrase.size() <= 13) if (found && hlist) logline << "<br> " << add_dots(QString::fromStdString(formattext(phrase,2,2)),50).toUtf8().constData() << "&emsp;&emsp;&emsp;" << ex1.str();
    if (phrase.size() <= 6) if (found && hlist) logline << "<br> " << add_dots(QString::fromStdString(formattext(phrase,2,2)),50).toUtf8().constData() << "&emsp;&emsp;&emsp;&emsp;" << ex1.str();
    if (found && hlist){
      buffer += QString::fromStdString(logline.str());
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

QString analyze(int dd, int mm, int year, QString Qphrase,bool hlist, int filter, bool eutime) {


    bool dogoanalize = true;
    std::string phrase = Qphrase.toUtf8().constData();

    readsolarfile(dd,mm,year);


    QString buffer;


        std::vector<std::string> v;

        std::istringstream is( phrase );

        std::string word;
        if (dogoanalize) buffer += runanalyze( dd, mm, year, phrase, hlist, filter,eutime);
        while ( is >> word && dogoanalize) {
        v.push_back( word );
        eraseAllSubStr(word,"<br>");
        eraseAllSubStr(phrase,"<br>");
        if (getwordnumericvalue(phrase,0,0) == getwordnumericvalue(word,0,0)) break;
        else buffer += runanalyze( dd, mm, year, word, hlist, filter,eutime);
        }
        dogoanalize = false;
        return buffer;
    }

QString date2history(int dd, int mm, int year,bool hlist, bool eutime,int filter)
{
    string line;
    stringstream logline;
    QString buffer;
    int d1,d2;
    if (eutime) {d1=dd;d2=mm;}
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
    }


    ifstream myfile;
    myfile.open("history.txt");
    if (myfile.is_open())
          {

            while ( getline (myfile,line) )
            {
                buffer += runanalyze(dd, mm, year, line, hlist,filter,eutime);

            }

    myfile.close();
    }
    return buffer;
}

QString listciphers(int reduced, int reversed) {
    int i = 1;
    double t1;
    stringstream ss;
    QString buffer;
    for (char c = 'A'; c <= 'Z'; c++)
    {
        std::string s(1,c);
        t1 = i;
        ss << c << " = " << getwordnumericvalue(s,reduced,reversed) << "\t";
        if (floor(i/9) == t1/9) ss << "\n";
        i++;
    }
    return QString::fromStdString(ss.str());
}

string charnumeric(int reduced, int reversed, string phrase) {
    int i = 1;
    stringstream ss,s1;
    for (i=0;i<phrase.length();i++)
    {
        s1 << phrase[i];
        if (i < phrase.length()-1) ss << getwordnumericvalue(s1.str(),reduced,reversed) << "+";
        else
        ss << getwordnumericvalue(s1.str(),reduced,reversed) << "=";
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
        if (getwordnumericvalue(line,0,0) == getwordnumericvalue(word,0,0)) break;
        else buffer += printword(word,save,header,simpleprint) + "<br>";
        }
        dogoanalize = false;
        return buffer;

}

QString printword(string line, char save, bool header, bool simpleprint)
{
    int ns1,ns2,ns3,ns4;
    string tabs;
    QString buffer,qs1,qs2,qs3,qs4,qs5;
    ns1 = getwordnumericvalue(line,0,0); //English Ordinal
    ns2 = getwordnumericvalue(line,1,0); //Full Reduction
    ns3 = getwordnumericvalue(line,0,1); //Reverse Ordinal
    ns4 = getwordnumericvalue(line,1,1); //Reverse Full Reduction
    eraseAllSubStr(line,"<br>");
    stringstream logline;
    if (header) logtime();
    logline.str("");
    if (line.size() <= 7) tabs = "&emsp;&emsp;&emsp;";
    if (line.size() < 16 && line.size() >= 8) tabs = "&emsp;&emsp;";
    if (line.size() < 24 && line.size() >= 16) tabs = "&emsp;";
    if (line.size() > 24) tabs = " ";
    if (save != 'D' && !simpleprint) {
    logline << "Statistic for phrase : " << formattext(line,1,1) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
    logline.str("");
    logline << "English Orinal : &emsp;" << charnumeric(0,0,line) << formattext(std::to_string(ns1),1,1) << " Prime? "<< isprime(ns1) << " Triangular? " << istriangular(ns1) << "<br>";
    buffer += QString::fromStdString(logline.str());
    savelog(logline.str());
    logline.str("");
    logline << "Full Reduction : &emsp;" << charnumeric(1,0,line) << formattext(std::to_string(ns2),1,1) << " Prime? "<< isprime(ns2) << " Triangular? " << istriangular(ns2) << "<br>";
    //logline << "Prime number&emsp;&emsp;" << isprime(ns1) << "" << isprime(ns2) << "" << isprime(ns3) << "" << isprime(ns4) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      logline.str("");
    //logline << "Triangular number&emsp;&emsp;" << istriangular(ns1) << "" << istriangular(ns2) << "" << istriangular(ns3) << "" << istriangular(ns4) << "<br><br>";
      logline << "Reverse Ordinal :&emsp; " << charnumeric(0,1,line) << formattext(std::to_string(ns3),1,1) << " Prime? "<< isprime(ns3) << " Triangular? " << istriangular(ns3) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
      logline.str("");
      logline << "Reverse Full Reduction : " << charnumeric(1,1,line) << formattext(std::to_string(ns4),1,1) << " Prime? "<< isprime(ns4) << " Triangular? " << istriangular(ns4) << "<br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());
    }
    if (simpleprint) {
     logline << "<br>" << line << tabs << ns1 << "&emsp;" << ns2 << "&emsp;" << ns3 << "&emsp;" << ns4 << "&emsp;";
     qs1 = add_dots(QString::fromStdString(formattext(line,2,2)),50);
     //qs1 = add_dots(QString::fromStdString(line),25);

     qs2 = QString::fromStdString(formattext(std::to_string(ns1),1,1));
     qs3 = QString::fromStdString(formattext(std::to_string(ns2),1,1));
     qs4 = QString::fromStdString(formattext(std::to_string(ns3),1,1));
     qs5 = QString::fromStdString(formattext(std::to_string(ns4),1,1));
     buffer += "<br>" + qs1 + "&emsp;" + qs2 + "&emsp;" + qs3 + "&emsp;" + qs4 + "&emsp;" + qs5 +"";
     savelog(logline.str());
    }
    if (save == 'D' && ns1 > 0) buffer += deletelastline();
    if (save == 'Y' && ns1 > 0) buffer += savetext(tolowerCase(line));
    return buffer;
}

QString searchwords(int pattern, bool simpleprint) //Ctrl-H
{
    string line;
    QString buffer;
    bool header=true;
    stringstream logline;
    int c1,c2 = 0, lines = 0;
    ifstream myfile;
    if (simpleprint) buffer += "&emsp;&emsp;&emsp;Ordinal - Full Red - Reverse Ord - Reverse F";
    myfile.open("history.txt");
    if (myfile.is_open())
          {
          c1 = pattern;
          while ( getline (myfile,line) )
            {
                  if (findword(c1,c2,4,line)) {
                      buffer += printword(line,0,header,simpleprint);
                      lines++;
                      header = false;
                  }
            }

    logline << "<br>" << formattext(std::to_string(lines),1,1) << " words found matching number " << formattext(std::to_string(c1),1,1) << "<br><br>";
      buffer += QString::fromStdString(logline.str());
      savelog(logline.str());

    myfile.close();
    //buffer += "</table>";
  }

  else buffer += "Unable to open file";
  return buffer;
}


QString searchhistory(int i, string phrase) {
    int ns;
    QString buffer;
    stringstream ss;
    switch(i) {

    case 1 :
        ns = getwordnumericvalue(phrase,0,0); //English Ordinal
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
        break;
    case 2 :
        ns = getwordnumericvalue(phrase,1,0); //Full Reduction
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
        break;
    case 3 :
        ns = getwordnumericvalue(phrase,0,1); //Reverse Ordinal
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
        break;
    case 4 :
        ns = getwordnumericvalue(phrase,1,1); //Reverse Full Reduction
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
        break;
    }
    return buffer;
}
