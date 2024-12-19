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



    void setLowCellVoltage(QString low_cell);

    void setHighCellVoltage(QString high_cell);

    void toggleHiCellVoltageSend();
    void toggleLoCellVoltageSend();


signals:

private:
    QCanBusDevice *canDevice;

    // QVector<QVector<uint8_t>> createPayloadArray(QString input_string);
    // QByteArray fillZeros(QByteArray &array);
    void startCAN();

    void sendHiCellVoltage();
    void sendLoCellVoltage();



private:
    QSerialPort* port;

    bool areWeSendinHiCellVoltage{false};
    bool areWeSendingLoCellVoltage{false};

    uint8_t message_id = 0x19;

    QQueue<QCanBusFrame> outBuffer;

    QString lowCellVoltage{"0"};
    QString highCellVoltage{"0"};


};

#endif // CANHANDLER_H
