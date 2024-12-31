//#ifndef SERVER_H
//#define SERVER_H

//#include <QObject>
//#include <QUdpSocket>
//#include <QSet>
//#include <QHostAddress>

//class Server : public QObject
//{
//    Q_OBJECT

//public:
//    explicit Server(QObject *parent = nullptr);
//    ~Server();
//    void startBroadcast(int roomId); // 新增：开始广播视频流

//private slots:
//    void processPendingDatagrams();

//private:
//    QUdpSocket *udpSocket;
//    QSet<QHostAddress> clients;
//    QHash<QHostAddress, quint16> clientPorts;
//    quint16 port = 5005;
//    QHostAddress multicastAddress; // 新增：组播地址
//};

//#endif // SERVER_H
