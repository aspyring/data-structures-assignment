#include <cstddef>
#include <stdexcept>
using namespace std;

template<typename T>
class DynamicArray {
    T*      data;
    size_t  capacity;
    size_t  length;

public:
    DynamicArray(): data(nullptr), capacity(0), length(0) {}
    ~DynamicArray() { delete[] data; }

    void push(const T& item) {
        if (length == capacity) resize();
        data[length++] = item;
    }

    // non-const access
    T& operator[](size_t idx) {
        if (idx >= length) throw out_of_range("Index out of range");
        return data[idx];
    }

    // const access
    const T& operator[](size_t idx) const {
        if (idx >= length) throw out_of_range("Index out of range");
        return data[idx];
    }

    size_t size() const { return length; }

private:
    void resize() {
        size_t new_cap = (capacity == 0 ? 10 : capacity * 2);
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < length; ++i) new_data[i] = data[i];
        delete[] data;
        data = new_data;
        capacity = new_cap;
    }
};
