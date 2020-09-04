#include "status.h"

Status::Status() {
    this->name = "";
    this->color = Qt::white;
    this->slackEmoji = "";
}

Status::Status(QString name, QColor color, QString slackEmoji) {
    this->name = name;
    this->color = color;
    this->slackEmoji = slackEmoji;
}

void Status::initializeSettingsStatuses(QSettings* settings) {
    if (!Status::areStatusesSaved(settings)) {
        Status statuses[] = { STATUSES[0], STATUSES[1], STATUSES[2] };
        saveStatusesToSettings(settings, statuses);
    }
}

Status* Status::getStatuses(QSettings* settings, Status* statuses) {
//    qDebug() << "--- GETTING ---";
    for (int i = 0; i < NUM_DEFAULT_STATUSES; i++) {
        QVariant value = settings->value(QString("status%1").arg(i + 1));
        Status s = value.value<Status>();
        statuses[i] = s;
        qDebug() << s.name << s.color << s.slackEmoji;
    }
    return statuses;
}

bool Status::areStatusesSaved(QSettings* settings) {
    bool isSaved = true;
    for (int i = 0; i < NUM_DEFAULT_STATUSES; i++) if (!settings->contains(QString("status%1").arg(i + 1))) isSaved = false;
    return isSaved;
}

void Status::saveStatusesToSettings(QSettings* settings, Status* statuses) {
//    qDebug() << "--- SAVING ---";

    for (int i = 0; i < NUM_DEFAULT_STATUSES; i++) {
        settings->setValue(QString("status%1").arg(i+1), QVariant::fromValue(statuses[i]));

        settings->sync(); // Ensure the token is saved

        // Notify user of error, if any
        if (settings->status() != QSettings::NoError) {
            QMessageBox errorBox;
            if (settings->status() == QSettings::AccessError) {
                errorBox.setText("Access Error");
                errorBox.exec();
            }
            else if (settings->status() == QSettings::FormatError) {
                errorBox.setText("Format Error");
                errorBox.exec();
            }
        }

        QVariant value = settings->value(QString("status%1").arg(i + 1));
        Status s = value.value<Status>();
        qDebug() << "STATUS" << s.name << s.color << s.slackEmoji;
    }
}

void Status::debugSettingsStatuses(QSettings* settings) {
    for (int i = 0; i < NUM_DEFAULT_STATUSES; i++) {
        QVariant value = settings->value(QString("status%1").arg(i + 1));
        Status s = value.value<Status>();
//        qDebug() << s.name << s.color << s.slackEmoji;
    }
}
