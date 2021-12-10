#include <fstream>
#include <iostream>
#include <string>
#define DEFAULT_ROW 100
#define DEFAULT_COL 100

/*문제 1 (난이도 :中)
일단 엑셀의 셀들의 정보 (일단 단순한 std::string 이라고 생각합시다) 에 대한 Cell
클래스가 있고 이 Cell 객체들을 모아두는 Table 클래스가 있습니다. Table
클래스에는 2차원 배열로 Cell 객체들에 대한 정보 (참고로 Cell 객체가 생성 될 때
마다 동적으로 Cell 객체를 생성합니다.) 가 보관되어 있습니다. 또한 Table 클래스에
전체 데이터를 출력하는 print_table 함수가 가상으로 정의되어 있습니다.

여러분은 Table 클래스를 상속 받는 TextTable, CSVTable, HTMLTable 클래스를
만들어서 print_table 함수를 오버라이드 할 함수들을 제작할 것입니다. 예를 들어
TextTable 클래스의 print_table 함수는 텍스트 형식으로, CSVTable 은 CSV 파일
형식으로 등등 만들어야 겠지요? 제가 아래 대충 프로그램의 골격을 잡아 놓았으니
여러분들은 이를 채우기만 하면 됩니다.
*/

class Table;
class TextTable;
class CSVTable;
class HTMLTable;
class Cell {
    Table* table;
    std::string data;
    int row, col;

    friend class Table;

   public:
    Cell(const std::string& data);
    Cell(const std::string& data, int row, int col);
    Cell(const std::string& data, int row, int col, Table* table);

    std::string get_data();
};

Cell::Cell(const std::string& data) : data(data) {
    row = 0;
    col = 0;
    table = NULL;
}

Cell::Cell(const std::string& data, int row, int col)
    : data(data), row(row), col(col) {
    table = NULL;
}

Cell::Cell(const std::string& data, int row, int col, Table* table)
    : data(data), row(row), col(col), table(table) {}

std::string Cell::get_data() { return data; }

class Table {
   protected:
    Cell*** data_base;
    int alloc_row, alloc_col;
    int size_row, size_col;

   public:
    Table();

    void copy_from(const TextTable& src);

    std::string get_info();
    virtual std::string print_table() = 0;
    void reg_cell(Cell* c, int row, int col);
    void reg_cell(std::string data, int row, int col);
    std::string get_cell(int row, int col);

    void clear();

    ~Table();
};

class TextTable : public Table {
   public:
    std::string print_table() override;
};

class CSVTable : public Table {
   public:
    std::string print_table() override;
};

class HTMLTable : public Table {
   public:
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

void Table::copy_from(const TextTable& src) {
    clear();
    alloc_row = src.alloc_row;
    alloc_col = src.alloc_col;
    size_row = src.size_row;
    size_col = src.size_col;

    data_base = new Cell**[alloc_row];
    for (int i = 0; i < alloc_row; i++) {
        data_base[i] = new Cell*[alloc_col];
        for (int j = 0; j < alloc_col; j++)
            data_base[i][j] = src.data_base[i][j];
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

void Table::reg_cell(Cell* c, int row, int col) {
    if (row < alloc_row && col < alloc_col) {
        data_base[row][col] = c;
        c->row = row;
        c->col = col;
        c->table = this;
    } else {
        // realloc
    }
    if (row >= size_row) size_row = row + 1;
    if (col >= size_col) size_col = col + 1;
}

void Table::reg_cell(std::string data, int row, int col) {
    Cell* new_cell = new Cell(data, row, col, this);
    reg_cell(new_cell, row, col);
}

std::string Table::get_cell(int row, int col) {
    return data_base[row][col]->get_data();
}

void Table::clear() {
    for (int i = 0; i < alloc_row; i++) {
        for (int j = 0; j < alloc_col; j++)
            if (data_base[i][j]) delete data_base[i][j];
        delete[] data_base[i];
    }
    delete data_base;
}

Table::~Table() { clear(); }

std::string TextTable::print_table() {
    std::string out = "";

    out = out + "=================================\n";
    out = out + "\t";

    for (int i = 0; i < size_col; i++) {
        char c_col;
        if (0 <= i && i < 26)
            c_col = i + 65;
        else
            c_col = '?';
        out = out + c_col + '\t';
    }
    out += '\n';

    for (int i = 0; i < size_row; i++) {
        out = out + std::to_string(i + 1) + '\t';
        for (int j = 0; j < size_col; j++) {
            if (data_base[i][j])
                out = out + data_base[i][j]->get_data() + '\t';
            else
                out = out + '\t';
        }
        out += '\n';
    }

    return out;
}

std::string CSVTable::print_table() {
    std::string out = "";
    for (int i = 0; i < size_row; i++) {
        for (int j = 0; j < size_col; j++) {
            if (data_base[i][j]) out += data_base[i][j]->get_data();
            if (j != size_col - 1) out += ", ";
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
            if (data_base[i][j]) out = out + data_base[i][j]->get_data();
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

int main() {
    std::ofstream out("./7-2/table.html");

    TextTable t;

    Cell a0("sex", 0, 0);
    t.reg_cell("fuck", 1, 0);
    t.reg_cell(&a0, 0, 0);
    t.reg_cell("suck", 1, 1);
    t.reg_cell("high", 5, 2);

    CSVTable csv;
    HTMLTable html;
    csv.copy_from(t);
    html.copy_from(t);

    std::cout << t.print_table() << std::endl;
    std::cout << csv.print_table() << std::endl;
    std::string html_table = html.print_table();
    std::cout << html_table << std::endl;
    html_table = "<html>";
    if (out.is_open())
        for (int i; i < html_table.length(); i++) out << html_table[i];

    return 0;
}