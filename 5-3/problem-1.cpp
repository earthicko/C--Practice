#include <iostream>

namespace MyArray {
class Array;
class Int;

class Array {
    friend Int;

    const int dim;
    int* size;

    struct Address {
        int level;   // how deep this address is
        void* next;  // pointer of the first child node. can be *int(deepest
                     // nodes) or *Address(the rest)
    };

    Address* start;  // the highest node

   public:
    Array(int _dim, int* _size) : dim(_dim) {
        size = new int[_dim];
        for (int i = 0; i < _dim; i++) size[i] = _size[i];
        start = new Address;
        start->level = 0;
        initialise_array(start);
    }

    Array(const Array& src) : dim(src.dim) {
        size = new int[dim];
        for (int i = 0; i < dim; i++) size[i] = src.size[i];
        start = new Address;
        start->level = 0;
        initialise_array(start);
        copy_address(src.start, start);
    }

    ~Array() {
        delete_array(start);
        delete start;
        delete[] size;
    }

    void initialise_array(Address* current) {
        if (!current) return;
        if (current->level == dim - 1) {
            current->next = new int[size[current->level]];
            return;
        }
        current->next = new Address[size[current->level]];
        for (int i = 0; i < size[current->level]; i++) {
            Address* next_address = static_cast<Address*>(current->next);
            (next_address + i)->level = current->level + 1;
            initialise_array(next_address + i);
        }
    }

    void delete_array(Address* current) {
        if (!current) return;
        if (current->level != dim - 1) {
            Address* next_address = static_cast<Address*>(current->next);
            for (int i = 0; i < size[current->level]; i++) {
                delete_array(next_address + i);
            }
            delete[] next_address;
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

    class Iterator {
        int* location;  // [2][1][3] -> {2, 1, 3}
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
            int i = arr->dim - 1;  // ++ from the furthest location
            do {
                location[i]++;
                if (location[i] >= arr->size[i] && i >= 1) {
                    // i == 0 -> don't carry, just let it be overflown
                    // [overflown][0][0]... means exhausted iterator
                    carry = true;
                    location[i] -= arr->size[i];
                    i--;
                } else {
                    carry = false;
                }
            } while (i >= 0 && carry);

            return (*this);
        }

        Iterator operator++(int) {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }

        Int operator*();

        Iterator& operator=(const Iterator& src) {
            arr = src.arr;
            if (location) delete[] location;
            location = new int[arr->dim];
            for (int i = 0; i < arr->dim; i++) location[i] = src.location[i];

            return (*this);
        }

        bool operator!=(const Iterator& src) {
            // mainly for usecases in for loops
            if (src.arr->dim != arr->dim) return true;
            for (int i = 0; i < arr->dim; i++) {
                if (src.location[i] != location[i]) return true;
            }
            return false;
        }

        bool operator==(const Iterator& src) {
            return !(this->operator!=(src));
        }
    };
    friend Iterator;

    Iterator begin() {  // [0][0][0]
        int* arr = new int[dim];
        for (int i = 0; i < dim; i++) arr[i] = 0;
        Iterator temp(this, arr);
        delete[] arr;
        return temp;
    }

    Iterator end() {  //[!!][0][0]
        int* arr = new int[dim];
        arr[0] = size[0];
        for (int i = 1; i < dim; i++) arr[i] = 0;
        Iterator temp(this, arr);
        delete[] arr;
        return temp;
    }

    Int operator[](const int index);
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
        void* next_addrs = static_cast<Array::Address*>(data)->next;
        if (level == array->dim - 1) {
            int* next_addr_start = static_cast<int*>(next_addrs);
            data = static_cast<void*>(next_addr_start + index);
        } else {
            Array::Address* next_addr_start =
                static_cast<Array::Address*>(next_addrs);
            data = static_cast<void*>(next_addr_start + index);
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
    for (int i = 1; i < arr->dim; i++) {
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
