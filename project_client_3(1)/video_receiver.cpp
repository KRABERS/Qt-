// video_receiver.cpp
#include "video_receiver.h"
#include <QLabel>
#include <QImage>

VideoReceiver::VideoReceiver(int roomNumber, QWidget* parent)
    : m_roomNumber(roomNumber), m_parent(parent) {
    m_udpSocket = new QUdpSocket(this);
    QString udpAddress = "127.0.0." + QString::number(roomNumber);
    bool success = m_udpSocket->bind(QHostAddress(udpAddress), 8080);
    if (!success) {
        qCritical() << "Failed to bind to UDP address" << udpAddress;
        return;
    }

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &VideoReceiver::handleReadyRead);
}

void VideoReceiver::handleReadyRead() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        decodeAndDisplayFrame(datagram);
    }
}

void VideoReceiver::decodeAndDisplayFrame(const QByteArray& frame) {
    QImage image;
    image.loadFromData(frame, "BMP");

    if (m_parent) {
        QLabel* label = m_parent->findChild<QLabel*>("videoLabel");
        if (label) {
            label->setPixmap(QPixmap::fromImage(image));
        }
    }
}
