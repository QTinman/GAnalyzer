#ifndef TOOLS_H
#define TOOLS_H
#include <string.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <QString>



    int dayNumber(int day, int month, int year);
    std::string dayname(int day);
    std::string getMonthName(int monthNumber);
    int numberOfDays (int monthNumber, int year);
    int getwordnumericvalue(std::string word, int reduced, int reversed, int type);
    bool findword(int c1, std::string line);
    void eraseAllSubStr(std::string & mainStr, const std::string & toErase);
    void savelog(std::string line);
    void logtime();
    std::string isprime(int prime);
    std::string istriangular(int tri);
    int reduce(int sum);
    int reverse(int sum);
    QString deletelastline();
    QString savetext(std::string line);
    std::string tolowerCase(std::string &str);
    int eu_amdate(int eu_am, int d1, int d2, int m1, int m2, int ns);
    int daynr(int dd,int mm,int year);
    int daynrleft(int dd, int mm, int year);
    int getnprime(int prime);
    int getntriangular(int tri);
    int eu_amdate(int eu_am, int d1, int d2, int m1, int m2, int ns);
    int spanofdate(int d2, int m2, int y2, int dd, int mm, int year);
    std::string formattext(std::string line, int color, int bold);
    QString listnumbers(char type);
    int valid_date(int dd, int mm, int yy);
    void savetotemp(std::string line);
    void replaceAll( std::string &s, const std::string &search, const std::string &replace );
    QString listhistory(QString filter);
    void createSettings(std::string file, std:: string entry);
    QString readSettings(std::string file, std::string entry);
    void writeSettings(char file[], std::string entry,std::string settings);
    QString Qformattext(std::string line, int color, int bold);



#endif // TOOLS_H
