#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>

class UdpReceiver : public QObject {
    Q_OBJECT

public:
    explicit UdpReceiver(QObject *parent = nullptr);
    void setForwardAddress(const QHostAddress &address, quint16 port);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
    QHostAddress forwardAddress;
    quint16 forwardPort;
};

#endif // UDPRECEIVER_H
