#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QStringList>
#include "lib/global.h"
#include "lib/jsonexe.h"
#include "lib/sqldiscount.h"
#include "lib/sqlquery.h"
#include "lib/sqlcart.h"

/**
 * @brief 序列化操作指令类
 */
class instruction
{
public:
    instruction();
    /**
     * @brief getResult 获取服务端传输数据的结果
     * @return
     */
    QString getResult();
    /**
     * @brief parseClass 序列化调用的类名
     * @param sL 操作指令
     */
    void parseClass(QStringList sL);
    /**
     * @brief parseJson 序列化调用的JSON类
     * @param method 调用的方法
     * @param params 传入的参数
     */
    void parseJson(QString method,QStringList params);
    /**
     * @brief parseQuery 序列化调用的sqlQuery类
     * @param method 调用的方法
     * @param params 传入的参数
     */
    void parseQuery(QString method,QStringList params);
    /**
     * @brief parseDiscount 序列化调用的sqlDiscount类
     * @param method 调用的方法
     * @param params 传入的参数
     */
    void parseDiscount(QString method,QStringList params);
    /**
     * @brief parseCart 序列化调用的sqlCart类
     * @param method 调用的方法
     * @param params 传入的参数
     */
    void parseCart(QString method,QStringList params);
private:
    QStringList classes;
    QStringList jsonMethods;
    QStringList queryMethod;
    QStringList discountMethods;
    QStringList cartMethods;
    QByteArray result;

    class jsonexe* json;
    class sqlQuery* sqlQ;
    class sqlDiscount* sqlD;
    class sqlCart* sqlC;
};

#endif // INSTRUCTION_H
