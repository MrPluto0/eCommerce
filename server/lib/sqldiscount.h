#ifndef SQLDISCOUNT_H
#define SQLDISCOUNT_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QDir>

/**
 * @brief 折扣数据库类
 */
class sqlDiscount
{
public:
    sqlDiscount();
    ~sqlDiscount();
    /**
     * @brief selectdb 获取某个店某个商品的折扣
     * @param name 商品名
     * @param store 点名
     * @return 折扣1-10
     */
    int selectdb(QString name,QString store);
    /**
     * @brief insertdb 插入某个商品的折扣
     * @param proType 商品类型
     * @param discount 折扣数
     * @param store 店名
     * @return 是否插入成功
     */
    bool insertdb(QString proType,QString discount,QString store);
    /**
     * @brief updatedb 更新某个商品的折扣
     * @param proType 商品类型
     * @param discount 折扣数
     * @param store 店名
     * @return 是否更新成功
     */
    bool updatedb(QString proType,QString discount,QString store);
private:
    QSqlDatabase db;
    QSqlQuery query;
    QString insert_sql;
    QString update_sql;
    QString delete_sql;
    QString select_sql;
};

#endif // SQLDISCOUNT_H
