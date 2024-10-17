//
// Created by Артем Подгайский on 11.03.24.
//

#ifndef T2_INVOICETABLE_H
#define T2_INVOICETABLE_H
#include <QTableWidget>
#include <Order.h>
#include "Invoice.h"

class InvoiceTable : public QTableWidget {
    Q_OBJECT
    Invoice invoice;

public:
    explicit InvoiceTable(QWidget* parent = nullptr);
    void Construct(const Order* ord, const PriceList* prList);
    void UpdateTable();
    void clean();

};


#endif //T2_INVOICETABLE_H
