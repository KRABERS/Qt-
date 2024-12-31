#ifndef ROOMNUMBER_H
#define ROOMNUMBER_H

#include <QWidget>
#include <QTcpSocket>
#include <QListWidgetItem>

namespace Ui {
class RoomNumber;
}

class RoomNumber : public QWidget
{
    Q_OBJECT

public:
    explicit RoomNumber(QTcpSocket* ptcpsocket, const QString& name, QWidget *parent = nullptr);
    ~RoomNumber();

public:
    void init_window();  // 给窗口做界面上的扩展

public:
    void add_createroom_id(const QString& createroomid);  // 房间号界面列表中加入服务器返回的房间号
    const QString& get_joinroomid();  // 获取加入的房间号

public slots:
    void createroombtn_clicked();  // 点击创建直播间的槽函数
    void joinroombtn_clicked();    // 点击加入直播间的槽函数
    void listwidgetitem_clicked(QListWidgetItem *item);  // 选中直播间号（点击 ListWidgetItem）的槽函数

private:
    Ui::RoomNumber *ui;
    QTcpSocket* m_ptcpsocket;  // 客户端套接字
    QString m_name;  // 客户的用户名
    QString m_joinroom_id;  // 要加入的房间号
};

#endif // ROOMNUMBER_H

//#ifndef ROOMNUMBER_H
//#define ROOMNUMBER_H

//#include <QWidget>
//#include <QTcpSocket>
//#include <QStringList>
//#include <QListWidget>
//#include <QPushButton>

//class RoomNumber : public QWidget
//{
//    Q_OBJECT

//public:
//    RoomNumber(QTcpSocket *socket, const QString &name, QWidget *parent = nullptr);
//    ~RoomNumber();

//    void add_createroom_id(const QString &roomId);
//    QString get_joinroomid() const;

//private slots:
//    void on_createButton_clicked();
//    void on_joinButton_clicked();
//    void on_roomList_itemClicked(QListWidgetItem *item);

//private:
//    QTcpSocket *m_socket;
//    QString m_name;
//    QString m_joinroomid;  // 确保声明 m_joinroomid
//    QListWidget *m_roomList;
//    QPushButton *m_createButton;
//    QPushButton *m_joinButton;
//};

//#endif // ROOMNUMBER_H
