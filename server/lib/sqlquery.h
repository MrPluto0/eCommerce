#ifndef SQLQUERY_H
#define SQLQUERY_H
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
 * @brief 商品数据库类
 */
class sqlQuery
{
public:
    sqlQuery();
    ~sqlQuery();
    /**
     * @brief selectdb 获取某个类型的商品列表
     * @param proType 商品类型
     * @return 商品列表的JSON
     */
    QByteArray selectdb(QString proType);
    /**
     * @brief selectStoredb 获取某个商品某个类型的商品列表
     * @param proType 商品类型
     * @param store 商家名
     * @return 商品列表的JSON
     */
    QByteArray selectStoredb(QString proType,QString store);
    /**
     * @brief searchdb 根据关键词模糊搜索得到的商品列表
     * @param searchContent 商品关键词
     * @return 商品列表的JSON
     */
    QByteArray searchdb(QString searchContent);
    /**
     * @brief transform 将QsqlQuery的内容转换为QByte类型
     * @param query sql的结果
     * @return 商品列表的JSON
     */
    QByteArray transform(QSqlQuery query);
    /**
     * @brief outPut 输入数据库内容
     */
    void outPut();
    /**
     * @brief insertdb 插入数据库内容
     * @param (sL[0]~sL[6]) => (name,intro,price,rest,type,store,reserved)
     * @return 是否插入成功
     */
    bool insertdb(QStringList sL);
    /**
     * @brief updatedb 更新某个商品列表
     * @param 两种类型，一种是更新展示的信息，一种是更新预定量和库存
     * @return 是否更新成功
     */
    bool updatedb(QStringList sL);
    /**
     * @brief updateResetDb 重置某商店某商品的预定量为0
     * @param name 商品名
     * @param store 预定量
     * @return 是否更新成功
     */
    bool updateResetDb(QString name,QString store);
    /**
     * @brief deletedb 删除某个商店的某个商品
     * @param name 商品名
     * @param store 店名
     * @return 是否删除成功
     */
    bool deletedb(QString name,QString store);
private:
    QStringList typeMap;
    QSqlDatabase db;
    QSqlQuery query;
    QString create_sql;
    QString insert_sql;
    QString update_sql,update_reserved_sql,update_reset_sql;
    QString delete_sql;
    QString select_all_sql,select_all_type;
    QString select_store_sql,select_store_type;
    QString search_sql;
};

#endif // SQLQUERY_H
