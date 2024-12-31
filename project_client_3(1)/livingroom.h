#ifndef LIVINGROOM_H
#define LIVINGROOM_H

#include <QWidget>
#include <QUdpSocket>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <video_receiver.h>
#include <videostreamer.h>
#include <QUdpSocket>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QLabel>


namespace Ui {
class LivingRoom;
}

class LivingRoom : public QWidget
{
    Q_OBJECT

public:
    explicit LivingRoom(const QString& name, const QString& ownername, bool isowner, const QString& createroomid, QWidget *parent = nullptr);
    ~LivingRoom();

public slots:
    void init_window();  // 扩展界面
    void add_name(const QString& name);  // 添加名字
    void processCapturedImage(int requestId, const QImage& img); // 处理捕获的图像



public slots:
    void startlivingbtn_clicked();   // 点击开启直播的槽函数
    void processPendingDatagrams();

private:
    Ui::LivingRoom *ui;
    QString m_name;
    QString m_ownername;
    bool m_isowner;
    QString m_createroom_id;
    QUdpSocket *m_udpSocket; // UDP套接字
    QCamera *m_camera;       // 摄像头
    QCameraViewfinder *m_viewfinder; // 预览窗口
    QCameraImageCapture *m_imageCapture; // 图像捕捉
    QVBoxLayout *m_layout;   // 布局
    void startVideoCapture(); // 开始视频捕捉
    void stopVideoCapture();  // 停止视频捕捉

    QVideoWidget *m_videoWidget;
    QMediaPlayer *m_mediaPlayer;
    QLabel * m_label_camera;
    void handleVideoData(const QString& roomId, const QByteArray& data);
};

#endif // LIVINGROOM_H
//#ifndef LIVINGROOM_H
//#define LIVINGROOM_H

//#include <QWidget>
//#include <QUdpSocket>
//#include <QVideoWidget>
//#include <QMediaPlayer>
//#include <QVBoxLayout>
//#include <QPushButton>
//#include <QListWidget>
//#include "pack.h"

//class LivingRoom : public QWidget
//{
//    Q_OBJECT

//public:
//    LivingRoom(const QString &username, const QString &ownername, bool isOwner, const QString &roomId, QWidget *parent = nullptr);
//    ~LivingRoom();

//    void add_name(const QString &name);

//private slots:
//    void processPendingDatagrams();
//    void startLive();
//    void stopLive();

//private:
//    QUdpSocket *m_udpSocket;
//    QVideoWidget *m_videoWidget;
//    QMediaPlayer *m_mediaPlayer;
//    QString m_roomId;
//    QString m_username;
//    QString m_ownername;
//    bool m_isOwner;
//    QPushButton *m_startButton;
//    QPushButton *m_stopButton;
//    QListWidget *m_userList;
//};

//#endif // LIVINGROOM_H
