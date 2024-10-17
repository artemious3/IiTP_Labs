//
// Created by artemious on 10.03.24.
//

#ifndef T2_PRICELIST_H
#define T2_PRICELIST_H
#include <QFile>
#include <QMap>
#include <optional>
#include <QJsonDocument>
#include "Product.h"

class PriceList {
    QMap<ulong, Product> dict_;

public:
    static PriceList* loadFromFile(QSharedPointer<QJsonDocument> file);
    std::optional<Product> getProduct(ulong id) const;
    void pushProduct(QString name, int price);
    bool isEmpty() const;
};


#endif //T2_PRICELIST_H
