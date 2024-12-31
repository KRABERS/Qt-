#include "livingroom.h"
#include "ui_livingroom.h"
#include <QMessageBox>
#include <QBuffer>
#include <QImageWriter>
#include <QTimer>
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QLabel>


LivingRoom::LivingRoom(const QString& name, const QString& ownername, bool isowner, const QString& createroomid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LivingRoom),
    m_name(name),
    m_ownername(ownername),
    m_isowner(isowner),
    m_createroom_id(createroomid),
    m_udpSocket(new QUdpSocket(this)),
    m_camera(new QCamera(this)),
    m_viewfinder(new QCameraViewfinder(this)),
    m_imageCapture(new QCameraImageCapture(m_camera, this)),
    m_layout(new QVBoxLayout(this)),
    m_videoWidget(new QVideoWidget(this)),
    m_mediaPlayer(new QMediaPlayer(this)),
    m_label_camera(new QLabel(this))

{
    ui->setupUi(this);



//    // 连接信号槽，当有数据到达时处理数据
//    connect(m_udpSocket, &QUdpSocket::readyRead, this, &LivingRoom::processPendingDatagrams);

    // 设置视频播放器和显示组件
    m_mediaPlayer->setVideoOutput(m_videoWidget);

    // 布局视频显示组件
//    QVBoxLayout *layout = new QVBoxLayout(this);
//    layout->addWidget(m_videoWidget);
//    setLayout(layout);

    // 点击开启直播按钮
    connect(ui->pushButton_startliving, &QPushButton::clicked, this, &LivingRoom::startlivingbtn_clicked);

    init_window();

    // 设置摄像头
    m_camera->setViewfinder(m_viewfinder);
    connect(m_imageCapture, &QCameraImageCapture::imageCaptured, this, &LivingRoom::processCapturedImage);

    // 添加预览窗口到布局
    QVBoxLayout *layout = new QVBoxLayout(ui->label_camera);
    layout->addWidget(m_viewfinder);
    ui->label_camera->setLayout(layout);




}

LivingRoom::~LivingRoom()
{
    delete ui;
}

// 扩展界面
void LivingRoom::init_window()
{
    setWindowTitle(m_name);
    ui->label_roomid->setText(m_createroom_id);

    // 创建房间进来
    if(m_isowner)
    {
        ui->listWidget_allusername->addItem(new QListWidgetItem(m_name));
        return;
    }

    // 加入房间进来
//    ui->listWidget_allusername->addItem(new QListWidgetItem(m_ownername));
//    ui->listWidget_allusername->addItem(new QListWidgetItem(m_name));

//    // 设置 UDP 套接字绑定的端口,接受画面
//    QString roomnum_live = "127.0." + m_ownername + m_ownername;
//    m_udpSocket->bind(QHostAddress(roomnum_live), 5005);
//    while (m_udpSocket->hasPendingDatagrams()) {
//                QByteArray datagram;
//                datagram.resize(m_udpSocket->pendingDatagramSize());
//                QHostAddress sender;
//                quint16 senderPort = 5006;

//                // 读取数据报
//                m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

//                // 将接收到的数据转换为QImage
//                QBuffer buffer(&datagram);
//                buffer.open(QIODevice::ReadOnly);
//                QImage image;
//                if (image.loadFromData(datagram, "JPEG")) {
//                    // 更新图像标签
//                    QVBoxLayout *layout = new QVBoxLayout(ui->widget_video);
//                    layout->addWidget(m_viewfinder);
//                    ui->widget_video->setLayout(layout);

//                } else {
//                    qDebug() << "Failed to load image from data.";
//                }
//            }
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray buffer_r;
        buffer_r.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(buffer_r.data(), buffer_r.size(), &sender, &senderPort);

        // 假设数据包的前缀包含房间号
        QString roomId = QString::fromUtf8(buffer_r.left(4));
        buffer_r.remove(0, 4);

        // 根据房间号处理接收到的数据
        handleVideoData(roomId, buffer_r);
    }


    return;
}
void LivingRoom::handleVideoData(const QString &roomId, const QByteArray &data)
{
    // 检查输入参数是否有效
    if (roomId.isEmpty() || data.isEmpty()) {
        qWarning("Invalid room ID or video data.");
        return;
    }

    // 假设房间号为4个字符，解析出房间号和视频数据
    if (data.size() < 4) {
        qWarning("Data size is too small to contain a valid room ID.");
        return;
    }

    // 提取房间号（前4个字符）
    QString receivedRoomId = QString::fromUtf8(data.left(4));

    // 提取视频数据（剩余部分）
    QByteArray videoData = data.mid(4);

    // 验证房间号是否匹配
    if (receivedRoomId != roomId) {
        qWarning("Received room ID does not match the expected room ID.");
        return;
    }

    // 处理视频数据（这里只是一个示例，具体处理逻辑取决于实际需求）
    qDebug() << "Handling video data for room:" << roomId;

    // 将视频数据显示在界面上
    QImage img;
    if (img.loadFromData(videoData, "JPEG")) {
        QPixmap pixmap = QPixmap::fromImage(img);

        // 假设 widget_video 是一个自定义的 WidgetVideo 类型的对象
        QVBoxLayout *layout = new QVBoxLayout(this);
        m_label_camera = new QLabel(this);
        m_label_camera->setAlignment(Qt::AlignCenter);
        layout->addWidget(m_label_camera);

        qDebug() << "Video frame displayed successfully.";
    } else {
        qWarning("Failed to load image from video data.");
    }


}
 // 添加名字
void LivingRoom::add_name(const QString& name)
{
     ui->listWidget_allusername->addItem(new QListWidgetItem(name));

     return;
}

// 点击开启直播的槽函数
void LivingRoom::startlivingbtn_clicked()
{
    if(!m_isowner)
    {
        QMessageBox::warning(this, "开启直播", "不是主播无法开启直播");

        return;
    }

    // 开启直播逻辑（使用UDP做视频音频传输，也可以使用流媒体协议）
    startVideoCapture();

}

void LivingRoom:: startVideoCapture()
{
    m_camera->start();
    m_imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    m_imageCapture->capture();
}

void LivingRoom::stopVideoCapture()
{
    m_camera->stop();

}

void LivingRoom::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "JPEG");

    QByteArray videoData;
    //发给服务器直播画面


        QString roomId = m_ownername;
        // 检查输入参数是否有效
        if (roomId.isEmpty() || videoData.isEmpty()) {
            qWarning("Invalid room ID or video data.");
            return;
        }

        // 创建一个新的数据包，包含房间号和视频数据
        QByteArray buffer_p;
        buffer_p.append(roomId.toUtf8());  // 添加房间号（假设房间号为4个字符）
        buffer_p.append(videoData);         // 添加视频数据

        // 发送数据包到服务器或广播地址
        if (m_udpSocket) {
            // 这里假设服务器的地址和端口是已知的
            QHostAddress serverAddress = QHostAddress("224.0."+roomId+roomId);  // 替换为实际服务器地址
            quint16 serverPort = 5006;  // 替换为实际服务器端口

            m_udpSocket->writeDatagram(buffer_p,serverAddress,serverPort);
        } else {
            qWarning("UDP socket is not initialized.");
        }


    // 捕捉下一帧
    QTimer::singleShot(30, this, [this](){ m_imageCapture->capture(); });
}
// livingroom.cpp
//#include "livingroom.h"
//#include "pack.h"
//#include <QBuffer>
//#include <QDebug>

//LivingRoom::LivingRoom(const QString &username, const QString &ownername, bool isOwner, const QString &roomId, QWidget *parent)
//    : QWidget(parent),
//      m_udpSocket(new QUdpSocket(this)),
//      m_videoWidget(new QVideoWidget(this)),
//      m_mediaPlayer(new QMediaPlayer(this)),
//      m_roomId(roomId),
//      m_username(username),
//      m_ownername(ownername),
//      m_isOwner(isOwner)
//{
//    // 设置 UDP 套接字绑定的端口
//    m_udpSocket->bind(QHostAddress::Any, 5005);

//    // 连接信号槽，当有数据到达时处理数据
//    connect(m_udpSocket, &QUdpSocket::readyRead, this, &LivingRoom::processPendingDatagrams);

//    // 设置视频播放器和显示组件
//    m_mediaPlayer->setVideoOutput(m_videoWidget);

//    // 布局视频显示组件
//    QVBoxLayout *layout = new QVBoxLayout(this);
//    layout->addWidget(m_videoWidget);

//    m_userList = new QListWidget(this);
//    layout->addWidget(m_userList);

//    m_startButton = new QPushButton("开启直播", this);
//    layout->addWidget(m_startButton);

//    m_stopButton = new QPushButton("关闭直播", this);
//    layout->addWidget(m_stopButton);

//    connect(m_startButton, &QPushButton::clicked, this, &LivingRoom::startLive);
//    connect(m_stopButton, &QPushButton::clicked, this, &LivingRoom::stopLive);

//    setLayout(layout);
//}

//LivingRoom::~LivingRoom()
//{
//}

//void LivingRoom::add_name(const QString &name)
//{
//    // Add the name to the list of users in the room
//    if (m_userList) {
//        m_userList->addItem(name);
//    }
//}

//void LivingRoom::processPendingDatagrams()
//{
//    while (m_udpSocket->hasPendingDatagrams()) {
//        QByteArray datagram;
//        datagram.resize(m_udpSocket->pendingDatagramSize());
//        m_udpSocket->readDatagram(datagram.data(), datagram.size());

//        // 处理接收到的数据包
//        QBuffer *buffer = new QBuffer(this);
//        buffer->setData(datagram);
//        buffer->open(QIODevice::ReadOnly);
//        m_mediaPlayer->setMedia(QMediaContent(), buffer);
//        m_mediaPlayer->play();
//    }
//}

//void LivingRoom::startLive()
//{
//    // 启动摄像头并开始直播
//    // 这里假设有一个函数 startCamera() 来启动摄像头
//    // startCamera();
//    qDebug() << "直播已开启";
//}

//void LivingRoom::stopLive()
//{
//    // 关闭摄像头并停止直播
//    // 这里假设有一个函数 stopCamera() 来关闭摄像头
//    // stopCamera();
//    qDebug() << "直播已关闭";

//    // 通知服务器移除直播间
//    Pack pack;
//    pack.set_type(TYPE_REMOVE_ROOM);
//    pack.append(m_roomId);
//    // 假设有一个 TCP 连接 m_socket 用于发送请求
//    // m_socket->write(pack.data(), pack.size());

//    // 返回房间号列表界面
//    close();
//}
