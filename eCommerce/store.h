#ifndef STORE_H
#define STORE_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QtDebug>
#include <QString>
#include <QSqlQuery>
#include <QTableView>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "lib/user.h"
#include "lib/sqldiscount.h"
#include "lib/sqlquery.h"
#include "lib/dialog.h"
#include "lib/jsonexe.h"
#include "lib/product.h"
#include "ui_store.h"
#include "mainwindow.h"

namespace Ui {
class store;
}

/**
 * @class store
 * @brief 商店(主)界面的窗口类
 */
class store : public QMainWindow
{
    Q_OBJECT

public:
    explicit store(QWidget *parent = nullptr);
    ~store();

    /**
     * @brief 获取用户在table选中行的商品信息
     * @return bool 是否选中商品
     */
    bool getProduct();

    /**
     * @brief 查看商品与管理商品模式的交换
     * @param mode 0:查看商品  1:管理商品
     */
    void changeMode(int mode);

private slots:

    /**
     * @brief 从登录界面得到数据的槽函数
     * @param QJsonObject 用户的JSON对象
     */
    void getData(QJsonObject);

    /**
     * @brief 设置tableView的表头
     */
    void setHeaders();

    /**
     * @brief 在tableView中展示商品列表
     * @param proType 表示展示全部商品(0)或某类商品(1-4)
     */
    void showProducts(int proType);



    /**
     * @brief 切换到用户信息(主页)页面
     */
    void on_toIndex_clicked();

    /**
     * @brief 切换到商品页面
     */
    void on_toStore_clicked();

    /**
     * @brief 切换到关于页面
     */
    void on_toAbout_clicked();


    /**
     * @fn void on_delPro_clicked();
     * @brief 删除商品按钮的对应事件
     */
    void on_delPro_clicked();

    /**
     * @brief 购买商品按钮的对应事件
     */
    void on_buyPro_clicked();

    /**
     * @fn void on_addPro_clicked();
     * @brief 添加商品按钮的对应事件
     */
    void on_addPro_clicked();

    /**
     * @brief 编辑商品按钮的对应事件
     */
    void on_editPro_clicked();

    /**
     * @brief 管理商品按钮的对应事件
     */
    void on_managePro_clicked();
    /**
     * @brief 搜索商品按钮的对应事件
     */
    void on_searchPro_clicked();

    /**
     * @brief 修改密码按钮的对应事件
     */
    void on_editPass_clicked();

    /**
     * @brief 充值按钮的对应事件
     */
    void on_recharge_clicked();

    /**
     * @brief 退出登录按钮的对应事件
     */
    void on_close_clicked();

    /**
     * @brief 打折按钮的对应事件
     */
    void on_discount_clicked();

private:
    Ui::store *ui;
    QStringList typeMap;
    QStandardItemModel* model;

    user* user;
    int proType,mode;
    class product* pro;
    sqlQuery* sqlQ;
    sqlDiscount* sqlD;
    dialog* dia;
    jsonexe* json;
};

#endif // STORE_H
