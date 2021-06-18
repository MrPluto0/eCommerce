#ifndef USER_H
#define USER_H

#include <QString>

/**
 * @brief 用户基类
 */
class user
{
private:
    QString username;
    QString password;
    int type;
    float acount;
    int score;
public:
    user(QString _username,QString _password,float _acount,int _score,int _type);
    QString getUsername() const;
    virtual int getUserType()=0;
    QStringList getInfoList() const;
    bool editPass(QStringList oldInfo);
    void recharge(int money);
    bool purchase(float price,int discount);
};

/**
 * @brief 用户子类：顾客
 */
class guest:public user
{
public:
    guest(QString _username,QString _password,int _acount,int _score);
    int getUserType();
};

/**
 * @brief 用户子类：商家
 */
class business:public user
{
public:
    business(QString _username,QString _password,int _acount,int _score);
    int getUserType();
    void addObj();
};

#endif // USER_H
