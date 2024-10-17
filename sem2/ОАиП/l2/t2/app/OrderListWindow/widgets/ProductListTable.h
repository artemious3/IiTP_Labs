//
// Created by Артем Подгайский on 11.03.24.
//

#ifndef T2_PRODUCTLISTTABLE_H
#define T2_PRODUCTLISTTABLE_H
#include "Order.h"
#include <QTableWidget>

class ProductListTable : public QTableWidget{
    Q_OBJECT

    Order* currentOrder_ = nullptr;
    void updateTable();

public:
  explicit ProductListTable(QWidget* parent = nullptr);

public slots:
    void constructNewProduct();
    void setOrderSlot(Order* ord);
    void removeChosenProduct();
    void editChosenProduct();

};


#endif //T2_PRODUCTLISTTABLE_H
