//
// Created by Артем Подгайский on 11.03.24.
//

#include "OrderListTable.h"
#include <QInputDialog>
#include <QHeaderView>
#include <QMessageBox>

int OrderListTable::id = 1;

OrderListTable::OrderListTable(QWidget* parent) : QTableWidget(parent){
  setColumnCount(3);
  setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
  setSelectionBehavior(QTableWidget::SelectionBehavior::SelectRows);
    setSelectionMode(QTableWidget::SelectionMode::SingleSelection);
  setHorizontalHeaderItem(0, new QTableWidgetItem("Number"));
  setHorizontalHeaderItem(1, new QTableWidgetItem("Client"));
  setHorizontalHeaderItem(2, new QTableWidgetItem("Date"));
    horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );

  connect(this, &OrderListTable::itemSelectionChanged,
          this, &OrderListTable::handleChangedOrder);
}

void OrderListTable::constructNewOrder() {
  bool isOk;
  auto clientInfo = QInputDialog::getText(this, "Client info", "Enter mame of your organization",
                                           QLineEdit::EchoMode::Normal, {}, &isOk);
  if(!isOk){
    return;
  }
  if(clientInfo.size() > 64 || clientInfo.isEmpty()){
      QMessageBox::warning(this, "Warning", "The name is too long or empty");
      return;
  }

  Order newOrder{QDate::currentDate(), id, clientInfo};
  orders_.push_back(newOrder);

  setRowCount(orders_.size());
  updateTable(orders_.size()-1);

  ++id;
}

void OrderListTable::updateTable(int ind) {
  auto order =  orders_[ind];
  setItem(ind, 0, new QTableWidgetItem(QString::number(order.getNum())));
  setItem(ind, 1, new QTableWidgetItem(order.getClient()));
  setItem(ind, 2, new QTableWidgetItem(order.getDate().toString()));
  handleChangedOrder();
}



void OrderListTable::removeOrder(int ind) {
  orders_.remove(ind);
  for(int i = ind; i < orders_.size(); ++i){
    updateTable(i);
  }
  setRowCount(orders_.size());
}

void OrderListTable::removeChosenOrder() {
    if(this->selectedItems().empty()){
        return;
    }
    removeOrder(currentRow());
    handleChangedOrder();
}

void OrderListTable::handleChangedOrder(){
  Order* ord;
  if(selectedItems().empty()){
    ord = nullptr;
  } else {
    ord = &orders_[currentRow()] ;
  }
  emit orderChanged(ord);
}

const Order *OrderListTable::getCurrentOrder() const {
    if(selectedItems().empty()){
        return nullptr;
    } else {
        return &orders_[currentRow()];
    }
}

const QVector<Order>& OrderListTable::getOrderList(){
  return orders_;
}

bool OrderListTable::isEmpty() {
  return orders_.empty();
}

void OrderListTable::setOrderList(const QVector<Order>& newOrders) {
  orders_ = newOrders;
    setRowCount(orders_.size());
    for(int i = 0; i < orders_.size(); ++i){
        updateTable(i);
    }
}

void OrderListTable::clean() {
    orders_.clear();
    setRowCount(0);
}

void OrderListTable::sortByName(){
  std::sort(orders_.begin(), orders_.end(), [](const Order& left, const Order& right){
    return left.getClient().compare(right.getClient(), Qt::CaseInsensitive ) < 0;
  });

  for(int i = 0; i < orders_.size(); ++i){
    updateTable(i);
  }
}


