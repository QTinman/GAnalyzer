/********************************************************************************
** Form generated from reading UI file 'cipherdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPHERDIALOG_H
#define UI_CIPHERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cipherDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QCheckBox *single_reduction;
    QCheckBox *francis_bacon;
    QCheckBox *satanic;
    QCheckBox *jewish;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *sumerian;
    QCheckBox *rev_sumerian;

    void setupUi(QDialog *cipherDialog)
    {
        if (cipherDialog->objectName().isEmpty())
            cipherDialog->setObjectName(QString::fromUtf8("cipherDialog"));
        cipherDialog->resize(316, 146);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icon/g_black.png"), QSize(), QIcon::Normal, QIcon::Off);
        cipherDialog->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(cipherDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(200, 20, 81, 111));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(cipherDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 20, 142, 116));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        single_reduction = new QCheckBox(layoutWidget);
        single_reduction->setObjectName(QString::fromUtf8("single_reduction"));

        verticalLayout->addWidget(single_reduction);

        francis_bacon = new QCheckBox(layoutWidget);
        francis_bacon->setObjectName(QString::fromUtf8("francis_bacon"));

        verticalLayout->addWidget(francis_bacon);

        satanic = new QCheckBox(layoutWidget);
        satanic->setObjectName(QString::fromUtf8("satanic"));

        verticalLayout->addWidget(satanic);

        jewish = new QCheckBox(layoutWidget);
        jewish->setObjectName(QString::fromUtf8("jewish"));

        verticalLayout->addWidget(jewish);

        layoutWidget1 = new QWidget(cipherDialog);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(180, 80, 137, 56));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        sumerian = new QCheckBox(layoutWidget1);
        sumerian->setObjectName(QString::fromUtf8("sumerian"));

        verticalLayout_2->addWidget(sumerian);

        rev_sumerian = new QCheckBox(layoutWidget1);
        rev_sumerian->setObjectName(QString::fromUtf8("rev_sumerian"));

        verticalLayout_2->addWidget(rev_sumerian);


        retranslateUi(cipherDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, cipherDialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, cipherDialog, &QDialog::reject);

        QMetaObject::connectSlotsByName(cipherDialog);
    } // setupUi

    void retranslateUi(QDialog *cipherDialog)
    {
        cipherDialog->setWindowTitle(QCoreApplication::translate("cipherDialog", "Dialog", nullptr));
        single_reduction->setText(QCoreApplication::translate("cipherDialog", "Single Reduction", nullptr));
        francis_bacon->setText(QCoreApplication::translate("cipherDialog", "Francis Bacon", nullptr));
        satanic->setText(QCoreApplication::translate("cipherDialog", "Satanic", nullptr));
        jewish->setText(QCoreApplication::translate("cipherDialog", "Jewish", nullptr));
        sumerian->setText(QCoreApplication::translate("cipherDialog", "Sumerian", nullptr));
        rev_sumerian->setText(QCoreApplication::translate("cipherDialog", "Reverse Sumerian", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cipherDialog: public Ui_cipherDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPHERDIALOG_H
