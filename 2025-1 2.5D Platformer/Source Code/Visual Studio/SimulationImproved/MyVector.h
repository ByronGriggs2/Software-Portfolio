#pragma once

template <typename T>
class MyVector {
public:
    MyVector() : _size(0), _capacity(1), _data(new T[_capacity]) {}

    MyVector(std::initializer_list<T> init) : _size(init.size()), _capacity(init.size()), _data(new T[_capacity]) {
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = *(init.begin() + i);
        }
    }

    ~MyVector() {
        delete[] _data;
    }

    void push_back(const T& value) {
        if (_size >= _capacity) {
            reserve(_capacity * 2);
        }
        _data[_size++] = value;
    }

    void pop_back() {
        if (_size > 0) {
            --_size;
        }
    }

    T& operator[](size_t index) {
        if (index >= _size) {
            throw "Index out of range";
        }
        return _data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= _size) {
            throw "Index out of range";
        }
        return _data[index];
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > _capacity) {
            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < _size; ++i) {
                new_data[i] = _data[i];
            }
            delete[] _data;
            _data = new_data;
            _capacity = new_capacity;
        }
    }

private:
    size_t _size;
    size_t _capacity;
    T* _data;
};
