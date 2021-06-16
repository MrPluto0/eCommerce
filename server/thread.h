#ifndef THREAD_H
#define THREAD_H

#include <QTcpSocket>
#include <QDataStream>
#include "instruction.h"

/**
 * @brief 自定义线程
 */
class thread:public QObject
{
    Q_OBJECT
private:
    int id;
    QDataStream in;
    QTcpSocket* socket;
    class instruction* ins;

public:
    thread(QTcpSocket* newSocket,int id);
    /**
     * @brief iniSocket 初始化线程
     */
    void iniSocket();
    /**
     * @brief getID 获取线程ID
     * @return 线程ID
     */
    int getID();
    /**
     * @brief sendData 向客户端发送数据
     * @param sendMsg
     */
    void sendData(QString sendMsg);

private slots:
    /**
     * @brief recData 监听是否接收客户端发来的数据
     */
    void recData();
    /**
     * @brief disconnect 断开客户端的连接
     */
    void disconnect();
signals:
    /**
     * @brief delThread 信号：触发进程池删除进程
     */
    void delThread(int);
};

#endif // THREAD_H
