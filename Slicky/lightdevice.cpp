#include "lightdevice.h"

#define MAX_STR 255
#define VENDOR_ID 0x04D8
#define PRODUCT_ID 0xEC24

LightDevice::LightDevice() {

    // Initialize the hidapi library
    int res = hid_init();
    if (res < 0) {
        qDebug("Error: Problem initializing the hidapi library.");
    }

    // Open the device using the VID, PID, and [optional] Serial number
    this->hidDevice = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
}

/*
 *
 * PUBLIC MEMBER FUNCTIONS
 *
 */

int LightDevice::setColor(char r, char g, char b, char w) {

    unsigned char buf[65]; // 65 bytes
    // The first byte is the write endpoint (0x00).
    // 0A 04 00 00 WW BB GG RR
    buf[0] = 0x00;
    buf[1] = 0x0A;
    buf[2] = 0x04;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = w;
    buf[6] = b;
    buf[7] = g;
    buf[8] = r;

    return this->writeBuffer(buf);
}

int LightDevice::turnOff() {

    unsigned char buf[65]; // 65 bytes
    // The first byte is the write endpoint (0x00).
    // 0A 04 00 00 WW BB GG RR
    buf[0] = 0x00;
    buf[1] = 0x0A;
    buf[2] = 0x04;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
    buf[8] = 0x00;

    return this->writeBuffer(buf);
}

bool LightDevice::isValid() {
    return this->hidDevice != nullptr;
}

/*
 *
 * PRIVATE MEMBER FUNCTIONS
 *
 */

int LightDevice::writeBuffer(unsigned char buf[65]) {

    int res = -1;
    if (isConnected()) {
        res = hid_write(this->hidDevice, buf, 65); // 65 on success, -1 on failure
        if (res < 0) qDebug("Error: Problem writing to hid device.");
    }
    else qDebug("Error: No device connected.");

    return res;
}

bool LightDevice::isConnected() {
    if (this->hidDevice != nullptr) hid_close(this->hidDevice);
    this->hidDevice = hid_open(VENDOR_ID, PRODUCT_ID, NULL);

    if (this->hidDevice == nullptr) {
        emit noDeviceConnected();
        return false;
    }
    emit deviceConnected();
    return true;
}

/*
 *
 * TEST FUNCTIONS
 *
 */
QString LightDevice::getManufacturerString() {
    if (isConnected()) {
        wchar_t wstr[MAX_STR];
        int res = hid_get_manufacturer_string(this->hidDevice, wstr, MAX_STR);
        if (res < 0) {
            qDebug("Error: Unable to read manufacturer string.");
            return "Error: Unable to read manufacturer string.";
        }
        return QString::fromWCharArray(wstr);
    }

    qDebug("Error: No device connected.");
    return "Error: No device connected.";
}

QString LightDevice::getProductString() {

    if (isConnected()) {
        wchar_t wstr[MAX_STR];
        int res = hid_get_product_string(this->hidDevice, wstr, MAX_STR);
        if (res < 0) {
            qDebug("Error: Unable to read product string.");
            return "Error: Unable to read product string.";
        }
        return QString::fromWCharArray(wstr);
    }

    qDebug("Error: No device connected.");
    return "Error: No device connected";
}

QString LightDevice::getSerialNumberString() {

    if (isConnected()) {
        wchar_t wstr[MAX_STR];
        int res = hid_get_serial_number_string(this->hidDevice, wstr, MAX_STR);
        if (res < 0) {
            qDebug("Error: Unable to read serial number string.");
            return "Error: Unable to read serial number string.";
        }
        return QString::fromWCharArray(wstr);
    }

    qDebug("Error: No device connected.");
    return "Error: No device connected";
}
