#include "slackdialog.h"
#include "ui_slackdialog.h"

SlackDialog::SlackDialog(QSettings *settings, QWidget *parent) : QDialog(parent), ui(new Ui::SlackDialog) {
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint ); // Removes the close, minimize, and maximize buttons

    appsettings = settings;

    // Create the request
    QString url = "https://slack.com/oauth/v2/authorize?user_scope=users.profile:write&client_id="+ client_id;
    QUrl link = QUrl(url);
    QDesktopServices::openUrl(link);
}

SlackDialog::~SlackDialog() {
    delete ui;
}

// PRIVATE DYNAMIC SLOTS

// Called when the user clicks "Ok"
void SlackDialog::accept() {
    QString code = ui->tokenLineEdit->text();
    if (code != "") {
        // get slack token
        // Create the request
        QUrl serviceUrl = QUrl("https://slack.com/api/oauth.v2.access"); // sign in to slack
        QNetworkRequest request(serviceUrl);
        QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
        connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(tokenReqFinished(QNetworkReply*)));

        // Set headers
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        // Build the query string
        QUrlQuery postData;
        postData.addQueryItem("client_id", client_id);
        postData.addQueryItem("client_secret", client_secret);
        postData.addQueryItem("code", code);

        // Send the request
        networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    }
}

// Called when the user clicks "Cancel"
void SlackDialog::reject() {
    qDebug("REJECT");
    this->done(0);
}

void SlackDialog::tokenReqFinished(QNetworkReply* reply) {
    QByteArray bytes = reply->readAll();
    QString str = QString::fromUtf8(bytes.data(), bytes.size());
    QJsonDocument jsonResp = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject jsonObj = jsonResp.object();

    if (jsonObj["ok"].toBool()) {
        QJsonObject authUser = jsonObj["authed_user"].toObject();
        QString token = authUser["access_token"].toString();

        qDebug() << "slack dialog found" << token;

        // encrypt token and write to config file
        encryptToken(token);

        // close slack dialog
        this->done(1);
    }
    else {
        qDebug() << "Error: " + jsonObj["error"].toString();
        QMessageBox msgBox;
        msgBox.setText("Error: " + jsonObj["error"].toString());
        msgBox.exec();
    }


}

void SlackDialog::requestFinished(QNetworkReply* reply) {
    QByteArray bytes = reply->readAll();
    QString str = QString::fromUtf8(bytes.data(), bytes.size());
    //int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

// Encrypt token and write out to settings
void SlackDialog::encryptToken(QString token) {
    SimpleCrypt crypto(Q_UINT64_C(0x0000000000000000)); // TODO: Change to some random number

    //Encryption
    QString result = crypto.encryptToString(token);

    // add to settings file
    appsettings->setValue("token", result);
    appsettings->sync(); // Ensure the token is saved

    if (appsettings->status() != QSettings::NoError) {
        QMessageBox errorBox;
        if (appsettings->status() == QSettings::AccessError) {
            errorBox.setText("Access Error");
            errorBox.exec();
        }
        else if (appsettings->status() == QSettings::FormatError) {
            errorBox.setText("Format Error");
            errorBox.exec();
        }
    }
}
