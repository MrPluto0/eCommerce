#ifndef SQLDISCOUNT_H
#define SQLDISCOUNT_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QDir>

class sqlDiscount
{
public:
    sqlDiscount();
    ~sqlDiscount();
    int selectdb(QString name,QString store);
    bool insertdb(QString proType,QString discount,QString store);
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
