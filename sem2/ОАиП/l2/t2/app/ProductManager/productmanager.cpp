//
// Created by artemious on 11.03.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ProductManager.h" resolved

#include "productmanager.h"
#include "ui_productmanager.h"
#include <QMessageBox>
#include <QFileDialog>

ProductManager::ProductManager(QWidget *parent, QJsonDocument* toSet) :
        QDialog(parent), ui(new Ui::ProductManager) {
    ui->setupUi(this);

    connect(ui->addBtn, &QPushButton::clicked,
            this, &ProductManager::pushEntry);
    connect(ui->removeBtn, &QPushButton::clicked,
            this, &ProductManager::removeEntry);

    connect(ui->newBtn, &QPushButton::clicked,
            this, &ProductManager::newFile);
    connect(ui->openBtn, &QPushButton::clicked,
            this, &ProductManager::openFile);
    connect(ui->saveBtn, &QPushButton::clicked,
            this, &ProductManager::saveToFile);

    connect(this, &ProductManager::finished,
            this, &ProductManager::onClose);

    connect(ui->tableView, &ProductTable::itemChanged,
             this, &ProductManager::onItemChange);


    ui->tableView->setJson(&docFile);
    if(docFile.doc){
      //ui->tableView->setJson(&docFile);
      ui->tableView->LoadFromJSON();
    }
}

ProductManager::~ProductManager() {
    delete ui;
}

void ProductManager::newFile() {
    if(!isSaved){
        auto toSave = QMessageBox::question(this, "Save dialog", "Save the file?");
        if(toSave == QMessageBox::StandardButton::Yes){
            saveToFile();
        }
    }

    ui->tableView->clearContents();
    isSaved = false;
    docFile.clear();
}

void ProductManager::saveToFile() {
    if(isSaved){
        return;
    }
    if(!docFile.file){
        auto savePath = QFileDialog::getSaveFileName(this, "Save", {},
                                                     "JSON Product data (*.json)");
        if(savePath.isEmpty()){
            return;
        }
        docFile.file = QSharedPointer<QFile>{ new QFile(savePath) };
    }
    if(ui->tableView->WriteJSON() == 0){
      isSaved = true;
    }
}


void ProductManager::openFile() {

    auto openPath = QFileDialog::getOpenFileName(this, "Open", {},
                                                 "JSON Product data (*.json)");

    if(!openPath.isEmpty()){
        newFile();
        docFile.file = QSharedPointer<QFile>(new QFile(openPath));
        //ui->tableView->setJson(&docFile);
        ui->tableView->LoadFromJSON();
    }
    isSaved = true;
}

void ProductManager::pushEntry() {
    ui->tableView->pushProduct();
    isSaved = false;
}

void ProductManager::removeEntry() {
    ui->tableView->removeSelectedProduct();
    isSaved = false;
}


JSONDocFile ProductManager::getJson() {
  return docFile;
}

void ProductManager::setJson(JSONDocFile json_) {
  docFile = json_;
  ui->tableView->LoadFromJSON();
}

void ProductManager::onClose() {
  if(isSaved){
    return;
  }
    auto toSave = QMessageBox::question(this, "Save dialog", "Save the file?");
    if(toSave == QMessageBox::StandardButton::Yes){
        saveToFile();
    }
}

void ProductManager::onItemChange() {
    isSaved = false;
}
