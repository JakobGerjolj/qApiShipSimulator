#ifndef CANHANDLER_H
#define CANHANDLER_H

#include <QObject>
#include <QCanBusDevice>
#include <QCanBusDevice>
#include <QCanBus>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QtEndian>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <cstdint>
#include <QByteArray>
#include <QtGlobal>
#include <QQueue>
#include "q_api_encoder.h"


class CanHandler : public QObject
{
    Q_OBJECT
public:
    explicit CanHandler(QObject *parent = nullptr);
    ~CanHandler();

    void transformCanPackage(QCanBusFrame const& frame, QByteArray &packedFrame);

    void sendToCL2000(const QCanBusFrame &frame);

    enum struct CRC16Type {
        CRC_16_IBM
    };

    uint16_t calculateCRC16(QByteArray data, CRC16Type crc16Type = CRC16Type::CRC_16_IBM);



    void setLowCellVoltage(QString low_cell){ lowCellVoltage = low_cell;}

    void setHighCellVoltage(QString high_cell){ highCellVoltage = high_cell;}

    void setChargerTemp(QString charger_temp){ chargerTemp = charger_temp;}

    void setChargerVoltage(QString voltage){ chargerVoltage = voltage;}

    void setChargerCurrent(QString current){ chargerCurrent = current;}

    void setChargerInVoltage(QString voltage){ chargerInVoltage = voltage;}

    void setChargerInInstance(QString instance){ chargerInVoltageInstance = instance;}

    void setChargerInCurrent(QString current){ chargerInCurrent = current;}

    void setChargerInCurrentInstance(QString instance){ chargerInCurrentInstance = instance;}

    void setPortMotorStatus(QString status){ portMotorStatus = status;}

    void setBatteryStatus(QString status){ batteryStatus = status;}

    void toggleHiCellVoltageSend();
    void toggleLoCellVoltageSend();
    void toggleChargerTempSend();
    void toggleChargerVoltageSend(){ areWeSendingChargerVoltage = !areWeSendingChargerVoltage; }
    void toggleChargerCurrentSend(){ areWeSendingChargerCurrent = !areWeSendingChargerCurrent; }
    void toggleChargerInVoltageSend(){ areWeSendingChargerInVoltage  = !areWeSendingChargerInVoltage; }
    void toggleChargerInCurrent(){ areWeSendingChargerInCurrent = !areWeSendingChargerInCurrent; }
    void togglePortMotor(){ areWeSendingPortMotorStatus = !areWeSendingPortMotorStatus; }
    void toggleBatteryS(){ areWeSendingBatteryStatus = !areWeSendingBatteryStatus; }

signals:

private:
    QCanBusDevice *canDevice;

    // QVector<QVector<uint8_t>> createPayloadArray(QString input_string);
    // QByteArray fillZeros(QByteArray &array);
    void startCAN();

    void sendHiCellVoltage();
    void sendLoCellVoltage();
    void sendChargerTemp();
    void sendChargerVoltage();
    void sendChargerCurrent();
    void sendChargerInVoltage();
    void sendChargerInCurrent();
    void sendPortMotorStatus();
    void sendBatteryStatus();

private:
    QSerialPort* port;

    bool areWeSendinHiCellVoltage{false};
    bool areWeSendingLoCellVoltage{false};
    bool areWeSendingChargerTemp{false};
    bool areWeSendingChargerVoltage{false};
    bool areWeSendingChargerCurrent{false};
    bool areWeSendingChargerInVoltage{false};
    bool areWeSendingChargerInCurrent{false};
    bool areWeSendingPortMotorStatus{false};
    bool areWeSendingBatteryStatus{false};

    uint8_t message_id = 0x19;

    QQueue<QCanBusFrame> outBuffer;

    QString lowCellVoltage{"0"};
    QString highCellVoltage{"0"};

    QString chargerTemp{"0"};
    QString chargerVoltage{"0"};
    QString chargerCurrent{"0"};
    QString chargerInVoltage{"0"};
    QString chargerInVoltageInstance{"0"};
    QString chargerInCurrent{"0"};
    QString chargerInCurrentInstance{"0"};
    QString portMotorStatus{"0"};
    QString batteryStatus{"0"};

};

#endif // CANHANDLER_H
