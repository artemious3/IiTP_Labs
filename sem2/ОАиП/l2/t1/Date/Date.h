#ifndef T1_DATE_H
#define T1_DATE_H

#include "DateInfo.h"
#include <string>

namespace L2 {

    class Date {
        long daysFromAD1_= 0;
        bool isLeap_ = false;
        short weekNumber_ = 0;

        int year_ = 0;
        Month month_ = January;
        short day_ = 0;


    public:
        Date() = default;
        Date(const Date&) = default;
        Date(short userDay, short userMonth, int userYear);
        Date(const std::string& formattedDate);
    public:
        Date(long daysTo0AD);

    public:
        int getYear() const;
        Month getMonth() const;
        short getDay() const;
        long GetDaysFromAD1() const;
        bool IsLeap() const;
        std::string ToString() const;

        Date NextDay() const;
        Date PreviousDay() const;
        long Duration(Date dt) const;
        int DaysTillYourBirthday(const Date* dt) const;
        DayOfWeek GetDayOfWeek() const;
        short WeekNumber() const;
        std::string GetDayOfWeekStr() const;
        bool operator ==(const Date& other) const;

    private:
        void SetDaysFromAD1(long newDays);
        void SetDate(short day, Month month, int year);

    public:
        static Date Today();
    };

} // L2

#endif //T1_DATE_H
