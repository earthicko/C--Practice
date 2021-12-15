#include <iostream>
#include <string>

using namespace std;

int main() {
    string float_str = "39928.39000";
    string int_str = "38810";
    string neg_int_str = "-2993";
    string nan = "nan";

    float a = stof(float_str);
    int b = stoi(int_str);
    int c = stoi(neg_int_str);

    float d = stof(int_str);
    float e = stof(neg_int_str);

    int f = stoi(float_str);

    int g;
    try {
        g = stoi(nan);
    } catch (const exception& expn) {
        cout << expn.what() << '\n';
    }

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    cout << e << endl;
    cout << f << endl;
    cout << g << endl;
}