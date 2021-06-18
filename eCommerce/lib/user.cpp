#include "user.h"
#include <QStringList>
#include <QtDebug>

user::user(QString _username, QString _password, float _acount, int _score, int _type)
{
    username = _username;
    password = _password;
    acount = _acount;
    score = _score;
    type = _type;
}

QString user::getUsername() const
{
    return username;
}

QStringList user::getInfoList() const
{
    QStringList sL;
    sL << username << QString::number(acount) << QString::number(score) << password;
    return sL;
}

bool user::editPass(QStringList oldInfo)
{
    if(username == oldInfo[1] && password == oldInfo[2]){
        password = oldInfo[3];
        return true;
    }else{
        return  false;
    }
}

void user::recharge(int money)
{
    acount += money;
}

bool user::purchase(float price,int discount)
{
    if(discount!=0)
        price = price*discount/10;
    if(acount >= price){
        acount -= price;
        return true;
    }
    else{
        return false;
    }
}

guest::guest(QString _username, QString _password,int _acount,int _score)
    :user(_username,_password,_acount,_score,0)
{
}

int guest::getUserType()
{
    return 0;
}

business::business(QString _username, QString _password,int _acount,int _score)
    :user(_username,_password,_acount,_score,1)
{
}

int business::getUserType()
{
    return 1;
}
