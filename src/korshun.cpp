#include "korshun.h"
#include <QDebug>

Korshun::Korshun() : Device("Коршун (ПРМ)") {}

bool Korshun::parse(const QByteArray& data){
    m_parameters.clear();

    if (data.size() < 2){
        qDebug() << "Сообщение слишком короткое";
        return false;
    }

    if (data[0] != START_BYTE || data[data.size() - 1] != END_BYTE){
        qDebug() << " Неверные признаки начала или конца сообщения";
        return false;
    }

    QByteArray payload = data.mid(1, data.size() - 2);

    if (payload.isEmpty()){
        qDebug() << "Нет данных в сообщении";
        return false;
    }

    int pos = 0;

    while (pos < payload.size())
    {
        if (pos + 1 >= payload.size())
        {
            qDebug() << "Неполный параметр";
            return false;
        }

        char paramCode = payload[pos];
        pos++;

        QString paramValue;
        int valueLength = 0;
        QString paramName;

        switch (paramCode)
        {
        case 'F':
            valueLength = 4;
            paramName = "Частота";
            break;
        case 'S':
            valueLength = 1;
            paramName = "Чувствительность";
            break;
        case 'C':
            valueLength = 2;
            paramName = "Рабочий канал";
            break;
        default:
            qDebug() << "Неизвестный код параметра:" << paramCode;
            return false;
        }

        if  (pos + valueLength > payload.size())
        {
            qDebug() << "Недостаточно данных для параметра";
            return false;
        }

        paramValue = QString::fromLatin1(payload.mid(pos, valueLength));
        pos += valueLength;

        bool valid = false;
        switch (paramCode)
        {
        case 'F':
            valid = validateFrequency(paramValue);
            if (valid)
                m_parameters[paramName] = paramValue + " Гц";
            break;
        case 'S':
            valid = validateSensitivity(paramValue);
            if (valid)
            {
                if (paramValue == "1") m_parameters[paramName] = "минимальная";
                else if (paramValue == "2") m_parameters[paramName] = "средняя";
                else if (paramValue == "3") m_parameters[paramName] = "максимальная";
            }
            break;
        case 'C':
            valid = validateChannel(paramValue);
            if (valid)
                m_parameters[paramName] = paramValue;
            break;
        }
        if (!valid)
        {
            qDebug() << "Неверное значение параметра";
            return false;
        }
    }
    return !m_parameters.isEmpty();
}

bool Korshun::validateFrequency(const QString& value)
{
    bool ok;
    int freq = value.toInt(&ok);
    if (!ok) return false;

    return (freq >= 100 && freq <= 5000 && freq % 10 == 0);
}

bool Korshun::validateSensitivity(const QString& value)
{
    return (value == "1" || value == "2" || value == "3");
}

bool Korshun::validateChannel(const QString& value)
{
    bool ok;
    int channel = value.toInt(&ok);
    if (!ok) return false;

    return (channel >= 1 && channel <= 16);
}