//
// Created by Артем Подгайский on 11.03.24.
//

#include "ProductListTable.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>


ProductListTable::ProductListTable(QWidget *parent) : QTableWidget(parent){
  setColumnCount(3);
  setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
  setSelectionBehavior(QTableWidget::SelectionBehavior::SelectRows);
  setSelectionMode(QTableWidget::SelectionMode::SingleSelection);
  setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
  setHorizontalHeaderItem(1, new QTableWidgetItem("ID"));
  setHorizontalHeaderItem(2, new QTableWidgetItem("Amount"));
  horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
}

void ProductListTable::constructNewProduct() {
  if(!currentOrder_){
      QMessageBox::information(this, "Info", "Choose order from the Order List, or create one.");
        return;
  }

  bool isOk;
  auto name = QInputDialog::getText(this, "New product", "Enter product name",
                                    {}, {}, &isOk);
  if(!isOk){ return; }
  if(name.size() > 64 || name.isEmpty()){
        QMessageBox::warning(this, "Warning", "The name is too long or empty");
        return;
    }

  auto amount = QInputDialog::getInt(this, "Amount", "Enter amount of products",
                                     1, 1, 999999, 1, &isOk);
  if(!isOk){ return; }
  ulong hash = qHash(name);
  currentOrder_->pushProduct(name, hash, amount);
  updateTable();
}

void ProductListTable::setOrderSlot(Order *ord) {
  currentOrder_ = ord;
  updateTable();
}

void ProductListTable::updateTable() {
  if(currentOrder_ == nullptr){
    setRowCount(0);
    return;
  }

  auto entries = currentOrder_->getEntries();
  setRowCount(entries.size());

  int i = 0;
  foreach(auto entry, entries){
      setItem(i, 0, new QTableWidgetItem(entry.productName_));
      setItem(i, 1, new QTableWidgetItem(QString::number(entry.productId_, 16)));
      setItem(i, 2, new QTableWidgetItem(QString::number(entry.amount)));
      ++i;
  }
}

void ProductListTable::removeChosenProduct() {
    if (!currentOrder_ || selectedItems().empty()) {
        return;
    }

    bool isOk;
    ulong key = item(currentRow(), 1)->text().toULong(&isOk, 16);
    if (!isOk) {
        qFatal("Could not read key");
    }

    currentOrder_->removeProduct(key);
    updateTable();
}

void ProductListTable::editChosenProduct() {
  if(selectedItems().isEmpty() || !currentOrder_ ){ return; }

  ulong selectedId = item(currentRow(), 1)->text().toULong(nullptr,16);

  bool isOk;
  auto amount = QInputDialog::getInt(this, "Amount", "Enter amount of products",
                                     1, 1, 999999, 1, &isOk);
  if(!isOk){ return; }

  currentOrder_->changeAmount(selectedId, amount);
  updateTable();
}