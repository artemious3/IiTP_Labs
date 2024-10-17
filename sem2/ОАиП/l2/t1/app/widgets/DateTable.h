//
// Created by artemious on 04.03.24.
//
#ifndef T1_DATETABLE_H
#define T1_DATETABLE_H

#include <QTableWidget>
#include "Date.h"

using namespace L2;
class DateTable : public QTableWidget {
private:
    QVector<Date> dates;

public:
    explicit DateTable(QWidget *parent = nullptr);

    void displayDate(const Date &dt, int ind);
    void loadFromString(QString *str = nullptr);
    void pushDate(const Date &dt);
    void changeDate(const Date& dt, int ind);
    void deleteDate(int ind);
    void clear();
    const Date* getCurrentDate() const;

    const QVector<Date>& GetDates() const;

};


#endif //T1_DATETABLE_H
