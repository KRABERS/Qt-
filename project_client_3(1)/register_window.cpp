#include "register_window.h"
#include "ui_register_window.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include "pack.h"

register_window::register_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::register_window),
    m_ptcpsocket(new QTcpSocket(this))  // 初始化网络连接
{
    ui->setupUi(this);
    connect(ui->pushButton_execu, &QPushButton::clicked, this, &register_window::on_pushButton_clicked);

    // 连接服务器
    m_ptcpsocket->connectToHost("127.0.0.1", 8077);
}

register_window::~register_window()
{
    delete ui;
}

void register_window::on_pushButton_clicked()
{
    handleRegistration();
}

void register_window::handleRegistration()
{
    QString username = ui->lineEdit_regname->text();
    QString password = ui->lineEdit_regpwd->text();
    QString confirmPassword = ui->lineEdit_conpwd->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "注册", "所有字段都是必填的");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册", "两次输入的密码不一致");
        return;

    }
    if (password.length() < 3 || password.length() > 12) {
            QMessageBox::warning(nullptr, "注册", "密码长度不符合要求[3-12]");
            return;
    }


    // 对密码进行哈希处理
    QByteArray hashedPwd = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray hashedConfirmPwd = QCryptographicHash::hash(confirmPassword.toUtf8(), QCryptographicHash::Md5).toHex();

    // 构建应用层协议包
    Pack pack;
    pack.set_type(TYPE_REGISTER);
    pack.append(username);
    pack.append(hashedPwd);
    pack.append(hashedConfirmPwd);

    // 发送给服务器
    m_ptcpsocket->write(pack.data(), pack.size());

    // 读取服务器返回的数据
    if (m_ptcpsocket->waitForReadyRead(3000)) {
        QByteArray response = m_ptcpsocket->readAll();
        Pack responsePack;
        memcpy(&responsePack, response.data(), response.size());

        Register_State registerState = (Register_State)responsePack.get_register_state();

        switch (registerState) {
            case SUCCESS_REGISTER:
                QMessageBox::information(this, "注册", "注册成功");
                emit registrationCompleted();
                break;
            case ERR_NAME_RULE_REGISTER:
                QMessageBox::warning(this, "注册", "用户名长度不符合要求[3-12]");
                break;
//            case ERR_PWD_RULE_REGISTER:
//                QMessageBox::warning(this, "注册", "密码不符合要求[3-12]");
                break;
            case ERR_REREGISTER_REGISTER:
                QMessageBox::warning(this, "注册", "用户名已存在");
                break;
            case ERR_DB_REGISTER:
                QMessageBox::warning(this, "注册", "数据库错误");
                break;
            case ERR_PWD_PWD_REGISTER:
                QMessageBox::warning(this, "注册", "两次输入的密码不一致");
                break;
            default:
                QMessageBox::warning(this, "注册", "未知错误");
                break;
        }
    } else {
        QMessageBox::warning(this, "注册", "无法连接到服务器");
    }
}
