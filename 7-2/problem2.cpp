#include <fstream>
#include <iostream>
#include <string>
#define DEFAULT_ROW 1
#define DEFAULT_COL 1

/*하지만 실제 엑셀의 경우 셀이 문자열 데이터만 들어가는 것이 아니라, 숫자나 날짜
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
    Table(const Table* src);

    void copy_from(const Table* src);

    std::string get_info();
    virtual std::string print_table() = 0;
    void reg_cell(Cell* c, int row, int col);
    void reg_cell(std::string data, int row, int col);
    std::string get_cell(int row, int col);

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
        for (int i = 0; i < alloc_row; i++) delete[] temp[i];
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
        Cell*** temp = data_base;
        data_base = new Cell**[row];
        for (int i = 0; i < row; i++) {
            if (i < alloc_row)
                data_base[i] = temp[i];
            else
                data_base[i] = new Cell*[alloc_col];
        }
        for (int i = 0; i < alloc_row; i++) delete[] temp[i];
        // delete temp;
        alloc_row = row;
    } else {
        // do nothing
    }
}

void Table::reg_cell(Cell* c, int row, int col) {
    if (row >= alloc_row || col >= alloc_col) alloc(row + 1, col + 1);

    data_base[row][col] = c;
    c->row = row;
    c->col = col;
    c->table = this;

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
    TextTable* text = new TextTable();

    Cell* a0 = new Cell("sex", 0, 0);
    text->reg_cell("fuck", 1, 0);
    text->reg_cell(a0, 0, 0);
    text->reg_cell("suck", 1, 1);
    text->reg_cell("high", 5, 2);

    CSVTable* csv = new CSVTable(text);
    HTMLTable* html = new HTMLTable(text);

    std::string text_content = text->print_table();
    std::string csv_content = csv->print_table();
    std::string html_content = html->print_table();

    std::cout << text_content << std::endl;
    std::cout << csv_content << std::endl;
    std::cout << html_content << std::endl;

    std::ofstream html_out("./7-2/table.html");
    if (html_out.is_open()) html_out << html_content;

    return 0;
}