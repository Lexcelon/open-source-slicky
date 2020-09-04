#include "customizestatuswidget.h"
#include "ui_customizestatuswidget.h"

CustomizeStatusWidget::CustomizeStatusWidget(QSettings *settings, bool isSlackEnabled, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomizeStatusWidget) {
    ui->setupUi(this);

    appsettings = settings;

    if (isSlackEnabled) {
        ui->slackEmojiLineEdit1->setDisabled(false);
        ui->slackEmojiLineEdit2->setDisabled(false);
        ui->slackEmojiLineEdit3->setDisabled(false);
    }

    // Load statuses from settings
    loadStatuses();

    // Initialize gui with statuses
    initializeGUI();
}

CustomizeStatusWidget::~CustomizeStatusWidget() {
    delete ui;
}

void CustomizeStatusWidget::loadStatuses() {
    // Load statuses from settings or default if none
    if (Status::areStatusesSaved(appsettings)) {
        for (int i = 0; i < NUM_DEFAULT_STATUSES; i++) {
            QVariant value = appsettings->value(QString("status%1").arg(i + 1));
            Status s = value.value<Status>();
            statuses[i] = s;
        }
    }
    else {
        // Load with default
        for (int i = 0; i < NUM_DEFAULT_STATUSES; i++) statuses[i] = STATUSES[i];
    }
}

void CustomizeStatusWidget::initializeGUI() {
    // Initialize gui with statuses
    ui->nameLineEdit1->setText(statuses[0].name);
    ui->nameLineEdit2->setText(statuses[1].name);
    ui->nameLineEdit3->setText(statuses[2].name);

    setColorWidgetColor(ui->colorWidget1, statuses[0].color);
    setColorWidgetColor(ui->colorWidget2, statuses[1].color);
    setColorWidgetColor(ui->colorWidget3, statuses[2].color);

    ui->slackEmojiLineEdit1->setText(statuses[0].slackEmoji);
    ui->slackEmojiLineEdit2->setText(statuses[1].slackEmoji);
    ui->slackEmojiLineEdit3->setText(statuses[2].slackEmoji);
}

void CustomizeStatusWidget::setColorWidgetColor(QWidget* widget, QColor color) {
    if (color.isValid()) {
         QPalette pal = palette();
         pal.setColor(QPalette::Background, color);
         widget->setAutoFillBackground(true);
         widget->setPalette(pal);
         widget->repaint();
    }
}

void CustomizeStatusWidget::colorButtonClicked(QWidget* widget, int statusIndex) {
    // Open color picker
    QColor selectedColor = QColorDialog::getColor(statuses[statusIndex].color, this);

    if (selectedColor.isValid()) {
         // Set the color preview
         setColorWidgetColor(widget, selectedColor);

         // Update the status
         statuses[statusIndex].color = selectedColor;
    }
}

void CustomizeStatusWidget::on_saveButton_clicked() {

    // Get status names and slack emojis from GUI
    statuses[0].name = ui->nameLineEdit1->text();
    statuses[1].name = ui->nameLineEdit2->text();
    statuses[2].name = ui->nameLineEdit3->text();
    statuses[0].slackEmoji = ui->slackEmojiLineEdit1->text();
    statuses[1].slackEmoji = ui->slackEmojiLineEdit2->text();
    statuses[2].slackEmoji = ui->slackEmojiLineEdit3->text();

    Status::saveStatusesToSettings(appsettings, statuses);
    isSavedFlag = true;
    this->close();
}

void CustomizeStatusWidget::on_colorButton1_clicked() {
    colorButtonClicked(ui->colorWidget1, 0);
}

void CustomizeStatusWidget::on_colorButton2_clicked() {
    colorButtonClicked(ui->colorWidget2, 1);
}

void CustomizeStatusWidget::on_colorButton3_clicked() {
    colorButtonClicked(ui->colorWidget3, 2);
}

void CustomizeStatusWidget::closeEvent( QCloseEvent* event ) {
    emit finished(isSavedFlag);
    event->accept();
}
