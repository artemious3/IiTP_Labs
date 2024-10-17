//
// Created by artemious on 10.03.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_OrderListWindow.h" resolved

#include "orderlistwindow.h"
#include "ui_orderlistwindow.h"
#include "productmanager.h"
#include <QMessageBox>
#include <QFileDialog>

OrderListWindow::OrderListWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::OrderListWindow) {
    ui->setupUi(this);

    setFixedSize(this->geometry().size());

    orderLoader = new OrderLoader(this);

  connect(ui->addOrder, &QPushButton::clicked,
          ui->orderList, &OrderListTable::constructNewOrder);
  connect(ui->deleteOrder, &QPushButton::pressed,
          ui->orderList, &OrderListTable::removeChosenOrder);
  connect(ui->sortOrderListBtn, &QPushButton::clicked,
          ui->orderList, &OrderListTable::sortByName);

  connect(ui->orderList, &OrderListTable::orderChanged,
          ui->productList, &ProductListTable::setOrderSlot);
  connect(ui->orderList, &OrderListTable::orderChanged,
          this, &OrderListWindow::changeProductListLabel);

  connect(ui->pushProduct, &QPushButton::clicked,
          ui->productList, &ProductListTable::constructNewProduct);
  connect(ui->removeProduct, &QPushButton::clicked,
          ui->productList, &ProductListTable::removeChosenProduct);
  connect(ui->editAmountBtn, &QPushButton::clicked,
          ui->productList, &ProductListTable::editChosenProduct);

  connect(ui->makeInvoice, &QPushButton::clicked,
          this,  &OrderListWindow::makeInvoice);

  connect(ui->manageProducts, &QPushButton::clicked,
          this, &OrderListWindow::manageProducts);

  connect(ui->newFile, &QPushButton::clicked,
          this, &OrderListWindow::newFile);
  connect(ui->openFile, &QPushButton::clicked,
          this, &OrderListWindow::openFile);
  connect(ui->saveButton, &QPushButton::clicked,
          this, &OrderListWindow::saveFile);

}

OrderListWindow::~OrderListWindow() {
  if(priceList){
    delete priceList;
  }
    if(ordersFile){
      delete ordersFile;
    }

    delete orderLoader;
    delete ui;
}

void OrderListWindow::makeInvoice(){
if(!priceList){
  QMessageBox::warning(this, "Warning", "Load list of products before making invoice");
  return;
}
  ui->invoiceTable->Construct(ui->orderList->getCurrentOrder(), priceList);
}

void OrderListWindow::manageProducts() {
    ProductManager mgr;
    mgr.setJson(jsonProducts);
    mgr.exec();
    jsonProducts = mgr.getJson();
    makePriceList();
}

void OrderListWindow::makePriceList() {
  if(!jsonProducts.doc){
    QMessageBox::warning(this, "Warning", "List of products is empty");
    return;
  }

  try{
      priceList = PriceList::loadFromFile(jsonProducts.doc);
  } catch (const std::invalid_argument& ex) {
      QMessageBox::warning(this, "Warning", "The product list is invalid:\nName of products should be unique");
      delete priceList;
      priceList = nullptr;
  }

  if(!priceList){
    QMessageBox::warning(this, "Warning", "List of products is empty");
    return;
  }
}

void OrderListWindow::changeProductListLabel() {
  auto curOrd = ui->orderList->getCurrentOrder();
  if(curOrd){
    ui->prodListLabel->setText( QString("Product List %1").arg(curOrd->getNum()));
  }
}

void OrderListWindow::saveFile() {
  if(!ordersFile){
    auto savePath = QFileDialog::getSaveFileName(this, "Save", {},
                                                 "JSON Product data (*.json)");
    if(!savePath.endsWith(".json")){
        savePath += ".json";
    }
    if(savePath.isEmpty()){
      return;
    }
    ordersFile = new QFile(savePath);
    ui->filePathLabel->setText(savePath);
  }
  orderLoader->writeToJson(ui->orderList->getOrderList(),  ordersFile);

}

void OrderListWindow::newFile() {
  bool toSave = false;
  if(!ui->orderList->isEmpty()){
    auto saveQuestion = QMessageBox::question(this, "Save dialog", "Save the file?");
    if(saveQuestion == QMessageBox::StandardButton::Yes){
      toSave = true;
    }
  } else {
    toSave = false;
  }
  if(toSave){
    saveFile();
  }

  if(ordersFile){
    ui->orderList->clearContents();
    ordersFile->close();
    delete ordersFile;
    ordersFile = nullptr;
  }
  ui->orderList->clean();
  ui->invoiceTable->clean();
  ui->filePathLabel->setText("[New File]");
}
void OrderListWindow::openFile() {
  auto openPath = QFileDialog::getOpenFileName(this, "Open", {},
                                               "JSON Product data (*.json)");
  if(!openPath.isEmpty()){
    newFile();
    ordersFile = new QFile(openPath);
    auto newEntryList = orderLoader->loadFromJson(ordersFile);
    ui->orderList->setOrderList(newEntryList);
    ui->filePathLabel->setText(openPath);
  }
}
