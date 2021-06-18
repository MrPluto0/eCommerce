#ifndef JSONEXE_H
#define JSONEXE_H

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
    jsonexe();
    ~jsonexe();

    /**
     * @brief 获取某用户的信息
     * @param sL[0]->username;sL[1]->password
     * @return 某用户的对象字符串
     */
    QByteArray getJson(QStringList sL);

    /**
     * @brief judgeJson 判断是否存在某用户
     * @param username 用户名
     * @return bool是否存在
     */
    bool judgeJson(QString username);

    /**
     * @brief 编辑用户uu的信息
     * @param sL[0]->username;sL[1]->acount;sL[2]score;sL[3]->password
     * @return 是否编辑成功
     */
    bool editJson(QStringList sL);

    /**
     * @brief appendJson 添加
     * @param sL[0]->username;sL[1]->password;sL[2]->type
     * @return
     */
    bool appendJson(QStringList sL);
private:
    QString path;
    QFile file;
    QJsonDocument jdoc;
    QJsonArray users;
};

#endif // JSONEXE_H
