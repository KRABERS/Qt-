#include "udpreceiver.h"
#include <QHostAddress>
#include <QDebug>

UdpReceiver::UdpReceiver(QObject *parent) : QObject(parent), udpSocket(new QUdpSocket(this)) {
    // 绑定到本地地址和端口
    if (!udpSocket->bind(QHostAddress::LocalHost, 5005)) {
        qDebug() << "无法绑定到端口";
        return;
    }

    // 连接 readyRead 信号到处理数据的槽
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpReceiver::processPendingDatagrams);
}

void UdpReceiver::setForwardAddress(const QHostAddress &address, quint16 port) {
    forwardAddress = address;
    forwardPort = port;
}


void UdpReceiver::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        // 读取数据报文
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // 处理接收到的数据
        qDebug() << "Received datagram from" << sender.toString() << ":" << senderPort;
        qDebug() << "Data:" << datagram;

        // 在这里可以对 datagram 进行进一步处理，比如转换为 QImage
    }
}
