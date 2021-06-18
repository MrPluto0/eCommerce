#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include "client.h"
#include "store.h"
#include "regis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief 登录界面的窗口类
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    /**
     * @brief sendData
     * @param QJsonObject 用户的JSON对象
     */
    void sendData(QJsonObject);//发送数据的信号，不需要实体函数，在头文件定义就可以
private slots:
    /**
     * @brief 登录按钮对应的事件
     */
    void on_login_clicked();
    /**
     * @brief 注册按钮对应的事件
     */
    void on_toRegister_clicked();


private:
    Ui::MainWindow *ui;
    class client* client;
    class store* st;
    class regis* re;
};
#endif // MAINWINDOW_H
