// video_receiver.h
#ifndef VIDEO_RECEIVER_H
#define VIDEO_RECEIVER_H

#include <QUdpSocket>
#include <QWidget>

class VideoReceiver : public QObject {
    Q_OBJECT

public:
    VideoReceiver(int roomNumber, QWidget* parent = nullptr);

private slots:
    void handleReadyRead();

private:
    void decodeAndDisplayFrame(const QByteArray& frame);

    int m_roomNumber;
    QWidget* m_parent;
    QUdpSocket* m_udpSocket;
};

#endif // VIDEO_RECEIVER_H
