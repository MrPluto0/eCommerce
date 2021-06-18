#include "jsonexe.h"
#include <QDebug>

jsonexe::jsonexe()
{
    path = QDir::currentPath() + "/../user.json";
    qDebug() << path;
    file.setFileName(path);
}

jsonexe::~jsonexe()
{

}

QByteArray jsonexe::getJson(QStringList sL)
{
    QJsonObject userobj,user;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        jdoc = QJsonDocument::fromJson(file.readAll());
        users = jdoc.array();
        int size = users.size();
        for (int i=0;i<size;i++) {
            user = users.at(i).toObject();
            if(user["username"] == sL[0] && user["password"] == sL[1]){
                userobj = user;
                break;
            }
        }
    }else{
        qDebug() << "open file failed";
    }
    file.close();
    jdoc.setObject(userobj);
    return jdoc.toJson();
}

bool jsonexe::judgeJson(QString username)
{
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll());
        QJsonArray users = jdoc.array();
        int size = users.size();
        for (int i=0;i<size;i++) {
            QJsonObject user = users.at(i).toObject();
            qDebug() << user["username"];
            if(user["username"] == username){
                return false;
            }
        }
    }
    file.close();
    return true;
}


bool jsonexe::editJson(QStringList sL)
{
    qDebug() << "EditJson" << sL;
    // 获取已存在的users
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "File opened failed";
        return false;
    }
    jdoc = QJsonDocument::fromJson(file.readAll());
    users = jdoc.array();
    file.close();
    // 验证信息，更改密码
    QJsonArray newUsers;
    for (int i=0;i<users.size();i++) {
        QJsonObject userobj = users.at(i).toObject();
        if(userobj["username"] == sL[0]){
            if(sL.length() == 2){
                QString acount = userobj["acount"].toString();
                float total = acount.toFloat() + sL[1].toFloat();
                userobj["acount"] = QString::number(total);
                qDebug() << "acount:" << userobj["acount"];
            }
            else{
                userobj["acount"] = sL[1];
                userobj["score"] = sL[2];
                userobj["password"] = sL[3];
            }
        }
        newUsers.append(userobj);
    }
    //将newUsers重新写入文件
    jdoc.setArray(newUsers);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug() << "File opened failed";
        return false;
    }
    file.write(jdoc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool jsonexe::appendJson(QStringList sL)
{
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "File opened failed";
        return false;
    }
    jdoc = QJsonDocument::fromJson(file.readAll());
    users = jdoc.array();
    file.close();

    if(this->judgeJson(sL[0])){
        QJsonObject userobj;
        userobj["username"] = sL[0];
        userobj["password"] = sL[1];
        userobj["type"] = sL[2];
        userobj["acount"] = 0;
        userobj["score"] = 0;
        users.append(userobj);
        jdoc.setArray(users);

        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.write(jdoc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }else{
        return false;
    }
}
