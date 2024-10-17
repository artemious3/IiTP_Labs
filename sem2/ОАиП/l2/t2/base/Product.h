//
// Created by artemious on 10.03.24.
//

#ifndef T2_PRODUCT_H
#define T2_PRODUCT_H
#include <QString>

class Product {
private:
    unsigned long id_;
    QString name_;
public:
    int Price;

public:
    explicit Product(const QString& name, int price = 0);
    Product() = delete;
    Product(const Product& prod) = default;

    QString getName() const;
};


#endif //T2_PRODUCT_H
