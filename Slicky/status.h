#ifndef STATUS_H
#define STATUS_H

#include <QtCore/QDataStream>
#include <QColor>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>

class Status {

public:
    Status();
    Status(QString name, QColor color, QString slackEmoji);
    QString name;
    QColor color;
    QString slackEmoji;

    static void initializeSettingsStatuses(QSettings* settings);
    static Status* getStatuses(QSettings* settings, Status* statuses);
    static bool areStatusesSaved(QSettings* settings);
    static void saveStatusesToSettings(QSettings* settings, Status* statuses);

    static void debugSettingsStatuses(QSettings* settings);

    friend QDataStream & operator << (QDataStream &out, const Status& object) {
        out << object.name << object.color << object.slackEmoji;
        return out;
    }

    friend QDataStream & operator >> (QDataStream &in, Status& object) {
        in >> object.name;
        in >> object.color;
        in >> object.slackEmoji;
        return in;
    }

private:
};
Q_DECLARE_METATYPE(Status);

const int NUM_DEFAULT_STATUSES = 3;
const Status STATUSES[] = { Status("Available", Qt::green, ":wave:"), Status("Busy", Qt::yellow, ":face_with_monocle:"), Status("Do Not Disturb", Qt::red, ":no_entry_sign:") };

#endif // STATUS_H
