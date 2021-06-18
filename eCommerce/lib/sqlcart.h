#ifndef SQLCART_H
#define SQLCART_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QDir>

class sqlCart
{
public:
    sqlCart();
    ~sqlCart();
    QSqlQuery selectdb(QString username);
    bool insertdb(QStringList sL);
    bool updatedb(QStringList sL);
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
