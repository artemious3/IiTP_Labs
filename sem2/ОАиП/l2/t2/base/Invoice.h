//
// Created by artemious on 10.03.24.
//

#ifndef T2_INVOICE_H
#define T2_INVOICE_H
#include <QVector>
#include "Product.h"
#include "Order.h"
#include "PriceList.h"

struct InvoiceEntry{
    Product product;
    int amount;
    long totalPrice;
};

class Invoice{
    QVector<InvoiceEntry> entries_;
    long totalCost;

public:
    static Invoice fromOrder(const Order* order, const PriceList* priceList);
    const QVector<InvoiceEntry>& getEntries() const;
    int getTotalCost() const;
    void clear();
};

#endif //T2_INVOICE_H
