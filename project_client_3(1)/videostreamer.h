// video_streamer.h
#ifndef VIDEO_STREAMER_H
#define VIDEO_STREAMER_H

#include <QUdpSocket>
#include <QImage>
#include <QBuffer>

class VideoStreamer {
public:
    VideoStreamer(int roomNumber, const QString& serverIp = "127.0.0.1", quint16 port = 8080);
    void sendFrame(const QImage& frame);

private:
    int m_roomNumber;
    QString m_serverIp;
    quint16 m_port;
    QUdpSocket* m_udpSocket;
};

#endif // VIDEO_STREAMER_H
