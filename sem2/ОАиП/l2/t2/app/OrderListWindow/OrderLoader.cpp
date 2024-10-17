//
// Created by artemious on 12.03.24.
//

#include "OrderLoader.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>


QVector<Order> OrderLoader::loadFromJson(QFile* file) {
    if(!file){
        return {};
    }

    file->open(QFile::ReadOnly);
    auto rawData = file->readAll();
    file->close();

    QJsonDocument doc (QJsonDocument::fromJson(rawData));

    if(doc.isNull() || !doc.isArray()){
        QMessageBox::warning(this, "Warning", "Could not read this file");
        return {};
    }

    QVector<Order> retval;

    auto arr = doc.array();
    foreach(auto orderJson, arr){
        auto obj = orderJson.toObject();
        if(!obj.contains("number") || !obj.contains("date") || !obj.contains("client")){
            QMessageBox::warning(this, "Warning", "Could not read the given file");
            return {};
        }

        Order ord {QDate::fromString( obj["date"].toString(), Qt::ISODate),
                   (int)obj["number"].toInteger(),
                   obj["client"].toString()};

        auto productList = obj["products"].toArray();
        foreach(auto productJson, productList){
          auto prodObj = productJson.toObject();
          if(!prodObj.contains("name") || !prodObj.contains("id")|| !prodObj.contains("amount")){
            QMessageBox::warning(this, "Warning", "Could not read the given file");
            return {};
          }

          ord.pushProduct(prodObj["name"].toString(),
                          (ulong)prodObj["id"].toInteger(),
                          prodObj["amount"].toInt());
        }

        retval.push_back(ord);
    }

    return retval;
}

void OrderLoader::writeToJson(const QVector <Order> &orders, QFile* file) {
    if(!file){
        qWarning() << "writeToJson: nullptr file";
        return;
    }

    QJsonArray arr;
    foreach(auto order, orders){
        QJsonObject orderJson;
        orderJson["date"] = order.getDate().toString(Qt::ISODate);
        orderJson["number"] = order.getNum();
        orderJson["client"] = order.getClient();


        QJsonArray products;
        foreach(auto product, order.getEntries()){
            QJsonObject productJson;
            productJson["name"] = product.productName_;
            productJson["id"] = (qint64)product.productId_;
            productJson["amount"] = product.amount;
            products.push_back(productJson);
        }

        orderJson["products"] = products;
        arr.push_back(orderJson);
    }

    QJsonDocument doc (arr);

    bool suc = file->open(QFile::WriteOnly);
    if(!suc){
        QMessageBox::warning(this, "Warning", "Could not write to file");
        return;
    }
    file->write(doc.toJson());
    file->close();
}

OrderLoader::OrderLoader(QWidget *par) : QWidget(par)
{}

