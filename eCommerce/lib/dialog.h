#ifndef DIALOG_H
#define DIALOG_H
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QScrollBar>

/**
 * @brief 提示信息的对话框类
 */
class dialog
{
public:
    dialog(QWidget* _parent);
    ~dialog();

    /**
     * @brief 设置提示框的Label文本内容
     * @param sL Label文本内容的字符串数组
     */
    void setLineList(QStringList sL);

    /**
     * @brief 提示框--修改商品框
     * @return 商品信息的字符串数组
     */
    QStringList getContent();

    /**
     * @brief 提示框--修改密码框
     * @return 密码信息的字符串数组
     */
    QStringList changePass();

    /**
     * @brief 提示框--用户充值框
     * @return 充值信息的字符串数组
     */
    QStringList recharge();

    /**
     * @brief 提示框--商品打折框
     * @return 打折信息的字符串数组
     */
    QStringList addDiscount();

    /**
     * @brief 提示框--购买商品框
     * @return 是否确认购买
     */
    bool buyProduct(float price,int discount);

private:
    QDialog* dia;
    QStringList lineList;
    QDialogButtonBox* buttonBox;
};

#endif // DIALOG_H
