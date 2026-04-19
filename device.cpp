#include "device.h"

Device::Device(const QString& name) : m_name(name)
{
}

QString Device::getStateString() const
{
    QString result = "Устройство: " + m_name + "\n";
    for (auto it = m_parameters.begin(); it != m_parameters.end(); it++)
    {
        result += it.key() + " = " + it.value() + "\n";
    }
    return result;
}
