#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "ui_httpdownload.h"

class HttpDownload : public QDialog {
    Q_OBJECT

signals:
    void dl_ready(const QString &source);

public:
    explicit HttpDownload(QWidget *parent = nullptr);
    ~HttpDownload();

private slots:
    void startRequest();
    void handleFinished();

private:
    Ui::HttpDownload *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply = nullptr;
    QString currentUrl;
    QString currentLabel;
};

#endif // HTTPDOWNLOAD_H
