#include "dialog.h"

dialog::dialog(QWidget *parent)
{
    this->dia = new QDialog(parent);
    this->dia->setMinimumSize(100,100);
    this->lineList << "" << "" << "" << "" << "";
    this->buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok,dia);
    buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}

dialog::~dialog()
{
    dia->close();
    delete this->buttonBox;
    delete this->dia;
}

void dialog::setLineList(QStringList sL)
{
    this->lineList.clear();
    this->lineList << sL[0] << sL[1] << sL[2] << sL[3] << sL[5];
}

QStringList dialog::getContent()
{
    QFormLayout form(dia);
    QLabel* label = new QLabel("商品:");
    label->setFont(QFont("Arial" , 14 , QFont::Thin));
    form.addRow(label);

    // Value1
    label = new QLabel("名称:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line1 = new QLineEdit(dia);
    line1->setText(lineList[0]);
    form.addRow(label, line1);
    // Value2
    label = new QLabel("简介:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line2 = new QLineEdit(dia);
    line2->setText(lineList[1]);
    form.addRow(label, line2);
    // Value3
    label = new QLabel("价格:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line3 = new QLineEdit(dia);
    line3->setText(lineList[2]);
    line3->setValidator(new QDoubleValidator(line3));
    form.addRow(label, line3);
    // Value4
    label = new QLabel("剩余:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line4 = new QLineEdit(dia);
    line4->setText(lineList[3]);
    line4->setValidator(new QIntValidator(line4));
    form.addRow(label, line4);
    // Value5
    label = new QLabel("类型:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line5 = new QLineEdit(dia);
    line5->setText(lineList[4]);
    form.addRow(label, line5);
    // Add Cancel and OK button
    form.addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dia, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dia, SLOT(reject()));

    if (dia->exec() == QDialog::Accepted) {
        lineList.clear();
        lineList << "1" << line1->text()<<line2->text()<<line3->text()<<line4->text()<<line5->text();
    }else{
        lineList << "0";
    }
    return lineList;
}

QStringList dialog::changePass()
{
    QFormLayout form(dia);
    QLabel* label = new QLabel("修改密码:");
    label->setFont(QFont("Arial" , 14 , QFont::Thin));
    form.addRow(label);

    // Value1
    label = new QLabel("账号:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line1 = new QLineEdit(dia);
    form.addRow(label, line1);
    // Value2
    label = new QLabel("输入旧密码:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line2 = new QLineEdit(dia);
    form.addRow(label, line2);
    // Value3
    label = new QLabel("输入新密码:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line3 = new QLineEdit(dia);
    form.addRow(label, line3);

    // Add Cancel and OK button
    form.addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dia, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dia, SLOT(reject()));

    if (dia->exec() == QDialog::Accepted) {
        lineList.clear();
        lineList << "1" << line1->text()<<line2->text()<<line3->text();
    }else{
        lineList << "0";
    }
    return lineList;
}

QStringList dialog::recharge()
{
    QFormLayout form(dia);
    QLabel* label = new QLabel("充值");
    label->setFont(QFont("Arial" , 14 , QFont::Thin));
    form.addRow(label);

    // Value1
    label = new QLabel("充值金额:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line1 = new QLineEdit(dia);
    line1->setValidator(new QIntValidator(line1));
    form.addRow(label, line1);

    // Radio 单选框
    label = new QLabel("支付方式:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    form.addRow(label);
    QRadioButton* rabtn1 = new QRadioButton("WeChat");
    rabtn1->setFont(QFont("Arial" , 12 , QFont::Thin));
    rabtn1->setChecked(true);
    QRadioButton* rabtn2 = new QRadioButton("支付宝");
    rabtn2->setFont(QFont("Arial" , 12 , QFont::Thin));
    form.addRow(rabtn1,rabtn2);

    // Add Cancel and OK button
    form.addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dia, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dia, SLOT(reject()));

    if (dia->exec() == QDialog::Accepted) {
        lineList.clear();
        lineList << "1" << line1->text();
    }else{
        lineList << "0";
    }
    return lineList;
}


QStringList dialog::addDiscount()
{
    QFormLayout form(dia);
    QLabel* label = new QLabel("打折");
    label->setFont(QFont("Arial" , 14 , QFont::Thin));
    form.addRow(label);

    label = new QLabel("商品类别:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line1 = new QLineEdit(dia);
    form.addRow(label, line1);

    label = new QLabel("折扣:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line2 = new QLineEdit(dia);
    line2->setValidator(new QIntValidator(1,10,line2));    //折扣1~9
    form.addRow(label, line2);

    // Add Cancel and OK button
    form.addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dia, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dia, SLOT(reject()));

    if (dia->exec() == QDialog::Accepted) {
        lineList.clear();
        lineList << "1" << line1->text() << line2->text();
    }else{
        lineList << "0";
    }
    return lineList;
}


bool dialog::buyProduct(float price,int discount)
{
    QFormLayout form(dia);
    QLabel* label = new QLabel("商品价格为: "+QString::number(price)+"元");
    label->setFont(QFont("Arial" , 14 , QFont::Thin));
    form.addRow(label);

    qDebug()<<discount;
    if(discount!=0 && discount!= 10){
        label = new QLabel("折扣为:"+QString::number(discount)+"折");
        label->setFont(QFont("Arial" , 14 , QFont::Thin));
        form.addRow(label);

        label = new QLabel("总费用为:"+QString::number(discount*price/10)+"元");
        label->setFont(QFont("Arial" , 14 , QFont::Thin));
        form.addRow(label);
    }

    label = new QLabel("确认购买吗？");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    form.addRow(label);

    // Add Cancel and OK button
    form.addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dia, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dia, SLOT(reject()));

    if (dia->exec() == QDialog::Accepted) {
        return true;
    }
    return false;
}

QStringList dialog::cartProduct(float price,int discount)
{
    QStringList sL;
    QFormLayout form(dia);
    QLabel* label = new QLabel("商品价格为: "+QString::number(price)+"元");
    label->setFont(QFont("Arial" , 14 , QFont::Thin));
    form.addRow(label);

    if(discount!=0 && discount!= 10){
        label = new QLabel("折扣为:"+QString::number(discount)+"折");
        label->setFont(QFont("Arial" , 14 , QFont::Thin));
        form.addRow(label);

        label = new QLabel("总费用为:"+QString::number(discount*price/10)+"元");
        label->setFont(QFont("Arial" , 14 , QFont::Thin));
        form.addRow(label);
    }

    label = new QLabel("购买数量:");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    QLineEdit *line2 = new QLineEdit(dia);
    line2->setValidator(new QIntValidator(line2));
    form.addRow(label, line2);

    label = new QLabel("确认添加购物车吗？");
    label->setFont(QFont("Arial" , 12 , QFont::Thin));
    form.addRow(label);

    // Add Cancel and OK button
    form.addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dia, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dia, SLOT(reject()));

    if (dia->exec() == QDialog::Accepted) {
        sL << "1";
        sL << line2->text();
        return sL;
    }
    sL << "0";
    return sL;
}

QString dialog::cartEdit()
{
    QFormLayout form(dia);
    QLabel* label = new QLabel("输入修改的商品数量：");
    label->setFont(QFont("Arial" , 14 , QFont::Thin));
    form.addRow(label);

    QLineEdit *line = new QLineEdit(dia);
    line->setValidator(new QIntValidator(line));
    form.addRow(line);

    // Add Cancel and OK button
    form.addRow(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dia, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dia, SLOT(reject()));

    if (dia->exec() == QDialog::Accepted) {
        return line->text();
    }
    return "-1";
}

