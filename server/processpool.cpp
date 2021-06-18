#include "processpool.h"

processPool::processPool(int max)
{
    maxSize = max;
    nowSize = 0;
    nextID = 0;
}

processPool::~processPool()
{
    qDeleteAll(pool.begin(),pool.end());
    pool.clear();
}

void processPool::erasePool(int processId)
{
    qDebug() << "移除线程 ID:" << processId;
    for(auto it = pool.begin();it!=pool.end();it++){
        if((*it)->getID() == processId){
            pool.removeOne(*it);
            break;
        }
    }
    nowSize --;
}

bool processPool::addPool(QTcpSocket* socket)
{
    try {
        if(nowSize >= maxSize) throw "已达最大容量";
        if(nextID > maxSize){
            nextID = 0;
        }
        class thread* th = new class thread(socket,nextID++);
        connect(th,SIGNAL(delThread(int)),this,SLOT(erasePool(int)));
        pool.push_back(th);
        nowSize++;
        return true;
    } catch (const char* msg) {
        qDebug() << "[ProcessPool]AddPool " << msg;
        return false;
    }

}

void processPool::clearPool()
{
    qDeleteAll(pool.begin(),pool.end());
    pool.clear();
    nowSize = 0;
}
