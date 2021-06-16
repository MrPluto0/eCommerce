#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include "processpool.h"
#include "lib/sqlquery.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief 服务端类
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @brief iniServer 初始化服务端
     */
    void iniServer();
    /**
     * @brief startSocket 有新的连接，则调用该槽函数
     */
    void startSocket();
    /**
     * @brief onDisconnect 断开监听
     */
    void onDisconnect();
    /**
     * @brief 测试按钮
     */
    void on_test_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer* server;
    processPool* pool;
};
#endif // MAINWINDOW_H
