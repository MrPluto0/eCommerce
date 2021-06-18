#ifndef JSONEXE_H
#define JSONEXE_H

#include "lib/user.h"
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

/**
 * @brief JSON文件的操作类
 */
class jsonexe
{
public:
    jsonexe(int type);
    ~jsonexe();

    /**
     * @brief 获取某用户的信息
     * @param username 用户名
     * @param password 密码
     * @return 某用户的JSON对象
     */
    QJsonObject getJson(QString username,QString password);

    /**
     * @brief 判断是否已存在某用户名
     * @return 是否存在某用户名
     */
    bool judgeJson(QString username);

    /**
     * @brief 编辑用户uu的信息
     * @param uu 某用户信息的类
     * @return 是否编辑成功
     */
    bool editJson(user* uu);

    /**
     * @brief appendJson 添加
     * @param username
     * @param password
     * @return
     */
    bool appendJson(QString username,QString password);
private:
    int type;
    QString path;
    QFile file;
    QJsonDocument jdoc;
    QJsonArray users;
};

#endif // JSONEXE_H
