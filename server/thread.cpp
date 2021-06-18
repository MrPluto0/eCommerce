#include "thread.h"

thread::thread(QTcpSocket *newSocket, int newId)
    :id(newId)
{
    socket = newSocket;
    ins = new instruction();
    iniSocket();
}

void thread::iniSocket()
{
    if(!socket){
        qDebug() << "[QT网络通信]未已正确获取客户端";
    }
    else{
        qDebug() << "[QT网络通信]已正确获取客户端 ID" << id;
        in.setDevice(socket);
        connect(socket,SIGNAL(readyRead()),this,SLOT(recData()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(disconnect()));
    }
}

int thread::getID()
{
    return id;
}

void thread::sendData(QString sendMsg)
{
    if(socket->isValid()){
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out << sendMsg;
        socket->write(block);
        if(socket->waitForBytesWritten())
            qDebug() << "[QT网络通信]服务端发送数据成功！socketId:" << id;
        else
            qDebug() << "[QT网络通信]服务端发送数据失败！socketId:" << id;
    }else{
        qDebug() << "[QT网络通信]套接字无效！socketId:" << id;
    }
}

void thread::recData()
{
    QString buffer;
    QStringList sL;
    in >> buffer;
    if(buffer!= ""){
        sL = buffer.split(" ");
        ins->parseClass(sL);
        qDebug() << "[QT网络通信]接收数据成功！socketId:" << id;
        sendData(ins->getResult());
    }
    else{
        qDebug() << "[QT网络通信]未正确接收数据！socketId:" << id;
    }
}

void thread::disconnect()
{
    qDebug() << "[QT客户端]客户端断开连接";
    emit delThread(id);
}
