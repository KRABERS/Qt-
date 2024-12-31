#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "pack.h"
#include "roomnumber.h"
#include "livingroom.h"
#include "register_window.h"
#include <QCryptographicHash>

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
    void init_network();  // 初始化网络
    void user_register_back(Pack& pack);  // 处理服务器返回的注册结果函数
    void user_login_back(Pack& pack);  // 处理服务器返回的注册结果函数
    void user_createroom_back(Pack& pack);  // 处理服务器返回的给创建者创建的房间号的函数
    void user_getnewroom_back(Pack& pack);  // 处理服务器返回的给登录成功的用户房间号的函数
    void user_flushroom_back(Pack& pack);  // 处理服务器返回的给登录成功的用户所有主播房间号的函数
    void user_joinroom_back(Pack& pack);  // 处理服务器返回的加入房间请求的函数
    void user_getnewuser_back(Pack& pack);  // 给房主返回加入者的名字

public slots:
   void connect_ok(); // 连接服务器成功
   void ready_read();  // 收到服务器返回信息的槽函数
   void registerbtn_clicked();  // 用户点击注册按钮的槽函数
   void loginbtn_clicked();  // 用户点击登录按钮的槽函数
   void onRegistrationCompleted(); // Handle registration completion
   void pwdshowbtn_pressed();
   void pwdshowbtn_released();

private:
    Ui::Widget *ui;
    QTcpSocket* m_ptcpsocket;
    RoomNumber* m_proomnumber;
    LivingRoom* m_plivingroom;
    register_window* m_pregisterwindow;
    QString m_name;
};
#endif // WIDGET_H
//#ifndef WIDGET_H
//#define WIDGET_H

//#include <QWidget>
//#include <QTcpSocket>
//#include "roomnumber.h"
//#include "register_window.h"
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
//    void registerbtn_clicked();
//    void loginbtn_clicked();
//    void pwdshowbtn_pressed();
//    void pwdshowbtn_released();
//    void connect_ok();
//    void ready_read();
//    void onRegistrationCompleted();

//private:
//    void init_network();
//    void user_login_back(Pack &pack);
//    void user_register_back(Pack &pack);

//    Ui::Widget *ui;
//    QTcpSocket *m_ptcpsocket;
//    RoomNumber *m_proomnumber;
//    register_window *m_pregisterwindow;
//    QString m_name;
//};

//#endif // WIDGET_H
