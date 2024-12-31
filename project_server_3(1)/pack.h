#ifndef PACK_H
#define PACK_H
#include <QStringList>

// 定义协议包的类型（客户端和服务器发送的数据是什么类型的业务可以通过以下标识区别）
enum Type
{
    TYPE_REGISTER,
    TYPE_LOGIN,
    TYPE_CREATE_ROOM,
    TYPE_GET_NEW_ROOM,
    TYPE_FLUSH_ROOM,
    TYPE_JOIN_ROOM,
    TYPE_GET_NEW_USER,
    TYPE_REMOVE_ROOM
//    TYPE_FLUSH_OTHER_USERS,
//    TYPE_DELETE_USER
};

// 定义登录的状态枚举值
enum Login_State
{
    SUCCESS_LOGIN,
    ERR_NAME_RULE_LOGIN,
    ERR_PWD_RULE_LOGIN,
    ERR_NAME_LOGIN,
    ERR_PWD_LOGIN,
    ERR_RELOGIN_LOGIN,
    ERR_DB_LOGIN

};

// 定义注册的状态枚举值
enum Register_State
{
    SUCCESS_REGISTER,
    ERR_NAME_RULE_REGISTER,
    ERR_PWD_RULE_REGISTER,
    ERR_PWD_PWD_REGISTER,
    ERR_REREGISTER_REGISTER,
    ERR_DB_REGISTER
};

// 构建一个包，后面把发送的数据都合成到这个包里面发送
class Pack
{
public:
    Pack();
    ~Pack();

public:
    void set_type(Type type)
    {
        m_type = type;
    }
    void set_lgoin_state(Login_State state)
    {
        m_state = state;
    }
    void set_register_state(Register_State state)
    {
        m_state = state;
    }
    unsigned char get_type()
    {
        return m_type;
    }
    unsigned char get_login_state()
    {
        return m_state;
    }
    unsigned char get_register_state()
    {
        return m_state;
    }

public:
    void append(const QString& val);
    QStringList get_data();
    char* data()
    {
        return (char*)this;
    }
    int size()
    {
        return m_size;
    }
    void clear();

public:
    unsigned int m_size;  // 包体总的大小（4个字节）
    unsigned char m_type;  // 0-255之间的整数（1个字节）
    unsigned char m_state;  // 0-255之间的整数（1个字节）
    char m_buf[4096];  // 足够大的缓存，后面发送的数据从里面取出具体的数据（不会超过4096个）
    int m_count;  // m_buf里面具体内容的长度(m_size的值 - 6）
};

#endif // PACK_H
//#ifndef PACK_H
//#define PACK_H

//#include <QStringList>

//// 定义协议包的类型（客户端和服务器发送的数据是什么类型的业务可以通过以下标识区别）
//enum Type
//{
//    TYPE_REGISTER,
//    TYPE_LOGIN,
//    TYPE_CREATE_ROOM,
//    TYPE_GET_NEW_ROOM,
//    TYPE_FLUSH_ROOM,
//    TYPE_JOIN_ROOM,
//    TYPE_GET_NEW_USER,
//    TYPE_REMOVE_ROOM
//    // TYPE_FLUSH_OTHER_USERS,
//    // TYPE_DELETE_USER
//};

//// 定义登录的状态枚举值
//enum Login_State
//{
//    SUCCESS_LOGIN,
//    ERR_NAME_RULE_LOGIN,
//    ERR_PWD_RULE_LOGIN,
//    ERR_NAME_LOGIN,
//    ERR_PWD_LOGIN,
//    ERR_RELOGIN_LOGIN,
//    ERR_DB_LOGIN
//};

//// 定义注册的状态枚举值
//enum Register_State
//{
//    SUCCESS_REGISTER,
//    ERR_NAME_RULE_REGISTER,
//    ERR_PWD_RULE_REGISTER,
//    ERR_PWD_PWD_REGISTER,
//    ERR_REREGISTER_REGISTER,
//    ERR_DB_REGISTER
//};

//// 构建一个包，后面把发送的数据都合成到这个包里面发送
//class Pack
//{
//public:
//    Pack();
//    ~Pack();

//    void set_type(Type type);
//    void set_login_state(Login_State state);
//    void set_register_state(Register_State state);
//    unsigned char get_type() const;
//    unsigned char get_login_state() const;
//    unsigned char get_register_state() const;

//    void append(const QString& val);
//    QStringList get_data() const;
//    char* data();
//    const char* data() const;
//    int size() const;
//    void clear();

//private:
//    unsigned int m_size;  // 包体总的大小（4个字节）
//    unsigned char m_type;  // 0-255之间的整数（1个字节）
//    unsigned char m_state;  // 0-255之间的整数（1个字节）
//    char m_buf[4096];  // 足够大的缓存，后面发送的数据从里面取出具体的数据（不会超过4096个）
//    int m_count;  // m_buf里面具体内容的长度(m_size的值 - 6）
//    QStringList m_data;  // 数据包中的数据
//};

//#endif // PACK_H
