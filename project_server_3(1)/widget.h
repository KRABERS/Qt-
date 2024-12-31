#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QVector>
#include <QUdpSocket>
#include "pack.h"
#include "mytcpsocket.h"
#include <QMap>
#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    void init_db();   // 初始化数据库（调用 DbHelper 的 init_db）
    void init_network();   // 初始化网络（监听）
    MyTcpSocket* find_mytcpsocket(QTcpSocket* ptcpsocket);  // 根据 QTcpSocket 从 vector 里面找到 MyTcpSocket

public slots:
    void new_connection();  // 新的连接到来并成功关联的槽函数
    void ready_read();      // 客户端信息到来的槽函数
    void dis_connected();   // 客户端套接字关闭的槽函数
    void processVideoData(); // 处理视频数据

public:
    void user_register(MyTcpSocket* pmytcpsocket, Pack& pack); // 用户注册功能(调用 DbHelper的 user_register）
    void user_login(MyTcpSocket* pmytcpsocket, Pack& pack); // 用户登录功能(调用 DbHelper的 user_login）
    void user_createroom(MyTcpSocket* pmytcpsocket, Pack& pack);  // 收到用户请求创建直播间
    void user_flushroom(MyTcpSocket* pmytcpsocket, Pack& pack);  // 收到用户请求拉取直播间号
    void user_joinroom(MyTcpSocket* pmytcpsocket, Pack& pack);  // 收到用户请求加入直播间

private:
    Ui::Widget *ui;
    QTcpServer* m_ptcpserver;
    QVector<MyTcpSocket*> m_pmytcpsocket_vec;
    unsigned int m_createromm_id;
    QUdpSocket* m_udpSocket;
    QMap<QString, QSet<MyTcpSocket*>> m_roomMap; // 维护每个房间的观众列表

};
#endif // WIDGET_H
//#ifndef WIDGET_H
//#define WIDGET_H

//#include <QWidget>
//#include <QTcpSocket>
//#include <QTcpServer>
//#include <QMap>
//#include <QSet>
//#include "mytcpsocket.h"
//#include "pack.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class Widget; }
//QT_END_NAMESPACE

//class Widget : public QWidget
//{
//    Q_OBJECT

//public:
//    Widget(QWidget *parent = nullptr);
//    ~Widget();

//private slots:
//    void on_newConnection();
//    void on_readyRead();
//    void on_disconnected();

//private:
//    void user_login_back(MyTcpSocket *pmytcpsocket, Pack &pack);
//    void user_createroom_back(MyTcpSocket *pmytcpsocket, Pack &pack);
//    void user_joinroom_back(MyTcpSocket *pmytcpsocket, Pack &pack);
//    void user_getnewroom_back(MyTcpSocket *pmytcpsocket, Pack &pack);
//    void user_getnewuser_back(MyTcpSocket *pmytcpsocket, Pack &pack);
//    void user_flushroom_back(MyTcpSocket *pmytcpsocket, Pack &pack);
//    void user_removeroom_back(MyTcpSocket *pmytcpsocket, Pack &pack);

//    Ui::Widget *ui;
//    QTcpServer *m_tcpServer;
//    QList<MyTcpSocket*> m_pmytcpsocket_vec;
//    QMap<QString, QSet<MyTcpSocket*>> m_roomMap; // 维护每个房间的观众列表
//};

//#endif // WIDGET_H
