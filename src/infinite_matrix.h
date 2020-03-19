#ifndef INFINITE_MATRIX_H
#define INFINITE_MATRIX_H

#include <vector>
#include <list>
#include <algorithm>
#include <numeric>
#include <functional>

struct ISizable
{
    virtual ~ISizable() = default;
    virtual std::size_t size() const = 0;
};


template <typename T, T def_value>
class infinite_matrix;

template <typename T, T def_value>
class value_interrior;


template <typename T, T def_value>
class matrix_row : ISizable
{
    using element = std::pair<std::size_t, T>;
public:
    matrix_row(std::size_t row_number) : _row_number(row_number) {}
    std::size_t size() const override {
        return _elements.size();
    }

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

    const T& operator[](std::size_t index) const {
        return brackets_impl<true>(index);
    }

    T& operator[](std::size_t index) {
        return brackets_impl<false>(index);
    }

private:
    template<bool is_const>
    typename std::conditional<is_const, const T&, T&>::type brackets_impl(std::size_t index)
    {
        auto predicate = [index](const element& elem) {return elem.first == index;};
        auto iter = std::find_if(_elements.begin(), _elements.end(), predicate);
        if(iter != _elements.end())
            return iter->second;
        else
            return T{def_value};
    }

private:
    std::vector<element> _elements;
    std::size_t _row_number;
};

template <typename T, T def_value>
class dummy_row
{
public:
    dummy_row(std::size_t row, infinite_matrix<T, def_value>& matrix) :
        _row(row), _matrix(matrix) {}

    value_interrior<T, def_value> operator[](std::size_t column) {
        return value_interrior<T, def_value>(_row, column, _matrix);
    }

private:
    std::size_t _row;
    infinite_matrix<T, def_value>& _matrix;
};

template <typename T, T def_value>
class infinite_matrix : ISizable
{
    using element = std::pair<std::size_t, matrix_row<T, def_value>>;
public:
    infinite_matrix(){}
    std::size_t size() const override {
        auto add = [](std::size_t sum, const element& elem){
            return sum + elem.second.size();
        };
        std::size_t begin = 0;
        return std::accumulate(_elements.begin(), _elements.end(), begin, add);
    }

    dummy_row<T, def_value> operator[](std::size_t index) {
        return dummy_row<T, def_value>(index, *this);
    }

    friend value_interrior<T, def_value>;

private:
    std::vector<element> _elements;
};

template <typename T, T def_value>
class value_interrior
{
public:
    value_interrior(std::size_t row, std::size_t column, infinite_matrix<T, def_value>& matrix) :
        _row(row), _column(column), _matrix(matrix) {}

    operator T() {
        auto predicate = [this](const typename infinite_matrix<T, def_value>::element& elem) {return elem.first == _row;};
        auto iter = std::find_if(_matrix._elements.begin(), _matrix._elements.end(), predicate);
        if(iter == _matrix._elements.end())
            return T{def_value};
        else
            return iter->second.get(_column);
    }

    value_interrior& operator=(const T& value)
    {
        auto predicate = [this](const typename infinite_matrix<T, def_value>::element& elem) {return elem.first == _row;};
        auto iter = std::find_if(_matrix._elements.begin(), _matrix._elements.end(), predicate);

        if(iter == _matrix._elements.end())
        {
            if(value == def_value)
                return *this;

            auto row = matrix_row<T, def_value>(_row);
            row.set(_column, value);
            _matrix._elements.emplace_back(std::make_pair(_row, std::move(row)));
        }
        else
        {
            iter->second.set(_column, value);
            if(iter->second.size() == 0)
                _matrix._elements.erase(iter);
        }

        return *this;
    }

private:
    std::size_t _row;
    std::size_t _column;
    infinite_matrix<T, def_value>& _matrix;
};

#endif // INFINITE_MATRIX_H
