#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

#include <QVector>
#include <QtAlgorithms>
#include "thread.h"

/**
 * @brief 进程池
 */
class processPool:public QObject
{
    Q_OBJECT
private:
    QVector<class thread*> pool;
    int nowSize,maxSize,nextID;
public:
    processPool(int max = 50);
    ~processPool();
    /**
     * @brief addPool 向进程池中添加某个socket进程
     * @param socket 客户端的套接字
     * @return 是否添加成功
     */
    bool addPool(QTcpSocket* socket);
    /**
     * @brief clearPool 清空进程池
     */
    void clearPool();
private slots:
    /**
     * @brief erasePool 获取线程的信号，根据进程ID，删除某个socket
     */
    void erasePool(int);
};

#endif // PROCESSPOOL_H
