////#include "widget.h"
////#include "ui_widget.h"
////#include "dbhelper.h"
////#include <QMessageBox>
////#include <QTcpSocket>
////#include <QDebug>
////#include "pack.h"
////#include "mytcpsocket.h"

////Widget::Widget(QWidget *parent)
////    : QWidget(parent)
////    , ui(new Ui::Widget)
////    , m_ptcpserver(new QTcpServer)
////    , m_pmytcpsocket_vec()
////    , m_createromm_id(0)
////{
////    ui->setupUi(this);

////    init_db(); // 初始化数据库（调用 DbHelper 的 init_db）
////    init_network(); // 初始化网络（监听）

////    // 绑定UDP端口，用于接收视频数据
////    m_udpSocket->bind(QHostAddress::Any, 5005);
////    connect(m_udpSocket, &QUdpSocket::readyRead, this, &Widget::processVideoData);

////}

////Widget::~Widget()
////{
////    delete ui;
////}

////// 初始化数据库（调用 DbHelper 的 init_db）
////void Widget::init_db()
////{
////    DbHelper::get_instance().init_db();

////    return;
////}

////// 初始化网络（监听）
////void Widget::init_network()
////{
////    if(!m_ptcpserver->listen(QHostAddress::Any, 8077))
////    {
////        QMessageBox::warning(this, "监听", "监听端口失败");
////        exit(1);
////    }

////    // 客户端连接到来并成功
////    connect(m_ptcpserver, &QTcpServer::newConnection, this, &Widget::new_connection);

////    return;
////}

////// 根据 QTcpSocket 从 vector 里面找到 MyTcpSocket
////MyTcpSocket *Widget::find_mytcpsocket(QTcpSocket *ptcpsocket)
////{
////    for(auto pmytcpsocket: m_pmytcpsocket_vec)
////    {
////        if(pmytcpsocket->get_tcpsocket() == ptcpsocket)
////        {
////            return pmytcpsocket;
////        }
////    }

////    return NULL;
////}

////// 新的连接到来并成功关联的槽函数
////void Widget::new_connection()
////{
////    // 取出和客户端通信的套接字
////    MyTcpSocket* pmytcpsocket = new MyTcpSocket(m_ptcpserver->nextPendingConnection());
////    m_pmytcpsocket_vec.push_back(pmytcpsocket);

////    // 客户端信息到来
////    connect(pmytcpsocket->get_tcpsocket(), &QTcpSocket::readyRead, this, &Widget::ready_read);

////    // 客户端套接字关闭
////    connect(pmytcpsocket->get_tcpsocket(), &QTcpSocket::disconnected, this, &Widget::dis_connected);
////}

////// 客户端信息到来触发的槽函数
////void Widget::ready_read()
////{
////    // 根据 QTcpSocket 找到 MyTcpSocket
////    MyTcpSocket* pmytcpsocket = find_mytcpsocket((QTcpSocket*)sender());

////    // 用pack包接收客户端发来的应用层协议包，然后开始解析
////    while (true) // 可能有多个应用层协议包发来
////    {
////        Pack pack;
////        int size = 0;

////        // 先解析包的长度值（4个字节）
////        int rst = pmytcpsocket->read((char*)&size, 4);
////        if(4 != rst)
////        {
////            break;
////        }

////        // 再解析包的长度后面的的字节（包总长度 - 4)
////        rst = pmytcpsocket->read(pack.data() + 4, size - 4);
////        if(size - 4 != rst)
////        {
////            break;
////        }

////        // 根据包的类型开始处理业务逻辑
////        switch ((Type)pack.get_type())
////        {
////        case TYPE_REGISTER:
////            user_register(pmytcpsocket, pack);
////            break;

////        case TYPE_LOGIN:
////            user_login(pmytcpsocket, pack);
////            break;

////        case TYPE_CREATE_ROOM:
////            user_createroom(pmytcpsocket, pack);
////            break;

////        case TYPE_FLUSH_ROOM:
////            user_flushroom(pmytcpsocket, pack);
////            break;

////        case TYPE_JOIN_ROOM:
////            user_joinroom(pmytcpsocket, pack);
////            break;
////        }
////    }

////    return;
////}

////// 客户端套接字关闭的槽函数
////void Widget::dis_connected()
////{
////    // 根据 QTcpSocket 找到 MyTcpSocket
////    MyTcpSocket* pmytcpsocket = find_mytcpsocket((QTcpSocket*)sender());

////    // 从 vector 里面移除和客户端通信的分机套接字
////    m_pmytcpsocket_vec.removeOne(pmytcpsocket);

////    // 归还该分机套接字给操作系统
////    pmytcpsocket->close();

////    return;
////}

////void Widget::processVideoData()
////{
////    while (m_udpSocket->hasPendingDatagrams()) {
////           QByteArray buffer;
////           buffer.resize(m_udpSocket->pendingDatagramSize());
////           QHostAddress sender;
////           quint16 senderPort;

////           m_udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

////           // 转发给所有其他客户端
////           for (auto pmytcpsocket : m_pmytcpsocket_vec) {
////               if (pmytcpsocket->get_udpSocket() != m_udpSocket) {
////                   pmytcpsocket->get_udpSocket()->writeDatagram(buffer, QHostAddress::Broadcast, 5005);
////               }
////           }
////       }
////}

////// 用户注册功能(调用 DbHelper的 user_register）
////void Widget::user_register(MyTcpSocket* pmytcpsocket, Pack& pack)
////{
////    QStringList strlist = pack.get_data();  // 取出包内容

////    QString name = strlist[0];  // 取出姓名
////    QString pwd = strlist[1];  // 取出密码
////    QString strConfirmPwd = strlist[2];

////    Register_State register_ret = DbHelper::get_instance().user_register(name, pwd, strConfirmPwd);

////    // 构建服务器返回给客户端的包
////    pack.clear();

////    pack.set_type(TYPE_REGISTER);
////    pack.set_register_state(register_ret);

////    pmytcpsocket->write(pack.data(), pack.size());

////    return;
////}

////void Widget::user_login(MyTcpSocket* pmytcpsocket, Pack& pack)
////{
////    QStringList strlist = pack.get_data();  // 取出包内容

////    QString name = strlist[0];  // 取出姓名
////    QString pwd = strlist[1];   // 取出密码

////    Login_State login_ret = DbHelper::get_instance().user_login(name, pwd);

////    // 客户端登录成功，服务器分机套接字里面记录客户的名字
////    if(SUCCESS_LOGIN == login_ret)
////    {
////        // 数据库中判断登录成功后，我们判断该用户是否已经在登录状态
////        for(auto pothermytcpsocket: m_pmytcpsocket_vec)
////        {
////            if(pothermytcpsocket->get_name() == name)
////            {
////                login_ret = ERR_RELOGIN_LOGIN;
////                break;
////            }
////        }

////        if(SUCCESS_LOGIN == login_ret)
////        {
////            pmytcpsocket->set_name(name);
////        }
////    }

////    // 构建服务器返回给客户端的包
////    pack.clear();

////    pack.set_type(TYPE_LOGIN);
////    pack.set_lgoin_state(login_ret);

////    pmytcpsocket->write(pack.data(), pack.size());

////    return;
////}

////// 收到用户请求创建直播间
////void Widget::user_createroom(MyTcpSocket* pmytcpsocket, Pack& pack)
////{
////    pack.set_type(TYPE_CREATE_ROOM);
////    pack.append(QString::number(++m_createromm_id));

////    // 给创建房间者返回直播间号
////    pmytcpsocket->write(pack.data(), pack.size());

////    // 和该客户端通信的分机套接字保存房间号
////    pmytcpsocket->set_createroom_id(QString::number(m_createromm_id));

////    // 重新构建协议包
////    pack.clear();
////    pack.set_type(TYPE_GET_NEW_ROOM);
////    pack.append(QString::number(m_createromm_id));

////    // 给已经登录成功的其它所有用户返回直播间号
////    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
////    {
////        if((!pothermytcpsocket->get_name().isEmpty()) && (pothermytcpsocket != pmytcpsocket))
////        {
////            pothermytcpsocket->write(pack.data(), pack.size());
////        }
////    }

////    return;
////}

////// 收到用户请求拉取直播间号
////void Widget::user_flushroom(MyTcpSocket *pmytcpsocket, Pack &pack)
////{
////    // 重新构建协议包
////    pack.clear();
////    pack.set_type(TYPE_FLUSH_ROOM);

////    // 获取所有主播的直播间号
////    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
////    {
////        if(!pothermytcpsocket->get_createroom_id().isEmpty())
////        {
////            pack.append(pothermytcpsocket->get_createroom_id());
////        }
////    }

////    pmytcpsocket->write(pack.data(), pack.size());

////    return;
////}

////// 收到用户请求加入直播间
////void Widget::user_joinroom(MyTcpSocket *pmytcpsocket, Pack &pack)
////{
////    QStringList strlist = pack.get_data();  // 取出包内容
////    QString joinroomid = strlist[0];  // 取出要加入的直播间号

////    QString ownername;

////    // 给房主和同直播间内其它用户发送加入者的姓名
////    // 重新构建协议包
////    pack.clear();
////    pack.set_type(TYPE_GET_NEW_USER);
////    pack.append(pmytcpsocket->get_name());

////    // 根据创建者的房间号找到房间拥有者
////    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
////    {
////        if((pothermytcpsocket->get_createroom_id() == joinroomid) ||
////                (pothermytcpsocket->get_joinroom_id() == joinroomid))
////        {
////            pothermytcpsocket->write(pack.data(), pack.size());  // 给房主或通直播内用户发送加入者的姓名
////            if(pothermytcpsocket->get_createroom_id() == joinroomid)
////            {
////                ownername = pothermytcpsocket->get_name();  // 取出房主的姓名
////            }
////        }
////    }

////    // 给加入者发送房主的姓名和同直播间内其他用户的姓名
////    // 重新构建协议包
////    pack.clear();
////    pack.set_type(TYPE_JOIN_ROOM);
////    pack.append(ownername);

////    // 找到同直播间内其他用户的名字
////    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
////    {
////        if(pothermytcpsocket->get_joinroom_id() == joinroomid)
////        {
////            pack.append(pothermytcpsocket->get_name());
////        }
////    }

////    pmytcpsocket->write(pack.data(), pack.size());  // 给加入者发送房主的姓名和同直播间内其他用户的姓名



////    pmytcpsocket->set_joinroom_id(joinroomid);

////    return;
////}

#include "widget.h"
#include "ui_widget.h"
#include "dbhelper.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>
#include "pack.h"
#include "mytcpsocket.h"
#include <QString>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_ptcpserver(new QTcpServer(this))
    , m_pmytcpsocket_vec()
    , m_createromm_id(0)
    , m_udpSocket(new QUdpSocket(this)) // 初始化UDP套接字
{
    ui->setupUi(this);

    init_db(); // 初始化数据库（调用 DbHelper 的 init_db）
    init_network(); // 初始化网络（监听）

    // 绑定UDP端口，用于接收视频数据
    m_udpSocket->bind(QHostAddress::Any, 5005);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &Widget::processVideoData);
}

Widget::~Widget()
{
    delete ui;
}

// 初始化数据库（调用 DbHelper 的 init_db）
void Widget::init_db()
{
    DbHelper::get_instance().init_db();
}

// 初始化网络（监听）
void Widget::init_network()
{
    if(!m_ptcpserver->listen(QHostAddress::Any, 8077))
    {
        QMessageBox::warning(this, "监听", "监听端口失败");
        exit(1);
    }

    // 客户端连接到来并成功
    connect(m_ptcpserver, &QTcpServer::newConnection, this, &Widget::new_connection);
}

// 根据 QTcpSocket 从 vector 里面找到 MyTcpSocket
MyTcpSocket *Widget::find_mytcpsocket(QTcpSocket *ptcpsocket)
{
    for(auto pmytcpsocket: m_pmytcpsocket_vec)
    {
        if(pmytcpsocket->get_tcpsocket() == ptcpsocket)
        {
            return pmytcpsocket;
        }
    }
    return nullptr;
}

// 新的连接到来并成功关联的槽函数
void Widget::new_connection()
{
    // 取出和客户端通信的套接字
    MyTcpSocket* pmytcpsocket = new MyTcpSocket(m_ptcpserver->nextPendingConnection());
    m_pmytcpsocket_vec.push_back(pmytcpsocket);

    // 客户端信息到来
    connect(pmytcpsocket->get_tcpsocket(), &QTcpSocket::readyRead, this, &Widget::ready_read);
    // 客户端套接字关闭
    connect(pmytcpsocket->get_tcpsocket(), &QTcpSocket::disconnected, this, &Widget::dis_connected);
}

// 客户端信息到来触发的槽函数
void Widget::ready_read()
{
    // 根据 QTcpSocket 找到 MyTcpSocket
    MyTcpSocket* pmytcpsocket = find_mytcpsocket((QTcpSocket*)sender());
    if (!pmytcpsocket) return;

    // 用pack包接收客户端发来的应用层协议包，然后开始解析
    while (true) // 可能有多个应用层协议包发来
    {
        Pack pack;
        int size = 0;

        // 先解析包的长度值（4个字节）
        int rst = pmytcpsocket->read((char*)&size, 4);
        if(4 != rst)
        {
            break;
        }

        // 再解析包的长度后面的的字节（包总长度 - 4)
        rst = pmytcpsocket->read(pack.data() + 4, size - 4);
        if(size - 4 != rst)
        {
            break;
        }

        // 根据包的类型开始处理业务逻辑
        switch ((Type)pack.get_type())
        {
        case TYPE_REGISTER:
            user_register(pmytcpsocket, pack);
            break;
        case TYPE_LOGIN:
            user_login(pmytcpsocket, pack);
            break;
        case TYPE_CREATE_ROOM:
            user_createroom(pmytcpsocket, pack);
            break;
        case TYPE_FLUSH_ROOM:
            user_flushroom(pmytcpsocket, pack);
            break;
        case TYPE_JOIN_ROOM:
            user_joinroom(pmytcpsocket, pack);
            break;
        }
    }
}

// 客户端套接字关闭的槽函数
void Widget::dis_connected()
{
    // 根据 QTcpSocket 找到 MyTcpSocket
    MyTcpSocket* pmytcpsocket = find_mytcpsocket((QTcpSocket*)sender());
    if (!pmytcpsocket) return;

    // 从 vector 里面移除和客户端通信的分机套接字
    m_pmytcpsocket_vec.removeOne(pmytcpsocket);

    // 归还该分机套接字给操作系统
    pmytcpsocket->close();
}

// 用户注册功能(调用 DbHelper的 user_register）
void Widget::user_register(MyTcpSocket* pmytcpsocket, Pack& pack)
{
    QStringList strlist = pack.get_data();  // 取出包内容
    QString name = strlist[0];  // 取出姓名
    QString pwd = strlist[1];  // 取出密码
    QString strConfirmPwd = strlist[2];

    Register_State register_ret = DbHelper::get_instance().user_register(name, pwd, strConfirmPwd);

    // 构建服务器返回给客户端的包
    pack.clear();
    pack.set_type(TYPE_REGISTER);
    pack.set_register_state(register_ret);

    pmytcpsocket->write(pack.data(), pack.size());
}

void Widget::user_login(MyTcpSocket* pmytcpsocket, Pack& pack)
{
    QStringList strlist = pack.get_data();  // 取出包内容
    QString name = strlist[0];  // 取出姓名
    QString pwd = strlist[1];   // 取出密码

    Login_State login_ret = DbHelper::get_instance().user_login(name, pwd);

    // 客户端登录成功，服务器分机套接字里面记录客户的名字
    if(SUCCESS_LOGIN == login_ret)
    {
        // 数据库中判断登录成功后，我们判断该用户是否已经在登录状态
        for(auto pothermytcpsocket: m_pmytcpsocket_vec)
        {
            if(pothermytcpsocket->get_name() == name)
            {
                login_ret = ERR_RELOGIN_LOGIN;
                break;
            }
        }

        if(SUCCESS_LOGIN == login_ret)
        {
            pmytcpsocket->set_name(name);
        }
    }

    // 构建服务器返回给客户端的包
    pack.clear();
    pack.set_type(TYPE_LOGIN);
    pack.set_lgoin_state(login_ret);

    pmytcpsocket->write(pack.data(), pack.size());
}

// 收到用户请求创建直播间
void Widget::user_createroom(MyTcpSocket* pmytcpsocket, Pack& pack)
{
    pack.set_type(TYPE_CREATE_ROOM);
    pack.append(QString::number(++m_createromm_id));

    // 给创建房间者返回直播间号
    pmytcpsocket->write(pack.data(), pack.size());

    // 和该客户端通信的分机套接字保存房间号
    pmytcpsocket->set_createroom_id(QString::number(m_createromm_id));

    //接受直播间数据
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        // 读取数据报
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    }


    // 重新构建协议包
    pack.clear();
    pack.set_type(TYPE_GET_NEW_ROOM);
    pack.append(QString::number(m_createromm_id));

    // 给已经登录成功的其它所有用户返回直播间号
    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
    {
        if((!pothermytcpsocket->get_name().isEmpty()) && (pothermytcpsocket != pmytcpsocket))
        {
            pothermytcpsocket->write(pack.data(), pack.size());
        }
    }
}

// 收到用户请求拉取直播间号
void Widget::user_flushroom(MyTcpSocket *pmytcpsocket, Pack &pack)
{
    // 重新构建协议包
    pack.clear();
    pack.set_type(TYPE_FLUSH_ROOM);

    // 获取所有主播的直播间号
    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
    {
        if(!pothermytcpsocket->get_createroom_id().isEmpty())
        {
            pack.append(pothermytcpsocket->get_createroom_id());
        }
    }

    pmytcpsocket->write(pack.data(), pack.size());
}

// 收到用户请求加入直播间
void Widget::user_joinroom(MyTcpSocket *pmytcpsocket, Pack &pack)
{
    QStringList strlist = pack.get_data();  // 取出包内容
    QString joinroomid = strlist[0];  // 取出要加入的直播间号

    QString ownername;

    // 给房主和同直播间内其它用户发送加入者的姓名
    // 重新构建协议包
    pack.clear();
    pack.set_type(TYPE_GET_NEW_USER);
    pack.append(pmytcpsocket->get_name());

    // 根据创建者的房间号找到房间拥有者
    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
    {
        if((pothermytcpsocket->get_createroom_id() == joinroomid) ||
                (pothermytcpsocket->get_joinroom_id() == joinroomid))
        {
            pothermytcpsocket->write(pack.data(), pack.size());  // 给房主或同直播内用户发送加入者的姓名
            if(pothermytcpsocket->get_createroom_id() == joinroomid)
            {
                ownername = pothermytcpsocket->get_name();  // 取出房主的姓名
            }
        }
    }

    // 给加入者发送房主的姓名和同直播间内其他用户的姓名
    // 重新构建协议包
    pack.clear();
    pack.set_type(TYPE_JOIN_ROOM);
    pack.append(ownername);

    // 找到同直播间内其他用户的名字
    for(auto pothermytcpsocket: m_pmytcpsocket_vec)
    {
        if(pothermytcpsocket->get_joinroom_id() == joinroomid)
        {
            pack.append(pothermytcpsocket->get_name());
        }
    }

    pmytcpsocket->write(pack.data(), pack.size());  // 给加入者发送房主的姓名和同直播间内其他用户的姓名

    pmytcpsocket->set_joinroom_id(joinroomid);

    // 更新房间的观众列表
    m_roomMap[joinroomid].insert(pmytcpsocket);


//    //给加入者发送直播内容数据

//    QString joiner_addr ="127.0." + QString(m_createromm_id + m_createromm_id) ;
//    for (auto it = joiner_addr.begin(); it != joiner_addr.end(); ++it) {
//        //                    const QHostAddress &clientAddress = ();
//        quint16 clientPort = 5005;

//        // 不要将数据回传给原始发送者
//        if (joiner_addr == m_createromm_id && clientPort == m_createromm_id) {
//            continue;

//        }
//    }
}
        // 处理视频数据
        void Widget::processVideoData()
        {
            while (m_udpSocket->hasPendingDatagrams()) {
                QByteArray buffer;
                buffer.resize(m_udpSocket->pendingDatagramSize());
                QHostAddress sender;
                quint16 senderPort;

                m_udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

                // 假设数据包的前缀包含房间号
                QString roomId = QString::fromUtf8(buffer.left(4));
                buffer.remove(0, 4);


                // 转发给房间内的所有观众
                if (m_roomMap.contains(roomId)) {
                    for (auto pmytcpsocket : m_roomMap[roomId]) {
                        pmytcpsocket->get_udpSocket()->writeDatagram(buffer, QHostAddress::Broadcast, 5006);
                    }
                }
            }
        }

    //#include "widget.h"
    //#include "ui_widget.h"
    //#include "mytcpsocket.h"
    //#include <QMessageBox>
    //#include <QDebug>

    //Widget::Widget(QWidget *parent)
    //    : QWidget(parent)
    //    , ui(new Ui::Widget)
    //    , m_tcpServer(new QTcpServer(this))
    //{
    //    ui->setupUi(this);

    //    connect(m_tcpServer, &QTcpServer::newConnection, this, &Widget::on_newConnection);

    //    if (!m_tcpServer->listen(QHostAddress::Any, 8077)) {
    //        QMessageBox::critical(this, "Server Error", "Unable to start the server");
    //        close();
    //        return;
    //    }

    //    qDebug() << "Server started on port" << m_tcpServer->serverPort();
    //}

    //Widget::~Widget()
    //{
    //    delete ui;
    //}

    //void Widget::on_newConnection()
    //{
    //    QTcpSocket *tcpSocket = m_tcpServer->nextPendingConnection();
    //    MyTcpSocket *pmytcpsocket = new MyTcpSocket(tcpSocket, this);  // 添加 this 作为 parent
    //    m_pmytcpsocket_vec.append(pmytcpsocket);

    //    connect(pmytcpsocket->get_tcpsocket(), &QTcpSocket::readyRead, this, &Widget::on_readyRead);
    //    connect(pmytcpsocket->get_tcpsocket(), &QTcpSocket::disconnected, this, &Widget::on_disconnected);

    //    qDebug() << "New connection from" << tcpSocket->peerAddress().toString();
    //}

    //void Widget::on_readyRead()
    //{
    //    MyTcpSocket *pmytcpsocket = qobject_cast<MyTcpSocket*>(sender());
    //    if (!pmytcpsocket) return;

    //    while (pmytcpsocket->get_tcpsocket()->bytesAvailable()) {
    //        Pack pack;
    //        int size = pmytcpsocket->get_tcpsocket()->read(reinterpret_cast<char*>(&size), 4);
    //        if (size != 4) return;

    //        pmytcpsocket->get_tcpsocket()->read(pack.data() + 4, size - 4);

    //        switch (static_cast<Type>(pack.get_type())) {
    //        case TYPE_LOGIN:
    //            user_login_back(pmytcpsocket, pack);
    //            break;
    //        case TYPE_CREATE_ROOM:
    //            user_createroom_back(pmytcpsocket, pack);
    //            break;
    //        case TYPE_JOIN_ROOM:
    //            user_joinroom_back(pmytcpsocket, pack);
    //            break;
    //        case TYPE_GET_NEW_ROOM:
    //            user_getnewroom_back(pmytcpsocket, pack);
    //            break;
    //        case TYPE_GET_NEW_USER:
    //            user_getnewuser_back(pmytcpsocket, pack);
    //            break;
    //        case TYPE_FLUSH_ROOM:
    //            user_flushroom_back(pmytcpsocket, pack);
    //            break;
    //        case TYPE_REMOVE_ROOM:
    //            user_removeroom_back(pmytcpsocket, pack);
    //            break;
    //        default:
    //            break;
    //        }
    //    }
    //}

    //void Widget::on_disconnected()
    //{
    //    MyTcpSocket *pmytcpsocket = qobject_cast<MyTcpSocket*>(sender());
    //    if (!pmytcpsocket) return;

    //    m_pmytcpsocket_vec.removeOne(pmytcpsocket);
    //    pmytcpsocket->deleteLater();  // 调用 deleteLater
    //}

    //void Widget::user_login_back(MyTcpSocket *pmytcpsocket, Pack &pack)
    //{
    //    // 验证用户名和密码
    //    // ...

    //    // 如果验证成功
    //    pmytcpsocket->set_name(pack.get_data()[0]);
    //    Pack responsePack;
    //    responsePack.set_type(TYPE_LOGIN);
    //    responsePack.set_login_state(SUCCESS_LOGIN);
    //    pmytcpsocket->get_tcpsocket()->write(responsePack.data(), responsePack.size());

    //    // 通知其他用户刷新房间列表
    //    user_flushroom_back(pmytcpsocket, pack);
    //}

    //void Widget::user_createroom_back(MyTcpSocket *pmytcpsocket, Pack &pack)
    //{
    //    // 创建新的直播间
    //    QString roomId = QString::number(qrand() % 10000); // 生成随机房间号
    //    m_roomMap[roomId].insert(pmytcpsocket);

    //    Pack responsePack;
    //    responsePack.set_type(TYPE_CREATE_ROOM);
    //    responsePack.append(roomId);
    //    pmytcpsocket->get_tcpsocket()->write(responsePack.data(), responsePack.size());

    //    // 通知所有用户有新房间
    //    for (MyTcpSocket *socket : qAsConst(m_pmytcpsocket_vec)) {
    //        if (socket != pmytcpsocket) {
    //            Pack newRoomPack;
    //            newRoomPack.set_type(TYPE_GET_NEW_ROOM);
    //            newRoomPack.append(roomId);
    //            socket->get_tcpsocket()->write(newRoomPack.data(), newRoomPack.size());
    //        }
    //    }
    //}

    //void Widget::user_joinroom_back(MyTcpSocket *pmytcpsocket, Pack &pack)
    //{
    //    QString roomId = pack.get_data()[0];
    //    if (!m_roomMap.contains(roomId)) return;

    //    m_roomMap[roomId].insert(pmytcpsocket);

    //    Pack responsePack;
    //    responsePack.set_type(TYPE_JOIN_ROOM);
    //    responsePack.append(pmytcpsocket->get_name());
    //    pmytcpsocket->get_tcpsocket()->write(responsePack.data(), responsePack.size());

    //    // 通知房主和其他观众
    //    for (MyTcpSocket *socket : qAsConst(m_roomMap[roomId])) {
    //        if (socket != pmytcpsocket) {
    //            Pack newUserPack;
    //            newUserPack.set_type(TYPE_GET_NEW_USER);
    //            newUserPack.append(pmytcpsocket->get_name());
    //            socket->get_tcpsocket()->write(newUserPack.data(), newUserPack.size());
    //        }
    //    }
    //}

    //void Widget::user_getnewroom_back(MyTcpSocket *pmytcpsocket, Pack &pack)
    //{
    //    // 将新房间号发送给所有用户
    //    for (MyTcpSocket *socket : qAsConst(m_pmytcpsocket_vec)) {
    //        Pack responsePack;
    //        responsePack.set_type(TYPE_GET_NEW_ROOM);
    //        responsePack.append(pack.get_data()[0]);
    //        socket->get_tcpsocket()->write(responsePack.data(), responsePack.size());
    //    }
    //}

    //void Widget::user_getnewuser_back(MyTcpSocket *pmytcpsocket, Pack &pack)
    //{
    //    // 将新用户发送给房主和其他观众
    //    QString roomId = pmytcpsocket->get_joinroom_id();
    //    if (!m_roomMap.contains(roomId)) return;

    //    for (MyTcpSocket *socket : qAsConst(m_roomMap[roomId])) {
    //        if (socket != pmytcpsocket) {
    //            Pack responsePack;
    //            responsePack.set_type(TYPE_GET_NEW_USER);
    //            responsePack.append(pack.get_data()[0]);
    //            socket->get_tcpsocket()->write(responsePack.data(), responsePack.size());
    //        }
    //    }
    //}

    //void Widget::user_flushroom_back(MyTcpSocket *pmytcpsocket, Pack &pack)
    //{
    //    // 发送所有房间号给新登录用户
    //    for (const QString &roomId : m_roomMap.keys()) {
    //        Pack responsePack;
    //        responsePack.set_type(TYPE_FLUSH_ROOM);
    //        responsePack.append(roomId);
    //        pmytcpsocket->get_tcpsocket()->write(responsePack.data(), responsePack.size());
    //    }
    //}

    //void Widget::user_removeroom_back(MyTcpSocket *pmytcpsocket, Pack &pack)
    //{
    //    QString roomId = pack.get_data()[0];
    //    if (!m_roomMap.contains(roomId)) return;

    //    m_roomMap.remove(roomId);

    //    // 通知所有用户房间已关闭
    //    for (MyTcpSocket *socket : qAsConst(m_pmytcpsocket_vec)) {
    //        Pack responsePack;
    //        responsePack.set_type(TYPE_REMOVE_ROOM);
    //        responsePack.append(roomId);
    //        socket->get_tcpsocket()->write(responsePack.data(), responsePack.size());
    //    }
    //}
