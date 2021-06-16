#ifndef SQLQUERY_H
#define SQLQUERY_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QDir>

class sqlQuery
{
public:
    sqlQuery(QString proType = NULL,QString store = NULL);
    ~sqlQuery();
    QSqlQuery selectdb(bool isAll);
    QSqlQuery selectStoredb(bool isAll);
    QSqlQuery searchdb(QString searchContent);
    void outPut();
    bool insertdb(QStringList sL);
    bool updatedb(QStringList sL,QString name,QString store);
    bool updateResetDb(QString name,QString store);
    bool deldb(QString name,QString store);
private:
    QSqlDatabase db;
    QSqlQuery query;
    QString proType,store;
    QString create_sql;
    QString insert_sql;
    QString update_sql,update_reserved_sql,update_reset_sql;
    QString delete_sql;
    QString select_all_sql,select_all_type;
    QString select_store_sql,select_store_type;
    QString search_sql;
};

#endif // SQLQUERY_H
