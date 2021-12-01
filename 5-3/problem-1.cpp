#include <iostream>

namespace MyArray {
class Array;
class Int;

class Array {
    friend Int;

    const int dim;
    int* size;

    struct Address {
        int level;
        void* next;
    };

    Address* start;

   public:
    Array(int _dim, int* _size) : dim(_dim) {
        size = new int[_dim];
        for (int i = 0; i < _dim; i++) size[i] = _size[i];
        start = new Address;
        start->level = 0;
        init(start);
    }
    Array(const Array& src) : dim(src.dim) {
        size = new int[dim];
        for (int i = 0; i < dim; i++) size[i] = src.size[i];
        start = new Address;
        start->level = 0;
        init(start);
        copy_address(src.start, start);
    }
    ~Array() {
        del(start);
        delete start;
        delete[] size;
    }
    void init(Address* current) {
        if (!current) return;
        if (current->level == dim - 1) {
            current->next = new int[size[current->level]];
            return;
        }
        current->next = new Address[size[current->level]];
        for (int i = 0; i < size[current->level]; i++) {
            (static_cast<Address*>(current->next) + i)->level =
                current->level + 1;
            init(static_cast<Address*>(current->next) + i);
        }
    }
    void del(Address* current) {
        if (!current) return;
        if (current->level != dim - 1) {
            for (int i = 0; i < size[current->level]; i++) {
                del(static_cast<Address*>(current->next) + i);
            }
            delete[] static_cast<Address*>(current->next);
            return;
        }
        delete[] static_cast<int*>(current->next);
    }
    void copy_address(Address* src, Address* dest) {
        if (!src || !dest) return;
        if (src->level != dest->level) return;
        if (src->level == dim - 1) {
            int* to = static_cast<int*>(dest->next);
            int* from = static_cast<int*>(src->next);
            for (int i = 0; i < size[src->level]; i++) *(to + i) = *(from + i);
            return;
        }
        Address* to = static_cast<Address*>(dest->next);
        Address* from = static_cast<Address*>(src->next);
        for (int i = 0; i < size[src->level]; i++)
            copy_address(from + i, to + i);
    }
    Int operator[](const int index);
    class Iterator {
        int* location;
        Array* arr;

       public:
        Iterator(Array* arr, int* _location) : arr(arr) {
            location = new int[arr->dim];
            for (int i = 0; i < arr->dim; i++)
                location[i] = (_location[i] != NULL ? _location[i] : 0);
        }
        Iterator(const Iterator& src) : arr(src.arr) {
            location = new int[arr->dim];
            for (int i = 0; i < arr->dim; i++) location[i] = src.location[i];
        }
        ~Iterator() { delete[] location; }
        Iterator& operator++() {
            if (location[0] >= arr->size[0]) return (*this);

            bool carry = false;
            int i = arr->dim - 1;
            do {
                location[i]++;
                if (location[i] >= arr->size[i] && i >= 1) {
                    carry = true;
                    location[i] -= arr->size[i];
                    i--;
                } else {
                    carry = false;
                }
            } while (i >= 0 && carry);

            return *this;
        }
        Iterator operator++(int) {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }
        Int operator*();
        Iterator& operator=(const Iterator& src) {
            arr = src.arr;
            if (!location) delete[] location;
            location = new int[arr->dim];
            for (int i = 0; i < arr->dim; i++) location[i] = src.location[i];

            return (*this);
        }
        bool operator!=(const Iterator& src) {
            if (src.arr->dim != arr->dim) return true;
            for (int i = 0; i < arr->dim; i++) {
                if (src.location[i] != location[i]) return true;
            }
            return false;
        }
    };
    friend Iterator;
    Iterator begin() {
        int* arr = new int[dim];
        for (int i = 0; i < dim; i++) arr[i] = 0;
        Iterator temp(this, arr);
        delete[] arr;
        return temp;
    }
    Iterator end() {
        int* arr = new int[dim];
        arr[0] = size[0];
        for (int i = 1; i < dim; i++) arr[i] = 0;
        Iterator temp(this, arr);
        delete[] arr;
        return temp;
    }
};
class Int {
    void* data;
    int level;
    Array* array;

   public:
    Int(int index, int _level = -1, void* _data = NULL, Array* _array = NULL)
        : level(_level), data(_data), array(_array) {
        if (_level < 0 || index >= array->size[_level]) {
            data = NULL;
            return;
        }
        void* next_addresses = static_cast<Array::Address*>(data)->next;
        if (level == array->dim - 1) {
            // 이제 data 에 우리의 int 자료형을 저장하도록 해야 한다.
            int* next_address = static_cast<int*>(next_addresses) + index;
            data = static_cast<void*>(next_address);
        } else {
            // 그렇지 않을 경우 data 에 그냥 다음 addr 을 넣어준다.
            Array::Address* next_address =
                static_cast<Array::Address*>(next_addresses) + index;
            data = static_cast<void*>(next_address);
        }
    }

    Int(const Int& i) : data(i.data), level(i.level), array(i.array) {}

    operator int() {
        if (data) return *static_cast<int*>(data);
        return -9999;
    }
    Int& operator=(const int& a) {
        if (data) *static_cast<int*>(data) = a;
        return *this;
    }
    Int operator[](const int index) {
        if (!data) return -9999;
        return Int(index, level + 1, data, array);
    }
};
Int Array::operator[](const int index) {
    return Int(index, 0, static_cast<void*>(start), this);
}
Int Array::Iterator::operator*() {
    Int start = (*arr)[location[0]];
    for (int i = 0; i < arr->dim - 1; i++) {
        start = start[location[i]];
    }
    return start;
}
}  // namespace MyArray
int main() {
    int size[] = {2, 3, 4};
    MyArray::Array arr(3, size);

    MyArray::Array::Iterator itr = arr.begin();
    for (int i = 0; itr != arr.end(); itr++, i++) {
        (*itr) = i;
    }
    for (itr = arr.begin(); itr != arr.end(); itr++)
        std::cout << *itr << std::endl;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                arr[i][j][k] = (i + 1) * (j + 1) * (k + 1) + arr[i][j][k];
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                std::cout << i << " " << j << " " << k << " " << arr[i][j][k]
                          << std::endl;
            }
        }
    }
    return 0;
}
