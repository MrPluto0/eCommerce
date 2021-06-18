#include "sqldiscount.h"

sqlDiscount::sqlDiscount()
{
    // 查看是否已经建立基于QSQLITE的数据库
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/../product.db");
    query = QSqlQuery(db); // 将sql进行封装时，需要加上该句
    insert_sql = "insert into discount (name,discount,store) values(?,?,?)";
    update_sql = "update discount set discount = ? where name = ? and store = ?";
    delete_sql = "delete from discount where name = ? and store = ?";
    select_sql = "select * from discount where name = ? and store = ?";
}

int sqlDiscount::selectdb(QString proType, QString store)
{
    if(db.open()){
        qDebug()<<"[Discount Select] Database Opened";
        //查询所有记录
        query.prepare(select_sql);
        query.addBindValue(proType);
        query.addBindValue(store);
        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
        while(query.next())
        {
            int discount = query.value(2).toInt();
            return discount;
        }
    }
    else{
        qDebug()<< "[Discount Select] Can't open database";
    }
    return 0;
}

bool sqlDiscount::insertdb(QString proType, QString discount, QString store)
{
    if(discount == "")
        return false;
    // 判断是修改还是创建折扣
    if(this->selectdb(proType,store)!=0){
        return this->updatedb(proType,discount,store);
    }else{
        if(db.open()){
            qDebug()<<"[Discount Insert] Database Opened";
            //查询所有记录
            query.prepare(insert_sql);
            query.addBindValue(proType);
            query.addBindValue(discount.toInt());
            query.addBindValue(store);
            if(!query.exec())
            {
                qDebug()<<query.lastError();
                return false;
            }
        }
        else{
            qDebug()<< "[Discount Insert] Can't open database";
            return false;
        }
        return true;
    }
}

bool sqlDiscount::updatedb(QString proType, QString discount, QString store)
{
    if(db.open()){
        qDebug()<<"[Discount Update] Database Opened";
        query.prepare(update_sql);
        query.addBindValue(discount.toInt());
        query.addBindValue(proType);
        query.addBindValue(store);
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug() << "[Discount Update] Updated Success!";
            return true;
        }
    }else{
        qDebug() << "[Discount Update] Updated Failed!";
        return false;
    }
}

