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
    insert_sql = "insert into product (name,intro,price,rest,type,store,reserved) values(?,?,?,?,?,?,?)";
    update_sql = "update product set name = ?,intro = ?,price = ?,rest = ? where name = ? and store = ?";
    update_reserved_sql = "update product set reserved = reserved + ?,rest = rest - ? where name = ? and store = ?";
    update_reset_sql = "update product set reserved = 0 where name = ? and store = ?";
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

QByteArray sqlQuery::selectdb(bool isAll)
{
    if(db.open()){
        qDebug()<<"[Product Select] Database Opened";
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
        qDebug()<< "[Product Select] Can't open database";
    }
    return transform(query);
}

QByteArray sqlQuery::selectStoredb(bool isAll)
{
    if(db.open()){
        qDebug()<<"[Product Select] Database Opened";
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
        qDebug()<< "[Product Select] Can't open database";
    }
    return transform(query);
}

QByteArray sqlQuery::searchdb(QString searchContent)
{

    if(db.open()){
        qDebug()<<"[Product Search] Database Opened";
        //查询所有记录
        query.prepare(search_sql);
        query.addBindValue("%"+searchContent+"%");
        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
    }else{
        qDebug()<< "[Product Search] Can't open database";
    }
    return transform(query);
}

QByteArray sqlQuery::transform(QSqlQuery query)
{
    QJsonDocument jdoc;
    QJsonArray jarr;
    while(query.next())
    {
        QJsonObject jobj;
        jobj.insert("name",query.value(1).toString());
        jobj.insert("intro",query.value(2).toString());
        jobj.insert("price",query.value(3).toString());
        jobj.insert("store",query.value(5).toString());
        jobj.insert("rest",query.value(4).toString());
        jobj.insert("type",query.value(6).toString());
        jarr.append(jobj);
    }
    jdoc.setArray(jarr);
    return jdoc.toJson();
}

void sqlQuery::outPut()
{
    if(db.open()){
        qDebug()<<"[Product Output] Database Opened";
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
            QString type = query.value(6).toString();
            QString reserved = query.value(7).toString();
            qDebug()<<QString("ID:%1  Name:%2  Address:%3 Price:%4 Rest:%5 Store:%6 Type:%7 Reserved:%8")
                      .arg(id).arg(name).arg(intro).arg(price).arg(rest).arg(store).arg(type).arg(reserved);
        }
    }else{
        qDebug()<< "[Product Output] Can't open database";
    }
}

bool sqlQuery::insertdb(QStringList sL)
{
    if(db.open()){
        qDebug()<<"[Product Insert] Database Opened";
        query.prepare(insert_sql);
        query.addBindValue(sL[0]);
        query.addBindValue(sL[1]);
        query.addBindValue(sL[2].toInt());
        query.addBindValue(sL[3].toInt());
        query.addBindValue(sL[4]);
        query.addBindValue(sL[5]);
        query.addBindValue(0);
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug() << "[Product Insert] Inserted Success!";
            return true;
        }
    }else{
        qDebug() << "[Product Insert] Inserted Failed!";
        return false;
    }
}

bool sqlQuery::updatedb(QStringList sL,QString name,QString store)
{
    if(db.open()){
        qDebug()<<"[Product Update] Database Opened";
        if(sL.length() == 1){
            query.prepare(update_reserved_sql);
            query.addBindValue(sL[0]);
            query.addBindValue(sL[0]);
            query.addBindValue(name);
            query.addBindValue(store);
        }else{
            query.prepare(update_sql);
            query.addBindValue(sL[0]);
            query.addBindValue(sL[1]);
            query.addBindValue(sL[2].toInt());
            query.addBindValue(sL[3].toInt());
            query.addBindValue(name);
            query.addBindValue(store);
        }
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug() << "[Product Update] Updated Success!";
            return true;
        }
    }else{
        qDebug() << "[Product Update] Updated Failed!";
        return false;
    }
}

bool sqlQuery::updateResetDb(QString name, QString store)
{
    if(db.open()){
        qDebug()<<"[Product Update] Database Opened";
        query.prepare(update_reset_sql);
        query.addBindValue(name);
        query.addBindValue(store);
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug() << "[Product Update] Updated Success!";
            return true;
        }
    }else{
        qDebug() << "[Product Update] Updated Failed!";
        return false;
    }
}


bool sqlQuery::deldb(QString name,QString store)
{
    if(db.open()){
        qDebug()<<"[Product Delete] Database Opened";
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
            qDebug() << "[Product Delete] Delete Success!";
            return true;
        }
    }else{
        qDebug() << "[Product Delete] Delete Failed!";
        return false;
    }
}
