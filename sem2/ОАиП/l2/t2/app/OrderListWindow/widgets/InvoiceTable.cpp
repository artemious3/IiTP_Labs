//
// Created by Артем Подгайский on 11.03.24.
//

#include "InvoiceTable.h"
#include <QMessageBox>
#include <QHeaderView>

InvoiceTable::InvoiceTable(QWidget *parent) : QTableWidget(parent) {
  setColumnCount(4);
  setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
  setSelectionBehavior(QTableWidget::SelectionBehavior::SelectRows);
  setSelectionMode(QTableWidget::SelectionMode::SingleSelection);
  setHorizontalHeaderItem(0, new QTableWidgetItem("Product name"));
  setHorizontalHeaderItem(1, new QTableWidgetItem("Price"));
  setHorizontalHeaderItem(2, new QTableWidgetItem("Amount"));
  setHorizontalHeaderItem(3, new QTableWidgetItem("Total Cost"));

  horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
}


void InvoiceTable::Construct(const Order *ord, const PriceList* prList) {
  if(ord == nullptr || prList == nullptr ){
    qWarning() << "INVOICE: NULLPTR ORDER OR PRICE LIST";
    return;
  }

try{
    invoice = Invoice::fromOrder(ord, prList);
    UpdateTable();
} catch (const std::invalid_argument& ex){
    QMessageBox::warning(this, "Warning", ex.what());
    clearContents();
  }
}

void InvoiceTable::UpdateTable() {
  auto entries = invoice.getEntries();
  setRowCount(entries.size() > 0 ? entries.size()+1 : 0);

  if(entries.size() == 0){
    return;
  }

  setSpan(0, 0, 1, 3);
  setItem(0,0, new QTableWidgetItem("TOTAL"));
  setItem(0,3, new QTableWidgetItem(QString::number(invoice.getTotalCost())));

  int i = 1;
  foreach(auto entry, entries){
    setItem(i, 0, new QTableWidgetItem(entry.product.getName()));
    setItem(i, 1, new QTableWidgetItem(QString::number(entry.product.Price)));
    setItem(i, 2, new QTableWidgetItem(QString::number(entry.amount)));
    setItem(i, 3, new QTableWidgetItem(QString::number(entry.totalPrice)));
    ++i;
  }
}

void InvoiceTable::clean() {
  invoice.clear();
  UpdateTable();
}

