#ifndef CUSTOMIZESTATUSWIDGET_H
#define CUSTOMIZESTATUSWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QSettings>
#include <QtCore/QDataStream>
#include <QColorDialog>
#include <QCloseEvent>

#include "status.h"

namespace Ui {
class CustomizeStatusWidget;
}

class CustomizeStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomizeStatusWidget(QSettings *settings, bool isSlackEnabled, QWidget *parent = nullptr);
    ~CustomizeStatusWidget();

    void closeEvent(QCloseEvent* event);

signals:
    void finished(bool isSaved);

private slots:
    void on_saveButton_clicked();
    void on_colorButton1_clicked();
    void on_colorButton2_clicked();
    void on_colorButton3_clicked();

private:
    Ui::CustomizeStatusWidget *ui;
    QSettings *appsettings;
    Status statuses[NUM_DEFAULT_STATUSES];
    bool isSavedFlag = false;

    void loadStatuses();
    void initializeGUI();
    void setColorWidgetColor(QWidget* widget, QColor color);

    void colorButtonClicked(QWidget* widget, int statusIndex);


};

#endif // CUSTOMIZESTATUSWIDGET_H
