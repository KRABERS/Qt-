#include "pack.h"


Pack::Pack()
{
    memset(m_buf, 0, 4096);
    m_count = 0;
    m_size = 6;
}


Pack::~Pack()
{

}

QStringList Pack::get_data()
{
    unsigned int read_count = 0; // 用来记录已经从buf中读取了多少个字节
    unsigned short size = 0;
    QStringList strlist;
    while(true)
    {
        memcpy(&size, m_buf + read_count, 2); // 先读取两个字节放进size里面，获取接下来要读取数据的长度。
        read_count += 2;
        char temp[size + 1] = {0}; // 根据才读取到的接下来要读取数据的长度，准备一个相应大小的内存

        memcpy(temp, m_buf + read_count, size); // 读取数据放进temp里面
        read_count += size;

        QString strdata = QString::fromLocal8Bit(temp); // 把temp里面的字节数据转为QString
        if(!strdata.isEmpty())
        {
            strlist.append(strdata);
        }
        if(0 == m_buf[read_count])  // 如果后续没有数据退出循环
        {
            break;
        }
    }

    //    memset(buf, 0, 4096);
    //    m_count = 0;

    return strlist;
}

void Pack::clear()
{
    memset(m_buf, 0, 4096);
    m_count = 0;
    m_size = 6;
}


void Pack::append(const QString& val)
{
    // 获取val里面数据内存里面有多少个字节（所有的数据在内存中都是字节方式存放的）。
    unsigned short size = strlen(val.toLocal8Bit().data());  // 获取val里面的数据占多少个字节
    memcpy(m_buf + m_count, &size, 2); // 把size里面前2个字节的内容拷贝到m_buf + m_count里面
    m_count += 2;

    memcpy(m_buf + m_count, val.toLocal8Bit().data(), size);
    m_count += size;

    m_size = 2 + 4 + m_count;

    return;
}
//#include "pack.h"
//#include <QByteArray>
//#include <cstring>

//// 构造函数
//Pack::Pack() : m_size(0), m_type(TYPE_REGISTER), m_state(0), m_count(0) {
//    clear();
//}

//// 析构函数
//Pack::~Pack() {
//}

//// 设置数据包类型
//void Pack::set_type(Type type) {
//    m_type = type;
//}

//// 设置登录状态
//void Pack::set_login_state(Login_State state) {
//    m_state = state;
//}

//// 设置注册状态
//void Pack::set_register_state(Register_State state) {
//    m_state = state;
//}

//// 获取数据包类型
//unsigned char Pack::get_type() const {
//    return m_type;
//}

//// 获取登录状态
//unsigned char Pack::get_login_state() const {
//    return m_state;
//}

//// 获取注册状态
//unsigned char Pack::get_register_state() const {
//    return m_state;
//}

//// 添加数据到数据包
//void Pack::append(const QString &val) {
//    QByteArray byteArray = val.toUtf8();
//    int length = byteArray.size();
//    memcpy(m_buf + m_size, &length, sizeof(length));
//    m_size += sizeof(length);
//    memcpy(m_buf + m_size, byteArray.data(), length);
//    m_size += length;
//    m_count++;
//}

//// 获取数据包中的数据
//QStringList Pack::get_data() const {
//    QStringList dataList;
//    int offset = 0;
//    for (int i = 0; i < m_count; ++i) {
//        int length = 0;
//        memcpy(&length, m_buf + offset, sizeof(length));
//        offset += sizeof(length);
//        QByteArray byteArray(m_buf + offset, length);
//        offset += length;
//        dataList.append(QString::fromUtf8(byteArray));
//    }
//    return dataList;
//}

//// 清除数据包中的数据
//void Pack::clear() {
//    m_size = 6;  // 初始化包体大小
//    m_count = 0;
//    memset(m_buf, 0, sizeof(m_buf));
//}

//// 获取数据缓冲区
//char* Pack::data() {
//    return (char*)this;
//}

//// 获取数据缓冲区（常量）
//const char* Pack::data() const {
//    return (char*)this;
//}

//// 获取数据包大小
//int Pack::size() const {
//    return m_size;
//}
