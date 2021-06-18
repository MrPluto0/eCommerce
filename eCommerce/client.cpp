#include "client.h"

client::client(QObject *parent): QObject(parent)
{
    // 连接服务端
    ip = "127.0.0.1";
    port = 5210;
    isLink = false;
    clientSocket = NULL;
    this->iniClient();
}

client::~client()
{
    if(clientSocket) delete this->clientSocket;
}

void client::iniClient()
{
    int times = 5;
    while(times){
        if(!clientSocket){
            clientSocket = new QTcpSocket();
            connect(clientSocket, SIGNAL(readyRead()), this, SLOT(recData()));
            connect(clientSocket,SIGNAL(disconnected()),this,SLOT(disconnect()));
        }
        if(!isLink){
            this->clientSocket->connectToHost(ip,port);
            if(this->clientSocket->waitForConnected(30000)){
                qDebug() << "[QT网络通信]连接服务端成功！";
                in.setDevice(clientSocket);
                isLink = true;
                return ;
            }else{
                qDebug() << "[QT网络通信]连接服务端失败,重新连接中！";
            }
        }
        times --;
    }
    qDebug() << "[QT网络通信]5次尝试连接服务端失败，请检查服务端是否开启";
}

bool client::sendData(QString object,QString method,QStringList params)
{
    params.insert(0,object);
    params.insert(1,method);
    QString sendMsg = params.join(" ");
    qDebug() << sendMsg;

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << sendMsg;

    clientSocket->write(block);
    if(!clientSocket->waitForBytesWritten()){
        qDebug() << "[QT网络通信]向服务端发送数据失败";
        this->disconnect();
        return false;
    }else{
        qDebug() << "[QT网络通信]向服务端发送数据成功";
        return waitForRead();
    }
}

bool client::waitForRead()
{
    if(clientSocket->waitForReadyRead(3000)){
        return true;
    }else{
        return false;
    }
}

void client::recData()
{
    QString buffer;
    in >> buffer;
    // 如果不是UTF8，无法解析为Json
    result = buffer.toUtf8();
}

void client::disconnect()
{
    qDebug() << "[QT客户端]客户端断开连接";
    this->isLink = false;
    this->iniClient();
}

QByteArray client::getResult()
{
    return result;
}
