#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QRegularExpression>

namespace Ui {
class HttpDownload;
}

class HttpDownload : public QDialog
{
    Q_OBJECT

public:
    explicit HttpDownload(QWidget *parent = 0);
    ~HttpDownload();

public:
    void startRequest(QUrl url);

signals:
    void dl_ready(const QString);

private slots:
    void on_downloadButton_clicked();

    void on_quitButton_clicked();

    //void on_urlList_returnPressed();

    // slot for readyRead() signal
    void httpReadyRead();

    // slot for finished() signal from reply
    void httpDownloadFinished();

    // slot for downloadProgress()
    void updateDownloadProgress(qint64, qint64);

    void enableDownloadButton();
    void cancelDownload();

private:
    Ui::HttpDownload *ui;
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QProgressDialog *progressDialog;
    QFile *file;
    bool httpRequestAborted;
    qint64 fileSize;

};

#endif // HTTPDOWNLOAD_H
