#ifndef KORSHUN_H
#define KORSHUN_H

#include "device.h"


class Korshun : public Device
{
public:
    Korshun();
    bool parse(const QByteArray& data) override;
private:
    static constexpr char START_BYTE = ':';
    static constexpr char END_BYTE = '!';

    bool validateFrequency(const QString& value);
    bool validateSensitivity(const QString& value);
    bool validateChannel(const QString& value);
};

#endif // KORSHUN_H
