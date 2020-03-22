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

    matrix_row(std::size_t row_number = 0) : _row_number(row_number) {}

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
    using elements_iterator = typename std::vector<element>::iterator;
    using iterator_value = std::tuple<std::size_t, std::size_t, T>;

    class matrix_iterator: public std::iterator<
            std::forward_iterator_tag, iterator_value>
    {
        using matrix_row_iterator = typename matrix_row<T, def_value>::elements_iterator;
    public:
        matrix_iterator(elements_iterator ext_iterator,
                        matrix_row_iterator int_iterator,
                        elements_iterator end_iterator,
                        matrix_row_iterator dummy_iterator) :
            _external_iterator(ext_iterator),
            _internal_iterator(int_iterator),
            _end_iterator(end_iterator),
            _dummy_iterator(dummy_iterator)
        {}

        matrix_iterator(elements_iterator ext_iterator,
                        matrix_row_iterator int_iterator) :
            _external_iterator(ext_iterator),
            _internal_iterator(int_iterator),
            _end_iterator(ext_iterator),
            _dummy_iterator(int_iterator)
        {}

        matrix_iterator operator++() {
            matrix_iterator tmp = *this;
            if(++_internal_iterator == _external_iterator->second.end()) {
                ++_external_iterator;
                if(_external_iterator != _end_iterator)
                    _internal_iterator = _external_iterator->second.begin();
                else
                    _internal_iterator = _dummy_iterator;
            }
            return tmp;
        }

        matrix_iterator operator++(int) {
            if(++_internal_iterator != _external_iterator->end()) {
                ++_external_iterator;
                if(_external_iterator != _end_iterator)
                    _internal_iterator = _external_iterator->begin();
                else
                    _internal_iterator = _dummy_iterator;
            }
            return *this;
        }
        iterator_value operator*() {
            _current_value = std::make_tuple(_external_iterator->first,
                                             _internal_iterator->first,
                                             _internal_iterator->second);
            return _current_value;
        }
        iterator_value* operator->() {
            return _current_value = std::make_tuple(_external_iterator->first,
                                                    _internal_iterator->first,
                                                    _internal_iterator->second);
            return &_current_value;
        }

        bool operator==(const matrix_iterator& rhs) {
            return (_external_iterator == rhs._external_iterator &&
                    _internal_iterator == rhs._internal_iterator);
        }
        bool operator!=(const matrix_iterator& rhs) {
            return (_external_iterator != rhs._external_iterator ||
                    _internal_iterator != rhs._internal_iterator);
        }


    private:
        elements_iterator _external_iterator;
        matrix_row_iterator _internal_iterator;
        iterator_value _current_value;

        elements_iterator _end_iterator;
        matrix_row_iterator _dummy_iterator;
    };

    infinite_matrix(){}

    matrix_iterator begin() {
        auto it_begin = _elements.begin();

        if(it_begin != _elements.end())
            return matrix_iterator(it_begin, it_begin->second.begin(), _elements.end(), _dummy.begin());
        else
            return matrix_iterator(it_begin, _dummy.begin());
    }

    matrix_iterator end() {
        auto it_end = _elements.end();
        return matrix_iterator(it_end, _dummy.begin());
    }


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
    matrix_row<T, def_value> _dummy;
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
