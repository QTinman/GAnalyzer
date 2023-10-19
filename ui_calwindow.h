/********************************************************************************
** Form generated from reading UI file 'calwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALWINDOW_H
#define UI_CALWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CalWindow
{
public:
    QVBoxLayout *verticalLayout;
    QCalendarWidget *calendarWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *date;
    QSpacerItem *horizontalSpacer;
    QPushButton *seconddate;

    void setupUi(QDialog *CalWindow)
    {
        if (CalWindow->objectName().isEmpty())
            CalWindow->setObjectName(QString::fromUtf8("CalWindow"));
        CalWindow->resize(346, 278);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icon/g_black.png"), QSize(), QIcon::Normal, QIcon::Off);
        CalWindow->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(CalWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        calendarWidget = new QCalendarWidget(CalWindow);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));

        verticalLayout->addWidget(calendarWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        date = new QPushButton(CalWindow);
        date->setObjectName(QString::fromUtf8("date"));

        horizontalLayout->addWidget(date);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        seconddate = new QPushButton(CalWindow);
        seconddate->setObjectName(QString::fromUtf8("seconddate"));

        horizontalLayout->addWidget(seconddate);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CalWindow);

        QMetaObject::connectSlotsByName(CalWindow);
    } // setupUi

    void retranslateUi(QDialog *CalWindow)
    {
        CalWindow->setWindowTitle(QCoreApplication::translate("CalWindow", "Dialog", nullptr));
        date->setText(QCoreApplication::translate("CalWindow", "Date", nullptr));
        seconddate->setText(QCoreApplication::translate("CalWindow", "Second date", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalWindow: public Ui_CalWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALWINDOW_H
