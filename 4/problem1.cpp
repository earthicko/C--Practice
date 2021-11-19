#include <iostream>

int& clip_int(int& orig, int start, int end) {
    if (orig < start) orig = start;
    if (end < orig) orig = end;
    return orig;
}

class Data {
    int year;
    int month;
    int day;
    int day_in_m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

   public:
    void add_day(int inc) {
        int added = day + inc;
        if (added < 1) {
            int new_inc = inc + day;
            day = day_in_m[(month - 2) % 12];
            add_month(-1);
            add_day(new_inc);
        } else if (day_in_m[month - 1] < added) {
            int new_inc = added - day_in_m[month - 1] - 1;
            day = 1;
            add_month(1);
            add_day(new_inc);
        } else {
            day = added;
        }
    }
    void add_month(int inc) {
        int added = month + inc;
        if (added < 1) {
            int new_inc = added + 12 - 1;
            month = 1;
            add_year(-1);
            add_month(new_inc);
        } else if (added > 12) {
            int new_inc = added - 12 - 1;
            month = 1;
            add_year(1);
            add_month(new_inc);
        } else {
            month = added;
        }
    }
    void add_year(int inc) {
        int added = year + inc;
        year = clip_int(added, 0, 5000);
    }
    void set_date(int _year, int _month, int _day) {
        year = clip_int(_year, 0, 5000);
        month = clip_int(_month, 1, 12);
        switch (month) {
            case 1:
                day = clip_int(_day, 1, 31);
                break;
            case 2:
                day = clip_int(_day, 1, 28);
                break;
            case 3:
                day = clip_int(_day, 1, 31);
                break;
            case 4:
                day = clip_int(_day, 1, 30);
                break;
            case 5:
                day = clip_int(_day, 1, 31);
                break;
            case 6:
                day = clip_int(_day, 1, 30);
                break;
            case 7:
                day = clip_int(_day, 1, 31);
                break;
            case 8:
                day = clip_int(_day, 1, 31);
                break;
            case 9:
                day = clip_int(_day, 1, 30);
                break;
            case 10:
                day = clip_int(_day, 1, 31);
                break;
            case 11:
                day = clip_int(_day, 1, 30);
                break;
            case 12:
                day = clip_int(_day, 1, 31);
                break;
            default:
                break;
        }
    }
    void print_date() {
        std::cout << year << "-" << month << "-" << day << std::endl;
    }
};

int main() {
    Data data;
    data.set_date(2021, 11, 19);
    data.print_date();

    std::cout << "Add 3 days" << std::endl;
    data.add_day(3);
    data.print_date();

    std::cout << "Add 12 days" << std::endl;
    data.add_day(12);
    data.print_date();

    std::cout << "Add 2 months" << std::endl;
    data.add_month(2);
    data.print_date();

    std::cout << "Add -1 years" << std::endl;
    data.add_year(-1);
    data.print_date();

    std::cout << "Add 5000 years" << std::endl;
    data.add_year(5000);
    data.print_date();

    std::cout << "Add -10000 years" << std::endl;
    data.add_year(-10000);
    data.print_date();

    std::cout << "Add 2021 years" << std::endl;
    data.add_year(2021);
    data.print_date();

    std::cout << "Add -35 days" << std::endl;
    data.add_day(-35);
    data.print_date();

    std::cout << "Add +100 days"<<std::endl;
    data.add_day(100);
    data.print_date();

    return 0;
}