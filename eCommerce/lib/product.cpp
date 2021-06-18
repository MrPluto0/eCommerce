#include "product.h"
#include <QStringList>

product::product(QString name, QString intro, float price, int rest, QString store)
{
    this->name = name;
    this->intro = intro;
    this->price = price;
    this->rest = rest;
    this->store = store;
}

float product::getPrice()
{
    return price;
}

QStringList product::getContentList()
{
    QStringList sL;
    sL << name << intro << QString::number(price) << QString::number(rest) << store << type;
    return sL;
}

void product::updateRest(int num)
{
    rest-=num;
}

food::food(QString name, QString intro, float price, int rest, QString store)
    :product(name, intro, price, rest, store)
{
    type = "食物";
}

clothes::clothes(QString name, QString intro, float price, int rest, QString store)
    :product(name, intro, price, rest,store)
{
    type = "衣服";
}

book::book(QString name, QString intro, float price, int rest, QString store)
    :product(name, intro, price, rest,store)
{
    type = "书籍";
}

machine::machine(QString name, QString intro, float price, int rest, QString store)
    :product(name, intro, price, rest,store)
{
    type = "电器";
}

other::other(QString name, QString intro, float price, int rest, QString store)
    :product(name, intro, price, rest,store)
{
    type = "其它";
}
