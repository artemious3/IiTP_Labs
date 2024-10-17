//
// Created by artemious on 29.02.24.
//

#include "DateInfo.h"
#include <iostream>

namespace L2 {

    bool DateInfo::IsLeap(int year) {
        return (year+1) % 4 == 0 && (year+1) % 100 != 0 || (year+1) % 400 == 0;
    }

    short DateInfo::GetDaysInMonth(short month, bool isLeapYear) {
        switch (month) {
            case 0: // January
            case 2: // March
            case 4: // May
            case 6: // July
            case 7: // August
            case 9: // October
            case 11: // December
                return 31;
            case 3: // April
            case 5: // June
            case 8: // September
            case 10: // November
                return 30;
            case 1: // February
                if (isLeapYear) {
                    return 29;
                } else {
                    return 28;
                }
            default:
                return -1; // Invalid month_
        }
    }

    short DateInfo::DaysFrom1JanTillCurMonth(Month month, bool isLeap) {
        short res{};
        for(int i = 0; i < (int)(month); ++i){
            res += GetDaysInMonth(i, isLeap);
        }
        return res;
    }

    /* Arguments in user bonds, that is no zero values */
    bool DateInfo::IsValidDate(short day, short month, int year) {
        return year > 0 &&
            month >= 1 && month <= cMonthMax &&
            day >= 1 && day <= GetDaysInMonth(month-1, IsLeap(year-1));
    }
}