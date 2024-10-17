//
// Created by artemious on 10.03.24.
//

#include "Order.h"

//-- OrderEntry --

bool OrderEntry::operator==(const OrderEntry &other) const{
  return this->productId_ == other.productId_;
}

//-- Order --

const QMap<ulong, OrderEntry> &Order::getEntries() const {
    return entryList_;
}

void Order::pushProduct(const QString& name, ulong prodId, int amount) {
  auto productEntry = entryList_.find(prodId);
  if(productEntry == entryList_.end()){
    OrderEntry newEntry;
    newEntry.productId_ = prodId;
    newEntry.amount = amount;
    newEntry.productName_ = name;
    entryList_[prodId] = newEntry;
  } else {
    productEntry->amount += amount;
  }
}

OrderEntry Order::getEntry(ulong id) const {
  return entryList_[id];
}

Order::Order(const QDate &date, int num, QString client) :
  orderDate_(date),
  num_(num),
  client_(client)
{}

const QDate &Order::getDate() const {
  return orderDate_;
}

int Order::getNum() const {
  return num_;
}

const QString& Order::getClient() const{
  return client_;
}

void Order::removeProduct(ulong id) {
  entryList_.remove(id);
}

void Order::changeAmount(ulong id, int amount) {
  entryList_[id].amount = amount;
}


