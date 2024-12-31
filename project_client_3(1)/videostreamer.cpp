// video_streamer.cpp
#include "videostreamer.h"
#include <QHostAddress>

VideoStreamer::VideoStreamer(int roomNumber, const QString& serverIp, quint16 port)
    : m_roomNumber(roomNumber), m_serverIp(serverIp), m_port(port) {
    m_udpSocket = new QUdpSocket();
}

void VideoStreamer::sendFrame(const QImage& frame) {
    if (frame.isNull()) return;

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    frame.save(&buffer, "BMP");  // 保存为 BMP 格式

    m_udpSocket->writeDatagram(imageData, QHostAddress(m_serverIp), m_port);
}
