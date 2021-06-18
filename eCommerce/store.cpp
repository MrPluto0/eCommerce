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

    client = new class client();
    dia = new dialog(this);

    user = NULL;
    pro = NULL;
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
    if(model) delete model;
    if(dia) delete this->dia;
    if(client) delete client;
    delete ui;
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

    QStringList params;     params<<this->user->getUsername();
    if(client->sendData("SqlCart","Selectdb",params)){
        QByteArray content= client->getResult();
        if(content == ""){
            qDebug() << "[QT网络通信]服务端出错";
            return ;
        }
        QJsonDocument jdoc = QJsonDocument::fromJson(content);
        QJsonArray jarr = jdoc.array();
        for (int row=0; row<jarr.size(); row++) {
            QJsonObject jobj = jarr.at(row).toObject();
            QStringList sL;
            sL << jobj["name"].toString();
            sL << jobj["price"].toString();
            sL << jobj["number"].toString();
            sL << jobj["store"].toString();
            sL << jobj["total"].toString();
            for (int i=0;i<=4;i++) {
                cartmodel->setItem(row,i,new QStandardItem(sL[i]));
                cartmodel->item(row,i)->setTextAlignment(Qt::AlignCenter);  //item居中！！！
            }
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
    this->proType = proType;
    // 清理除了表头的数据
    this->model->removeRows(0,model->rowCount());
    this->setHeaders();
    // 初始化sqlQ
    typeMap[proType],user->getUsername();

    // 根据模式获取数据库查询结果
    QStringList params;
    QByteArray content;
    if(mode == 0){
        params << QString::number(proType);
        if(client->sendData("SqlQuery","Selectdb",params)){
            content = client->getResult();
        }else{
            qDebug() << "[QT网络通信]服务端出错";
        }
    }
    else{
        params << QString::number(proType) << user->getUsername();
        if(client->sendData("SqlQuery","SelectStoredb",params)){
            content = client->getResult();
        }else{
            qDebug() << "[QT网络通信]服务端出错";
        }
    }
    // 将结果写入表格
    QJsonDocument jdoc = QJsonDocument::fromJson(content);
    QJsonArray jarr = jdoc.array();
    for (int row=0; row<jarr.size(); row++) {
        QJsonObject jobj = jarr.at(row).toObject();
        QStringList sL; //QStringList不能用普通的索引set，但是可以get
        sL << jobj["type"].toString();
        sL << jobj["name"].toString();
        sL << jobj["intro"].toString();
        sL << jobj["price"].toString();
        sL << jobj["rest"].toString();
        sL << jobj["store"].toString();
        qDebug() << sL;
        for (int i=0;i<=5;i++) {
            model->setItem(row,i,new QStandardItem(sL[i]));
            model->item(row,i)->setTextAlignment(Qt::AlignCenter);  //item居中！！！
        }
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
        QStringList params; params << sL[5] << sL[4];
        client->sendData("SqlDiscount","Selectdb",params);
        int discount = client->getResult().toInt();
        if(this->dia->buyProduct(price,discount)){
            if(user->purchase(price,discount)){
                QStringList sLL; sLL << sL[4];
                if(discount!=0) sLL << QString::number(price*discount/10,'f',1);
                else sLL << QString::number(price);
                client->sendData("JsonExe","EditJson",user->getInfoList()); //用户付款
                client->sendData("JsonExe","EditJson",sLL); //商家收款
                pro->updateRest(1);
                QStringList params = pro->getContentList();
                params.insert(4,params[0]);
                params.insert(5,params[5]);
                if(client->sendData("SqlQuery","Updatedb",params)){
                    if(client->getResult()!="1")
                        QMessageBox::information(NULL, "QT网络通信", "服务端出错");
                }
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
        if(client->sendData("SqlQuery","Insertdb",dsL)){
            if(client->getResult()!="1")
                QMessageBox::information(NULL, "QT网络通信", "服务端出错");
        }
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
            dsL.insert(4,sL[0]);dsL.insert(5,sL[4]);
            if(client->sendData("SqlQuery","Updatedb",dsL)){
                if(client->getResult()!="1")
                    QMessageBox::information(NULL, "QT网络通信", "服务端出错");
            }
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
        QStringList params;
        params << pro->getContentList()[0] << user->getUsername();
        // 在数据库中删除
        if(client->sendData("SqlQuery","Deletedb",params)){
            if(client->getResult()!="1")
                QMessageBox::information(NULL, "QT网络通信", "服务端出错");
        }
        this->showProducts(proType);
    }
    else{
        QMessageBox::information(NULL, "提示", "请选择某行进行删除");
    }
}

// 点击“搜索商品”按钮
void store::on_searchPro_clicked()
{
    qDebug() << "[Search Pro]" << ui->searchContent->text();
    model->removeRows(0,model->rowCount());
    QStringList params;
    params << ui->searchContent->text();
    if(client->sendData("SqlQuery","Searchdb",params)){
        QByteArray content = client->getResult();
        if(content=="") {
            QMessageBox::information(NULL, "QT网络通信", "服务端出错");
            return;
        }
        QJsonDocument jdoc = QJsonDocument::fromJson(content);
        QJsonArray jarr = jdoc.array();
        for (int row=0; row<jarr.size(); row++) {
            QJsonObject jobj = jarr.at(row).toObject();
            QStringList sL; //QStringList不能用普通的索引set，但是可以get
            sL << jobj["type"].toString();
            sL << jobj["name"].toString();
            sL << jobj["intro"].toString();
            sL << jobj["price"].toString();
            sL << jobj["rest"].toString();
            sL << jobj["store"].toString();
            for (int i=0;i<=5;i++) {
                model->setItem(row,i,new QStandardItem(sL[i]));
                model->item(row,i)->setTextAlignment(Qt::AlignCenter);  //item居中！！！
            }
        }
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
        else if(client->sendData("JsonExe","EditJson",user->getInfoList())){
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
        if(client->sendData("JsonExe","EditJson",user->getInfoList())){
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
    QStringList params;
    params << sL[1] << sL[2] << user->getUsername();
    client->sendData("SqlDiscount","Insertdb",params);
    if(client->getResult()=="1"){
        QMessageBox::information(NULL, "提示", "打折成功");
    }else{
        QMessageBox::information(NULL, "提示", "输入不完整");
    }
}

// 点击“生成订单”按钮
void store::on_order_clicked()
{
    float amount=0;
    selectedList =  ui->cartlist->selectionModel()->selectedRows();
    for (int i = 0; i < selectedList.count(); i++)
    {
        int row = selectedList.at(i).row();
        QString name = this->cartmodel->data(this->cartmodel->index(row,0)).toString();
        QString reserved = this->cartmodel->data(this->cartmodel->index(row,2)).toString();
        QString store = this->cartmodel->data(this->cartmodel->index(row,3)).toString();
        float total = this->cartmodel->data(this->cartmodel->index(row,4)).toFloat();
        QStringList params;
        params << reserved << name << store;
        if(client->sendData("SqlQuery","Updatedb",params)){
            if(client->getResult()!="1"){
                QMessageBox::information(NULL, "QT网络通信", "服务端出错");
                return ;
            }
        }
        amount += total;
    }
    ui->amount->setText(QString::number(amount));
    ui->cancelorder->setVisible(true);
    ui->pay->setVisible(true);
}

// 点击“取消订单”按钮
void store::on_cancelorder_clicked()
{
    for (int i = 0; i < selectedList.count(); i++){
        int row = selectedList.at(i).row();
        QString name = this->cartmodel->data(this->cartmodel->index(row,0)).toString();
        QString reserved = this->cartmodel->data(this->cartmodel->index(row,2)).toString();
        QString store = this->cartmodel->data(this->cartmodel->index(row,3)).toString();
        QStringList params;
        params << QString::number(reserved.toInt()*-1) << name << store;
        if(client->sendData("SqlQuery","Updatedb",params)){
            if(client->getResult()!="1")
                QMessageBox::information(NULL, "QT网络通信", "服务端出错");
        }
    }
    ui->amount->setText("XX");
    ui->cancelorder->setVisible(false);
    ui->pay->setVisible(false);
}

// 点击“结算”按钮
void store::on_pay_clicked()
{
    if(user->purchase(ui->amount->text().toFloat(),0)){
        client->sendData("JsonExe","EditJson",user->getInfoList());
        for (int i = 0; i < selectedList.count(); i++){
            int row = selectedList.at(i).row();
            QString name = this->cartmodel->data(this->cartmodel->index(row,0)).toString();
            QString store = this->cartmodel->data(this->cartmodel->index(row,3)).toString();
            QString total = this->cartmodel->data(this->cartmodel->index(row,4)).toString();
            QStringList params;     params << name << store;
            if(client->sendData("SqlQuery","UpdateResetDb",params)){
                if(client->getResult()!="1")
                    QMessageBox::information(NULL, "QT网络通信", "服务端出错");
            }
            QStringList sL,sLL;
            sL << name << this->user->getUsername();
            sLL << store << total;
            client->sendData("SqlCart","Deletedb",sL);
            client->sendData("JsonExe","EditJson",sLL); //商家收款
        }
        ui->amount->setText("XX");
        this->on_toCart_clicked();
    }else{
        QMessageBox::information(NULL, "提示", "余额不足");
    }
    ui->pay->setVisible(false);
    ui->cancelorder->setVisible(false);
}

// 顾客：点击“添加购物车”按钮
void store::on_cartPro_clicked()
{
    if(this->dia) delete this->dia;
    dia = new dialog(this);

    if(this->getProduct(0)){
        QStringList sL = pro->getContentList();
        float price = pro->getPrice();
        QStringList params; params << sL[5] << sL[4];
        client->sendData("SqlDiscount","Selectdb",params);
        int discount = client->getResult().toInt();
        QStringList dsL = this->dia->cartProduct(price,discount);
        if(dsL[0] == "1" && !dsL[1].isEmpty()){
            if(dsL[1].toInt() <= sL[3].toInt()){
                sL.removeAt(5);
                sL.removeAt(1);
                if(discount!=0) sL[1] = QString::number(discount*price/10);
                sL[2] = dsL[1]; //修改为购买数量
                sL << this->user->getUsername();
                client->sendData("SqlCart","Insertdb",sL);
            }else{
                QMessageBox::information(NULL, "提示", "购买数量超过库存");
            }
        }else{
            QMessageBox::information(NULL, "提示", "购买数量为空");
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
            client->sendData("SqlCart","Updatedb",sL);
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
        client->sendData("SqlCart","Deletedb",sL);
        this->on_toCart_clicked();
    }
    else{
        QMessageBox::information(NULL, "提示", "请选择某行进行删除");
    }
}

