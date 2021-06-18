#include "mainwindow.h"
#include "ui_mainwindow.h" //系统将ui文件在编译过程中自动生成头文件

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    st = new store();
    re = new regis();
    client = new class client();
    connect(this,SIGNAL(sendData(QJsonObject)),st,SLOT(getData(QJsonObject)));
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::on_login_clicked()
{
    QStringList params;
    params << ui->user->text() << ui->pass->text();
    if(client->sendData("JsonExe","GetJson",params)){
        qDebug() << client->getResult();
        QJsonObject user = QJsonDocument::fromJson(client->getResult()).object();
        if(!user.isEmpty()){
            emit sendData(user);
            QMessageBox::information(NULL, "登录成功", "进入用户界面");
            this->st->show();
            this->close();
        }else{
            QMessageBox::information(NULL, "登录失败", "您输入的账号或密码错误");
        }
    }else{
        qDebug() << "[QT网络通信]服务端出错";
    }
}

void MainWindow::on_toRegister_clicked()
{
    // 跳转到注册页面
    this->re->show();
    this->close();
}
