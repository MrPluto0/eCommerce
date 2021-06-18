#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QWidget>
#include <QSqlTableModel>


class QMySQlQueryModel:public QSqlQueryModel

{
public:
    explicit  QMySQlQueryModel();
public:
    QVariant data(const QModelIndex &index, int role) const;
protected:
};

#endif // MYTABLEMODEL_H
