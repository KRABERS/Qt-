#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QUdpSocket>

class MyTcpSocket
{
public:
    MyTcpSocket(QTcpSocket* ptcpsocket);

public:
    QTcpSocket* get_tcpsocket();  // 获取 QTcpSocket
    qint64 read(char *data, qint64 maxlen);  // 调用 QTcpSocket 的 read 实现读逻辑
    qint64 write(const char *data, qint64 len); // 调用 QTcpSocket 的 write 实现写逻辑
    void close();

    QUdpSocket* get_udpSocket(); // 获取 QUdpSocket

public:
    void set_name(const QString& name);  // 在服务器端分机套接字中记录客户端的名字
    const QString& get_name();  // 从服务器端分机套接字中返回客户端的名字
    void set_createroom_id(const QString& createroomid);  // 在服务器端分机套接字中记录客户端的创建的房间号
    const QString &get_createroom_id();  // 从服务器端分机套接字中返回客户端的创建的房间号
    void set_joinroom_id(const QString& joinroomid);  // 在服务器端分机套接字中记录客户端的加入的房间号
    const QString& get_joinroom_id();  // 从服务器端分机套接字中返回客户端加入的房间号

private:
    QTcpSocket* m_ptcpsocket;
    QUdpSocket* m_udpSocket;
    QString m_name;
    QString m_createroom_id;
    QString m_joinroom_id;
};

#endif // MYTCPSOCKET_H
//#ifndef MYTCPSOCKET_H
//#define MYTCPSOCKET_H

//#include <QTcpSocket>
//#include <QUdpSocket>

//class MyTcpSocket : public QObject  // 继承自 QObject
//{
//    Q_OBJECT

//public:
//    MyTcpSocket(QTcpSocket* ptcpsocket, QObject *parent = nullptr);  // 添加 parent 参数

//    QTcpSocket* get_tcpsocket();  // 获取 QTcpSocket
//    qint64 read(char *data, qint64 maxlen);  // 调用 QTcpSocket 的 read 实现读逻辑
//    qint64 write(const char *data, qint64 len); // 调用 QTcpSocket 的 write 实现写逻辑
//    void close();

//    QUdpSocket* get_udpSocket(); // 获取 QUdpSocket

//    void set_name(const QString& name);  // 在服务器端分机套接字中记录客户端的名字
//    const QString& get_name();  // 从服务器端分机套接字中返回客户端的名字
//    void set_createroom_id(const QString& createroomid);  // 在服务器端分机套接字中记录客户端的创建的房间号
//    const QString &get_createroom_id();  // 从服务器端分机套接字中返回客户端的创建的房间号
//    void set_joinroom_id(const QString& joinroomid);  // 在服务器端分机套接字中记录客户端的加入的房间号
//    const QString& get_joinroom_id();  // 从服务器端分机套接字中返回客户端加入的房间号

//private:
//    QTcpSocket* m_ptcpsocket;
//    QUdpSocket* m_udpSocket;  // 添加 UDP 套接字
//    QString m_name;
//    QString m_createroom_id;
//    QString m_joinrooom_id;
//};

//#endif // MYTCPSOCKET_H
