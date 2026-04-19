#ifndef DROFA_H
#define DROFA_H

#include "device.h"


class Drofa : public Device
{
public:
    Drofa();
    bool parse(const QByteArray& data) override;

private:
    static constexpr quint16 START_SIGN = 0xFFFF;
    static constexpr quint16 END_SIGN = 0xEFEF;

    bool validateFrequency(quint16 freq);
    bool validateMode(quint8 mode);
    bool validatePower(quint8 power);

    QString modeToString(quint8 mode);
};

#endif // DROFA_H
