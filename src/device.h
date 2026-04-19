#ifndef DEVICE_H
#define DEVICE_H

#include <QMap>
#include <QString>
#include <QByteArray>


class Device
{
public:
    Device(const QString& name);
    virtual ~Device() = default;

    virtual bool parse(const QByteArray& data) = 0;
    QString getStateString() const;
    QString getName() const {return m_name;}

protected:
    QString m_name;
    QMap<QString, QString> m_parameters;
};

#endif // DEVICE_H
