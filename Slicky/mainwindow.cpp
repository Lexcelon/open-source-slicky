#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    // Setup the UI
    ui->setupUi(this);

    // Init settings file
    settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    qDebug() << settingsFile;
    settings = new QSettings(settingsFile, QSettings::IniFormat);

    Status::initializeSettingsStatuses(settings);
    handleStatusButtons();

    // Create Connect to Slack Button
    ui->connectToSlackButton->setIcon(QIcon(":/images/add_to_slack.png"));
    QSize buttonSize = QSize(93, 30);
    ui->connectToSlackButton->setFixedSize(buttonSize);
    ui->connectToSlackButton->setIconSize(buttonSize);

    // Initialize Member Variables
    this->lightDevice = new LightDevice();
    this->slackToken = "";

    // Connect the signals and slots
    connect(this->lightDevice, SIGNAL(noDeviceConnected()), this, SLOT(noDeviceConnected()));
    connect(this->lightDevice, SIGNAL(deviceConnected()), this, SLOT(deviceConnected()));

    // Initialize Member Variables with New Status
    handleSelectedStatus(statuses[0]);

    setSlackToken();

    // Create Tool Tip for Check Boxes
    this->ui->updateLightCheckBox->setToolTip(QString("Check to enable light color change on status change"));
    this->ui->updateSlackCheckBox->setToolTip(this->ui->updateSlackCheckBox->isEnabled() ? QString("Check to enable updates to slack on status change") : QString("Click the 'Add to Slack' button below to enable"));
}

MainWindow::~MainWindow() {
    delete ui;
}

/*
 *
 * HANDLERS
 *
 */

// Handles setting the color member varibale, preview, brightness slider, and optionally the light color
void MainWindow::handleSelectedColor(QColor color) {

    // Set the color state of the gui
    setColor(color);

    // Set the color preview
    setColorPreview(color);

    // Set the brightness slider value
    setBrightnessSliderValue(color);

    // Handle the light color update, if selected
    if (this->ui->updateLightCheckBox->isChecked()) {
        updateLightColor(color);
    }
}

// Handles setting the status member variable, status and color preview, and optionally the slack status
void MainWindow::handleSelectedStatus(Status status) {

    // Set the status preview
    setStatusPreview(status);

    // Handle the selected color
    handleSelectedColor(status.color);

    // Update slack status, if selected
    if (this->ui->updateSlackCheckBox->isChecked()) {
        updateSlackStatus(status);
    }
}

// Handles setting the color with brightness member variable, color preview, and optionally the light color
void MainWindow::handleSelectedBrightness(QColor color) {
    // Set the member variable
    setColorWithBrightness(color);

    // Set the color preview
    setColorPreview(color);

    // Handle the light color update, if selected
    if (this->ui->updateLightCheckBox->isChecked()) {
        updateLightColor(color);
    }
}

void MainWindow::handleStatusButtons() {
    Status::getStatuses(settings, statuses);
    ui->statusButton1->setText(statuses[0].name);
    ui->statusButton2->setText(statuses[1].name);
    ui->statusButton3->setText(statuses[2].name);
}

/*
 *
 * OPTIONAL UPDATES
 *
 */

// Only updates the light color
void MainWindow::updateLightColor(QColor color) {
    if (color.isValid()) {
        // Get the RGB values for the selected color
        int r, g, b;
        color.getRgb(&r, &g, &b);

        // Turn the light device to the selected color
        this->lightDevice->setColor((char)r, (char)g, (char)b);
    }
}

// Only updates the user's slack status
void MainWindow::updateSlackStatus(Status status) {
    // Make sure the update slack checkbox is selected
    if (this->ui->updateSlackCheckBox->isChecked()) {
        if (slackToken == NULL) {
            QMessageBox msgBox;
            msgBox.setText("Unable to send status to Slack. Please reconnect your Slack Account by going to menu option: Customize > Reconfigure Slack.");
            msgBox.exec();
        } else {
            // Create the request
            QUrl serviceUrl = QUrl("https://slack.com/api/users.profile.set");
            QNetworkRequest request(serviceUrl);
            QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
            connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));

            // Set headers
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            QString tokenString = "Bearer ";
            tokenString.append(slackToken);
            request.setRawHeader(QByteArray("Authorization"), tokenString.toUtf8());

            // Build the profile string
            QUrlQuery postData;
            QString profileString = QString("{ \"status_text\": \"");
            profileString.append(status.name);
            profileString.append(QString("\", \"status_emoji\": \""));
            profileString.append(status.slackEmoji);
            profileString.append(QString("\", \"status_expiration\": 0 }"));
            postData.addQueryItem("profile", profileString);

            // Send the request
            networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
        }
    }
}

/*
 *
 * MEMBER VARIABLE SETTERS
 *
 */

// Sets the color member variable
void MainWindow::setColor(QColor color) {
    if (color.isValid()) {
        // Update the member variable
        this->color = color;
    }
}

// Sets the color with brightness member variable
void MainWindow::setColorWithBrightness(QColor color) {
    if (color.isValid()) {
        // Set the member variable
        this->colorWithBrightness = color;
    }
}

/*
 *
 * UI SETTERS
 *
 */

// Only sets the color preview of the gui
void MainWindow::setColorPreview(QColor color) {
    if (color.isValid()) {
         // Set the color preview
         QPalette pal = palette();
         pal.setColor(QPalette::Background, color);
         ui->colorPreviewWidget->setAutoFillBackground(true);
         ui->colorPreviewWidget->setPalette(pal);

         ui->colorPreviewWidget->repaint();
    }
}

// Only sets the status state of the gui (status name and color)
void MainWindow::setStatusPreview(Status status) {
    // Set Status
    this->status = status;
    this->ui->statusLabel->setText(status.name);

    // Set Color and Color Preview
    setColorPreview(status.color);
}

// Only sets the slider value
void MainWindow::setBrightnessSliderValue(QColor color) {
    if (color.isValid()) {
        // Set the brightness slider value
        int r, g, b;
        color.getRgb(&r, &g, &b);
        int sliderValue = maxRGB(r, g, b) / (255.0 / 99) - 1;
        ui->brightnessSlider->setValue(sliderValue);
    }
}

// Enables or disables all the set status buttons
void MainWindow::setStatusButtonsEnabled(bool isEnabled) {
    ui->statusButton1->setEnabled(isEnabled);
    ui->statusButton2->setEnabled(isEnabled);
    ui->statusButton3->setEnabled(isEnabled);
}

/*
 *
 * HELPER FUNCTIONS
 *
 */

// Returns the max value of the RGB values passed
int MainWindow::maxRGB(int r, int g, int b) {

    // Return the largest of the RGB values
    if (r > b && r > g) {
        return r;
    }
    else if (b > g) {
        return b;
    }
    else return g;
}



QString MainWindow::decryptToken(QString token) {
    SimpleCrypt crypto(Q_UINT64_C(0x0000000000000000)); //TODO: Change to some random number hardcoded
    return crypto.decryptToString(token);
}

/*
 *
 * UI SLOTS
 *
 */

void MainWindow::on_colorPickerButton_clicked() {
    //  the color picker
    QColor selectedColor = QColorDialog::getColor(this->color, this );

    // Handle the selected color
    handleSelectedColor(selectedColor);

//    qDebug("%s", qUtf8Printable(QString("Color Chosen: %1").arg(selectedColor.name()));
}

void MainWindow::on_statusButton1_clicked() {
    handleSelectedStatus(statuses[0]);
}

void MainWindow::on_statusButton2_clicked() {
    handleSelectedStatus(statuses[1]);
}

void MainWindow::on_statusButton3_clicked() {
    handleSelectedStatus(statuses[2]);
}

void MainWindow::on_brightnessSlider_valueChanged(int value) {
    // Find the max of r, g, b values
    int r, g, b;
    this->color.getRgb(&r, &g, &b);

    // Find the quotient for the max value
    int maxVal = maxRGB(r, g, b);
    double maxQuotient = (255.0 / 99.0) / maxVal;

    // Calculate the modified color values
    int rVal = (int)(r * maxQuotient * value);
    int gVal = (int)(g * maxQuotient * value);
    int bVal = (int)(b * maxQuotient * value);

    handleSelectedBrightness(QColor(rVal, gVal, bVal));
}

void MainWindow::on_connectToSlackButton_clicked() {
    // TODO: Maybe we shouldn't open it if they aren't connected to the internet or open it and show an error message
    this->slackDialog = new SlackDialog(settings, nullptr);
    connect(this->slackDialog, SIGNAL(finished(int)), this, SLOT(setSlackToken()));
    this->slackDialog->exec();

}

void MainWindow::on_updateLightCheckBox_stateChanged(int arg1) {
    // Update Light Box Checked
    if (arg1 > 0) {
        handleSelectedColor(this->color);
    }

    // Update Light Box Unchecked
    else {
        // Remove error message
        this->ui->errorMessage->setText("");
    }
}

/*
 *
 * DYNAMIC SLOTS
 *
 */

void MainWindow::noDeviceConnected() {
    // Set the error message
    ui->errorMessage->setStyleSheet("QLabel { color : red; }");
    ui->errorMessage->setText("No device connected. Please reconnect your device and try again.");
}

void MainWindow::deviceConnected() {
    // Clear the error message
    ui->errorMessage->setText("");
}

void MainWindow::serviceRequestFinished(QNetworkReply* reply) {
    // Check if the rate limit is exceeded and that a retry-after header was sent
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 429 && reply->hasRawHeader(QByteArray("Retry-After"))) {
        // Disable the Status Buttons for retry-after seconds
        setStatusButtonsEnabled(false);
        QTimer::singleShot(3000, this, SLOT(makeButtonsClickable()));
        ui->slackErrorLabel->setStyleSheet("QLabel { color : red; }");
        ui->slackErrorLabel->setText("Rate limit exceeded. One moment.");
    }

    // Debug: Print the error
//    qDebug() << reply->error();

    // Debug: Print the error code
//    QByteArray bytes = reply->readAll();
//    QString str = QString::fromUtf8(bytes.data(), bytes.size());
//    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//    qDebug() << QVariant(statusCode).toString();
}

void MainWindow::makeButtonsClickable() {
    // Enable update status buttons and remove error message
    setStatusButtonsEnabled(true);
    ui->slackErrorLabel->setText("");
}

void MainWindow::setSlackToken() {
    // get token from settings
    QString token = settings->value("token", "").toString();
    if (token != "") {
        this->slackToken = decryptToken(token);

        // Enable the 'Update Slack' checkbox
        this->ui->updateSlackCheckBox->setDisabled(false);
        this->ui->updateSlackCheckBox->setChecked(true);
        this->ui->updateSlackCheckBox->setToolTip(QString("Check to enable updates to slack on status change"));

        // Remove the 'Add to Slack' button and move to 'Customize' menu
        this->ui->connectToSlackButton->setVisible(false);
        this->ui->actionReconfigure_Slack->setVisible(true);

        isSlackEnabled = true;

        qDebug() << "Slack token found";
    } else {
        qDebug() << "slack token not found";
    }
}

void MainWindow::closeStatusWidget(bool isSaved) {
    if (isSaved) {
        // Update the GUI buttons
        handleStatusButtons();
    }
}

void MainWindow::on_actionCustomize_Statuses_triggered() {

    this->customizeStatusWidget = new CustomizeStatusWidget(settings, isSlackEnabled, nullptr);
    connect(this->customizeStatusWidget, SIGNAL(finished(bool)), this, SLOT(closeStatusWidget(bool)));
    this->customizeStatusWidget->show();
}

void MainWindow::on_actionReconfigure_Slack_triggered() {
    // Same as the 'Add to Slack' button
    on_connectToSlackButton_clicked();
}
