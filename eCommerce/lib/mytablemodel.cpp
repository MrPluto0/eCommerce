#include "mytablemodel.h"
#include <QSqlDatabase>

QVariant QMySQlQueryModel::data(const QModelIndex &item, int role) const

{
    QVariant value = QSqlQueryModel::data(item, role);
    if(role ==  Qt::TextAlignmentRole )
    {
        value   =   (Qt::AlignCenter);
        return value;
    }
    return value;
}
