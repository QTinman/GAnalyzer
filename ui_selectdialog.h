/********************************************************************************
** Form generated from reading UI file 'selectdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTDIALOG_H
#define UI_SELECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_selectDialog
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QWidget *widget;
    QGridLayout *gridLayout;
    QRadioButton *radioButton1;
    QRadioButton *Jewish;
    QRadioButton *radioButton2;
    QRadioButton *SingleRed;
    QRadioButton *radioButton3;
    QRadioButton *Francis;
    QRadioButton *radioButton4;
    QRadioButton *Satanic;
    QRadioButton *Sumerian;
    QRadioButton *rev_sumerian;

    void setupUi(QDialog *selectDialog)
    {
        if (selectDialog->objectName().isEmpty())
            selectDialog->setObjectName(QString::fromUtf8("selectDialog"));
        selectDialog->resize(400, 201);
        layoutWidget = new QWidget(selectDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(120, 160, 168, 31));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setAutoDefault(false);

        gridLayout_2->addWidget(pushButton_2, 0, 1, 1, 1);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setLayoutDirection(Qt::LeftToRight);

        gridLayout_2->addWidget(pushButton, 0, 2, 1, 1);

        widget = new QWidget(selectDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 11, 371, 146));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        radioButton1 = new QRadioButton(widget);
        radioButton1->setObjectName(QString::fromUtf8("radioButton1"));
        radioButton1->setChecked(true);

        gridLayout->addWidget(radioButton1, 0, 0, 1, 1);

        Jewish = new QRadioButton(widget);
        Jewish->setObjectName(QString::fromUtf8("Jewish"));

        gridLayout->addWidget(Jewish, 0, 1, 1, 1);

        radioButton2 = new QRadioButton(widget);
        radioButton2->setObjectName(QString::fromUtf8("radioButton2"));

        gridLayout->addWidget(radioButton2, 1, 0, 1, 1);

        SingleRed = new QRadioButton(widget);
        SingleRed->setObjectName(QString::fromUtf8("SingleRed"));

        gridLayout->addWidget(SingleRed, 1, 1, 1, 1);

        radioButton3 = new QRadioButton(widget);
        radioButton3->setObjectName(QString::fromUtf8("radioButton3"));

        gridLayout->addWidget(radioButton3, 2, 0, 1, 1);

        Francis = new QRadioButton(widget);
        Francis->setObjectName(QString::fromUtf8("Francis"));

        gridLayout->addWidget(Francis, 2, 1, 1, 1);

        radioButton4 = new QRadioButton(widget);
        radioButton4->setObjectName(QString::fromUtf8("radioButton4"));

        gridLayout->addWidget(radioButton4, 3, 0, 1, 1);

        Satanic = new QRadioButton(widget);
        Satanic->setObjectName(QString::fromUtf8("Satanic"));

        gridLayout->addWidget(Satanic, 3, 1, 1, 1);

        Sumerian = new QRadioButton(widget);
        Sumerian->setObjectName(QString::fromUtf8("Sumerian"));

        gridLayout->addWidget(Sumerian, 4, 0, 1, 1);

        rev_sumerian = new QRadioButton(widget);
        rev_sumerian->setObjectName(QString::fromUtf8("rev_sumerian"));

        gridLayout->addWidget(rev_sumerian, 4, 1, 1, 1);


        retranslateUi(selectDialog);
        QObject::connect(pushButton, &QPushButton::clicked, selectDialog, &QDialog::accept);
        QObject::connect(pushButton_2, &QPushButton::clicked, selectDialog, &QDialog::close);

        QMetaObject::connectSlotsByName(selectDialog);
    } // setupUi

    void retranslateUi(QDialog *selectDialog)
    {
        selectDialog->setWindowTitle(QCoreApplication::translate("selectDialog", "Select searchpattern", nullptr));
        pushButton_2->setText(QCoreApplication::translate("selectDialog", "Cancel", nullptr));
        pushButton->setText(QCoreApplication::translate("selectDialog", "Ok", nullptr));
        radioButton1->setText(QCoreApplication::translate("selectDialog", "English Ordenal", nullptr));
        Jewish->setText(QCoreApplication::translate("selectDialog", "Jewish", nullptr));
        radioButton2->setText(QCoreApplication::translate("selectDialog", "Full Reduction", nullptr));
        SingleRed->setText(QCoreApplication::translate("selectDialog", "Single Reduction", nullptr));
        radioButton3->setText(QCoreApplication::translate("selectDialog", "Reverse Ordinal", nullptr));
        Francis->setText(QCoreApplication::translate("selectDialog", "Francis Bacon", nullptr));
        radioButton4->setText(QCoreApplication::translate("selectDialog", "Reverse full Reduction", nullptr));
        Satanic->setText(QCoreApplication::translate("selectDialog", "Satanic", nullptr));
        Sumerian->setText(QCoreApplication::translate("selectDialog", "Sumerian", nullptr));
        rev_sumerian->setText(QCoreApplication::translate("selectDialog", "Reverse Sumerian", nullptr));
    } // retranslateUi

};

namespace Ui {
    class selectDialog: public Ui_selectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTDIALOG_H
