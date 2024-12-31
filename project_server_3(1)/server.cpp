//#include "server.h"
//#include <QNetworkDatagram>

//Server::Server(QObject *parent) : QObject(parent), udpSocket(new QUdpSocket(this))
//{
//    udpSocket->bind(QHostAddress::Any, port);
//    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::processPendingDatagrams);
//}

//Server::~Server()
//{
//    delete udpSocket;
//}

//// 新增：开始广播视频流
//void Server::startBroadcast(int roomId)
//{
//    multicastAddress = QHostAddress(QString("224.0.0.%1").arg(roomId));
//}

//void Server::processPendingDatagrams()
//{
//    while (udpSocket->hasPendingDatagrams()) {
//        QNetworkDatagram datagram = udpSocket->receiveDatagram();
//        QHostAddress senderAddress = datagram.senderAddress();
//        quint16 senderPort = datagram.senderPort();
//        clients.insert(senderAddress);
//        clientPorts[senderAddress] = senderPort;

//        // 转发给所有已连接的客户端
//        for (const QHostAddress &client : clients) {
//            if (client != senderAddress) { // 不发回给发送者
//                udpSocket->writeDatagram(datagram.data(), client, clientPorts[client]);
//            }
//        }
//    }
//}
