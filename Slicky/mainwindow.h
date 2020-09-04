#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QTimer>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkReply>
#include <QSettings>
#include <QMessageBox>

#include "lightdevice.h"
#include "slackdialog.h"
#include "customizestatuswidget.h"
#include "status.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI Slots
    void on_colorPickerButton_clicked();
    void on_statusButton1_clicked();
    void on_statusButton2_clicked();
    void on_statusButton3_clicked();
    void on_brightnessSlider_valueChanged(int value);
    void on_connectToSlackButton_clicked();
    void on_updateLightCheckBox_stateChanged(int arg1);
    void on_actionCustomize_Statuses_triggered();
    void on_actionReconfigure_Slack_triggered();

    // Dynamic Slots
    void noDeviceConnected();
    void deviceConnected();
    void serviceRequestFinished(QNetworkReply* reply);
    void makeButtonsClickable();
    void setSlackToken();
    void closeStatusWidget(bool isSaved);

private:
    Ui::MainWindow *ui;
    SlackDialog *slackDialog;
    CustomizeStatusWidget *customizeStatusWidget;
    Status statuses[NUM_DEFAULT_STATUSES];
    bool isSlackEnabled = false;

    Status status;
    QColor color;
    QColor colorWithBrightness;
    LightDevice *lightDevice;
    QString slackToken;

    QString settingsFile;
    QSettings *settings;

    // Handlers
    void handleSelectedColor(QColor color);
    void handleSelectedStatus(Status status);
    void handleSelectedBrightness(QColor color);
    void handleStatusButtons();

    // Optional Updates
    void updateLightColor(QColor color);
    void updateSlackStatus(Status status);

    // Member Variable Setters
    void setColor(QColor color);
    void setColorWithBrightness(QColor color);

    // UI Setters
    void setColorPreview(QColor color);
    void setStatusPreview(Status status);
    void setBrightnessSliderValue(QColor color);
    void setStatusButtonsEnabled(bool isEnabled);

    // Helper Functions
    int maxRGB(int r, int g, int b);
    void initializeSettings();
    QString decryptToken(QString token);

};
#endif // MAINWINDOW_H
