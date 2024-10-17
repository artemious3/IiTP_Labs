//
// Created by artemious on 10.03.24.
//

#ifndef T2_ORDER_H
#define T2_ORDER_H
#include <QDate>
#include <QVector>


struct OrderEntry{
    int orderNumber_;
    QString productName_;
    unsigned long productId_;
    int amount;

    bool operator==(const OrderEntry& other) const;
};

inline ulong qHash(const OrderEntry& key, ulong seed){
  return key.productId_;
}

class Order {
    QDate orderDate_;
    int num_;
    QString client_;
    QMap<ulong, OrderEntry> entryList_;

public:
    explicit Order(const QDate& date, int num, QString client);
    const QMap<ulong, OrderEntry>& getEntries() const ;
    void pushProduct(const QString& name, ulong productId, int amount);
    void removeProduct(ulong id);
    void changeAmount(ulong id, int amount);

    OrderEntry getEntry(ulong id) const;
    const QDate& getDate() const;
    int getNum() const;
    const QString& getClient() const;
};




#endif //T2_ORDER_H
