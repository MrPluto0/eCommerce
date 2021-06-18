#include "mainwindow.h"
#include "ui_mainwindow.h" //系统将ui文件在编译过程中自动生成头文件

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    json = NULL;
    st = new store();
    re = new regis();
    connect(this,SIGNAL(sendData(QJsonObject)),st,SLOT(getData(QJsonObject)));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(json) delete json;
}

void MainWindow::on_login_clicked()
{
    json = new jsonexe(0);
    QJsonObject user = json->getJson(ui->user->text(),ui->pass->text());
    if(!user.isEmpty()){
        emit sendData(user);
        QMessageBox::information(NULL, "登录成功", "进入用户界面");
        this->st->show();
        this->close();
    }else{
        QMessageBox::information(NULL, "登录失败", "您输入的账号或密码错误");
    }
}

void MainWindow::on_toRegister_clicked()
{
    // 跳转到注册页面
    this->re->show();
    this->close();
}
