#include "regis.h"
#include "mainwindow.h"
#include "store.h"
#include "ui_regis.h"
#include <QFile>
#include <QMessageBox>
#include <QRadioButton>
#include <QTextStream>
#include <QtDebug>

regis::regis(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::regis)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    client = new class client();
}

regis::~regis()
{
    delete client;
    delete ui;
}


void regis::on_complete_clicked()
{
    QString path;
    // 账号与密码格式正确，前后两次密码相同
    QRegExp rx("^[A-Za-z0-9]+$");
    try {
        if(!rx.exactMatch(ui->username->text())){
            throw "您输入的账号格式不正确";
        }
        else if(!rx.exactMatch(ui->password->text())){
            throw "您输入的密码格式不正确";
        }
        else if(ui->password->text() != ui->password2->text()){
            throw "您输入的两次密码不相同";
        }
        else{
            QStringList params;
            params << ui->username->text() << ui->password->text();
            if(ui->business->isChecked())
                params << "1";
            else if (ui->guest->isChecked())
                params << "0";
            if(client->sendData("JsonExe","AppendJson",params)){
                if(client->getResult()=="1"){
                    QMessageBox::information(NULL, "注册成功", "请重新登录");
                    MainWindow* mw = new MainWindow;
                    mw->show();
                    this->close();
                }
                else  throw "您输入的账号已存在";
            }
            else  throw "服务端出错";
        }
    } catch (const char* msg) {
        QMessageBox::information(NULL, "注册失败", msg);
    }

}

void regis::on_goback_clicked()
{
    MainWindow* mw = new MainWindow;
    mw->show();
    this->close();
}
