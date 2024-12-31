#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QThread>
#include "pack.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_ptcpsocket(new QTcpSocket)
    , m_proomnumber(NULL)
    , m_plivingroom(NULL)
    , m_pregisterwindow(NULL)
    , m_name()
{
    ui->setupUi(this);

    // 用户点击注册按钮
    connect(ui->pushButton_register, &QPushButton::clicked, this, &Widget::registerbtn_clicked);


    // 用户点击登录按钮
    connect(ui->pushButton_login, &QPushButton::clicked, this, &Widget::loginbtn_clicked);

    // 连接服务器成功
    connect(m_ptcpsocket, &QTcpSocket::connected, this, &Widget::connect_ok);

    // 连接服务器
    init_network();

    //按下显示密码
    connect(ui->pushButton_show_pwd, &QPushButton::pressed, this, &Widget::pwdshowbtn_pressed);

    //松开隐藏密码
    connect(ui->pushButton_show_pwd, &QPushButton::released, this, &Widget::pwdshowbtn_released);

}

Widget::~Widget()
{
    delete ui;
    delete m_pregisterwindow;
}

// 初始化网络
void Widget::init_network()
{
    m_ptcpsocket->connectToHost("127.0.0.1", 8077);
}

void Widget::user_login_back(Pack &pack)
{
    Login_State login_ret = (Login_State)pack.get_login_state();  // 获取登录结果

    switch (login_ret)
    {
    case SUCCESS_LOGIN:
        QMessageBox::information(this, "登录", "登录成功");

        hide();  // 隐藏登录注册窗口
        m_proomnumber = new RoomNumber(m_ptcpsocket, m_name);
        m_proomnumber->show();  // 弹出房间号列表界面
        break;

    case ERR_NAME_RULE_LOGIN:
        QMessageBox::information(this, "登录", "用户名长度不符合要求[3-12]");
        break;

    case ERR_PWD_RULE_LOGIN:
        QMessageBox::information(this, "登录", "密码不符合要求[3-12]");
        break;

    case ERR_NAME_LOGIN:
        QMessageBox::information(this, "登录", "用户名错误");
        break;

    case ERR_PWD_LOGIN:
        QMessageBox::information(this, "登录", "密码错误");
        break;

    case ERR_RELOGIN_LOGIN:
        QMessageBox::information(this, "登录", "重复登录");
        break;

    case ERR_DB_LOGIN:
        QMessageBox::information(this, "登录", "数据库错误）");
        break;

    default:
        break;
    }

    return;
}

// 处理服务器返回的创建的房间号的函数
void Widget::user_createroom_back(Pack &pack)
{
    QStringList strlist = pack.get_data();
    QString strcreateroomid = strlist[0];
    m_proomnumber->add_createroom_id(strcreateroomid);

    // 从房间号列表界面进入直播间界面
    m_proomnumber->hide();
    m_plivingroom = new LivingRoom(m_name, m_name, true, strcreateroomid);
    m_plivingroom->show();

    return;
}

// 处理服务器返回的给登录成功的用户房间号的函数
void Widget::user_getnewroom_back(Pack &pack)
{
    QStringList strlist = pack.get_data();
    QString strcreateroomid = strlist[0];
    m_proomnumber->add_createroom_id(strcreateroomid);

    return;
}

// 处理服务器返回的给登录成功的用户所有主播房间号的函数
void Widget::user_flushroom_back(Pack &pack)
{
    QStringList strlist = pack.get_data();

    for(auto onestrlist: strlist)
    {
        m_proomnumber->add_createroom_id(onestrlist);
    }

    return;
}

// 处理服务器返回的加入房间请求的函数
void Widget::user_joinroom_back(Pack &pack)
{
    QStringList strlist = pack.get_data();
    QString strownername = strlist[0];  // 取出房主的名字

    // 从房间号列表界面进入直播间界面
    m_proomnumber->hide();
    m_plivingroom = new LivingRoom(m_name, strownername, false, m_proomnumber->get_joinroomid());
    m_plivingroom->show();

    // 加入同直播间内其他用户的名字
    for(auto strothername: strlist)
    {
        if(strothername != strownername)
        {
            m_plivingroom->add_name(strothername);
        }
    }

    return;
}

// 给房主或通直播其它用户返回加入者的名字
void Widget::user_getnewuser_back(Pack &pack)
{
    QStringList strlist = pack.get_data();
    QString strname = strlist[0];  // 取出加入者的名字
    m_plivingroom->add_name(strname);

    return;
}

// 处理服务器返回的注册结果函数
void Widget::user_register_back(Pack& pack)
{
    Register_State register_ret = (Register_State)pack.get_register_state();  // 获取注册结果

    switch (register_ret)
    {
    case SUCCESS_REGISTER:
        QMessageBox::information(this, "注册", "注册成功");
        break;

    case ERR_NAME_RULE_REGISTER:
        QMessageBox::information(this, "注册", "用户名长度不符合要求[3-12]");
        break;

    case ERR_PWD_RULE_REGISTER:
        QMessageBox::information(this, "注册", "密码不符合要求[3-12]");
        break;

    case ERR_REREGISTER_REGISTER:
        QMessageBox::information(this, "注册", "重复注册");
        break;

    case ERR_DB_REGISTER:
        QMessageBox::information(this, "注册", "数据库错误");
        break;
    default:
        break;
    }

    return;
}

// 连接服务器成功
void Widget::connect_ok()
{
    ui->pushButton_register->setEnabled(true);
    ui->pushButton_login->setEnabled(true);
    QMessageBox::information(this, "连接", "连接服务器成功");

    connect(m_ptcpsocket, &QTcpSocket::readyRead, this, &Widget::ready_read);

    return;
}

// 收到服务器返回信息的槽函数
void Widget::ready_read()
{
    while (true) // 可能有多个应用层协议包发来
    {
        Pack pack;
        int size = 0;

        // 先解析包的长度值（4个字节）
        int rst = m_ptcpsocket->read((char*)&size, 4);
        if(4 != rst)
        {
            break;
        }

        // 再解析包的长度后面的的字节（包总长度 - 4)
        rst = m_ptcpsocket->read(pack.data() + 4, size - 4);
        if(size - 4 != rst)
        {
            break;
        }

        // 根据包的类型开始处理业务逻辑
        switch ((Type)pack.get_type())
        {
        case TYPE_REGISTER:
            user_register_back(pack);
            break;

        case TYPE_LOGIN:
            user_login_back(pack);
            break;

        case TYPE_CREATE_ROOM:
            user_createroom_back(pack);
            break;

        case TYPE_GET_NEW_ROOM:
            user_getnewroom_back(pack);
            break;

        case TYPE_FLUSH_ROOM:
            user_flushroom_back(pack);
            break;

        case TYPE_JOIN_ROOM:
            user_joinroom_back(pack);
            break;

        case TYPE_GET_NEW_USER:
            user_getnewuser_back(pack);
            break;
        }
    }
}

// 用户点击注册按钮的槽函数
void Widget::registerbtn_clicked()
{
    hide();
    // 创建并显示注册窗口
    if (!m_pregisterwindow) {
        m_pregisterwindow = new register_window();
//        m_pregisterwindow->show();
        connect(m_pregisterwindow, &register_window::registrationCompleted, this, &Widget::onRegistrationCompleted);
         qDebug() << "Register window created";
    }

    m_pregisterwindow->show();
    qDebug() << "Register window shown";

    // 隐藏当前窗口

    qDebug() << "Main window hidden";
}

void Widget::onRegistrationCompleted()
{
    // 当注册完成后，重新显示主窗口
    m_pregisterwindow->hide();
    show();
}

void Widget::pwdshowbtn_pressed()
{
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Normal);
}

void Widget::pwdshowbtn_released()
{
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
}

// 用户点击登录按钮的槽函数
void Widget::loginbtn_clicked()
{
    QString strname = ui->lineEdit_name->text();
    QString strpwd = ui->lineEdit_pwd->text();

    if(strname.isEmpty()||strpwd.isEmpty()){
        QMessageBox::warning(this, "登录", "用户名、密码不能为空");
    }

    if(strname.length()<3||strname.length()>12){
        QMessageBox::warning(this, "登录", "密码输入不符合规则3-12");
    }

    // 记录下用户输入的用户名，给其它窗口显示用
    m_name = strname;
    QByteArray hashedPwd = QCryptographicHash::hash(strpwd.toUtf8(), QCryptographicHash::Md5).toHex();

    // 发送给服务器
    Pack pack;  // 构建应用层协议包
    pack.set_type(TYPE_LOGIN);  // 设置状态为注册包
    pack.append(strname);  // 包内容含有用户名
    pack.append(hashedPwd);   // 包内容含有密码

    m_ptcpsocket->write(pack.data(), pack.size());

    return;
}

//#include "widget.h"
//#include "ui_widget.h"
//#include <QMessageBox>
//#include <QThread>
//#include <QCryptographicHash>
//#include "pack.h"

//Widget::Widget(QWidget *parent)
//    : QWidget(parent)
//    , ui(new Ui::Widget)
//    , m_ptcpsocket(new QTcpSocket(this))
//    , m_proomnumber(nullptr)
//    , m_pregisterwindow(nullptr)
//    , m_name()
//{
//    ui->setupUi(this);

//    // 用户点击注册按钮
//    connect(ui->pushButton_register, &QPushButton::clicked, this, &Widget::registerbtn_clicked);

//    // 用户点击登录按钮
//    connect(ui->pushButton_login, &QPushButton::clicked, this, &Widget::loginbtn_clicked);

//    // 连接服务器成功
//    connect(m_ptcpsocket, &QTcpSocket::connected, this, &Widget::connect_ok);

//    // 连接服务器
//    init_network();

//    // 按下显示密码
//    connect(ui->pushButton_show_pwd, &QPushButton::pressed, this, &Widget::pwdshowbtn_pressed);

//    // 松开隐藏密码
//    connect(ui->pushButton_show_pwd, &QPushButton::released, this, &Widget::pwdshowbtn_released);
//}

//Widget::~Widget()
//{
//    delete ui;
//    delete m_pregisterwindow;
//    delete m_proomnumber;
//}

//// 初始化网络
//void Widget::init_network()
//{
//    m_ptcpsocket->connectToHost("127.0.0.1", 8077);
//}

//void Widget::user_login_back(Pack &pack)
//{
//    Login_State login_ret = static_cast<Login_State>(pack.get_login_state());  // 获取登录结果

//    switch (login_ret)
//    {
//    case SUCCESS_LOGIN:
//        QMessageBox::information(this, "登录", "登录成功");

//        hide();  // 隐藏登录注册窗口
//        m_proomnumber = new RoomNumber(m_ptcpsocket, m_name);
//        m_proomnumber->show();  // 弹出房间号列表界面
//        break;

//    case ERR_NAME_RULE_LOGIN:
//        QMessageBox::information(this, "登录", "用户名长度不符合要求[3-12]");
//        break;

//    case ERR_PWD_RULE_LOGIN:
//        QMessageBox::information(this, "登录", "密码不符合要求[3-12]");
//        break;

//    case ERR_NAME_LOGIN:
//        QMessageBox::information(this, "登录", "用户名错误");
//        break;

//    case ERR_PWD_LOGIN:
//        QMessageBox::information(this, "登录", "密码错误");
//        break;

//    case ERR_RELOGIN_LOGIN:
//        QMessageBox::information(this, "登录", "重复登录");
//        break;

//    case ERR_DB_LOGIN:
//        QMessageBox::information(this, "登录", "数据库错误");
//        break;

//    default:
//        break;
//    }
//}

//void Widget::user_register_back(Pack& pack)
//{
//    Register_State register_ret = static_cast<Register_State>(pack.get_register_state());  // 获取注册结果

//    switch (register_ret)
//    {
//    case SUCCESS_REGISTER:
//        QMessageBox::information(this, "注册", "注册成功");
//        break;

//    case ERR_NAME_RULE_REGISTER:
//        QMessageBox::information(this, "注册", "用户名长度不符合要求[3-12]");
//        break;

//    case ERR_PWD_RULE_REGISTER:
//        QMessageBox::information(this, "注册", "密码不符合要求[3-12]");
//        break;

//    case ERR_REREGISTER_REGISTER:
//        QMessageBox::information(this, "注册", "重复注册");
//        break;

//    case ERR_DB_REGISTER:
//        QMessageBox::information(this, "注册", "数据库错误");
//        break;
//    default:
//        break;
//    }
//}

//// 连接服务器成功
//void Widget::connect_ok()
//{
//    ui->pushButton_register->setEnabled(true);
//    ui->pushButton_login->setEnabled(true);
//    QMessageBox::information(this, "连接", "连接服务器成功");

//    connect(m_ptcpsocket, &QTcpSocket::readyRead, this, &Widget::ready_read);
//}

//// 收到服务器返回信息的槽函数
//void Widget::ready_read()
//{
//    while (true) // 可能有多个应用层协议包发来
//    {
//        Pack pack;
//        int size = 0;

//        // 先解析包的长度值（4个字节）
//        int rst = m_ptcpsocket->read(reinterpret_cast<char*>(&size), 4);
//        if (4 != rst)
//        {
//            break;
//        }

//        // 再解析包的长度后面的的字节（包总长度 - 4)
//        rst = m_ptcpsocket->read(pack.data() + 4, size - 4);
//        if (size - 4 != rst)
//        {
//            break;
//        }

//        // 根据包的类型开始处理业务逻辑
//        switch (static_cast<Type>(pack.get_type()))
//        {
//        case TYPE_REGISTER:
//            user_register_back(pack);
//            break;

//        case TYPE_LOGIN:
//            user_login_back(pack);
//            break;

//        // 其他类型处理
//        default:
//            break;
//        }
//    }
//}

//// 用户点击注册按钮的槽函数
//void Widget::registerbtn_clicked()
//{
//    hide();
//    // 创建并显示注册窗口
//    if (!m_pregisterwindow) {
//        m_pregisterwindow = new register_window();
//        connect(m_pregisterwindow, &register_window::registrationCompleted, this, &Widget::onRegistrationCompleted);
//        qDebug() << "Register window created";
//    }

//    m_pregisterwindow->show();
//    qDebug() << "Register window shown";
//}

//void Widget::onRegistrationCompleted()
//{
//    // 当注册完成后，重新显示主窗口
//    m_pregisterwindow->hide();
//    show();
//}

//void Widget::pwdshowbtn_pressed()
//{
//    ui->lineEdit_pwd->setEchoMode(QLineEdit::Normal);
//}

//void Widget::pwdshowbtn_released()
//{
//    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
//}

//// 用户点击登录按钮的槽函数
//void Widget::loginbtn_clicked()
//{
//    QString strname = ui->lineEdit_name->text();
//    QString strpwd = ui->lineEdit_pwd->text();

//    if (strname.isEmpty() || strpwd.isEmpty()) {
//        QMessageBox::warning(this, "登录", "用户名、密码不能为空");
//        return;
//    }

//    if (strname.length() < 3 || strname.length() > 12) {
//        QMessageBox::warning(this, "登录", "用户名长度不符合规则[3-12]");
//        return;
//    }

//    if (strpwd.length() < 3 || strpwd.length() > 12) {
//        QMessageBox::warning(this, "登录", "密码长度不符合规则[3-12]");
//        return;
//    }

//    // 记录下用户输入的用户名，给其它窗口显示用
//    m_name = strname;
//    QByteArray hashedPwd = QCryptographicHash::hash(strpwd.toUtf8(), QCryptographicHash::Md5).toHex();

//    // 发送给服务器
//    Pack pack;  // 构建应用层协议包
//    pack.set_type(TYPE_LOGIN);  // 设置状态为登录包
//    pack.append(strname);  // 包内容含有用户名
//    pack.append(hashedPwd);   // 包内容含有密码

//    m_ptcpsocket->write(pack.data(), pack.size());
//}
