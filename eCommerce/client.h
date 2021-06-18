#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QMessageBox>
#include <QTimer>

/**
 * @brief 客户端启动类，处理数据与服务端的接收与传输
 */
class client: public QObject
{
    Q_OBJECT
private:
    QTcpSocket *clientSocket;
    QByteArray result;
    QDataStream in;
    QString ip;
    int port;
    bool isLink;

public:
    explicit client(QObject *parent = nullptr);
    ~client();
    void iniClient();
    bool sendData(QString object,QString method,QStringList params);
    bool waitForRead();
    QByteArray getResult();

private slots:
    void recData();
    void disconnect();
};

#endif // CLIENT_H
