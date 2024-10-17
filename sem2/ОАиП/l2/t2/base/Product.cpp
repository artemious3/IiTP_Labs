//
// Created by artemious on 10.03.24.
//

#include "Product.h"
#include <QHash>

Product::Product(const QString& name, int price) {
    id_ = qHash(name);
    name_ = name;
    Price = price;
}

QString Product::getName() const {
  return name_;
}
