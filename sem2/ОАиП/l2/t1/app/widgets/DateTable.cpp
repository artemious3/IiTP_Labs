//
// Created by artemious on 04.03.24.
//

#include "DateTable.h"
#include "Date.h"
#include <QHeaderView>
#include <QInputDialog>

DateTable::DateTable(QWidget *parent) : QTableWidget(parent) {
    setColumnCount(3);
    setRowCount(0);

    setHorizontalHeaderItem(0, new QTableWidgetItem("Date"));
    setHorizontalHeaderItem(1, new QTableWidgetItem("Next Day"));
    setHorizontalHeaderItem(2, new QTableWidgetItem("Days to next"));

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void DateTable::loadFromString(QString *str) {
    if(!str)
        return;

    QTextStream ts (str, QTextStream::OpenModeFlag::ReadOnly);
    while(!ts.atEnd()){
        QString curDateStr = ts.readLine();
        if(curDateStr.isEmpty()){
            continue; //Ignore empty lines in file
        }
        L2::Date dt (curDateStr.toStdString());
        dates.push_back(dt);
    }

    this->setRowCount(dates.count());

    for(int i = 0; i < dates.count(); ++i){
        displayDate(dates[i], i);
    }
}

void DateTable::displayDate(const Date &dt, int ind) {
    auto dateItem = new QTableWidgetItem(QString::fromStdString(dt.ToString()));
    this->setItem(ind, 0, dateItem);

    auto nextDayItem = new QTableWidgetItem(QString::fromStdString(dt.NextDay().ToString()));
    this->setItem(ind, 1, nextDayItem);

    if(ind != dates.count()-1){
        long span = dt.Duration(dates[ind + 1]);
        auto spanItem = new QTableWidgetItem(QString::number(span));
        this->setItem(ind, 2, spanItem);
    } else {
        auto spanItem = new QTableWidgetItem("--");
        this->setItem(ind, 2, spanItem);
    }
}

void DateTable::pushDate(const Date &dt) {
    setRowCount(rowCount()+1);
    dates.push_back(dt);
    displayDate(dt, rowCount() - 1);
    if(rowCount() >= 2){
        displayDate(dates[rowCount() - 2], rowCount() - 2);
    }
}

void DateTable::changeDate(const Date &dt, int ind) {
    dates[ind] = dt;
    if(ind > 0){
        displayDate(dates[ind-1], ind-1);
    }
    displayDate(dt, ind);
}

void DateTable::deleteDate(int ind) {
  dates.remove(ind);
  for(int i = ind>0 ? ind-1 : ind; i < dates.count(); ++i){
    displayDate(dates[i], i);
  }
  setRowCount(dates.count());
}

const QVector<Date> &DateTable::GetDates() const {
  return dates;
}

void DateTable::clear() {
    //QTableWidget::clear();
    QTableWidget::clearContents();
    dates.clear();
}

const Date *DateTable::getCurrentDate() const {
    if(selectedItems().empty())
        return nullptr;
    else{
        return &dates[currentRow()];
    }
}

