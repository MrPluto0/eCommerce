#include "sqlquery.h"


sqlQuery::sqlQuery(QString proType,QString store)
{
    this->proType = proType;
    this->store = store;
    // 查看是否已经建立基于QSQLITE的数据库
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/../product.db");
    query = QSqlQuery(db); // 将sql进行封装时，需要加上该句
    insert_sql = "insert into product (name,intro,price,rest,type,store) values(?,?,?,?,?,?)";
    update_sql = "update product set name = ?,intro = ?,price = ?,rest = ? where name = ? and store = ?";
    delete_sql = "delete from product where name = ? and store = ?";
    select_all_sql = "select * from product";
    select_all_type = "select * from product where type = ?";
    select_store_sql = "select * from product where store = ?";
    select_store_type = "select * from product where store = ? and type = ?";
    search_sql = "select * from product where name like ?";
}

sqlQuery::~sqlQuery()
{
    db.close();
}

QSqlQuery sqlQuery::selectdb(bool isAll)
{
    if(db.open()){
        qDebug()<<"Database Opened:select";
        //查询所有记录
        if(isAll){
            query.prepare(select_all_sql);
        }
        else{
            query.prepare(select_all_type);
            query.addBindValue(proType);
        }
        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
    }else{
        qDebug()<< "Can't open database";
    }
    return query;
}

QSqlQuery sqlQuery::selectStoredb(bool isAll)
{
    if(db.open()){
        qDebug()<<"Database Opened:select";
        //查询所有记录
        if(isAll){
            query.prepare(select_store_sql);
            query.addBindValue(store);
        }else{
            query.prepare(select_store_type);
            query.addBindValue(store);
            query.addBindValue(proType);
        }
        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
    }else{
        qDebug()<< "Can't open database";
    }
    return query;
}

QSqlQuery sqlQuery::searchdb(QString searchContent)
{
    if(db.open()){
        qDebug()<<"Database Opened:search";
        //查询所有记录
        query.prepare(search_sql);
        query.addBindValue("%"+searchContent+"%");
        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
    }else{
        qDebug()<< "Can't open database";
    }
    return query;
}



void sqlQuery::outPut()
{
    if(db.open()){
        qDebug()<<"Database Opened:output";
        //查询所有记录
        query.prepare(select_all_sql);
        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString intro = query.value(2).toString();
            float price = query.value(3).toInt();
            int rest = query.value(4).toInt();
            QString store = query.value(5).toString();
            QString typee = query.value(6).toString();
            qDebug()<<QString("ID:%1  Name:%2  Address:%3 Price:%4 Rest:%5 Store:%6 Type:%7")
                      .arg(id).arg(name).arg(intro).arg(price).arg(rest).arg(store).arg(typee);
        }
    }else{
        qDebug()<< "Can't open database";
    }
}

bool sqlQuery::insertdb(QStringList sL)
{
    if(db.open()){
        qDebug()<<"Database Opened:insert";
        query.prepare(insert_sql);
        query.addBindValue(sL[0]);
        query.addBindValue(sL[1]);
        query.addBindValue(sL[2].toInt());
        query.addBindValue(sL[3].toInt());
        query.addBindValue(sL[4]);
        query.addBindValue(sL[5]);
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug() << "inserted Success!";
            return true;
        }
    }else{
        qDebug() << "inserted Failed!";
        return false;
    }
}

bool sqlQuery::updatedb(QStringList sL,QString name,QString store)
{
    if(db.open()){
        qDebug()<<"Database Opened:update";
        query.prepare(update_sql);
        query.addBindValue(sL[0]);
        query.addBindValue(sL[1]);
        query.addBindValue(sL[2].toInt());
        query.addBindValue(sL[3].toInt());
        query.addBindValue(name);
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

bool sqlQuery::deldb(QString name,QString store)
{
    if(db.open()){
        qDebug()<<"Database Opened:delete";
        query.prepare(delete_sql);
        query.addBindValue(name);
        query.addBindValue(store);
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug() << "delete Success!";
            return true;
        }
    }else{
        qDebug() << "delete Failed!";
        return false;
    }
}






