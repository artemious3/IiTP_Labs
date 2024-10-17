//
// Created by artemious on 11.03.24.
//

#ifndef T2_PRODUCTMANAGER_H
#define T2_PRODUCTMANAGER_H

#include <QFile>
#include <QDialog>
#include <QJsonDocument>
#include <QSharedPointer>
#include "ProductTable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class ProductManager; }
QT_END_NAMESPACE

class ProductManager : public QDialog {
    Q_OBJECT

    JSONDocFile docFile;
    bool isSaved = false;

public:
    explicit ProductManager(QWidget *parent = nullptr, QJsonDocument* toSet = nullptr);
    ~ProductManager() override;

    JSONDocFile getJson();
    void setJson(JSONDocFile doc);


private:
    Ui::ProductManager *ui;

private slots:
    void openFile();
    void saveToFile();
    void newFile();

    void pushEntry();
    void removeEntry();
    void onClose();

    void onItemChange();
};


#endif //T2_PRODUCTMANAGER_H
