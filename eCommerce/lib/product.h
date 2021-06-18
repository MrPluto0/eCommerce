#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

/**
 * @brief 商品基础
 */
class product
{
private:
    QString name;
    QString intro;
    float price;
    int rest;
    QString store;
public:
    QString type;
    product(QString name,QString intro,float price,int rest, QString store);
    virtual float getPrice();
    QStringList getContentList();
    void updateRest(int num);
};

/**
 * @brief 商品子类:食物类
 */
class food :public product
{
private:
public:
    food(QString name,QString intro,float price,int rest, QString store);
};

/**
 * @brief 商品子类:衣服类
 */
class clothes :public product
{
private:
public:
    clothes(QString name,QString intro,float price,int rest, QString store);
};

/**
 * @brief 商品子类:书籍类
 */
class book :public product
{
private:
public:
    book(QString name,QString intro,float price,int rest, QString store);
};

/**
 * @brief 商品子类:电器类
 */
class machine :public product
{
private:
public:
    machine(QString name,QString intro,float price,int rest, QString store);
};


/**
 * @brief 商品子类:其它类
 */
class other :public product
{
private:
public:
   other(QString name,QString intro,float price,int rest, QString store);
};

#endif // PRODUCT_H
