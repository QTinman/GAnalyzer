#include "tools.h"
#include "mainwindow.h"
#include <string.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <QString>
#include <QDate>
#include <QDebug>


#define BUFFERSIZE 256

//#include <algorithm>
using namespace std;


int dayNumber(int day, int month, int year)
{

    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1,
                       4, 6, 2, 4 };
    year -= month < 3;
    return ( year + year/4 - year/100 +
             year/400 + t[month-1] + day) % 7;
}

int valid_date(int dd, int mm, int yy) {
    if (mm < 1 || mm > 12) {
        return 0;
    }
    if (dd < 1) {
        return 0;
    }

    int days = 31;
    if (mm == 2) {
        days = 28;
        if (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)) {
            days = 29;
        }
    } else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
        days = 30;
    }

    if (dd > days) {
        return 0;
    }
    return 1;
}

string formattext(string line, int color, int bold)
{

    string formatedcolor;
    string formatedtag;
    int i;
    if (color != 10 && color != 20) {
    switch (color) {
        case 0: //none
        formatedcolor = line;
        break;
        case 1: // red
        formatedcolor = "<font color=\"red\">" +line+ "</font>";
        break;
        case 2: // blue
        formatedcolor = "<font color=\"blue\">" +line+ "</font>";
        break;


    }
    switch (bold) {
        case 0: //none
        formatedtag = formatedcolor;
        break;
        case 1: //bold
        formatedtag = "<b>" +formatedcolor+"</b>";
        break;
        case 2: //italic
        formatedtag = "<i>" +formatedcolor+"</i>";
        break;
        case 3: //bold & italic
        formatedtag = "<i><b>" +formatedcolor+"</b></i>";
        break;

    }
    }
    else formatedtag = line;
    //qDebug() <<  QString::fromStdString(formatedtag);
    if (color == 10) for (i=1;i<bold;i++) formatedtag=formatedtag+"&emsp;"; //tab after
    if (color == 20) for (i=1;i<bold;i++) formatedtag="&emsp;"+formatedtag; //tab before
    // html == "#include &lt;QtCore&gt;"
    return formatedtag;
}


QString listnumbers(char type) {
    int i = 1;
    double t1;
    stringstream ss;
    do {
        if (type == 'T'){
            if (getntriangular(i) != 0) {
                ss << i << " - " << getntriangular(i) << "    \t";
                t1 = getntriangular(i);
                if (floor(getntriangular(i)/5) == t1/5) ss << "\n";
            }

        }
        if (type == 'P') {
            if (i == 1) i=2;
            if (getnprime(i) != 0) {
                ss << i << " - " << getnprime(i) << "    \t";
                t1 = getnprime(i);
                if (floor((getnprime(i))/5) == t1/5) ss << "\n";
            }
        }
        i++;
    } while ((type == 'T' && getntriangular(i) <= 100) || (type == 'P' && getnprime(i) <= 100));
    return QString::fromStdString(ss.str());
}

std::string dayname(int day) {
       switch(day) {
          case 0 :
              return "Sunday";
              break;
          case 1 :
              return "Monday";
              break;
          case 2 :
              return "Tuesday";
              break;
          case 3 :
              return "Wednesday";
              break;
          case 4 :
              return "Thursday";
              break;
          case 5 :
              return "Friday";
              break;
          case 6 :
              return "Saturday";
              break;
       }
}

/*
  A Function that returns the name of the month
  with a given month number

  Month Number     Name
  0                January
  1                February
  2                March
  3                April
  4                May
  5                June
  6                July
  7                August
  8                September
  9                October
  10               November
  11               December */
std::string getMonthName(int monthNumber)
{
    std::string months[] = {"January&emsp;", "February", "March&emsp;",
                       "April&emsp;", "May&emsp;", "June&emsp;",
                       "July&emsp;", "August&emsp;", "September",
                       "October&emsp;", "November&emsp;", "December&emsp;"
                      };

    return (months[monthNumber]);
}

/* A Function to return the number of days in
   a month

  Month Number     Name        Number of Days
  0                January     31
  1                February    28 (non-leap) / 29 (leap)
  2                March       31
  3                April       30
  4                May         31
  5                June        30
  6                July        31
  7                August      31
  8                September   30
  9                October     31
  10               November    30
  11               December    31

*/
int numberOfDays (int monthNumber, int year)
{
    // January
    if (monthNumber == 0)
        return (31);

    // February
    if (monthNumber == 1)
    {
        // If the year is leap then February has
        // 29 days
        if (year % 400 == 0 ||
                (year % 4 == 0 && year % 100 != 0))
            return (29);
        else
            return (28);
    }

    // March
    if (monthNumber == 2)
        return (31);

    // April
    if (monthNumber == 3)
        return (30);

    // May
    if (monthNumber == 4)
        return (31);

    // June
    if (monthNumber == 5)
        return (30);

    // July
    if (monthNumber == 6)
        return (31);

    // August
    if (monthNumber == 7)
        return (31);

    // September
    if (monthNumber == 8)
        return (30);

    // October
    if (monthNumber == 9)
        return (31);

    // November
    if (monthNumber == 10)
        return (30);

    // December
    if (monthNumber == 11)
        return (31);
}

int spanofdate(int d2, int m2, int y2, int dd, int mm, int year) {
    int i, numberofdays;
    tm date = {};
    date.tm_year = y2 - 1900;
    date.tm_mon = m2 - 1;
    date.tm_mday = d2;
    mktime( &date );
    numberofdays = date.tm_yday +1;
    date.tm_year = y2 - 1900;
    date.tm_mon = 12 - 1;
    date.tm_mday = 31;
    mktime( &date );
    numberofdays = date.tm_yday - numberofdays+1;


    if (y2 < year)
        for(i=y2+1; i<=year; ++i){
            date.tm_year = i - 1900;
            date.tm_mon = 12 - 1;
            date.tm_mday = 31;
            mktime( &date );
            numberofdays += date.tm_yday +1;
        }
    date.tm_year = year - 1900;
    date.tm_mon = mm - 1;
    date.tm_mday = dd;
    mktime( &date );
    i = date.tm_yday +1;
    date.tm_year = year - 1900;
    date.tm_mon = 12 - 1;
    date.tm_mday = 31;
    mktime( &date );
    numberofdays -= date.tm_yday - i+1;
    return(numberofdays);
}

void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;

    // Search for the substd::string in std::string in a loop untill nothing is found
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
    {
        // If found then erase it from std::string
        mainStr.erase(pos, toErase.length());
    }
}

int reduce(int sum){
            int s1, s2;
           if (sum > 18)
              {
                s2 = sum-20;
                s1 = 2;
           } else if (sum > 9)
              {
                s2 = sum-10;
                s1 = 1;
           } else
              {
                s2 = sum;
                s1 = 0;
              }
    return (s1 + s2);

}
int reverse(int sum){
    sum = 27-sum;
    return(sum);
}


int getnprime(int prime)
{
    {
      int i,i2, nprime = 3;
      bool isPrime = true;
      for(i = 2; i <= prime / 2; ++i)
      {
          if(prime % i == 0)
          {
              isPrime = false;
              break;
          }
      }

      if (isPrime) {
          for(i2 = 2; i2 <= prime; ++i2) {
              isPrime = true;
              for(i = 2; i <= i2 / 2; ++i)
                  if(i2 % i == 0)
                  {
                      isPrime = false;
                      break;
                  }
              if (isPrime) nprime ++;
          }
          return nprime-3;
      }
      else
          return 0;
    }
}

int getntriangular(int tri)
{
    int i, ntri = 0;
    bool isTri = true;
    if (floor(sqrt(8*tri+1)) != sqrt(8*tri+1)) isTri = false;
    if (isTri) {
        for(i = 1; i <= tri; ++i)
            if (floor(sqrt(8*i+1)) == sqrt(8*i+1)) ntri++;
        return ntri;
    }
    else
        return 0;

}

int getwordnumericvalue(std::string word, int reduced, int reversed, int type) // type 0 english ordenal, 1 Single Reduction, 2 Francis Bacon, 3 Satanic, 4 Jewish
{
        int s1 = 0, sum = 0;// for summing the letter values.
        if (type != 2) tolowerCase(word);
        for(size_t i=0; i<word.size(); ++i){	    	// loop through the string 1 char at a time
            //qDebug() << int(word[i]) << "\n";
            if (int(word[i]) <= 122 && int(word[i]) >= 97) {
               sum = (int(word[i]-96));// using the ascii value for each letter
               if (type == 3) sum += 35; //Satanic
            }
            if (int(word[i]) <= 90 && int(word[i]) >= 65 && type == 2) sum = (int(tolower(word[i])-96)) + 26; //Francis
            if ((int((word[i]) <= 122 && int(word[i]) >= 97) || int(word[i]) == 38) && type == 4) { // Jewish
                if (int(word[i]) == 116) sum = 100; //t
                if (int(word[i]) == 117) sum = 200; //u
                if (int(word[i]) == 120) sum = 300; //x
                if (int(word[i]) == 121) sum = 400; //y
                if (int(word[i]) == 122) sum = 500; //z
                if (int(word[i]) == 106) sum = 600; //j
                if (int(word[i]) == 118) sum = 700; //v
                if (int(word[i]) == 38) sum = 800; //&
                if (int(word[i]) == 119) sum = 900; //w
                if (int(word[i]) <= 115 && int(word[i]) >= 107) { // 10-90
                    sum = ((int(word[i])-96) - 10)*10;
                }

            }
            if (reversed == 1 && int(word[i]) <= 122 && int(word[i]) >= 97) sum = reverse(sum);
            if (reduced == 1 && int(word[i]) <= 122 && int(word[i]) >= 97) sum = reduce(sum);
            if (type == 1 && int(word[i]) <= 122 && int(word[i]) >= 97 && tolower(word[i]) != 's') { //Single reduction
                sum = reduce(sum);
               } else if (tolower(word[i]) == 's' && type == 1) sum = 10;
            if (int(word[i]) == 38 && type == 4) s1 += sum;
            if (int(word[i]) <= 122 && int(word[i]) >= 97) s1 += sum;
            if (int(word[i]) <= 90 && int(word[i]) >= 65) s1 += sum;
         if (int(word[i]) <= 57 && int(word[i]) >= 49) s1 += int(word[i]-48);
        }
        //qDebug() << word[i] << " = " << s1;
        return(s1);
}

bool findword(int c1, std::string line)
{
    int ns1 = 0,ns2 = 0,ns3 = 0,ns4 = 0,ns5 = 0,ns6 = 0,ns7 = 0;
    ns1 = getwordnumericvalue(line,0,0,0);
    ns2 = getwordnumericvalue(line,1,0,0);
    ns3 = getwordnumericvalue(line,0,1,0);
    ns4 = getwordnumericvalue(line,1,1,0);
    if (single_r_on) ns5 = getwordnumericvalue(line,0,0,1);
    if (francis_on) ns6 = getwordnumericvalue(line,0,0,2);
    if (satanic_on) ns7 = getwordnumericvalue(line,0,0,3);
    if (jewish_on) ns7 = getwordnumericvalue(line,0,0,4);
     if (c1 == ns1 || c1 == ns2 || c1 == ns3 || c1 ==ns4 || c1 ==ns5 || c1 ==ns6 || c1 ==ns7)
        {
            return true;
        }


return false;
}


void savetotemp(std::string line)
{
    //std::string stext;
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    std::ofstream fout;  // Create Object of Ofstream
    std::ifstream fin;
    fin.open("temp.txt");
   fout.open ("temp.txt",ios::app); // Append mode
    if(fin.is_open())
        fout<< line; // Writing data to file
    fin.close();
    fout.close(); // Closing the file
    line = "";
}


void savelog(std::string line)
{
    //std::string stext;
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    //eraseAllSubStr(line,"");
    std::ofstream fout;  // Create Object of Ofstream
    std::ifstream fin;
    fin.open("log.html");
   fout.open ("log.html",ios::app); // Append mode
    if(fin.is_open())
        fout<< line; // Writing data to file
    fin.close();
    fout.close(); // Closing the file
    line = "";
}

QString savetext(string line)
{
    string stext;
    QString buffer;
    int add = 1;
    ofstream fout;  // Create Object of Ofstream
    ifstream fin;
    eraseAllSubStr(line,"\n");
    fin.open("history.txt");
    while ( getline (fin,stext) )
    //	cout << line <<"&emsp;" << stext;
   // pause();
    {
        if (line == stext) add = 0;
    }
    fout.open ("history.txt",ios::app); // Append mode
    if(fin.is_open() && add == 1)
        fout<< line<<"\n"; // Writing data to file
    if (add == 1) buffer = "<br> Saved to history<br>";
    if (add == 0) buffer = "<br> Already in history<br>";
    fin.close();
    fout.close(); // Closing the file
    return buffer;
}

//string replacesubstr(string str,string pattern, string replacement)
void replaceAll( string &s, const string &search, const string &replace ) { // replace in "s" the "replace" for "search"
    for( size_t pos = 0; ; pos += replace.length() ) {
        // Locate the substring to replace
        pos = s.find( search, pos );
        if( pos == string::npos ) break;
        // Replace by erasing and inserting
        s.erase( pos, search.length() );
        s.insert( pos, replace );
    }
}

void logtime() {
    // current date/time based on current system
    std::stringstream logline;
       //time_t now = time(0);

       // convert now to std::string form
       // char* dt = ctime(&now);

       char s[1000];

       time_t t = time(NULL);
       struct tm * p = localtime(&t);

       strftime(s, 1000, "%a, %b %d %Y %H:%M:%S", p);

       //printf("%s<br>", s);


       logline << "<br>------------------------ " << s << " ------------------------<br>";
       savelog(logline.str());
       logline.str(std::string());
}

std::string isprime(int prime)
{
    {
      std::stringstream ss;
      int i,i2, nprime = 3;
      bool isPrime = true;
      for(i = 2; i <= prime / 2; ++i)
      {
          if(prime % i == 0)
          {
              isPrime = false;
              break;
          }
      }

      if (isPrime) {
          for(i2 = 2; i2 <= prime; ++i2) {
              isPrime = true;
              for(i = 2; i <= i2 / 2; ++i)
                  if(i2 % i == 0)
                  {
                      isPrime = false;
                      break;
                  }
              if (isPrime) nprime ++;
          }
         // if (nprime-3 <10)ss << "Yes-" << nprime-3 << "th&emsp;";
         // else ss << "Yes-" << nprime-3 << "th&emsp;";
          ss << "Yes-" << nprime-3 << "th&emsp;";
          return formattext(ss.str(),2,2);
      }
      else
          return "No&emsp;";
    }
}

QString deletelastline() {
    std::string line;
    std::vector<std::string> lines;
    std::ifstream inputStream("history.txt");

    while (getline(inputStream,line)) {
        lines.push_back(line);
    }
    inputStream.close();

    std::fstream outputStream("history.txt", ios::out | ios::trunc);
    if (outputStream.is_open())
    {
        for (int i=0; i < lines.size()-1; i++)
        {
            //cout << i << " " << lines.size() << " " << lines[i] << " " << line << "<br>";
           outputStream << lines[i] << "\n";
           //QVector c=lines[lines.size()-1];
           if (i==lines.size()-2) return  "<br>Phrase " + QString::fromStdString(lines[lines.size()-1]) + " Deleted from history.txt<br>";
           //if (i==lines.size()-2) return  "<br>Phrase " + QVector(lines[lines.size()-1]) + " Deleted from history.txt<br>";
        }
        outputStream.close();

    }

}

std::string istriangular(int tri)
{
    int i, ntri = 0;
    std::stringstream ss;
    bool isTri = true;
    if (floor(sqrt(8*tri+1)) != sqrt(8*tri+1)) isTri = false;
    if (isTri) {
        for(i = 1; i <= tri; ++i)
            if (floor(sqrt(8*i+1)) == sqrt(8*i+1)) ntri++;
        ss << "Yes-" << ntri << "th&emsp;";
        return formattext(ss.str(),2,2);
    }
    else
        return "No&emsp;";

}

std::string tolowerCase(std::string &str)
{
    const int length = str.length();
    for(int i=0; i < length; ++i)
    {
        str[i] = std::tolower(str[i]);
    }
    return str;
}

int eu_amdate(int eu_am, int d1, int d2, int m1, int m2,int ns) {

    int eudate, amdate;
    eudate = d1*1000+d2*100+m1*10+m2;
    amdate = m1*1000+m2*100+d1*10+d2;
    if (d1 == 0) amdate = m1*100+m2*10+d2;
    if (m1 == 0) eudate = d1*100+d2*10+m2;
    if (d2 == 0) {
        amdate = m1*100+m2*10+d1;
        eudate = d1*100+m1*10+m2;
    }
    if (m2 == 0) {
        amdate = m1*100+d1*10+d2;
        eudate = d1*100+d2*10+m1;
    }

    if (d1==0 && m1==0) {
        eudate = d2*10+m2;
        amdate = m2*10+d2;
    }
    if (d1==0 && m2==0) {
            eudate = d2*10+m1;
            amdate = m1*10+d2;
        }
    if (d2==0 && m2==0) {
            eudate = d1*10+m1;
            amdate = m1*10+d1;
        }
    if (d2==0 && m1==0) {
            eudate = d1*10+m2;
            amdate = m2*10+d1;
        }
    if (ns != eudate && eu_am == 2) {
    eudate = d1*1000+d2*100+m1*10+m2;
    if (m1 == 0) eudate = d1*100+d2*10+m2;
    }
    if (ns != amdate && eu_am == 1) {
    amdate = m1*1000+m2*100+d1*10+d2;
    if (d1 == 0) amdate = m1*100+m2*10+d2;
    }
   if (eu_am == 1) return amdate;
   else return eudate;
}

int daynr(int dd,int mm,int year) {
    tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = mm - 1;
    date.tm_mday = dd;
    mktime( &date );
    return date.tm_yday +1;

}

int daynrleft(int dd, int mm, int year)
{
 tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = 12 - 1;
    date.tm_mday = 31;
    mktime( &date );
    return date.tm_yday - daynr(dd,mm,year)+1;
}
