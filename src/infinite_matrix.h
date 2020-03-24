#ifndef INFINITE_MATRIX_H
#define INFINITE_MATRIX_H

#include "interfaces.h"
#include "matrix_row.h"
#include "value_keeper.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>


/**
 * @brief Класс, реализующий бесконечную двумерную матрицу
 */

template <typename T, T def_value>
class infinite_matrix : ISizable
{
    /**
     * @brief Строка матрицы
     */
    using element = std::pair<std::size_t, matrix_row<T, def_value>>;
public:

    /**
     * @brief Тип итератора на сроку матрицы
     */
    using elements_iterator = typename std::vector<element>::iterator;

    /**
     * @brief Тип возращаемый итератором бесконечной матрицы
     */
    using iterator_value = std::tuple<std::size_t, std::size_t, T>;

    /**
     * @brief Класс, реализующий итератор по матрице
     */
    class matrix_iterator: public std::iterator<
            std::forward_iterator_tag, iterator_value>
    {
        using matrix_row_iterator = typename matrix_row<T, def_value>::elements_iterator;
    public:
        /**
         * @brief Конструктор итератора для не пустой матрицы
         * @param ext_iterator - итератор по строкам матрицы
         * @param int_iterator - итератор внутри строки матрицы
         * @param end_iterator - итератор за последней строкой матрицы
         * @param dummy_iterator - итератор сигнализатор пустой строки
         */
        matrix_iterator(elements_iterator ext_iterator,
                        matrix_row_iterator int_iterator,
                        elements_iterator end_iterator,
                        matrix_row_iterator dummy_iterator) :
            _external_iterator(ext_iterator),
            _internal_iterator(int_iterator),
            _end_iterator(end_iterator),
            _dummy_iterator(dummy_iterator)
        {}

        /**
         * @brief Конструктор итератора для пустой матрицы
         * @param end_iterator - итератор за последней строкой матрицы
         * @param dummy_iterator - итератор сигнализатор пустой строки
         */
        matrix_iterator(elements_iterator ext_iterator,
                        matrix_row_iterator int_iterator) :
            _external_iterator(ext_iterator),
            _internal_iterator(int_iterator),
            _end_iterator(ext_iterator),
            _dummy_iterator(int_iterator)
        {}

        /**
         * @brief Оператор постфиксного инкремента
         */
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

        /**
         * @brief Оператор префиксного инкремента
         */
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

        /**
         * @brief Оператор разыменования указателя
         */
        iterator_value operator*() {
            _current_value = std::make_tuple(_external_iterator->first,
                                             _internal_iterator->first,
                                             _internal_iterator->second);
            return _current_value;
        }

        /**
         * @brief Оператор стрелка
         */
        iterator_value* operator->() {
            return _current_value = std::make_tuple(_external_iterator->first,
                                                    _internal_iterator->first,
                                                    _internal_iterator->second);
            return &_current_value;
        }

        /**
         * @brief Оператор равенства
         */
        bool operator==(const matrix_iterator& rhs) {
            return (_external_iterator == rhs._external_iterator &&
                    _internal_iterator == rhs._internal_iterator);
        }

        /**
         * @brief Оператор неравенства
         */
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

    /**
     * @brief Конструктор матрицы
     */
    infinite_matrix(){}

    /**
     * @brief Позволяет получить итератор на начало матрицы
     * @return итератор на начало матрицы
     */
    matrix_iterator begin() {
        auto it_begin = _elements.begin();

        if(it_begin != _elements.end())
            return matrix_iterator(it_begin, it_begin->second.begin(), _elements.end(), _dummy.begin());
        else
            return matrix_iterator(it_begin, _dummy.begin());
    }

    /**
     * @brief Позволяет получить итератор на конец матрицы
     * @return итератор на конец матрицы
     */
    matrix_iterator end() {
        auto it_end = _elements.end();
        return matrix_iterator(it_end, _dummy.begin());
    }


    /**
     * @brief Позволяет получить количество элементов, содержащихся в матрице
     * @return количество элементов в матрице
     */
    std::size_t size() const override {
        auto add = [](std::size_t sum, const element& elem){
            return sum + elem.second.size();
        };
        std::size_t begin = 0;
        return std::accumulate(_elements.begin(), _elements.end(), begin, add);
    }

    /**
     * @brief Оператор квадратные скобки
     * @return промежуточный объект, позволяющий позже сконструировать элемент доступа к ячейке матрицы
     */
    row_accessor<T, def_value> operator[](std::size_t index) {
        return row_accessor<T, def_value>(index, *this);
    }

    friend value_keeper<T, def_value>;

private:
    /**
     * @brief Получение итератора на начало внутреннего контейнера, содержащего строки
     * @return итератор на начало внутреннего контейнера, содержащего строки
     */
    elements_iterator elems_begin() {
        return _elements.begin();
    }

    /**
     * @brief Получение итератора на конец внутреннего контейнера, содержащего строки
     * @return итератор на конец внутреннего контейнера, содержащего строки
     */
    elements_iterator elems_end() {
        return _elements.end();
    }

    /**
     * @brief добавление строки
     * @param row - rvalue ссылка на добавляемую строку
     */
    void insert_row(element&& row) {
        _elements.emplace_back(std::move(row));
    }

    /**
     * @brief удаление строки
     * @param iter - итератор на удаляемю строку
     */
    void erase_row(elements_iterator iter) {
        _elements.erase(iter);
    }

private:
    std::vector<element> _elements;
    matrix_row<T, def_value> _dummy;
};

#endif // INFINITE_MATRIX_H
