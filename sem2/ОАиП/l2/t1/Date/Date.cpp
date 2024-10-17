//
// Created by artemious on 27.02.24.
//

#include "Date.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>

#define DEBUG(var) std::cout << #var << " : " << var << '\n';

namespace L2 {

    void Date::SetDate(short day, Month month, int signedYear) {
        int year = std::abs(signedYear);
        isLeap_ = DateInfo::IsLeap(year);
        int leapYears = std::floor(year / 4) - std::floor(year / 100) + std::floor(year / 400);
        int unleapYears = year - leapYears;

        long daysFromAd1  = leapYears * 366 + unleapYears * 365
                            + DateInfo::DaysFrom1JanTillCurMonth(month, isLeap_)
                            + day;

        SetDaysFromAD1(daysFromAd1);
    }


    /*We trust the most horrible part of this task to this very function.*/
    void Date::SetDaysFromAD1(long newDays) {
        daysFromAD1_ = newDays;
        year_ = 0;

        long rem = std::abs(daysFromAD1_);

        {
            long full400 = rem / DateInfo::cDaysIn400Years;
            year_ += full400 * 400;
            rem = rem % DateInfo::cDaysIn400Years;
        }

        {
            //A horrible private case, when we reach the last century of 400 years period
            //God damn human's calendar system.
            if(rem == 4 * DateInfo::cDaysIn100Years){
                year_ += 399;
                month_ = December;
                day_ = 30;
                isLeap_ = true;
                weekNumber_ = (365 + (daysFromAD1_ - 365)%7 ) / 7;
                return;
            }
            else{
                long full100 = rem / DateInfo::cDaysIn100Years;
                rem = rem % DateInfo::cDaysIn100Years;
                year_ += full100 * 100;
            }

        }

        {
            long full4;
            full4 = rem / DateInfo::cDaysIn4Years;
            rem = rem % DateInfo::cDaysIn4Years;
            year_ += full4 * 4;
        }

        {
            //One more horrible private case, when we reach the last year of 4 years period
            //God damn human's calendar system.
            if(rem == 365*4){
                year_ += 3;
                month_ = December;
                day_ = 30;
                isLeap_ = true;
                weekNumber_ = (365 + (daysFromAD1_ - 365)%7 ) / 7;
                return;
            } else {
                long full1 = rem / 365;
                year_ += full1;
                rem = rem % 365;
            }
        }


        {
            isLeap_ = DateInfo::IsLeap(year_);
            short day = 0,
                    month = 0;
            while (day <= rem) {
                day += DateInfo::GetDaysInMonth(month, isLeap_);
                month++;
            }
            day -= DateInfo::GetDaysInMonth(month - 1, isLeap_);

            month_ = static_cast<Month>(month - 1);
            weekNumber_ = (rem +  (daysFromAD1_-rem)%7) / 7;
            day_ = rem - day;
        }
    }

    long Date::GetDaysFromAD1() const {
        return daysFromAD1_;
    }


    std::string Date::ToString() const{
        std::stringstream ss;

        ss << std::setfill('0') << std::setw(2) << day_ + 1 << '.'
           << std::setfill('0') << std::setw(2) << month_ + 1 << '.'
           << std::setfill('0') << std::setw(4) << year_ + 1;
        return ss.str();
    }

    Date Date::NextDay() const{
        Date nextDay(*this);
        nextDay.SetDaysFromAD1(GetDaysFromAD1() + 1);
        return nextDay;
    }

    Date Date::PreviousDay() const{
        Date prevDay(*this);
        prevDay.SetDaysFromAD1(GetDaysFromAD1() - 1);
        return prevDay;
    }


    long Date::Duration(Date dt) const{
        return std::abs(dt.daysFromAD1_ - this->daysFromAD1_);
    }

    Date::Date(short userDay, short userMonth, int userYear) {
        SetDate(userDay - 1, static_cast<Month>(userMonth-1), userYear -1 );
    }

    Date::Date(long daysTo1AD) {
        SetDaysFromAD1(daysTo1AD);
    }

    Date::Date(const std::string& formattedDate) {
        std::istringstream ss(formattedDate);
        int year;
        short month, day;
        char sep;
        ss >> day >> sep >> month >> sep >> year;
        if(!DateInfo::IsValidDate(day, month, year) ||
        ss.fail() ||
        ss.tellg() != -1 ||
        formattedDate.size() != DateInfo::cDateLength-1 /* \n is the last symbol */){
            throw std::runtime_error("Wrong data format: \n\"" + (formattedDate.size()<11 ?
                                                                    formattedDate :
                                                                    formattedDate.substr(0, 10) + "..." ) + "\"");
        }

        SetDate(day-1, static_cast<Month>(month-1), year>=1 ? year-1 : year);

    }

    bool Date::IsLeap() const {
        return isLeap_;
    }

    DayOfWeek Date::GetDayOfWeek() const {
        return static_cast<DayOfWeek> (((long)DateInfo::cDayOfWeekOfJan1AD1 + daysFromAD1_) % 7);
    }

    int Date::getYear() const {
        return year_>=0 ? year_+1 : year_-1;
    }


    Month Date::getMonth() const {
        return month_;
    }

    short Date::getDay() const {
        return day_+1;
    }

    Date Date::Today() {
        Date today(DateInfo::cDaysTo1Jan1970AD +
        std::time(nullptr)/DateInfo::cSecondsInDay);

        return today;
    }

    std::string Date::GetDayOfWeekStr() const {
        DayOfWeek dw = GetDayOfWeek();
        switch (dw) {
            case Monday:
                return "Monday";
            case Tuesday:
                return "Tuesday";
            case Wednesday:
                return "Wednesday";
            case Thursday:
                return "Thursday";
            case Friday:
                return "Friday";
            case Saturday:
                return "Saturday";
            case Sunday:
                return "Sunday";
        }
        return "wtf????";
    }

    int Date::DaysTillYourBirthday(const Date* dt) const{
        Date thisYearBD {};
        if( dt->day_ == 28 && dt->month_ == February){
            thisYearBD.SetDate(isLeap_ ? 28 : 27, February, this->year_);
        } else {
            thisYearBD.SetDate(dt->day_, dt->month_, this->year_);
        }

        int span;

        if(thisYearBD.daysFromAD1_ - this->daysFromAD1_ >= 0){
            span = thisYearBD.daysFromAD1_ - this->daysFromAD1_;
        } else {
            Date nextYearBD{};

            if(dt->day_ == 28 && dt->month_ == February){
                nextYearBD.SetDate(DateInfo::IsLeap(year_ + 1) ? 28 : 27, February, this->year_+1);
            } else {
                nextYearBD.SetDate(dt->day_, dt->month_, this->year_+1);
            }
            span = nextYearBD.daysFromAD1_ - this->daysFromAD1_;
        }

        return span;
    }

    short Date::WeekNumber() const {
        return weekNumber_+1;
    }

    bool Date::operator==(const Date &other) const{
        return other.daysFromAD1_ == this->daysFromAD1_;
    }


} // L2