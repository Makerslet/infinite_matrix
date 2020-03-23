#ifndef MATRIX_ROW_H
#define MATRIX_ROW_H

#include "interfaces.h"
#include "value_keeper.h"

#include <vector>
#include <algorithm>


template <typename T, T def_value>
class matrix_row : ISizable
{
    using element = std::pair<std::size_t, T>;
public:

    matrix_row(std::size_t row_number = 0) : _row_number(row_number) {}
    matrix_row(std::size_t row_number,
               element&& elem) : _row_number(row_number) {
        _elements.emplace_back(std::move(elem));
    }

    using elements_iterator = typename std::vector<element>::iterator;
    elements_iterator begin() {
        return _elements.begin();
    }

    elements_iterator end() {
        return _elements.end();
    }

    std::size_t size() const override {
        return _elements.size();
    }

private:
    T get(std::size_t column) {
        auto predicate = [column](const element& elem) {return elem.first == column;};
        auto iter = std::find_if(_elements.begin(), _elements.end(), predicate);
        if(iter != _elements.end())
            return iter->second;
        else
            return T{def_value};
    }

    void set(std::size_t column, const T& value) {
        auto predicate = [column](const element& elem) {return elem.first == column;};
        auto iter = std::find_if(_elements.begin(), _elements.end(), predicate);

        if(iter != _elements.end()) {
            if(value == def_value)
                _elements.erase(iter);
            else
                iter->second = value;
        }
        else {
            if(value != def_value)
                _elements.emplace_back(std::make_pair(column, value));
        }
    }

    friend value_keeper<T, def_value>;
private:
    std::vector<element> _elements;
    std::size_t _row_number;
};

#endif // MATRIX_ROW_H
