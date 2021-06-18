#ifndef REGIS_H
#define REGIS_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QRadioButton>
#include <QTextStream>
#include <QtDebug>
#include "lib/jsonexe.h"
#include "mainwindow.h"
#include "store.h"
#include "ui_regis.h"

namespace Ui {
class regis;
}

/**
 * @class regis
 * @brief 注册界面的窗口类
 */
class regis : public QMainWindow
{
    Q_OBJECT

public:
    explicit regis(QWidget *parent = nullptr);
    ~regis();

private slots:
    /**
     * @brief 注册完成按钮的对应事件
     */
    void on_complete_clicked();

    /**
     * @brief 返回主界面按钮的对应事件
     */
    void on_goback_clicked();

private:
    Ui::regis *ui;
    jsonexe* json;
};

#endif // REGIS_H
