#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class register_window;
}

class register_window : public QWidget
{
    Q_OBJECT

public:
    explicit register_window(QWidget *parent = nullptr);
    ~register_window();

signals:
    void registrationCompleted();

private slots:
    void on_pushButton_clicked();

private:
    Ui::register_window *ui;
    QTcpSocket* m_ptcpsocket;  // 添加网络连接

    void handleRegistration();  // 处理注册逻辑
};

#endif // REGISTER_WINDOW_H
