#ifndef LIGHTDEVICE_H
#define LIGHTDEVICE_H

#include <QString>
#include <QObject>
#include "hidapi.h"

class LightDevice : public QObject
{
    Q_OBJECT

public:
    LightDevice();

    // Public Member Functions
    int setColor(char r, char g, char b, char w = 0x00);
    int turnOff();
    bool isValid();

signals:
    void noDeviceConnected();
    void deviceConnected();

private:
    hid_device *hidDevice;

    // Private Member Functions
    bool isConnected();
    int writeBuffer(unsigned char buf[65]);

    // Test Functions
    QString getManufacturerString();
    QString getProductString();
    QString getSerialNumberString();
};

#endif // LIGHTDEVICE_H
