#include "httpdownload.h"
#include "ui_httpdownload.h"
#include "headdialog.h"
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QDomDocument>
#include <QDateTime>

QString fileheadlines = "fetched_headlines.txt";  // Global for headDialog compatibility
QString tempstring;

HttpDownload::HttpDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpDownload)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);

    QFile sources("newssources.txt");
    if (sources.open(QIODevice::ReadOnly)) {
        while (!sources.atEnd()) {
            QString line = sources.readLine().trimmed();
            if (!line.isEmpty()) {
                int sep = line.indexOf('|');
                if (sep != -1)
                    ui->urlList->addItem(line.left(sep), line.mid(sep + 1));
            }
        }
        sources.close();
    }
    ui->urlList->addItem("Add new source...", "");  // triggers dialog when selected
    connect(ui->downloadButton, &QPushButton::clicked, this, &HttpDownload::startRequest);
    connect(ui->quitButton, &QPushButton::clicked, this, &HttpDownload::close);
}

HttpDownload::~HttpDownload() {
    delete ui;
}

void HttpDownload::startRequest() {
    QString url = ui->urlList->currentData().toString();
    currentUrl = url;
    currentLabel = ui->urlList->currentText();  // this is what user sees

    QString tempstring = url;

    if (url.isEmpty()) {
        QMessageBox::warning(this, "No URL", "No news source URL selected.");
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &HttpDownload::handleFinished);
}

void HttpDownload::handleFinished() {
    if (reply->error()) {
        QMessageBox::critical(this, "Download Error", reply->errorString());
        reply->deleteLater();
        return;
    }

    QDomDocument doc;
    QByteArray rawData = reply->readAll();  // Read once
    reply->deleteLater();                   // Then delete
    if (!doc.setContent(rawData)) {
        QMessageBox::warning(this, "Parse Error", "Failed to parse RSS XML content.");
        return;
    }


    QFile outFile(fileheadlines);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not write headlines to file.");
        reply->deleteLater();
        return;
    }

    QTextStream out(&outFile);
    QDomNodeList items = doc.elementsByTagName("item");
    for (int i = 0; i < items.count(); ++i) {
        QDomElement item = items.at(i).toElement();
        QString title = item.firstChildElement("title").text().trimmed();
        QString link = item.firstChildElement("link").text().trimmed();

        if (!title.isEmpty()) {
            out << "<a href=\"" + link + "\">[link]</a> " + title + "\n";
        }
    }
    outFile.close();
    reply->deleteLater();

    headDialog *dialog = new headDialog(fileheadlines, currentLabel, this);
    emit dl_ready(fileheadlines);
    dialog->exec();
}
