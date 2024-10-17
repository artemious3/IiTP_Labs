//
// Created by artemious on 12.03.24.
//

#ifndef T2_ORDERLOADER_H
#define T2_ORDERLOADER_H
#include "ProductTable.h"
#include "Order.h"

class OrderLoader : public QWidget{

public:
    OrderLoader(QWidget* parent = nullptr);
    QVector<Order> loadFromJson(QFile* file);
    void writeToJson(const QVector<Order>& orders, QFile* file);



};


#endif //T2_ORDERLOADER_H
