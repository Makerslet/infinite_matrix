#ifndef VALUE_KEEPER_H
#define VALUE_KEEPER_H

#include <cstddef>

template <typename T, T def_value>
class infinite_matrix;

template <typename T, T def_value>
class value_keeper
{
public:
    value_keeper(std::size_t row, std::size_t column, infinite_matrix<T, def_value>& matrix) :
        _row(row), _column(column), _matrix(matrix) {}

    operator T() {
        auto predicate = [this](const typename infinite_matrix<T, def_value>::element& elem) {return elem.first == _row;};
        auto iter = std::find_if(_matrix._elements.begin(), _matrix._elements.end(), predicate);
        if(iter == _matrix._elements.end())
            return T{def_value};
        else
            return iter->second.get(_column);
    }

    value_keeper& operator=(const T& value)
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

template <typename T, T def_value>
class row_accessor
{
public:
    row_accessor(std::size_t row, infinite_matrix<T, def_value>& matrix) :
        _row(row), _matrix(matrix) {}

    value_keeper<T, def_value> operator[](std::size_t column) {
        return value_keeper<T, def_value>(_row, column, _matrix);
    }

private:
    std::size_t _row;
    infinite_matrix<T, def_value>& _matrix;
};

#endif // VALUE_KEEPER_H
