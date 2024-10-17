//
// Created by artemious on 10.03.24.
//

#include "Invoice.h"

Invoice Invoice::fromOrder(const Order *order, const PriceList *priceList) {
    Invoice inv;

    unsigned long accumulator = 0;
    auto entries = order->getEntries();
    foreach(auto entry, entries){
        auto prodOpt = priceList->getProduct(entry.productId_);
        if(!prodOpt){
            throw std::invalid_argument("Product with name " + entry.productName_.toStdString() + " does not exist.");
        }
        auto prod = prodOpt.value();

        long totalPrice = prod.Price * (long)entry.amount;
        inv.entries_.push_back(InvoiceEntry{prod,
                                            entry.amount,
                                         totalPrice});
        accumulator += totalPrice;
    }
    inv.totalCost = accumulator;
    return inv;
}

const QVector<InvoiceEntry> &Invoice::getEntries() const {
  return entries_;
}

int Invoice::getTotalCost() const {
  return totalCost;
}

void Invoice::clear() {
    entries_.clear();
}
