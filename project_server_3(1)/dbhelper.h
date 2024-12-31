#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlDatabase>
#include "pack.h"

class DbHelper
{
private:
    DbHelper();
    DbHelper(const DbHelper&);
    ~DbHelper();

public:
    static DbHelper& get_instance()
    {
        static DbHelper instance;

        return instance;
    }

public:
    bool init_db();  // 初始化数据库（做连接）
    Register_State user_register(const QString& strname, const QString& strpwd, const QString& strConfirmPwd);  // 用户注册功能
    Login_State user_login(const QString& strname, const QString& strpwd);     // 用户登录功能

private:
    QSqlDatabase m_db;
};

#endif // DBHELPER_H
