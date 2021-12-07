#include <iostream>
#include <string>

class Employee {
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

    void print_info() {
        std::cout << name << " (" << position << " , " << age << ") ==> "
                  << calculate_pay() << " * 10^4 KRW" << std::endl;
    }

    int calculate_pay() { return 200 + rank * 50; }
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

    void alloc(int new_alloc) {
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
            alloc((alloc_employees + 1) * 2);
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
        std::cout << "Total wage: " << total_pay << " * 10^4 KRW" << std::endl;
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
    emp_list.add_employee(new Employee("유재석", 41, "부장", 7));
    emp_list.add_employee(new Employee("정준하", 43, "과장", 4));
    emp_list.add_employee(new Employee("박명수", 43, "차장", 5));
    emp_list.add_employee(new Employee("정형돈", 36, "대리", 2));
    emp_list.add_employee(new Employee("길", 36, "인턴", -2));
    
    emp_list.print_info();

    return 0;
}