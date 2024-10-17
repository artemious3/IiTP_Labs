//
// Created by artemious on 11.03.24.
//

#include "ProductTable.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QHeaderView>
#include <QLineEdit>

void JSONDocFile::clear() {
    file.clear();
    doc.clear();
}


ProductTable::ProductTable(QWidget* parent) : QTableWidget(parent) {
    setColumnCount(2);
    horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
    setHorizontalHeaderItem(0, new QTableWidgetItem("Product name"));
    setHorizontalHeaderItem(1, new QTableWidgetItem("Price"));

    setItemDelegateForColumn(1, new PriceItemDelegate);
    setItemDelegateForColumn(0, new NameItemDelegate);
}

void ProductTable::MakeJSON() {
  if(!json){
    return;
  }

    QJsonArray productsArrayJson;
    for(int i = 0; i < rowCount(); ++i){
        QJsonObject product;
        bool ok;
        product["name"] = item(i,0)->text();
        product["price"] = (qint64)item(i,1)->text().toULong(&ok);
        if(!ok){
            QMessageBox::warning(this, "Watning",
                                 "The data on either ID or price on row "
                                 + QString::number(i+1)
                                 + " is not numeric.");
            return;
        }
        productsArrayJson.push_back(product);
    }

    if(json->doc){
        json->doc.clear();
        json->doc = nullptr;
    }
    json->doc = QSharedPointer<QJsonDocument>(new QJsonDocument(productsArrayJson));

}

void ProductTable::LoadFromJSON() {
  if(!json){
    return;
  }
    if(!json->file){
      return;
    }

    auto file = json->file;
    QJsonParseError err;
    file->open(QFile::ReadOnly);
    auto jsonData = file->readAll();
    json->doc = QSharedPointer<QJsonDocument>(new QJsonDocument(QJsonDocument::fromJson(jsonData, &err)));
    file->close();

    if(json->doc->isNull()){
        QMessageBox::warning(this, "Warning", "Failed to open the given file."
                                              "Make sure it'd made by this software and not corrupted");
        json->clear();
        setRowCount(0);
        return;
    }

    QJsonArray arr = json->doc->array();
    setRowCount(arr.size());
    int row = 0;
    foreach(auto val, arr){
        auto obj = val.toObject();
        auto name = obj.value("name");
        auto price = obj.value("price");
        if(name.isUndefined()  || price.isUndefined()){
            QMessageBox::warning(this, "Warning", "Failed to process this file.");
            json->clear();
            setRowCount(0);
            return;
        }

        setItem(row,0, new QTableWidgetItem(name.toString()));
        setItem(row, 1, new QTableWidgetItem(QString::number(price.toInteger())));
        ++row;
    }
}





void ProductTable::pushProduct() {
    setRowCount(rowCount()+1);
    setItem(rowCount()-1, 0, new QTableWidgetItem("Product Name"));
    setItem(rowCount()-1, 1, new QTableWidgetItem(QString::number(0)));
}

int ProductTable::WriteJSON() {
  if(!json){
    qDebug() << "no json";
    return 1;
  }
    if(!json->file ){
      qDebug() << "File is null";
      return 1;
    }

    auto file = json->file;
    MakeJSON();

    if(!json->doc){
        qWarning() << "Can't write. Document is nullptr";
        return 1;
    }

    file->open(QFile::OpenModeFlag::WriteOnly);
    file->write(json->doc->toJson());
    file->close();
    return 0;
}

void ProductTable::removeSelectedProduct() {
    if(!selectedItems().empty()){
        removeRow(currentRow());
    }
}

void ProductTable::setJson(JSONDocFile* externalJson) {
  json = externalJson;
}



QWidget* PriceItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                                         const QModelIndex & index) const {
    QLineEdit *lineEdit = new QLineEdit(parent);
    QIntValidator *validator = new QIntValidator(0, 9, lineEdit);
    validator->setTop(99999999);
    validator->setBottom(0);
    lineEdit->setValidator(validator);
    return lineEdit;
}

QWidget* NameItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                                            const QModelIndex & index) const {
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setMaxLength(64);
    return lineEdit;
}
