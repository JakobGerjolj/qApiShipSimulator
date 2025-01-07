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

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":10,"v":%1})";

    QString modified_string = string_message.arg(highCellVoltage);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendinHiCellVoltage){

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

void CanHandler::sendLoCellVoltage()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":9,"v":%1})";

    QString modified_string = string_message.arg(lowCellVoltage);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingLoCellVoltage){

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

void CanHandler::sendChargerTemp()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":4,"v":%1})";

    QString modified_string = string_message.arg(chargerTemp);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingChargerTemp){

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

void CanHandler::sendChargerVoltage()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":5,"v":%1})";

    QString modified_string = string_message.arg(chargerVoltage);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingChargerVoltage){

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




void CanHandler::sendChargerCurrent()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":6,"v":%1})";

    QString modified_string = string_message.arg(chargerCurrent);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingChargerCurrent){

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

void CanHandler::sendChargerInVoltage()
{

    //{"cmd":"svv","sid":1,"vid":1,"iid":0,"v":1234} example of data with instance

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":7,"iid":%1,"v":%2})";

    QString modified_string = string_message.arg(chargerInVoltageInstance).arg(chargerInVoltage);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingChargerInVoltage){

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

void CanHandler::sendChargerInCurrent()
{

    QString string_message = R"({"cmd":"svv","sid":1028,"vid":8,"iid":%1,"v":%2})";

    QString modified_string = string_message.arg(chargerInCurrentInstance).arg(chargerInCurrent);

    QVector<QVector<uint8_t>> vectorToSendToCAN;

    Encoder encoder1{modified_string, 0x19};

    vectorToSendToCAN = encoder1.createPayloadArray();

    int counter = 0;

    if(areWeSendingChargerInCurrent){

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
