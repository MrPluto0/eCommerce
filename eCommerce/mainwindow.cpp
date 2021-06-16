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

    // 连接服务端
    clientSocket = new QTcpSocket();
    clientSocket->connectToHost("127.0.0.1",5550);
    if(!clientSocket->waitForConnected()){
        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
    }
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(recData()));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(json) delete json;
}

void MainWindow::recData()
{
    char recMsg[1024] = {0};
    if(clientSocket->read(recMsg,1024)==-1){
        QMessageBox::information(this, "QT网络通信", "接收服务端数据失败！");
        return;
    }
    qDebug() << recMsg;
}

void MainWindow::on_login_clicked()
{
//    QString sendMsg = "你好啊";
//    char sendMsgChar[1024] = {0};
//    strcpy_s(sendMsgChar,sendMsg.toStdString().c_str());
//    if(clientSocket->write(sendMsgChar,strlen(sendMsgChar))==-1){
//        QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
//        return;
//    }

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
