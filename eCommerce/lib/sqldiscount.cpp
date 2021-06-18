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
        qDebug()<<"Database Opened:select";
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
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            int discount = query.value(2).toInt();
            QString store = query.value(3).toString();
            qDebug()<<QString("ID:%1  Name:%2  Discount:%3 Store:%4")
                      .arg(id).arg(name).arg(discount).arg(store);
            return discount;
        }
    }
    else{
        qDebug()<< "Can't open database";
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
            qDebug()<<"Database Opened:insert";
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
            qDebug()<< "Can't open database";
            return false;
        }
        return true;
    }
}

bool sqlDiscount::updatedb(QString proType, QString discount, QString store)
{
    if(db.open()){
        qDebug()<<"Database Opened:update";
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
            qDebug() << "updated Success!";
            return true;
        }
    }else{
        qDebug() << "updated Failed!";
        return false;
    }
}

