#include "server.h"
#include "ui_server.h"
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connect,SIGNAL(clicked()),this,SLOT(iniServer()));
    connect(ui->disconnect,SIGNAL(clicked()),this,SLOT(onDisconnect()));

    server = NULL;
    pool = NULL;
}

MainWindow::~MainWindow()
{
    if(server) delete server;
    delete pool;
    delete ui;
}


void MainWindow::iniServer()
{
    if(!server){
        server = new QTcpServer();
    }
    if(!pool){
        pool = new processPool();
    }
    int port = ui->port->text().toInt();
    if(!server->listen(QHostAddress::Any,port)){
        QMessageBox::information(this,"QT服务端","服务端监听失败");
    }
    else{
        QMessageBox::information(this,"QT服务端","服务端监听成功");
        connect(server,SIGNAL(newConnection()),this,SLOT(startSocket()));
    }
}

void MainWindow::startSocket()
{
    if(!pool->addPool(server->nextPendingConnection()))
        qDebug() << "[QT服务端]addPool失败";
}


void MainWindow::onDisconnect()
{
    if(server) {
        server->close();
        delete server;
        server = NULL;
        delete pool;
        pool = NULL;
    }
}

void MainWindow::on_test_clicked()
{
    qDebug() << "测试";
    sqlQuery* sql = new sqlQuery();
    sql->selectdb("0");
}
