#include "canhandler.h"

CanHandler::CanHandler(QObject *parent)
    : QObject{parent}
{
    this -> startCAN();

    QTimer *timer  = new QTimer(this);
    connect(timer, &QTimer::timeout,this, &CanHandler::sendHiCellVoltage);
    timer->start(1000);

    QTimer *timer2  = new QTimer(this);
    connect(timer2, &QTimer::timeout,this, &CanHandler::sendLoCellVoltage);
    timer2->start(1000);

    QTimer *timer3  = new QTimer(this);
    connect(timer3, &QTimer::timeout,this, &CanHandler::sendChargerTemp);
    timer3->start(1000);

    QTimer *timer4  = new QTimer(this);
    connect(timer4, &QTimer::timeout,this, &CanHandler::sendChargerVoltage);
    timer4->start(1000);

    QTimer *timer5  = new QTimer(this);
    connect(timer5, &QTimer::timeout,this, &CanHandler::sendChargerCurrent);
    timer5->start(1000);

    QTimer *timer6 = new QTimer(this);
    connect(timer6, &QTimer::timeout,this, &CanHandler::sendChargerInVoltage);
    timer6->start(1000);

    QTimer *timer7 = new QTimer(this);
    connect(timer7, &QTimer::timeout, this, &CanHandler::sendChargerInCurrent);
    timer7->start(1000);

    QTimer *timer8 = new QTimer(this);
    connect(timer8, &QTimer::timeout, this, &CanHandler::sendPortMotorStatus);
    timer8 -> start(1000);

    QTimer *timer9 = new QTimer(this);
    connect(timer9, &QTimer::timeout, this, &CanHandler::sendBatteryStatus);
    timer9 -> start(1000);

    QTimer *timer10 = new QTimer(this);
    connect(timer10, &QTimer::timeout, this, &CanHandler::sendChargerStatus);
    timer10 -> start(1000);

    QTimer *timer11 = new QTimer(this);
    connect(timer11, &QTimer::timeout, this, &CanHandler::sendStbMotorStatus);
    timer11 -> start(1000);

    QTimer *timer12 = new QTimer(this);
    connect(timer12, &QTimer::timeout, this, &CanHandler::sendDCDCVoltage);
    timer12 -> start(1000);

    QTimer *timer13 = new QTimer(this);
    connect(timer13, &QTimer::timeout, this, &CanHandler::sendDCDCCurrent);
    timer13 -> start(1000);

    QTimer *timer14 = new QTimer(this);
    connect(timer14, &QTimer::timeout, this, &CanHandler::sendDCDCStatus);
    timer14 -> start(1000);

    QTimer *timer15 = new QTimer(this);
    connect(timer15, &QTimer::timeout, this, &CanHandler::sendMaxPortPower);
    timer15 -> start(1000);

    QTimer *timer16 = new QTimer(this);
    connect(timer16, &QTimer::timeout, this, &CanHandler::sendFullChargeBattery);
    timer16 -> start(1000);

    QTimer *timer17 = new QTimer(this);
    connect(timer17, &QTimer::timeout, this, &CanHandler::sendShoreLimit);
    timer17 -> start(1000);

}

void CanHandler::transformCanPackage(const QCanBusFrame &frame, QByteArray &packedFrame)
{

    QByteArray payloadArray;
    QDataStream payloadStream(&payloadArray, QIODevice::ReadWrite);
    payloadStream.setByteOrder(QDataStream::ByteOrder::BigEndian);

    payloadStream << static_cast<quint8>(0x03);

    /* Write CAN ID (Big endian) */
    quint32 canID = frame.frameId() & 0x1FFFFFFF;
    if( frame.hasExtendedFrameFormat() ) {
        canID |= 0x20000000;
    }
    payloadStream << canID;

    int dataLength = frame.payload().length();
    if(0 <= dataLength && dataLength <= 8) {
        auto DLC = static_cast<quint8>(dataLength);
        payloadStream << DLC;
    }

    payloadStream.writeRawData(frame.payload().data(), dataLength);

    /* Calculate checksum */
    quint16 checksum = calculateCRC16(payloadArray);

    /* Write checksum */
    payloadStream << checksum;

    /* Escape any 0x7E sequences */
    packedFrame.append(0x07E);
    for (char const& byteValue: payloadArray) {
        if( byteValue == 0x7E || byteValue == 0x7D) {
            packedFrame.append(static_cast<quint8>(0x7D));
            packedFrame.append(static_cast<quint8>(byteValue ^ 0b00100000) );
        } else {
            packedFrame.append(byteValue );
        }
    }
    packedFrame.append(0x07E);

}

void CanHandler::sendToCL2000(const QCanBusFrame &frame)
{

    QByteArray packedFrame;
    transformCanPackage(frame, packedFrame);

    port->write(packedFrame);

}

uint16_t CanHandler::calculateCRC16(QByteArray data, CRC16Type crc16Type)
{
    uint16_t const initial = 0x0000u;
    uint16_t const polynomial = 0xA001;
    uint16_t const final = 0x0000;

    uint16_t crc = initial;

    for(char const& value: data) {
        // Reinterpret from signed to unsigned.
        uint8_t d = value;

        crc ^= d;

        for(auto i = 0; i < 8; ++i) {
            if (crc & 0x0001u) {
                crc >>= 1;
                crc ^= polynomial;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc ^ final;
}

void CanHandler::sendHiCellVoltage()
{

    //Id is gonna be 0x115C

    //{"cmd":"svv","sid":1,"vid":1,"iid":0,"v":1234} example of data with instance
    //{"cmd":"svv","sid":1,"vid":1,"v":1234} example of data without instance

    //{"cmd":"svv","sid":1028,"vid":10,"v":1000} should be high cell voltage

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":10,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":10,"v":%1})").arg(highCellVoltage);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendinHiCellVoltage){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(highCellVoltage);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendinHiCellVoltage){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendLoCellVoltage()
{

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":9,"v":%1})";


    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":9,"v":%1})").arg(lowCellVoltage);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());


    if(areWeSendingLoCellVoltage){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(lowCellVoltage);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingLoCellVoltage){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendChargerTemp()
{

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":4,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":4,"v":%1})").arg(chargerTemp);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingChargerTemp){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(chargerTemp);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingChargerTemp){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendChargerVoltage()
{

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":5,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":5,"v":%1})").arg(chargerVoltage);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingChargerVoltage){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(chargerVoltage);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingChargerVoltage){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}




void CanHandler::sendChargerCurrent()
{

    // QString string_message = R"({"cmd":"svv","sid":1028,"vid":6,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":6,"v":%1})").arg(chargerCurrent);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());


    if(areWeSendingChargerCurrent){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(chargerCurrent);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingChargerCurrent){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendChargerInVoltage()
{

    QString Qmessage;

    //{"cmd":"svv","sid":1,"vid":1,"iid":0,"v":1234} example of data with instance

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":7,"iid":%1,"v":%2})";

    if(chargerInVoltage.toInt() == 2000){

        Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":7,"iid":%1,"v":%2})").arg(chargerInVoltageInstance).arg("65535");

    }else{

        Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":7,"iid":%1,"v":%2})").arg(chargerInVoltageInstance).arg(chargerInVoltage);

    }
    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingChargerInVoltage){

        sendNMEA2KQMessage(QPreamble);

    }


    // QString modified_string = string_message.arg(chargerInVoltageInstance).arg(chargerInVoltage);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingChargerInVoltage){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendChargerInCurrent()
{

    QString Qmessage;

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":8,"iid":%1,"v":%2})";

    if(chargerInCurrent.toInt() == 2000){

        Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":8,"iid":%1,"v":%2})").arg(chargerInCurrentInstance).arg("32767");

    }else{

        Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":8,"iid":%1,"v":%2})").arg(chargerInCurrentInstance).arg(chargerInCurrent);

    }

    //QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":8,"iid":%1,"v":%2})").arg(chargerInCurrentInstance).arg(chargerInCurrent);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingChargerInCurrent){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(chargerInCurrentInstance).arg(chargerInCurrent);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingChargerInCurrent){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendPortMotorStatus()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":13,"iid":0,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":13,"iid":0,"v":%1})").arg(portMotorStatus);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingPortMotorStatus){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(portMotorStatus);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingPortMotorStatus){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendBatteryStatus()
{

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":14,"iid":0,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":14,"iid":0,"v":%1})").arg(batteryStatus);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingBatteryStatus){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(batteryStatus);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingBatteryStatus){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendChargerStatus()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":15,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":15,"v":%1})").arg(chargerStatus);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingChargerStatus){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(chargerStatus);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingChargerStatus){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }


}

void CanHandler::sendStbMotorStatus()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":13,"iid":1,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":13,"iid":1,"v":%1})").arg(stbMotorStatus);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingStbMotorStatus){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(stbMotorStatus);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingStbMotorStatus){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendDCDCVoltage()
{

   // QString string_message = R"({"cmd":"svv","sid":1028,"vid":11,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":11,"v":%1})").arg(dcdcVoltage);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingDCDCVoltage){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(dcdcVoltage);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingDCDCVoltage){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendDCDCCurrent()
{


    QString string_message = R"({"cmd":"svv","sid":1028,"vid":12,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":12,"v":%1})").arg(dcdcCurrent);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    if(areWeSendingDCDCCurrent){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(dcdcCurrent);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingDCDCCurrent){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendDCDCStatus()
{

    //QString string_message = R"({"cmd":"svv","sid":1028,"vid":16,"v":%1})";

    QString Qmessage = QString(R"({"cmd":"svv","sid":1028,"vid":16,"v":%1})").arg(dcdcStatus);

    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());


    if(areWeSendingDCDCStatus){

        sendNMEA2KQMessage(QPreamble);

    }

    // QString modified_string = string_message.arg(dcdcStatus);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{modified_string, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // if(areWeSendingDCDCStatus){

    //     for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //         QCanBusFrame frame;
    //         frame.setFrameId(0x0000115C);
    //         QByteArray payload;
    //         payload.resize(8);

    //         for(int y = 0; y < 8; y++){

    //             payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //         }

    //         frame.setPayload(payload);

    //         sendToCL2000(frame);
    //         canDevice -> writeFrame(frame);

    //     }

    // }

}

void CanHandler::sendMaxPortPower()
{

    QString string_message = R"({"cmd":"svv","sid":1,"vid":22,"iid":0,"v":%1})";

    QString modified_string = string_message.arg(portMotorMaxPower);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingPortMaxPower){

        for(int i = 0; i < vectorToSendToCAN.size(); i++){

            QCanBusFrame frame;
            frame.setFrameId(0x0000115C);
            QByteArray payload;
            payload.resize(8);

            for(int y = 0; y < 8; y++){

                payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

            }

            frame.setPayload(payload);

            sendToCL2000(frame);
            canDevice -> writeFrame(frame);

        }

    }

}

void CanHandler::sendFullChargeBattery()
{
    // {"cmd":"set-setting-value","gid":1028,"sid":5,"v":15552, “token”: “rD6oy58WmTSc/5nSq1jxdA==”}

    QString string_message = R"({"cmd":"set-setting-value","gid":1028,"sid":5,"v":%1,"token":"rD6oy58WmTSc/5nSq1jxdA=="})";

    QString modified_string = string_message.arg(fullChargeBattery);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingFullCharBatt){

        for(int i = 0; i < vectorToSendToCAN.size(); i++){

            QCanBusFrame frame;
            frame.setFrameId(0x0000115C);
            QByteArray payload;
            payload.resize(8);

            for(int y = 0; y < 8; y++){

                payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

            }

            frame.setPayload(payload);

            sendToCL2000(frame);
            canDevice -> writeFrame(frame);

        }

    }


}

void CanHandler::sendShoreLimit()
{

    //R"({"cmd":"set-setting-value","gid":1028,"sid":1,"v":%1,"token":"rD6oy58WmTSc/5nSq1jxdA=="})";

    QString string_message = R"({"cmd":"set-setting-value","gid":1028,"sid":1,"v":%1,"token":"rD6oy58WmTSc/5nSq1jxdA=="})";

    QString modified_string = string_message.arg(shoreLimit);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingShoreLimit){

        for(int i = 0; i < vectorToSendToCAN.size(); i++){

            QCanBusFrame frame;
            frame.setFrameId(0x0000115C);
            QByteArray payload;
            payload.resize(8);

            for(int y = 0; y < 8; y++){

                payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

            }

            frame.setPayload(payload);

            sendToCL2000(frame);
            canDevice -> writeFrame(frame);

        }

    }

}

void CanHandler::sendEnableValue()
{

   // QString string_message = R"({"cmd":"enable-value","enable":true,"sid":7,"vid":12})";

    QString Qmessage = QString(R"({"cmd":"enable-value","enable":true,"sid":7,"vid":12})");


    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    sendNMEA2KQMessage(QPreamble);

    //QString modified_string = string_message.arg(shoreLimit);

    // QVector<QVector<uint8_t>> vectorToSendToCAN;

    // Encoder encoder1{string_message, 0x19};

    // vectorToSendToCAN = encoder1.createPayloadArray();

    // int counter = 0;

    // for(int i = 0; i < vectorToSendToCAN.size(); i++){

    //     QCanBusFrame frame;
    //     frame.setFrameId(0x0000115C);
    //     QByteArray payload;
    //     payload.resize(8);

    //     for(int y = 0; y < 8; y++){

    //         payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

    //     }

    //     frame.setPayload(payload);

    //     sendToCL2000(frame);
    //     canDevice -> writeFrame(frame);

    // }



}

void CanHandler::sendEnableShoreCurrentLimit()
{
//{"cmd":"enable-value","enable":true,"sid":7,"vid":12}
    QString Qmessage = QString(R"({"cmd":"enable-value","enable":true,"sid":1028,"vid":1,"iid":0})");


    QByteArray QPreamble;
    QPreamble.append("\x30\x80\x83\x01");
    QPreamble.append(static_cast<char>(0x00));
    QPreamble.append(Qmessage.toUtf8());

    sendNMEA2KQMessage(QPreamble);

}

void CanHandler::sendEnableFullChargeMaintenance() //This is max power gauge not maintenance
{

    QString string_message = R"({"cmd":"enable-value","enable":true,"sid":1,"vid":22})";

    //QString modified_string = string_message.arg(shoreLimit);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{string_message, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    for(int i = 0; i < vectorToSendToCAN.size(); i++){

        QCanBusFrame frame;
        frame.setFrameId(0x0000115C);
        QByteArray payload;
        payload.resize(8);

        for(int y = 0; y < 8; y++){

            payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

        }

        frame.setPayload(payload);

        sendToCL2000(frame);
        canDevice -> writeFrame(frame);

    }

}

void CanHandler::sendEnableMaintenance()
{

    QString string_message = R"({"cmd":"enable-setting","enable":true,"gid":1028,"sid":5"})";

    //QString modified_string = string_message.arg(shoreLimit);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{string_message, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    for(int i = 0; i < vectorToSendToCAN.size(); i++){

        QCanBusFrame frame;
        frame.setFrameId(0x0000115C);
        QByteArray payload;
        payload.resize(8);

        for(int y = 0; y < 8; y++){

            payload[y] = static_cast<char>(vectorToSendToCAN[i][y]);

        }

        frame.setPayload(payload);

        sendToCL2000(frame);
        canDevice -> writeFrame(frame);

    }


}

void CanHandler::sendNMEA2KQMessage(QByteArray data)
{

    const int FIRST_PAYLOAD_SIZE = 6;
    const int SUBSEQUENT_PAYLOAD_SIZE = 7;

    qDebug() << "\nData size: "<<data.size()<<"\n";
    int totalSize = static_cast<uint8_t>(data.size());
    int remainingSize = totalSize - FIRST_PAYLOAD_SIZE;
    int numSubsequentFrames = (remainingSize + SUBSEQUENT_PAYLOAD_SIZE - 1) / SUBSEQUENT_PAYLOAD_SIZE; // Ceiling division

    // Handle the first 6 bytes
    QByteArray firstPayload(FIRST_PAYLOAD_SIZE + 2, 0); // 6 bytes + 2 for headers
    firstPayload[0] = 0x00 | 0x00; // Example header for the first frame
    firstPayload[1] = totalSize; // Example header value
    for (int i = 0; i < FIRST_PAYLOAD_SIZE; ++i) {
        firstPayload[i + 2] = data[i];
    }

    QCanBusFrame firstFrame;
    firstFrame.setFrameId(0x0DF0D511);
    firstFrame.setPayload(firstPayload);
    sendToCL2000(firstFrame); // Placeholder for actual sending logic
    //qDebug() << "Sent first frame with payload:" << firstPayload.toHex();

    // Handle the remaining bytes in chunks of 7 bytes
    for (int i = 0; i < numSubsequentFrames; ++i) {
        QCanBusFrame frame;
        frame.setFrameId(0x0DF0D511);
        QByteArray payload(SUBSEQUENT_PAYLOAD_SIZE + 1, 0); // 7 bytes + 1 for header

        int offset = FIRST_PAYLOAD_SIZE + i * SUBSEQUENT_PAYLOAD_SIZE;
        int chunkSize = qMin(SUBSEQUENT_PAYLOAD_SIZE, totalSize - offset);

        payload[0] = 0x00 | (((i + 1)) & 0x0F); // Example header for subsequent frames
        for (int j = 0; j < chunkSize; ++j) {
            payload[j + 1] = data[offset + j];
        }

        frame.setPayload(payload);
        sendToCL2000(frame); // Placeholder for actual sending logic
        //qDebug() << "Sent subsequent frame" << i << "with payload:" << payload.toHex();


    }


}


void CanHandler::toggleLoCellVoltageSend()
{

    areWeSendingLoCellVoltage = !areWeSendingLoCellVoltage;

}

void CanHandler::toggleChargerTempSend()
{

    areWeSendingChargerTemp = !areWeSendingChargerTemp;

}

void CanHandler::toggleHiCellVoltageSend()
{

    areWeSendinHiCellVoltage = !areWeSendinHiCellVoltage;

}

void CanHandler::startCAN(){

    port = new QSerialPort("COM9");

    if(!port->open(QSerialPort::OpenModeFlag::ReadWrite)){
        qDebug() << "Failed to open port" << port->portName() << ", error:" << port->errorString();

    }

    const QString pluginName = "virtualcan"; //clx000can

    canDevice = QCanBus::instance()->createDevice(pluginName, "can0");
    if (canDevice && canDevice->connectDevice()) {
        qDebug() << "Connected to CAN bus";
        //connect(canDevice, &QCanBusDevice::framesReceived, this, &CanHandler::readAndProcessCANpodatke);
    } else {
        qDebug() << "Failed to connect to CAN bus";
    }

}

CanHandler::~CanHandler(){

    port -> close();
    canDevice -> disconnectDevice();
    delete port;

}
