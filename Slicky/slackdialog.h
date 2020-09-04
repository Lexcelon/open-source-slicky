#ifndef SLACKDIALOG_H
#define SLACKDIALOG_H

#include <QDialog>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkReply>
#include <QTimer>
#include <QDesktopServices>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QDir>
#include <QSettings>
#include <QThread>
#include <QMessageBox>

#include "simplecrypt.h"

namespace Ui {
class SlackDialog;
}

class SlackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlackDialog(QSettings *settings, QWidget *parent = nullptr);
    ~SlackDialog();

private slots:
    void accept();
    void reject();

    void requestFinished(QNetworkReply* reply);
    void tokenReqFinished(QNetworkReply* reply);

private:
    Ui::SlackDialog *ui;
    QProcess* process;
    QSettings *appsettings;
    QString client_id = ""; // TODO: Put Slack Client ID Here
    QString client_secret = ""; // TODO: Put Slack Client Secret Here


    void encryptToken(QString token);
};

#endif // SLACKDIALOG_H
