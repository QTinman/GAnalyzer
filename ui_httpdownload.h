/********************************************************************************
** Form generated from reading UI file 'httpdownload.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HTTPDOWNLOAD_H
#define UI_HTTPDOWNLOAD_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_HttpDownload
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *statusLabel;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QComboBox *urlList;
    QLabel *urlLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *HttpDownload)
    {
        if (HttpDownload->objectName().isEmpty())
            HttpDownload->setObjectName(QString::fromUtf8("HttpDownload"));
        HttpDownload->resize(367, 141);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icon/g_black.png"), QSize(), QIcon::Normal, QIcon::Off);
        HttpDownload->setWindowIcon(icon);
        gridLayout = new QGridLayout(HttpDownload);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        statusLabel = new QLabel(HttpDownload);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        horizontalLayout_2->addWidget(statusLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        downloadButton = new QPushButton(HttpDownload);
        downloadButton->setObjectName(QString::fromUtf8("downloadButton"));

        horizontalLayout_3->addWidget(downloadButton);

        quitButton = new QPushButton(HttpDownload);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));

        horizontalLayout_3->addWidget(quitButton);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 3);

        urlList = new QComboBox(HttpDownload);
        urlList->setObjectName(QString::fromUtf8("urlList"));
        urlList->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(urlList, 0, 1, 1, 2);

        urlLabel = new QLabel(HttpDownload);
        urlLabel->setObjectName(QString::fromUtf8("urlLabel"));

        gridLayout->addWidget(urlLabel, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(291, 27, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 2, 1, 1);


        retranslateUi(HttpDownload);

        QMetaObject::connectSlotsByName(HttpDownload);
    } // setupUi

    void retranslateUi(QDialog *HttpDownload)
    {
        HttpDownload->setWindowTitle(QCoreApplication::translate("HttpDownload", "HttpDownload", nullptr));
        statusLabel->setText(QCoreApplication::translate("HttpDownload", "Select newssource or emty to add.", nullptr));
        downloadButton->setText(QCoreApplication::translate("HttpDownload", "Download", nullptr));
        quitButton->setText(QCoreApplication::translate("HttpDownload", "Close", nullptr));
        urlLabel->setText(QCoreApplication::translate("HttpDownload", "Source:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HttpDownload: public Ui_HttpDownload {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HTTPDOWNLOAD_H
