#include "store.h"

store::store(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::store)
{
    // 设置关闭窗口调用析构函数
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    proType = 0;    //是否分类模式
    mode = 0;       //是否用户模式
    sqlQ = new sqlQuery();
    sqlD = new sqlDiscount();
    sqlC = new sqlCart();
    dia = new dialog(this);
    user = NULL;
    pro = NULL;
    json = NULL;
    typeMap << "" << "食物" << "衣服" << "图书" << "电器";

    // 利用lambda表达式向槽函数传参
    connect(ui->food,&QPushButton::clicked,this,[=](){showProducts(1);});
    connect(ui->clothes,&QPushButton::clicked,this,[=](){showProducts(2);});
    connect(ui->book,&QPushButton::clicked,this,[=](){showProducts(3);});
    connect(ui->machine,&QPushButton::clicked,this,[=](){showProducts(4);});

    // 设置两个tableView的表头
    this->setHeaders();
    this->setCartHeaders();

    // 设置两个按钮不可见
    ui->cancelorder->setVisible(false);
    ui->pay->setVisible(false);
}

store::~store()
{
    if(this->cartmodel->rowCount()!=0){
        this->on_cancelorder_clicked();
    }

    delete ui;
    if(sqlQ) delete sqlQ;
    if(model) delete model;
    if(dia) delete this->dia;
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
    QStringList headerList;
    headerList<<"类型"<<"名称"<<"简介"<<"价格"<<"剩余"<<"商家";
    this->model = new QStandardItemModel(this);
    this->model->setHorizontalHeaderLabels(headerList);
    ui->list->setModel(model);
    ui->list->horizontalHeader()->setVisible(true);
    ui->list->setColumnWidth(0, 60);    //设置列的宽度
    ui->list->setColumnWidth(1, 120);
    ui->list->setColumnWidth(2, 250);
    ui->list->setColumnWidth(3, 50);
    ui->list->setColumnWidth(4, 50);
    ui->list->setColumnWidth(5, 70);
}

void store::setCartHeaders()
{
    QStringList headerList;
    headerList<<"名称"<<"价格"<<"数量"<<"商家"<<"总计";
    this->cartmodel = new QStandardItemModel(this);
    this->cartmodel->setHorizontalHeaderLabels(headerList);
    ui->cartlist->setModel(cartmodel);
    ui->cartlist->horizontalHeader()->setVisible(true);
    ui->cartlist->setColumnWidth(0, 100);    //设置列的宽度
    ui->cartlist->setColumnWidth(1, 100);
    ui->cartlist->setColumnWidth(2, 100);
    ui->cartlist->setColumnWidth(3, 100);
    ui->cartlist->setColumnWidth(4, 100);
}

// 跳转主页
void store::on_toIndex_clicked()
{
    QStringList sL = this->user->getInfoList();
    ui->stackedWidget->setCurrentIndex(0);
    ui->username->setText(sL[0]);
    ui->acount->setText(sL[1]);
    ui->score->setText(sL[2]);
}

// 跳转购物车
void store::on_toCart_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    this->cartmodel->removeRows(0,cartmodel->rowCount());

    QSqlQuery query= sqlC->selectdb(this->user->getUsername());;
    while(query.next())
    {
        int row = this->cartmodel->rowCount();
        QStringList sL; //QStringList不能用普通的索引set，但是可以get
        sL << query.value(1).toString();
        sL << query.value(2).toString();
        sL << query.value(3).toString();
        sL << query.value(4).toString();
        sL << QString::number(query.value(2).toInt()*query.value(3).toInt());
        for (int i=0;i<=4;i++) {
            this->cartmodel->setItem(row,i,new QStandardItem(sL[i]));
            this->cartmodel->item(row,i)->setTextAlignment(Qt::AlignCenter);  //item居中！！！
        }
    }
}

// 跳转商店
void store::on_toStore_clicked()
{
    this->changeMode(0);
    // 跳转页面
    ui->stackedWidget->setCurrentIndex(2);
    showProducts(0);
}

// 跳转关于
void store::on_toAbout_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// 列出商品信息
void store::showProducts(int proType)
{
    int row=0;
    QSqlQuery query;

    this->proType = proType;

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
bool store::getProduct(int flag)
{
    int row,price,rest;
    QString type,name,intro,store;
    if(flag == 0){
        row = ui->list->currentIndex().row();
        if(row==-1) return false;
        type = model->data(model->index(row,0)).toString();
        name = model->data(model->index(row,1)).toString();
        intro = model->data(model->index(row,2)).toString();
        price = model->data(model->index(row,3)).toInt();
        rest = model->data(model->index(row,4)).toInt();
        store = model->data(model->index(row,5)).toString();
    }
    else{
        row = ui->cartlist->currentIndex().row();
        if(row==-1) return false;
        type = "cart";
        name = cartmodel->data(cartmodel->index(row,0)).toString();
        price =cartmodel->data(cartmodel->index(row,1)).toInt();
        intro = "cart";
        rest = -1;
        store =cartmodel->data(cartmodel->index(row,3)).toString();
    }


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
    ui->cartPro->setVisible(mode!=1);
}

// 点击“管理商品”按钮
void store::on_managePro_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);
    this->changeMode(1);
    showProducts(0);
}

// 点击“购买商品”按钮
void store::on_buyPro_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    if(this->getProduct(0)){
        QStringList sL = pro->getContentList();
        float price = pro->getPrice();
        int discount = sqlD->selectdb(sL[5],sL[4]);
        if(this->dia->buyProduct(price,discount)){
            if(user->purchase(price,discount)){
                json->editJson(user->getInfoList());
                pro->updateRest(1);
                QStringList sL = pro->getContentList();
                sqlQ->updatedb(sL,sL[0],sL[4]);
                this->showProducts(proType);
            }else{
                QMessageBox::information(NULL, "提示", "余额不足");
            }
        }
    }else{
        QMessageBox::information(NULL, "提示", "请选择商品进行购买");
    }
}

// 商家：点击“添加商品”按钮
void store::on_addPro_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    QStringList dsL = this->dia->getContent();
    if(dsL[0] == "1" && dsL[1] != ""){
        dsL.removeAt(0);
        dsL<<user->getUsername();
        sqlQ->insertdb(dsL);
        this->showProducts(proType);
    }else{
        QMessageBox::information(NULL,"提示","请输入完整的信息");
    }
}

// 商家：点击“编辑商品”按钮
void store::on_editPro_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    if(this->getProduct(0)){
        QStringList sL = pro->getContentList();
        this->dia->setLineList(sL);
        QStringList dsL = this->dia->getContent();
        if(dsL[0] == "1"){
            dsL.removeAt(0);
            sqlQ->updatedb(dsL,sL[0],sL[4]);
            //  sqlQ->outPut();
            this->showProducts(proType);
        }

    }
    else{
        QMessageBox::information(NULL, "提示", "请选择某行进行编辑");
    }
}

// 商家：点击“删除商品”按钮
void store::on_delPro_clicked()
{
    // tableView中删除remove导致下面获取data失败
    if(this->getProduct(0)){
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
    qDebug() << "[Search Pro]" << ui->searchContent->text();
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
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    // 获取更改密码的信息
    QStringList sL = this->dia->changePass();
    if(sL[0] == "1"){
        if(!user->editPass(sL)){
            QMessageBox::information(NULL, "提示", "账号或旧密码错误");
        }
        else if(json->editJson(user->getInfoList())){
            QMessageBox::information(NULL, "提示", "密码修改成功");
        }
    }
}

// 点击“充值”按钮
void store::on_recharge_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    QStringList sL = this->dia->recharge();
    if(sL[0] == "1"){
        user->recharge(sL[1].toInt());
        if(json->editJson(user->getInfoList())){
            QStringList sL = this->user->getInfoList();
            ui->acount->setText(sL[1]);
            QMessageBox::information(NULL, "提示", "账户充值成功");
        }
    }
}

// 点击“打折”按钮
void store::on_discount_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    QStringList sL = this->dia->addDiscount();
    if(sqlD->insertdb(sL[1],sL[2],user->getUsername())){
        QMessageBox::information(NULL, "提示", "打折成功");
    }else{
        QMessageBox::information(NULL, "提示", "输入不完整");
    }
}

// 点击“生成订单”按钮
void store::on_order_clicked()
{
    float amount=0;
    for (int i=0;i<this->cartmodel->rowCount();i++) {
        QString name = this->cartmodel->data(this->cartmodel->index(i,0)).toString();
        QString reserved = this->cartmodel->data(this->cartmodel->index(i,2)).toString();
        QString store = this->cartmodel->data(this->cartmodel->index(i,3)).toString();
        float total = this->cartmodel->data(this->cartmodel->index(i,4)).toFloat();
        QStringList sL;     sL << reserved;
        sqlQ->updatedb(sL,name,store);
        amount += total;
    }
    ui->amount->setText(QString::number(amount));
    ui->cancelorder->setVisible(true);
    ui->pay->setVisible(true);
}

// 点击“取消订单”按钮
void store::on_cancelorder_clicked()
{
    for (int i=0;i<this->cartmodel->rowCount();i++) {
        QString name = this->cartmodel->data(this->cartmodel->index(i,0)).toString();
        QString reserved = this->cartmodel->data(this->cartmodel->index(i,2)).toString();
        QString store = this->cartmodel->data(this->cartmodel->index(i,3)).toString();
        QStringList sL;     sL << QString::number(reserved.toInt()*-1);
        sqlQ->updatedb(sL,name,store);
    }
    ui->amount->setText("XX");
    ui->cancelorder->setVisible(false);

}

// 点击“结算”按钮
void store::on_pay_clicked()
{
    QStringList sL;
    sL << this->user->getUsername();
    if(user->purchase(ui->amount->text().toFloat(),0)){
        json->editJson(user->getInfoList());
        for (int i=0;i<this->cartmodel->rowCount();i++) {
            QString name = this->cartmodel->data(this->cartmodel->index(i,0)).toString();
            QString store = this->cartmodel->data(this->cartmodel->index(i,3)).toString();
            sqlQ->updateResetDb(name,store);
        }
        this->cartmodel->removeRows(0,this->cartmodel->rowCount());
        this->sqlC->deletedb(sL);
        ui->amount->setText("XX");
    }else{
        QMessageBox::information(NULL, "提示", "余额不足");
    }
    ui->pay->setVisible(false);
}

// 顾客：点击“添加购物车”按钮
void store::on_cartPro_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    if(this->getProduct(0)){
        QStringList sL = pro->getContentList();
        float price = pro->getPrice();
        int discount = sqlD->selectdb(sL[5],sL[4]);
        QStringList dsL = this->dia->cartProduct(price,discount);
        if(dsL[0] == "1"){
            if(dsL[1].toInt() <= sL[3].toInt()){
                sL.removeAt(5);
                sL.removeAt(1);
                if(discount!=0) sL[1] = QString::number(discount*price/10);
                sL[2] = dsL[1]; //修改为购买数量
                sL << this->user->getUsername();
                this->sqlC->insertdb(sL);
            }else{
                QMessageBox::information(NULL, "提示", "购买数量超过库存");
            }
        }
    }else{
        QMessageBox::information(NULL, "提示", "请选择商品进行购买");
    }
}

// 顾客：点击“购物车编辑”按钮
void store::on_editCart_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    if(this->getProduct(1)){
        QStringList sL;
        sL << this->dia->cartEdit() << pro->getContentList()[0] << this->user->getUsername();
        if(sL[0] != "-1"){
            this->sqlC->updatedb(sL);
        }
        this->on_toCart_clicked();
    }
    else{
        QMessageBox::information(NULL, "提示", "请选择某行进行编辑");
    }
}

// 顾客：点击“购物车删除”按钮
void store::on_deleteCart_clicked()
{
    if(this->getProduct(1)){
        QStringList sL;
        sL << pro->getContentList()[0] << this->user->getUsername();
        this->sqlC->deletedb(sL);
        this->on_toCart_clicked();
    }
    else{
        QMessageBox::information(NULL, "提示", "请选择某行进行删除");
    }
}

