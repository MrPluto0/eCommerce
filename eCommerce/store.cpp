#include "store.h"

store::store(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::store)
{
    ui->setupUi(this);
    proType = 0;    //是否分类模式
    mode = 0;       //是否用户模式
    sqlQ = NULL;    //空指针，未分配内存
    sqlD = new sqlDiscount();
    user = NULL;
    pro = NULL;
    json = NULL;
    dia = NULL;
    typeMap << "" << "食物" << "衣服" << "图书" << "电器";
    // 利用lambda表达式向槽函数传参
    connect(ui->food,&QPushButton::clicked,this,[=](){showProducts(1);});
    connect(ui->clothes,&QPushButton::clicked,this,[=](){showProducts(2);});
    connect(ui->book,&QPushButton::clicked,this,[=](){showProducts(3);});
    connect(ui->machine,&QPushButton::clicked,this,[=](){showProducts(4);});
}

store::~store()
{
    delete ui;
    if(sqlQ) delete sqlQ;
    if(model) delete model;
    if(dia) delete dia;
    if(json) delete json;
}

void store::on_close_clicked()
{
    MainWindow* mw = new MainWindow();
    mw->show();
    this->close();
}


void store::getData(QJsonObject user)
{
    ui->stackedWidget->setCurrentIndex(0);
    if(QString::number(user["type"].toInt()) == "1"){
        this->user = new class business(user["username"].toString(),user["password"].toString(),user["acount"].toString().toInt(),user["score"].toString().toInt());
        ui->type->setText("商家");
    }
    else{
        this->user = new class guest(user["username"].toString(),user["password"].toString(),user["acount"].toString().toInt(),user["score"].toString().toInt());
        ui->type->setText("客户");
    }
    QStringList sL = this->user->getInfoList();
    ui->username->setText(sL[0]);
    ui->acount->setText(sL[1]);
    ui->score->setText(sL[2]);
    ui->managePro->setVisible(this->user->getUserType()==1);
    // 初始化json
    this->json = new jsonexe(this->user->getUserType());
}

void store::setHeaders()
{
    this->model = new QStandardItemModel(this);
    QStringList headerList;
    headerList<<"类型"<<"名称"<<"简介"<<"价格"<<"剩余"<<"商家";
    model->setHorizontalHeaderLabels(headerList);
    ui->list->setModel(model);
    ui->list->horizontalHeader()->setVisible(true);
    ui->list->setColumnWidth(0, 60);    //设置列的宽度
    ui->list->setColumnWidth(1, 120);
    ui->list->setColumnWidth(2, 250);
    ui->list->setColumnWidth(3, 50);
    ui->list->setColumnWidth(4, 50);
    ui->list->setColumnWidth(5, 70);
}

void store::on_toIndex_clicked()
{
    QStringList sL = this->user->getInfoList();
    ui->stackedWidget->setCurrentIndex(0);
    ui->username->setText(sL[0]);
    ui->acount->setText(sL[1]);
    ui->score->setText(sL[2]);
}

void store::on_toStore_clicked()
{
    changeMode(0);
    // 跳转页面
    ui->stackedWidget->setCurrentIndex(1);
    showProducts(0);
}

void store::on_toAbout_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// 列出商品信息
void store::showProducts(int proType)
{
    int row=0;
    QSqlQuery query;

    this->proType = proType;
    this->setHeaders();

    // 清理除了表头的数据
    this->model->removeRows(0,model->rowCount());

    // 初始化sqlQ
    if(this->sqlQ)  delete this->sqlQ;
    this->sqlQ = new sqlQuery(typeMap[proType],user->getUsername());

    // 根据模式获取数据库查询结果
    if(mode == 0)
        query = this->sqlQ->selectdb(proType==0);
    else
        query = this->sqlQ->selectStoredb(proType==0);
    while(query.next())
    {
        QStringList sL; //QStringList不能用普通的索引set，但是可以get
        sL << query.value(6).toString();    //商品类型type
        sL << query.value(1).toString();
        sL << query.value(2).toString();
        sL << query.value(3).toString();
        sL << query.value(4).toString();
        sL << query.value(5).toString();
        for (int i=0;i<=5;i++) {
            model->setItem(row,i,new QStandardItem(sL[i]));
            model->item(row,i)->setTextAlignment(Qt::AlignCenter);  //item居中！！！
        }
        row++;
    }
}

// 获取选中行的商品信息
bool store::getProduct()
{
    int row = ui->list->currentIndex().row();
    if(row==-1) // 未选中
        return false;
    QString type = model->data(model->index(row,0)).toString();
    QString name = model->data(model->index(row,1)).toString();
    QString intro = model->data(model->index(row,2)).toString();
    int price = model->data(model->index(row,3)).toInt();
    int rest = model->data(model->index(row,4)).toInt();
    QString store = model->data(model->index(row,5)).toString();
    switch (typeMap.indexOf(type)) {
    case 1:
        pro = new food(name,intro,price,rest,store);
        break;
    case 2:
        pro = new clothes(name,intro,price,rest,store);
        break;
    case 3:
        pro = new book(name,intro,price,rest,store);
        break;
    case 4:
        pro = new machine(name,intro,price,rest,store);
        break;
    default:
        pro = new other(name,intro,price,rest,store);
        break;
    }
    return true;
}

// 改变模式:管理模式/购买模式
void store::changeMode(int mode)
{
    this->mode = mode;
    ui->discount->setVisible(mode==1);
    ui->addPro->setVisible(mode==1);
    ui->editPro->setVisible(mode==1);
    ui->delPro->setVisible(mode==1);
    ui->buyPro->setVisible(mode!=1);
}

// 点击“管理商品”按钮
void store::on_managePro_clicked()
{

    ui->stackedWidget->setCurrentIndex(1);
    changeMode(1);
    showProducts(0);
}

// 点击“购买商品”按钮
void store::on_buyPro_clicked()
{
    if(getProduct()){
        QStringList sL = pro->getContentList();
        float price = pro->getPrice();
        int discount = sqlD->selectdb(sL[5],sL[4]);
        dia = new dialog(this);
        if(dia->buyProduct(price,discount)){
            if(user->purchase(price,discount)){
                json->editJson(user);
                pro->updateRest(1);
                QStringList sL = pro->getContentList();
                sqlQ->updatedb(sL,sL[0],sL[4]);
                this->showProducts(proType);
            }else{
                QMessageBox::information(NULL, "提示", "余额不足");
            }
        }
        delete dia;
    }else{
        QMessageBox::information(NULL, "提示", "请选择商品进行购买");
    }
}

// 点击“添加商品”按钮
void store::on_addPro_clicked()
{
    dia = new dialog(this);
    QStringList dsL = dia->getContent();
    if(dsL[0] == "1" && dsL[1] != ""){
        dsL.removeAt(0);
        dsL<<user->getUsername();
        sqlQ->insertdb(dsL);
        this->showProducts(proType);
    }else{
        QMessageBox::information(NULL,"提示","请输入完整的信息");
    }
    delete dia;
}

// 点击“编辑商品”按钮
void store::on_editPro_clicked()
{
    if(getProduct()){
        QStringList sL = pro->getContentList();
        dia = new dialog(this);
        dia->setLineList(sL);
        QStringList dsL = dia->getContent();
        if(dsL[0] == "1"){
            dsL.removeAt(0);
            sqlQ->updatedb(dsL,sL[0],sL[4]);
            //  sqlQ->outPut();
            this->showProducts(proType);
        }

        delete dia;
    }
    else{
        QMessageBox::information(NULL, "提示", "请选择某行进行编辑");
    }
}

// 点击“删除商品”按钮
void store::on_delPro_clicked()
{
    // tableView中删除remove导致下面获取data失败
    if(getProduct()){
        QString name = pro->getContentList()[0];
        // 在数据库中删除
        sqlQ->deldb(name,user->getUsername());
        this->showProducts(proType);
    }
    else{
        QMessageBox::information(NULL, "提示", "请选择某行进行删除");
    }
}

// 点击“搜索商品”按钮
void store::on_searchPro_clicked()
{
    int row=0;
    qDebug() << ui->searchContent->text();
    QSqlQuery query = sqlQ->searchdb(ui->searchContent->text());
    model->removeRows(0,model->rowCount());
    while(query.next())
    {
        QStringList sL; //QStringList不能用普通的索引set，但是可以get
        sL << query.value(6).toString();    //商品类型type
        sL << query.value(1).toString();
        sL << query.value(2).toString();
        sL << query.value(3).toString();
        sL << query.value(4).toString();
        sL << query.value(5).toString();
        for (int i=0;i<=5;i++) {
            model->setItem(row,i,new QStandardItem(sL[i]));
            model->item(row,i)->setTextAlignment(Qt::AlignCenter);  //item居中！！！
        }
        row++;
    }
}

// 点击“修改密码”按钮
void store::on_editPass_clicked()
{
    // 获取更改密码的信息
    dia = new dialog(this);
    QStringList sL = dia->changePass();
    if(sL[0] == "1"){
        if(!user->editPass(sL)){
            QMessageBox::information(NULL, "提示", "账号或旧密码错误");
        }
        else if(json->editJson(user)){
            QMessageBox::information(NULL, "提示", "密码修改成功");
        }
    }
    delete dia;
}

// 点击“充值”按钮
void store::on_recharge_clicked()
{
    dia = new dialog(this);
    QStringList sL = dia->recharge();
    if(sL[0] == "1"){
        user->recharge(sL[1].toInt());
        if(json->editJson(user)){
            QStringList sL = this->user->getInfoList();
            ui->acount->setText(sL[1]);
            QMessageBox::information(NULL, "提示", "账户充值成功");
        }
    }
    delete dia;
}

// 点击“打折”按钮
void store::on_discount_clicked()
{
    dia = new dialog(this);
    QStringList sL = dia->addDiscount();
    if(sqlD->insertdb(sL[1],sL[2],user->getUsername())){
        QMessageBox::information(NULL, "提示", "打折成功");
    }else{
        QMessageBox::information(NULL, "提示", "输入不完整");
    }
    delete dia;
}
