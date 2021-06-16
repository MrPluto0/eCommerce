#include "sqlcart.h"

sqlCart::sqlCart()
{
    // 查看是否已经建立基于QSQLITE的数据库
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/../product.db");
    query = QSqlQuery(db); // 将sql进行封装时，需要加上该句
    insert_sql = "insert into cart (name,price,number,store,user) values(?,?,?,?,?)";
    update_sql = "update cart set number = ? where name = ? and user = ?";
    delete_sql = "delete from cart where name = ? and user = ?";
    delete_all_sql = "delete from cart where user = ?";
    select_sql = "select * from cart where user = ?";
}

QByteArray sqlCart::selectdb(QString username)
{
    QJsonDocument jdoc;
    QJsonArray jarr;
    if(db.open()){
        qDebug()<<"[Cart Select] Database Opened";
        //查询所有记录
        query.prepare(select_sql);
        query.addBindValue(username);
        if(!query.exec())
        {
            qDebug()<<query.lastError();
        }
        while(query.next())
        {
            QJsonObject jobj;
            jobj.insert("name",query.value(1).toString());
            jobj.insert("price",query.value(2).toString());
            jobj.insert("number",query.value(3).toString());
            jobj.insert("store",query.value(4).toString());
            jobj.insert("total",QString::number(query.value(2).toFloat()*query.value(3).toInt()));
            jarr.append(jobj);
        }

        jdoc.setArray(jarr);
    }
    else{
        qDebug()<< "[Cart Select] Can't open database";
    }
    return jdoc.toJson();
}

bool sqlCart::insertdb(QStringList sL)
{

    if(db.open()){
        qDebug()<<"[Cart Insert] Database Opened";
        //查询所有记录
        query.prepare(insert_sql);
        query.addBindValue(sL[0]);
        query.addBindValue(sL[1]);
        query.addBindValue(sL[2]);
        query.addBindValue(sL[3]);
        query.addBindValue(sL[4]);
        if(!query.exec())
        {
            qDebug()<<query.lastError();
            return false;
        }        else
        {
            qDebug() << "[Cart Insert] Inserted Success!";
            return true;
        }
    }
    else{
        qDebug()<< "[Cart Insert] Can't open database";
        return false;
    }
    return true;
}

bool sqlCart::updatedb(QStringList sL)
{
    if(db.open()){
        qDebug()<<"[Cart Update] Database Opened";
        query.prepare(update_sql);
        query.addBindValue(sL[0]);
        query.addBindValue(sL[1]);
        query.addBindValue(sL[2]);
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }else
        {
            qDebug() << "[Cart Update] Updated Success!";
            return true;
        }
    }else{
        qDebug() << "[Cart Update] Updated Failed!";
        return false;
    }
}

bool sqlCart::deletedb(QStringList sL)
{
    if(db.open()){
        qDebug()<<"[Cart Delete] Database Opened";
        if(sL.length() == 2){
            query.prepare(delete_sql);
            query.addBindValue(sL[0]);
            query.addBindValue(sL[1]);
        }else{
            query.prepare(delete_all_sql);
            query.addBindValue(sL[0]);
        }

        if(!query.exec())
        {
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug() << "[Cart Delete] Delete Success!";
            return true;
        }
    }else{
        qDebug() << "[Cart Delete] Delete Failed!";
        return false;
    }
}
