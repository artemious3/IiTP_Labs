//
// Created by artemious on 11.03.24.
//

#ifndef T2_PRODUCTTABLE_H
#define T2_PRODUCTTABLE_H
#include <QTableWidget>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QItemDelegate>

struct JSONDocFile{
    QSharedPointer<QJsonDocument> doc;
    QSharedPointer<QFile> file;
    void clear();
};

class ProductTable : public QTableWidget{
    Q_OBJECT
public:
    JSONDocFile* json = nullptr;

public:
    explicit ProductTable(QWidget* parent = nullptr);
    void LoadFromJSON();
    void MakeJSON();
    int WriteJSON();
    //file is stored in parent window
    void setJson(JSONDocFile* file);
    ~ProductTable() = default;

public slots:
    void pushProduct();
    void removeSelectedProduct();
};

class PriceItemDelegate : public QItemDelegate
{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                          const QModelIndex & index) const override;
};

class NameItemDelegate : public QItemDelegate{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                          const QModelIndex & index) const override;
};


#endif //T2_PRODUCTTABLE_H
