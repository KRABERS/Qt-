#include "roomnumber.h"
#include "ui_roomnumber.h"
#include "pack.h"
#include <QMessageBox>

RoomNumber::RoomNumber(QTcpSocket* ptcpsocket, const QString& name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomNumber),
    m_ptcpsocket(ptcpsocket),
    m_name(name),
    m_joinroom_id()
{
    ui->setupUi(this);

    init_window();

    // 点击创建直播间按钮
    connect(ui->pushButton_createroom, &QPushButton::clicked, this, &RoomNumber::createroombtn_clicked);

    // 点击加入直播间按钮
    connect(ui->pushButton_joinroom, &QPushButton::clicked, this, &RoomNumber::joinroombtn_clicked);

    // 选中直播间号（点击 ListWidgetItem）
    connect(ui->listWidget_roomnumber, &QListWidget::itemClicked, this, &RoomNumber::listwidgetitem_clicked);

    // 登录成功，向服务器拉取直播间号
    Pack pack;  // 构建应用层协议包
    pack.set_type(TYPE_FLUSH_ROOM);  // 设置状态为注册包
    m_ptcpsocket->write(pack.data(), pack.size());
}

RoomNumber::~RoomNumber()
{
    delete ui;
}

void RoomNumber::init_window()
{
    // 给窗口起个名字
    setWindowTitle(m_name);
}

// 房间号界面列表中加入服务器返回的房间号
void RoomNumber::add_createroom_id(const QString& createroomid)
{
    ui->listWidget_roomnumber->addItem(new QListWidgetItem(createroomid));

    return;
}

// 获取加入的房间号
const QString &RoomNumber::get_joinroomid()
{
    return m_joinroom_id;
}

 // 点击创建直播间的槽函数, 告诉服务器我要创建直播间，请求服务器返回一个直播间号
void RoomNumber::createroombtn_clicked()
{
    Pack pack;  // 构建应用层协议包
    pack.set_type(TYPE_CREATE_ROOM);  // 设置包类型为创建房间

    m_ptcpsocket->write(pack.data(), pack.size());

    return;
}

// 点击加入直播间的槽函数
void RoomNumber::joinroombtn_clicked()
{
    if(m_joinroom_id.isEmpty())
    {
        QMessageBox::warning(this, "加入直播间", "请先选中要加入的直播间号");
        return;
    }

    Pack pack;  // 构建应用层协议包
    pack.set_type(TYPE_JOIN_ROOM);  // 设置包类型为创建房间
    pack.append(m_joinroom_id);

    m_ptcpsocket->write(pack.data(), pack.size());

    return;
}

// 选中直播间号（点击 ListWidgetItem）的槽函数
void RoomNumber::listwidgetitem_clicked(QListWidgetItem* item)
{
    m_joinroom_id = item->text();

    return;
}
//#include "roomnumber.h"
//#include "pack.h"
//#include <QVBoxLayout>
//#include <QMessageBox>
//#include <QDebug>

//RoomNumber::RoomNumber(QTcpSocket *socket, const QString &name, QWidget *parent)
//    : QWidget(parent), m_socket(socket), m_name(name), m_joinroomid("")
//{
//    QVBoxLayout *layout = new QVBoxLayout(this);

//    m_roomList = new QListWidget(this);
//    layout->addWidget(m_roomList);

//    m_createButton = new QPushButton("创建直播", this);
//    layout->addWidget(m_createButton);

//    m_joinButton = new QPushButton("加入直播间", this);
//    layout->addWidget(m_joinButton);

//    connect(m_createButton, &QPushButton::clicked, this, &RoomNumber::on_createButton_clicked);
//    connect(m_joinButton, &QPushButton::clicked, this, &RoomNumber::on_joinButton_clicked);
//    connect(m_roomList, &QListWidget::itemClicked, this, &RoomNumber::on_roomList_itemClicked);
//}

//RoomNumber::~RoomNumber()
//{
//}

//void RoomNumber::add_createroom_id(const QString &roomId)
//{
//    // Add the room ID to the list widget
//    if (m_roomList) {
//        m_roomList->addItem(roomId);
//    }
//}

//QString RoomNumber::get_joinroomid() const
//{
//    return m_joinroomid;
//}

//void RoomNumber::on_createButton_clicked()
//{
//    // 发送创建房间请求到服务器
//    Pack pack;
//    pack.set_type(TYPE_CREATE_ROOM);
//    m_socket->write(pack.data(), pack.size());
//}

//void RoomNumber::on_joinButton_clicked()
//{
//    QListWidgetItem *selectedItem = m_roomList->currentItem();
//    if (selectedItem) {
//        QString roomId = selectedItem->text();
//        // 发送加入房间请求到服务器
//        Pack pack;
//        pack.set_type(TYPE_JOIN_ROOM);
//        pack.append(roomId);
//        m_socket->write(pack.data(), pack.size());

//        // Store the join room ID for later use
//        m_joinroomid = roomId;
//    } else {
//        QMessageBox::warning(this, "Warning", "Please select a room to join.");
//    }
//}

//void RoomNumber::on_roomList_itemClicked(QListWidgetItem *item)
//{
//    // Handle room list item clicked event if necessary
//    qDebug() << "Room list item clicked: " << item->text();
//}
