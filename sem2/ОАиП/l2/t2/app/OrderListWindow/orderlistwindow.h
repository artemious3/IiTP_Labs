//
// Created by artemious on 10.03.24.
//

#ifndef T2_ORDERLISTWINDOW_H
#define T2_ORDERLISTWINDOW_H

#include <QWidget>
#include "PriceList.h"
#include "ProductTable.h"
#include "OrderLoader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OrderListWindow; }
QT_END_NAMESPACE



class OrderListWindow : public QWidget {
Q_OBJECT

public:
    explicit OrderListWindow(QWidget *parent = nullptr);
    ~OrderListWindow() override;

private:
    Ui::OrderListWindow *ui;

    PriceList* priceList = nullptr;
    JSONDocFile jsonProducts;

    QFile* ordersFile = nullptr;
    OrderLoader* orderLoader = nullptr;


    void makePriceList();

private slots:
    void makeInvoice();
    void manageProducts();
    void changeProductListLabel();

    void openFile();
    void saveFile();
    void newFile();



};


#endif //T2_ORDERLISTWINDOW_H
