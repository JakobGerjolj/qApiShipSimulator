#ifndef Q_API_ENCODER_H
#define Q_API_ENCODER_H

#include <QString>
#include <QVector>

class Encoder {
public:
    Encoder(const QString &input, quint8 message_id)
        : input_string(input), message_id(message_id) {}


    QByteArray fillZeros(QByteArray &array)
    {
        QByteArray result = array;
        while((result.size() % 6) != 0)
        {
            result.resize(result.size()+1, 0x00);
        }
        return result;
    }

    QVector<QVector<quint8>> createPayloadArray() {
        auto array = input_string.toUtf8();
        quint16 num_of_bytes = array.size();
        uint8_t byte1 = (num_of_bytes >> 8) & 0xFF;
        uint8_t byte2 = num_of_bytes & 0xFF;

        auto arrayWithZeros = fillZeros(array);


        QVector<QVector<quint8>> payloadArray;

        QVector<quint8> header{message_id, 0x00, byte2, byte1, 0x00, 0x00, 0x00, 0x00};
        payloadArray.push_back(header);

        int num_of_messages = 0;
        QVector<quint8> payload;
        for(int i=0; i<arrayWithZeros.size();i++)
        {
            if(i%6 == 0)
            {
                num_of_messages++;
                payload = {message_id,
                           num_of_messages,
                           arrayWithZeros[i],
                           arrayWithZeros[i+1],
                           arrayWithZeros[i+2],
                           arrayWithZeros[i+3],
                           arrayWithZeros[i+4],
                           arrayWithZeros[i+5]};
                payloadArray.push_back(payload);
            }
        }


        return payloadArray;
    }

private:
    QString input_string;
    quint8 message_id;
};
#endif // Q_API_ENCODER_H
