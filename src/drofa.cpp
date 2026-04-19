#include "drofa.h"
#include <QDebug>


Drofa::Drofa() : Device("Дрофа (ПРД") {}

bool Drofa::parse(const QByteArray& data)
{
    m_parameters.clear();

    if (data.size() != 8)
    {
        qDebug() << "Неверный размер сообщения Дрофа (должен быть 8 байт)";
        return false;
    }

    quint16 startCheck = (static_cast<quint8>(data[0]) << 8) | static_cast<quint8>(data[1]);

    if (startCheck != START_SIGN)
    {
        qDebug() << "Неверный признак начала сообщения";
        return false;
    }

    quint16 endCheck = (static_cast<quint8>(data[6]) << 8) | static_cast<quint8>(data[7]);
    if (endCheck != END_SIGN)
    {
        qDebug() << "Неверный признак конца сообщения";
        return false;
    }

    quint16 frequency = (static_cast<quint8>(data[2]) << 8) | static_cast<quint8>(data[3]);

    quint8 mode = static_cast<quint8>(data[4]);

    quint8 power = static_cast<quint8>(data[5]);

    if(!validateFrequency(frequency))
    {
        qDebug() << "Неверное значение частоты";
        return false;
    }

    if (!validateMode(mode))
    {
        qDebug() << "Неверный режим работы";
        return false;
    }

    if (!validatePower(power))
    {
        qDebug() << "Неверное значение мощности";
        return false;
    }

    m_parameters["Частота"] = QString::number(frequency) + " Гц";
    m_parameters["Режим работы"] = modeToString(mode);
    m_parameters["Мощность"] = QString::number(power) + " %";

    return true;

}

bool Drofa::validateFrequency(quint16 freq)
{
    return (freq >= 0x1388 && freq <= 0xC350);
}

bool Drofa::validateMode(quint8 mode)
{
    return (mode == 0x01 || mode == 0x02 || mode == 0x0A);
}

bool Drofa::validatePower(quint8 power)
{
    return (power >= 0x00 && power <= 0x64 && power % 10 == 0);
}

QString Drofa::modeToString(quint8 mode)
{
    switch (mode)
    {
    case 0x01: return "A1";
    case 0x02: return "A2";
    case 0x0A: return "F5R";
    default: return "Неизвестный";
    }
}
