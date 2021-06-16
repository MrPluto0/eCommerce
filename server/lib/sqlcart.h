#ifndef SQLCART_H
#define SQLCART_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QDir>

/**
 * @brief 购物车数据库类
 */
class sqlCart
{
public:
    sqlCart();
    ~sqlCart();
    /**
     * @brief selectdb 获取某个用户下的购物车商品
     * @param username 用户名
     * @return 商品列表Json信息
     */
    QByteArray selectdb(QString username);
    /**
     * @brief insertdb 向购物车数据库添加某个商品
     * @param (sL[0]~sL[4]) => (name,price,number,store,user)
     * @return 是否添加成功
     */
    bool insertdb(QStringList sL);
    /**
     * @brief updatedb  更新某个用户的某个商品的购买量
     * @param (sL[0]~sL[2]) => (number,name,user)
     * @return 是否更新成功
     */
    bool updatedb(QStringList sL);
    /**
     * @brief deletedb
     * @param 两种情况一个是全删除，一个是删除某个商品
     * @return 是否删除成功
     */
    bool deletedb(QStringList sL);

private:
    QSqlDatabase db;
    QSqlQuery query;
    QString insert_sql;
    QString update_sql;
    QString delete_sql,delete_all_sql;
    QString select_sql;
};

#endif // SQLCART_H
