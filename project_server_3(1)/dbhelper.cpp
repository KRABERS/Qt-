#include "dbhelper.h"
#include <QDebug>
#include <QSqlQuery>

DbHelper::DbHelper():m_db(QSqlDatabase::addDatabase("QODBC"))
{

}

DbHelper::DbHelper(const DbHelper&)
{

}

DbHelper::~DbHelper()
{

}

// 初始化数据库（做连接）
bool DbHelper::init_db()
{
    m_db.setHostName("127.0.0.1");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setDatabaseName("projectdb");

    return m_db.open();
}

// 用户注册功能（0.注册成功;1.用户名长度不符合要求;2.用户名密码不符合要求;3重复注册;4.数据库错误）
Register_State DbHelper::user_register(const QString& strname, const QString& strpwd, const QString& strConfirmPwd)
{
    // 1.用户名长度不符合要求
    if(strname.length() < 3 || strname.length() > 12)
    {
        return ERR_NAME_RULE_REGISTER;
    }

//        // 2.密码长度不符合要求
//        if(strpwd.length() < 3 || strpwd.length() > 12)
//        {
//                return ERR_PWD_RULE_REGISTER;
//        }

    // 3.两次输入的密码是否一致
    if(strpwd != strConfirmPwd)
    {
        return ERR_PWD_PWD_REGISTER;
    }
    // 4.重复注册
    QString strsql = QString("select * from user where name='%1';").arg(strname);
    QSqlQuery query;
    if(!query.exec(strsql))  // 数据库错误
    {
        return ERR_DB_REGISTER;
    }
    else
    {
        if(query.next())  // 注册的用户名已存在
        {
            return ERR_REREGISTER_REGISTER;
        }
    }

    // 执行注册语句
    strsql = QString("insert into user(name, pwd) values('%1', '%2');").arg(strname).arg(strpwd);

    if(!query.exec(strsql))  // 数据库错误
    {
        return ERR_DB_REGISTER;
    }

    return SUCCESS_REGISTER;  // 插入注册数据成功
}

// 用户登录功能（0.登录成功;1.用户名长度不符合要求;2.用户名密码不符合要求;3.用户名错误;4.密码错误;5.重复登录;6.数据库错误）
Login_State DbHelper::user_login(const QString &strname, const QString &strpwd)
{
    // 1.用户名长度不符合要求
    if(strname.length() < 3 || strname.length() > 12)
    {
        return ERR_NAME_RULE_LOGIN;
    }

//    // 2.密码长度不符合要求
//    if(strpwd.length() < 3 || strpwd.length() > 12)
//    {
//        return ERR_PWD_RULE_LOGIN;
//    }

    // 3.根据用户名查询密码
    QString strsql = QString("select pwd from user where name='%1';").arg(strname);
    QSqlQuery query;
    if(!query.exec(strsql))  // 数据库错误
    {
        return ERR_DB_LOGIN;
    }
    else
    {
        if(query.next())  // 用户名存在
        {
            if(query.value("pwd") == strpwd)  // 密码相同登录成功
            {
                return SUCCESS_LOGIN;
            }
            else
            {
                return ERR_PWD_LOGIN;
            }
        }
        else  // 用户名不存在
        {
            return ERR_NAME_LOGIN;
        }
    }

    return SUCCESS_LOGIN;
}
