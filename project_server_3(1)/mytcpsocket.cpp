#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QTcpSocket* ptcpsocket):
    m_ptcpsocket(ptcpsocket),
    m_udpSocket(new QUdpSocket(ptcpsocket)),
    m_name(),
    m_createroom_id()
{
}

// 获取 QTcpSocket
QTcpSocket *MyTcpSocket::get_tcpsocket()
{
    return m_ptcpsocket;
}

// 获取 QUdpSocket
QUdpSocket *MyTcpSocket::get_udpSocket()
{
    return m_udpSocket;
}

// 调用 QTcpSocket 的 read 实现读逻辑
qint64 MyTcpSocket::read(char *data, qint64 maxlen)
{
    return m_ptcpsocket->read(data, maxlen);
}

// 调用 QTcpSocket 的 write 实现写逻辑
qint64 MyTcpSocket::write(const char *data, qint64 len)
{
    return m_ptcpsocket->write(data, len);
}

void MyTcpSocket::close()
{
    return m_ptcpsocket->close();
}

// 在服务器端分机套接字中记录客户端的名字
void MyTcpSocket::set_name(const QString &name)
{
    m_name = name;

    return;
}

// 从服务器端分机套接字中返回客户端的名字
const QString &MyTcpSocket::get_name()
{
    return m_name;
}

// 在服务器端分机套接字中记录客户端的创建的房间号
void MyTcpSocket::set_createroom_id(const QString &createroomid)
{
    m_createroom_id = createroomid;

    return;
}

// 从服务器端分机套接字中返回客户端的创建的房间号
const QString &MyTcpSocket::get_createroom_id()
{
    return m_createroom_id;
}

// 在服务器端分机套接字中记录客户端的加入的房间号
void MyTcpSocket::set_joinroom_id(const QString &joinroomid)
{
    m_joinroom_id = joinroomid;

    return;
}

// 从服务器端分机套接字中返回客户端加入的房间号
const QString &MyTcpSocket::get_joinroom_id()
{
    return m_joinroom_id;
}
//#include "mytcpsocket.h"

//MyTcpSocket::MyTcpSocket(QTcpSocket* ptcpsocket, QObject *parent)
//    : QObject(parent), m_ptcpsocket(ptcpsocket), m_udpSocket(new QUdpSocket(this))
//{
//}

//// 获取 QTcpSocket
//QTcpSocket* MyTcpSocket::get_tcpsocket()
//{
//    return m_ptcpsocket;
//}

//// 调用 QTcpSocket 的 read 实现读逻辑
//qint64 MyTcpSocket::read(char *data, qint64 maxlen)
//{
//    return m_ptcpsocket->read(data, maxlen);
//}

//// 调用 QTcpSocket 的 write 实现写逻辑
//qint64 MyTcpSocket::write(const char *data, qint64 len)
//{
//    return m_ptcpsocket->write(data, len);
//}

//// 关闭套接字
//void MyTcpSocket::close()
//{
//    m_ptcpsocket->close();
//    m_udpSocket->close();
//}

//// 获取 QUdpSocket
//QUdpSocket* MyTcpSocket::get_udpSocket()
//{
//    return m_udpSocket;
//}

//// 在服务器端分机套接字中记录客户端的名字
//void MyTcpSocket::set_name(const QString &name)
//{
//    m_name = name;
//}

//// 从服务器端分机套接字中返回客户端的名字
//const QString& MyTcpSocket::get_name()
//{
//    return m_name;
//}

//// 在服务器端分机套接字中记录客户端的创建的房间号
//void MyTcpSocket::set_createroom_id(const QString &createroomid)
//{
//    m_createroom_id = createroomid;
//}

//// 从服务器端分机套接字中返回客户端的创建的房间号
//const QString& MyTcpSocket::get_createroom_id()
//{
//    return m_createroom_id;
//}

//// 在服务器端分机套接字中记录客户端的加入的房间号
//void MyTcpSocket::set_joinroom_id(const QString &joinroomid)
//{
//    m_joinrooom_id = joinroomid;
//}

//// 从服务器端分机套接字中返回客户端加入的房间号
//const QString& MyTcpSocket::get_joinroom_id()
//{
//    return m_joinrooom_id;
//}
