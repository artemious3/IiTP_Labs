//
// Created by artemious on 29.02.24.
//

#ifndef T1_DATEINFO_H
#define T1_DATEINFO_H

namespace L2 {

    enum Month {
        January,
        February,
        March,
        April,
        May,
        June,
        July,
        August,
        September,
        October,
        November,
        December
    };

    enum DayOfWeek {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };

    class DateInfo {
    public:
        static short GetDaysInMonth(short month, bool isLeapYear);

//        static constexpr int cDaysIn4Years = 365 * 3 + 366;
//        static constexpr int cDaysIn100Years = cDaysIn4Years * 25 - 1;
//        static constexpr int cDaysIn400Years = cDaysIn100Years * 4 + 1;

        static constexpr int cDaysIn4Years = 365 * 3 + 366;
        static constexpr int cDaysIn100Years = cDaysIn4Years * 25 - 1;
        static constexpr int cDaysIn400Years = cDaysIn100Years * 4 + 1;

        //count from 0
        static const short cMonthMax = 12;

        //static const int cYearMax = 9999;
        static const int cYearMin = 1;


        static const int cDateLength = 11;

        static const int cDayOfWeekOfJan1AD1 = 0;

        static const int cDaysTo1Jan1970AD = 719162;
        static constexpr int cSecondsInDay = 60*60*24;

        static bool IsLeap(int year);
        static bool IsValidDate(short day, short month, int year);
        static short DaysFrom1JanTillCurMonth(Month month, bool isLeap);
    };

}


#endif //T1_DATEINFO_H
