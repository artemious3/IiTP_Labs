#include <iostream>
#include "Date.h"

using namespace L2;

void PrevNextDayTest(){
    for(int i = 0; i < 1'000'000; ++i){
        Date dt(i);
        if(dt.ToString() == dt.NextDay().ToString() || dt.ToString() == dt.PreviousDay().ToString()){
            std::cout << dt.ToString() << '\n';
        }
    }
}

void DayTest(){
    for(int i = 1; i <= 9999; ++i){
        for(int m = 1; m <= 12; ++m){
            for(int d = 1; d < 29; ++d){
                Date dt (d,m,i);
                if(dt.getDay() != d || dt.getMonth() != (Month)(m-1) || dt.getYear() != i){
                    std::cout << dt.ToString() << '\n';
                    std::cout << "exp: " << d << ' ' << m << ' ' << i << '\n';
                    std::cout << "___________\n";
                }
            }
        }
    }
}

void BorderDayTest(){
    for(int y = 1; y <= 9999; ++y){
        for(int m = 1; m <= 12; ++m){
            int lastDay = DateInfo::GetDaysInMonth(m-1, DateInfo::IsLeap(y-1));
            Date dt(lastDay, m, y);
            if(dt.NextDay().getDay() != 1){
                std::cout << dt.NextDay().ToString() << '\n';
                std::cout << "_____________\n";
            }
        }
    }
}

int main() {
    PrevNextDayTest();
    DayTest();
    BorderDayTest();
}

