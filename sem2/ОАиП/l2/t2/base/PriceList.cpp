//
// Created by artemious on 10.03.24.
//

#include "PriceList.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

PriceList* PriceList::loadFromFile(QSharedPointer<QJsonDocument> file) {
  auto prList = new PriceList;
  auto jsonArr = file->array();
  foreach(auto value, jsonArr){
    auto obj = value.toObject();
    auto name = obj["name"];
    auto price = obj["price"];
    if(name.isUndefined() || price.isUndefined()){
      delete prList;
      return nullptr;
    }
    prList->pushProduct(name.toString(), price.toInt());
  }
  return prList;
}

std::optional<Product> PriceList::getProduct(ulong id) const {
    auto it = dict_.find(id);
    if(it != dict_.end()){
        return {*it};
    } else {
        return {};
    }
}

void PriceList::pushProduct(QString name, int price) {
  ulong hash = qHash(name);
  Product product {name, price};
  if(dict_.find(hash) != dict_.end()){
    throw std::invalid_argument("Product with the given name already exists");
  } else {
    dict_.insert(hash, product);
  }
}

bool PriceList::isEmpty() const {
    return dict_.isEmpty();
}
