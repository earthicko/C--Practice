#include <time.h>

#include <fstream>
#include <iostream>
#include <string>

#include "stack.h"
#include "vector.h"
#define DEFAULT_ROW 1
#define DEFAULT_COL 1

/*
하지만 실제 엑셀의 경우 셀이 문자열 데이터만 들어가는 것이 아니라, 숫자나 날짜
심지어 수식 까지도 들어갈 수 있습니다. 따라서 우리는 Cell 을 상속 받는 4 개의
StringCell. NumberCell, DateCell, ExprCell 클래스들을 만들어야 합니다.

또한 Cell 클래스에 to_numeric (데이터를 숫자로 바꾼다)과 std::stringify
(데이터를 문자열로 바꾼다) 함수들을 가상으로 정의하고, 4개의 클래스들이 이를
상속 받아서 구현할 수 있게 해야 합니다. (참고로 문자열을 숫자로 변환하면 그냥 0
이 되게 하면 됩니다)

또한 ExprCell 의 경우 간단한 수식에 대한 정보를 가지는 객체로, Cell 들 간의
연산을 사용할 수 있습니다. 에를 들어서 A1+B2+C6-6 와 같은 데이터가 들어 있는
ExprCell 에 to_numeric 함수를 호출하면A1, B2, C6 의 값을 더하고 (각 셀에
to_numeric 을 해서), 6 을 빼준 결과값이 나와야 합니다.

참고로 프로그래밍의 편의를 위해서 ExprCell 의 경우, 셀을 지칭하는 것은 딱 두
글자 (A1, Z9 처럼) 로 하고, 숫자는 오직 한 자리 수 정수, 그리고 가능한 연산자는
+, -, *, / 로 하겠습니다.
*/

class Table;
class TextTable;
class CSVTable;
class HTMLTable;
class Cell {
   protected:
    Table* table;
    int row, col;

    friend class Table;

   public:
    Cell();
    Cell(int row, int col);
    Cell(int row, int col, Table* table);

    virtual std::string to_string() = 0;
    virtual float to_numeric() = 0;
};

Cell::Cell() : row(0), col(0), table(NULL) {}

Cell::Cell(int row, int col) : row(row), col(col), table(NULL) {}

Cell::Cell(int row, int col, Table* table) : row(row), col(col), table(table) {}

class StringCell : public Cell {
    std::string data;

   public:
    StringCell(const std::string& data);
    std::string to_string() override;
    float to_numeric() override;
};

class NumberCell : public Cell {
    float data;

   public:
    NumberCell(const std::string& src);
    NumberCell(const float& src);
    std::string to_string() override;
    float to_numeric() override;
};

class DateCell : public Cell {
    time_t data;

   public:
    DateCell(const std::string& data);
    DateCell(const int& year, const int& month, const int& date);
    std::string to_string() override;
    float to_numeric() override;
};

class ExprCell : public Cell {
    std::string data;
    MyExcel::Vector expressions;

   public:
    ExprCell(const std::string& data);
    std::string to_string() override;
    float to_numeric() override;

    int precedence(char c);
    void describe();
};

StringCell::StringCell(const std::string& data) : Cell(), data(data) {}
NumberCell::NumberCell(const std::string& src) : Cell() {
    // string to float
    float converted = 0.0;
    data = converted;
}
NumberCell::NumberCell(const float& src) : Cell(), data(src) {}
DateCell::DateCell(const std::string& src) : Cell() {
    // string to date
    int year = atoi(src.c_str());
    int month = atoi(src.c_str() + 5);
    int day = atoi(src.c_str() + 8);

    time(&data);
    struct tm* timeinfo = localtime(&data);

    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;

    data = mktime(timeinfo);
}
DateCell::DateCell(const int& year, const int& month, const int& day) : Cell() {
    time(&data);
    std::cout << "Before time_t: " << data << '\n';
    struct tm* timeinfo = localtime(&data);
    // timeinfo = localtime(&data);

    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;

    data = mktime(timeinfo);
    std::cout << "After time_t: " << data << '\n';
}
ExprCell::ExprCell(const std::string& src) : Cell() {
    /*
    1. 피연산자 (셀 이름이나 숫자) 일 경우 그냥 exp_vec 에 넣습니다.
    2. 여는 괄호( (, [, { 와 같은 것들 ) 을 만날 경우 스택에 push 합니다.
    3. 닫는 괄호( ), ], } 와 같은 것들 ) 을 만날 경우 여는 괄호가 pop 될 때 까지
    pop 되는 연산자들을 exp_vec 에 넣습니다.
    4. 연산자일 경우 자기 보다 우선순위가 낮은 연산자가 스택 최상단에 올 때 까지
    (혹은 스택이 빌 때 까지) 스택을 pop 하고 (낮은 것은 pop 하지 않습니다), pop
    된 연산자들을 exp_vec 에 넣습니다. 그리고 마지막에 자신을 스택에 push
    합니다.
    */
    data = src;
    MyExcel::StrStack stack = MyExcel::StrStack();
    data.insert(0, "(");
    data.push_back(')');
    for (int i = 0; i < data.length(); i++) {
        if ('A' <= data[i] && data[i] <= 'Z') {
            // Cell name
            expressions.push_back(data.substr(i, 2));
            i++;
        } else if ('0' <= data[i] && data[i] <= '9') {
            // number
            expressions.push_back(data.substr(i, 1));
        } else if (data[i] == '(' || data[i] == '[' || data[i] == '{') {
            // opening
            stack.push(data.substr(i, 1));
        } else if (data[i] == ')' || data[i] == ']' || data[i] == '}') {
            // closing
            while (!stack.is_empty()) {
                std::string popped = stack.pop();
                if (popped == "(" || popped == "[" || popped == "{") break;
                expressions.push_back(popped);
            }
        } else if (data[i] == '+' || data[i] == '-' || data[i] == '*' ||
                   data[i] == '/') {
            // operator
            int this_presedence = precedence(data[i]);
            while (true) {
                std::string popped = stack.peek();
                if (precedence(popped[0]) >= this_presedence) {
                    popped = stack.pop();
                    expressions.push_back(popped);
                    continue;
                }
                if (precedence(popped[0]) < this_presedence || stack.is_empty())
                    break;
            }
            stack.push(data.substr(i, 1));
        }
    }
    data = src;
}

std::string StringCell::to_string() { return data; }
std::string NumberCell::to_string() {
    std::string converted = std::to_string(data);
    return converted;
}
std::string DateCell::to_string() {
    char buf[50];
    tm temp = *(localtime(&data));
    strftime(buf, 50, "%F", &temp);
    return std::string(buf);
}
std::string ExprCell::to_string() {
    std::string converted = std::to_string(to_numeric());
    return converted;
}

class Table {
   protected:
    Cell*** data_base;
    int alloc_row, alloc_col;
    int size_row, size_col;
    std::string delimiter;

   public:
    Table();
    Table(const Table* src);

    void copy_from(const Table* src);

    std::string get_info();
    virtual std::string print_table() = 0;
    void reg_cell(Cell* c, int row, int col);
    void reg_cell(const std::string& data, int row, int col);
    Cell* get_cell(int row, int col);
    Cell* get_cell(const std::string& name);

    void alloc(int row, int col);
    void clear();

    ~Table();
};

class TextTable : public Table {
   public:
    TextTable() : Table() {}
    TextTable(const Table* src) : Table(src) {}
    std::string print_table() override;
};

class CSVTable : public Table {
    std::string delimiter = ", ";

   public:
    CSVTable() : Table() {}
    CSVTable(const Table* src) : Table(src) {}
    std::string print_table() override;
};

class HTMLTable : public Table {
   public:
    HTMLTable() : Table() {}
    HTMLTable(const Table* src) : Table(src) {}
    std::string print_table() override;
};

Table::Table() {
    alloc_row = DEFAULT_ROW;
    alloc_col = DEFAULT_COL;

    size_row = 0;
    size_col = 0;

    data_base = new Cell**[alloc_row];
    for (int i = 0; i < alloc_row; i++) {
        data_base[i] = new Cell*[alloc_col];
        for (int j = 0; j < alloc_col; j++) data_base[i][j] = NULL;
    }
}

Table::Table(const Table* src) { copy_from(src); }

void Table::copy_from(const Table* src) {
    clear();
    alloc_row = src->alloc_row;
    alloc_col = src->alloc_col;
    size_row = src->size_row;
    size_col = src->size_col;

    data_base = new Cell**[alloc_row];
    for (int i = 0; i < alloc_row; i++) {
        data_base[i] = new Cell*[alloc_col];
        for (int j = 0; j < alloc_col; j++)
            data_base[i][j] = src->data_base[i][j];
    }
}

std::string Table::get_info() {
    std::string out = "Size:\n";
    out = out + "Rows: " + std::to_string(size_row) +
          " Cols: " + std::to_string(size_col) + "\n";
    out = out + "Allocated:\n";
    out = out + "Rows: " + std::to_string(alloc_row) +
          " Cols: " + std::to_string(alloc_col) + "\n";
    return out;
}

void Table::alloc(int row, int col) {
    // std::cout << "Before realloc" << '\n';
    // for (int i = 0; i < alloc_row; i++) {
    //     for (int j = 0; j < alloc_col; j++) {
    //         std::cout << '[' << i << ']' << '[' << j << ']' << ':'
    //                   << data_base[i][j] << '\n';
    //     }
    // }
    if (row > alloc_row && col > alloc_col) {
        // realloc all
        Cell*** temp = data_base;
        data_base = new Cell**[row];
        for (int i = 0; i < row; i++) {
            data_base[i] = new Cell*[col];
            for (int j = 0; j < col; j++) {
                if (i < alloc_row && j < alloc_col)
                    data_base[i][j] = temp[i][j];
                else
                    data_base[i][j] = NULL;
            }
        }
        // for (int i = 0; i < alloc_row; i++) delete[] temp[i];
        // delete temp;
        alloc_row = row;
        alloc_col = col;
    } else if (row <= alloc_row && col > alloc_col) {
        // only realloc col
        for (int i = 0; i < alloc_row; i++) {
            Cell** temp = data_base[i];
            data_base[i] = new Cell*[col];
            for (int j = 0; j < col; j++) {
                if (j < alloc_col)
                    data_base[i][j] = temp[j];
                else
                    data_base[i][j] = NULL;
            }
            // delete temp;
        }
        alloc_col = col;
    } else if (row > alloc_row && col <= alloc_col) {
        // only realloc row
        // 메모리가 한칸씩 밀림
        Cell*** temp = data_base;
        data_base = new Cell**[row];
        for (int i = 0; i < row; i++) {
            if (i < alloc_row) {
                data_base[i] = temp[i];
            } else {
                data_base[i] = new Cell*[alloc_col];
                for (int j = 0; j < alloc_col; j++) data_base[i][j] = NULL;
            }
        }
        alloc_row = row;
    } else {
        // do nothing
    }
    // std::cout << "Realloc result" << '\n';
    // for (int i = 0; i < alloc_row; i++) {
    //     for (int j = 0; j < alloc_col; j++) {
    //         std::cout << '[' << i << ']' << '[' << j << ']' << ':'
    //                   << data_base[i][j] << '\n';
    //     }
    // }
}

void Table::reg_cell(Cell* c, int row, int col) {
    if (row >= alloc_row || col >= alloc_col) alloc(row + 1, col + 1);
    if (data_base[row][col]) delete data_base[row][col];
    data_base[row][col] = c;
    c->row = row;
    c->col = col;
    c->table = this;

    if (row >= size_row) size_row = row + 1;
    if (col >= size_col) size_col = col + 1;
}

void Table::reg_cell(const std::string& data, int row, int col) {
    StringCell* new_cell = new StringCell(data);
    reg_cell(new_cell, row, col);
}

Cell* Table::get_cell(int row, int col) { return data_base[row][col]; }

Cell* Table::get_cell(const std::string& name) {
    char col_name = name[0] - 'A';
    char row_name = name[1] - '1';
    return data_base[row_name][col_name];
}

void Table::clear() {
    for (int i = alloc_row - 1; i >= 0; i--) {
        for (int j = alloc_col - 1; j >= 0; j--)
            if (data_base[i][j]) delete data_base[i][j];
        delete[] data_base[i];
    }
    delete data_base;
}

Table::~Table() { clear(); }

std::string TextTable::print_table() {
    /*    8       5    3
      |---A----|  B  | C |
    ======================
    1 | string | 0.5 |   |
    2 | 2      | du  | 1 |
    */
    std::string out = "  |";

    int* col_width = new int[size_col];
    for (int i = 0; i < size_col; i++) {
        int max = 0;
        for (int j = 0; j < size_row; j++) {
            if (data_base[j][i]) {
                int len = data_base[j][i]->to_string().length();
                if (len > max) max = len;
            }
        }
        col_width[i] = max + 2;
    }
    for (int i = 0; i < size_col; i++) {
        char c_col;
        if (0 <= i && i < 26)
            c_col = i + 'A';
        else
            c_col = '?';
        int space_count = col_width[i] - 1;
        out += std::string(space_count / 2, ' ');
        out += c_col;
        out += std::string(space_count / 2, ' ');
        if (space_count % 2) out += ' ';
        out += '|';
    }
    int row_width = out.length();
    out += '\n';
    out += std::string(row_width, '=');
    out += '\n';
    for (int i = 0; i < size_row; i++) {
        out = out + std::to_string(i + 1) + " |";
        for (int j = 0; j < size_col; j++) {
            std::string cell = " ";
            std::string content = "";
            if (data_base[i][j]) content = data_base[i][j]->to_string();
            cell += content;
            cell += std::string(col_width[j] - content.length() - 1, ' ');
            cell += '|';
            out += cell;
        }
        out += '\n';
    }

    return out;
}

std::string CSVTable::print_table() {
    std::string out = "";
    for (int i = 0; i < size_row; i++) {
        for (int j = 0; j < size_col; j++) {
            if (data_base[i][j]) out += data_base[i][j]->to_string();
            if (j != size_col - 1) out += delimiter;
        }
        out += '\n';
    }
    return out;
}

std::string HTMLTable::print_table() {
    /*  <!DOCTYPE html PUBLIC>
        <html>
        <head>
        <meta charset="EUC-KR">
        <title>초간단 테이블</title>
        </head>
        <body>
            <table border="1">
            <th>테이블</th>
            <th>만들기</th>
            <tr><!-- 첫번째 줄 시작 -->
                <td>첫번째 칸</td>
                <td>두번째 칸</td>
            </tr><!-- 첫번째 줄 끝 -->
            <tr><!-- 두번째 줄 시작 -->
                <td>첫번째 칸</td>
                <td>두번째 칸</td>
            </tr><!-- 두번째 줄 끝 -->
            </table>
        </body>
        </html>
    */
    std::string out = "<!DOCTYPE html PUBLIC>\n";
    out +=
        "<html>\n<head>\n<meta charset=\"EUC-KR\">\n<title>초간단 "
        "테이블</title>\n</head>\n";
    out += "<body>\n\t<table border=\"1\">\n";
    out += "\t<th> </th>\n";
    for (int i = 0; i < size_col; i++) {
        char c_col;
        if (0 <= i && i < 26)
            c_col = i + 65;
        else
            c_col = '?';
        out = out + "\t<th>" + c_col + "</th>\n";
    }
    for (int i = 0; i < size_row; i++) {
        out = out + "\t<tr>\n";
        out += "\t\t<td>" + std::to_string(i + 1) + "</td>\n";
        for (int j = 0; j < size_col; j++) {
            out = out + "\t\t<td>";
            if (data_base[i][j]) out = out + data_base[i][j]->to_string();
            out = out + "</td>\n";
        }
        out += "\t</tr>\n";
    }
    out += "\t</table>\n</body>\n</html>\n";

    return out;
}

std::ostream& operator<<(std::ostream& o, Table& t) {
    o << t.print_table();
    return o;
}
float StringCell::to_numeric() {
    float converted;
    try {
        converted = std::stof(data);
    } catch (const std::exception& expn) {
        converted = 0.0;
    }
    return converted;
}
float NumberCell::to_numeric() { return data; }
float DateCell::to_numeric() { return static_cast<int>(data); }
float ExprCell::to_numeric() {
    MyExcel::NumStack stack = MyExcel::NumStack();
    MyExcel::Vector expr(expressions);

    while (expr.size() > 0) {
        std::string target = expr[0];
        expr.remove(0);
        char rep = target[0];
        if ('0' <= rep && rep <= '9') {
            // number
            stack.push(std::stoi(target));
        } else if ('A' <= rep && rep <= 'Z') {
            // cell
            stack.push(table->get_cell(target)->to_numeric());
        } else if (rep == '+' || rep == '-' || rep == '*' || rep == '/') {
            // operator
            float b = stack.pop();
            float a = stack.pop();
            switch (rep) {
                case '+':
                    stack.push(a + b);
                    break;
                case '-':
                    stack.push(a - b);
                    break;
                case '*':
                    stack.push(a * b);
                    break;
                case '/':
                    stack.push(a / b);
                    break;

                default:
                    break;
            }
        }
        // std::cout << "============================\n";
        // std::cout << "Stack: ";
        // stack.print_all();
        // std::cout << "Vector: ";
        // expr.print_all();
    }
    float output = stack.pop();
    return output;
}

int ExprCell::precedence(char c) {
    switch (c) {
        case '(':
        case '[':
        case '{':
            return 0;
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
    }
    return 0;
}
void ExprCell::describe() {
    for (int i = 0; i < expressions.size(); i++) {
        std::cout << expressions[i] << ' ';
    }
    std::cout << std::endl;
}

class Excel {
    Table* table;

   public:
    Excel(int type);

    int parse_input(const std::string& input);
    void command_line();
};

Excel::Excel(int type) {
    switch (type) {
        case 0:
            table = new TextTable();
            break;
        case 1:
            table = new CSVTable();
            break;
        default:
            table = new HTMLTable();
            break;
    }
}

int Excel::parse_input(const std::string& input) {
    int next = 0;
    std::string command = "";
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ' ') {
            command = input.substr(0, i);
            next = i + 1;
            break;
        } else if (i == input.length() - 1) {
            command = input.substr(0, i + 1);
            next = i + 1;
            break;
        }
    }

    std::string to = "";
    for (int i = next; i < input.length(); i++) {
        if (input[i] == ' ' || i == input.length() - 1) {
            to = input.substr(next, i - next);
            next = i + 1;
            break;
        } else if (i == input.length() - 1) {
            to = input.substr(0, i + 1);
            next = i + 1;
            break;
        }
    }

    int col = to[0] - 'A';
    int row = atoi(to.c_str() + 1) - 1;

    std::string rest = input.substr(next);

    if (command == "sets") {
        // set string
        table->reg_cell(new StringCell(rest), row, col);
    } else if (command == "setn") {
        // set number
        table->reg_cell(new NumberCell(std::stof(rest)), row, col);
    } else if (command == "setd") {
        // set date
        table->reg_cell(new DateCell(rest), row, col);
    } else if (command == "sete") {
        // set expression
        table->reg_cell(new ExprCell(rest), row, col);
    } else if (command == "out") {
        // output
        std::ofstream out(to);
        out << *table;
        std::cout << "Saved to " << to << '\n';
    } else if (command == "exit") {
        return 0;
    }
    return 1;
}

void Excel::command_line() {
    std::string s;
    std::getline(std::cin, s);
    while (parse_input(s)) {
        std::cout << *table << std::endl << ">> ";
        std::getline(std::cin, s);
    }
}

int main() {
    std::cout << "Input table type: " << '\n';
    std::cout << "0: Text, 1: CSV, 2: HTML" << '\n';

    int type;
    std::cin >> type;

    Excel excel(type);
    excel.command_line();

    return 0;
}