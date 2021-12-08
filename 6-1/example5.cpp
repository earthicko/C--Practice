#include <iostream>
#include <string>

class Employee {
   protected:
    std::string name;
    int age;
    std::string position;
    int rank;

   public:
    Employee(std::string name, int age, std::string position, int rank)
        : name(name), age(age), position(position), rank(rank) {}

    Employee(const Employee& src) {
        name = src.name;
        age = src.age;
        position = src.position;
        rank = src.rank;
    }

    Employee() {}

    virtual void print_info() {
        std::cout << "노동자 " << name << " (" << position << " , " << age
                  << "세) ==> " << calculate_pay() << " 만원" << std::endl;
    }

    virtual int calculate_pay() { return 200 + rank * 50; }
};

class Manager : public Employee {
    int year_of_service;

   public:
    Manager(std::string name, int age, std::string position, int rank,
            int year_of_service)
        : year_of_service(year_of_service),
          Employee(name, age, position, rank) {}

    Manager(const Manager& src)
        : Employee(src.name, src.age, src.position, src.rank),
          year_of_service(src.year_of_service) {}

    Manager() : Employee() {}

    int calculate_pay() override {
        return 200 + rank * 50 + year_of_service * 5;
    }

    void print_info() override {
        std::cout << "관리자 " << name << " (" << position << ", " << age
                  << "세, " << year_of_service << "년차) ==> " << calculate_pay()
                  << "만원" << std::endl;
    }
};

class EmployeeList {
    int alloc_employees;
    int current_employees;
    Employee** employees;

   public:
    EmployeeList(int alloc_employees) : alloc_employees(alloc_employees) {
        employees = new Employee*[alloc_employees];
        current_employees = 0;
    }

    void alloc_employee(int new_alloc) {
        if (current_employees < new_alloc) {
            Employee** temp = employees;
            employees = new Employee*[new_alloc];
            for (int i = 0; i < current_employees; i++) employees[i] = temp[i];
            alloc_employees = new_alloc;
            delete[] temp;
        }
    }

    void add_employee(Employee* new_employee) {
        if (alloc_employees == current_employees)
            alloc_employee((alloc_employees + 1) * 2);
        employees[current_employees] = new_employee;
        current_employees++;
    }

    int length() { return current_employees; }

    void print_info() {
        int total_pay = 0;
        for (int i = 0; i < current_employees; i++) {
            employees[i]->print_info();
            total_pay += employees[i]->calculate_pay();
        }
        std::cout << "총 임금: " << total_pay << " 만원" << std::endl;
    }

    ~EmployeeList() {
        for (int i = 0; i < current_employees; i++) delete employees[i];
        delete[] employees;
    }
};

int main() {
    EmployeeList emp_list(10);

    emp_list.add_employee(new Employee("노홍철", 34, "평사원", 1));
    emp_list.add_employee(new Employee("하하", 34, "평사원", 1));
    emp_list.add_employee(new Manager("유재석", 41, "부장", 7, 12));
    emp_list.add_employee(new Manager("정준하", 43, "과장", 4, 15));
    emp_list.add_employee(new Manager("박명수", 43, "차장", 5, 13));
    emp_list.add_employee(new Employee("정형돈", 36, "대리", 2));
    emp_list.add_employee(new Employee("길", 36, "인턴", -2));
    emp_list.print_info();
    return 0;
}