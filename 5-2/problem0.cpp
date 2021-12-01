#include <iostream>
/*
N 차원 배열을 제공하는 클래스를 만들어보세요. 이는 여러분이 여태까지 배운 내용을
시험할 것입니다. 참고로, 원소에 접근하기 위해서는 a[1][2][3] 과 같은 방법으로
접근하겠지요. 다만 N 차원 배열이기 때문에 (N은 객체 생성시에 사용자가
입력합니다) 2 차원 배열은 a[1][2], 3 차원 배열은 a[1][2][3] 과 같은 방식으로
접근할 것입니다. (난이도 : 最上)
*/
class Array {
    int data;
    Array* members;
    int num_members;

   public:
    Array();
    Array(const Array& src);

    void init_members(const int num);
    Array& operator[](int index) { return members[index]; }
    operator int() {
        int& data_ref = data;
        return data_ref;
    }
    Array& operator=(int src) {
        data = src;
        return *this;
    }
};
int main() {
    Array array;
    array.init_members(5);
    for (int i = 0; i < 5; i++) {
        array[i].init_members(3);
        for (int j = 0; j < 4; j++) {
            array[i][j] = i * j;
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << array[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
Array::Array() : data(0), members(NULL), num_members(0) {}
Array::Array(const Array& src)
    : data(src.data), members(src.members), num_members(src.num_members) {}
void Array::init_members(const int num) {
    data = 0;
    members = new Array[num];
    num_members = num;
    for (int i = 0; i < num; i++) members[i] = Array();
}