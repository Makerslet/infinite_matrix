#ifndef INFINITE_MATRIX_H
#define INFINITE_MATRIX_H

#include "interfaces.h"
#include "matrix_row.h"
#include "value_keeper.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>



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

    row_accessor<T, def_value> operator[](std::size_t index) {
        return row_accessor<T, def_value>(index, *this);
    }

    friend value_keeper<T, def_value>;

private:
    std::vector<element> _elements;
    matrix_row<T, def_value> _dummy;
};

#endif // INFINITE_MATRIX_H
