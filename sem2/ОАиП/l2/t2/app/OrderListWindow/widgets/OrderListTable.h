//
// Created by Артем Подгайский on 11.03.24.
//

#ifndef T2_ORDERLISTTABLE_H
#define T2_ORDERLISTTABLE_H
#include <QTableWidget>
#include "Order.h"

class OrderListTable : public QTableWidget {
Q_OBJECT

    QVector<Order> orders_;
    static int id;

private:
    void updateTable(int ind);
public:
    explicit OrderListTable(QWidget* parent = nullptr);
    const Order* getCurrentOrder() const;
    void removeOrder(int ind);

    void setOrderList(const QVector<Order>& newOrders);
    const QVector<Order>& getOrderList();
    bool isEmpty();
    void clean();


private slots:
    void handleChangedOrder();

public slots:
    void constructNewOrder();
    void removeChosenOrder();
    void sortByName();

signals:
  void orderChanged(Order* order);
};


#endif //T2_ORDERLISTTABLE_H
