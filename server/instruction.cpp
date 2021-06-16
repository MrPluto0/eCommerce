#include "instruction.h"

instruction::instruction()
{
    this->classes << "JsonExe" << "SqlQuery" << "SqlDiscount" << "SqlCart";
    this->jsonMethods << "GetJson" << "JudgeJson" << "EditJson" << "AppendJson";
    this->queryMethod << "Selectdb" << "Insertdb" << "Updatedb" << "Deletedb"
                      << "SelectStoredb" << "Searchdb" << "UpdateResetDb" << "OutPut";
    this->discountMethods << "Selectdb" << "Insertdb" << "Updatedb";
    this->cartMethods << "Selectdb" << "Insertdb" << "Updatedb" << "Deletedb";

    json = new jsonexe();
    sqlQ = new sqlQuery();
    sqlD = new sqlDiscount();
    sqlC = new sqlCart();
}

QString instruction::getResult()
{
    return QString(result);
}

void instruction::parseClass(QStringList sL)
{
    qDebug() << "[ParseClass]";
    QString cla = sL[0];
    QString method = sL[1];
    sL.removeAt(0);sL.removeAt(0);
    QStringList params = sL;
    switch (classes.indexOf(cla)) {
    case JsonExe:
        parseJson(method,params);
        break;
    case SqlQuery:
        parseQuery(method,params);
        break;
    case SqlDiscount:
        parseDiscount(method,params);
        break;
    case SqlCart:
        parseCart(method,params);
        break;
    default:
        qDebug() << "error class";
    }
}

void instruction::parseJson(QString method, QStringList params)
{
    qDebug() << "[ParseJson]";
    switch (jsonMethods.indexOf(method)) {
    case GetJson:
        result = json->getJson(params);
        break;
    case JudgeJson:
        if(json->judgeJson(params[0])) result = "1";
        else result = "0";
        break;
    case EditJson:
        if(json->editJson(params)) result = "1";
        else result = "0";
        break;
    case AppendJson:
        if(json->appendJson(params)) result = "1";
        else result = "0";
        break;
    default:
        qDebug() << "error method";
    }
}

void instruction::parseQuery(QString method, QStringList params)
{
    qDebug() << "[ParseQuery]";
    switch (queryMethod.indexOf(method)) {
    case Selectdb:
        result = sqlQ->selectdb(params[0]);
        break;
    case Insertdb:
        if(sqlQ->insertdb(params)) result = "1";
        else result = "0";
        break;
    case Updatedb:
        if(sqlQ->updatedb(params)) result = "1";
        else result = "0";
        break;
    case Deletedb:
        if(sqlQ->deletedb(params[0],params[1])) result = "1";
        else result = "0";
        break;
    case SelectStoredb:
        result = sqlQ->selectStoredb(params[0],params[1]);
        break;
    case Searchdb:
        result = sqlQ->searchdb(params[0]);
        break;
    case UpdateResetDb:
        if(sqlQ->updateResetDb(params[0],params[1])) result = "1";
        else result = "0";
        break;
    case OutPut:
        sqlQ->outPut();
        break;
    default:
        qDebug() << "error method";
    }
}

void instruction::parseDiscount(QString method, QStringList params)
{
    qDebug() << "[ParseDiscount]";
    switch (discountMethods.indexOf(method)) {
    case Selectdb:
        result = QByteArray::number(sqlD->selectdb(params[0],params[1]));
        break;
    case Insertdb:
        if(sqlD->insertdb(params[0],params[1],params[2])) result = "1";
        else result = "0";
        break;
    case Updatedb:
        if(sqlD->updatedb(params[0],params[1],params[2])) result = "1";
        else result = "0";
        break;
    default:
        qDebug() << "error method";
    }
}

void instruction::parseCart(QString method, QStringList params)
{
    qDebug() << "[ParseCart]";
    switch (cartMethods.indexOf(method)) {
    case Selectdb:
        result = sqlC->selectdb(params[0]);
        break;
    case Insertdb:
        if(sqlC->insertdb(params)) result = "1";
        else result = "0";
        break;
    case Updatedb:
        if(sqlC->updatedb(params)) result = "1";
        else result = "0";
        break;
    case Deletedb:
        if(sqlC->deletedb(params)) result = "1";
        else result = "0";
        break;
    default:
        qDebug() << "error method";
    }
}
