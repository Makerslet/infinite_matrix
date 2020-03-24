#ifndef MATRIX_ROW_H
#define MATRIX_ROW_H

#include "interfaces.h"
#include "value_keeper.h"

#include <vector>
#include <algorithm>

/**
 * @brief Класс, реализующий строку матрицы
 */


template <typename T, T def_value>
class matrix_row : ISizable
{
    /**
     * @brief Элемент строки
     */
    using element = std::pair<std::size_t, T>;
public:

    /**
     * @brief Конструктор строки
     * @param row_number - номер строки
     */
    matrix_row(std::size_t row_number = 0) : _row_number(row_number) {}

    /**
     * @brief Конструктор строки принимающий сразу же элемент
     * @param row_number - номер строки
     * @param elem - элемент строки
     */
    matrix_row(std::size_t row_number,
               element&& elem) : _row_number(row_number) {
        _elements.emplace_back(std::move(elem));
    }

    /**
     * @brief тип итератора по элементам строки
     */
    using elements_iterator = typename std::vector<element>::iterator;

    /**
     * @brief Позволяет получить итератор на начало строки
     * @return итератор на начало строки
     */
    elements_iterator begin() {
        return _elements.begin();
    }

    /**
     * @brief Позволяет получить итератор на конец строки
     * @return итератор на конец строки
     */
    elements_iterator end() {
        return _elements.end();
    }

    /**
     * @brief Позволяет получить количество элементов, содержащихся в строке
     * @return количество элементов в строке
     */
    std::size_t size() const override {
        return _elements.size();
    }

private:
    /**
     * @brief Позволяет получить значение хранящееся в строке или значение по умолчанию, если такого значения нет
     * @param column - столбец
     * @return хранящееся значение или значение по умолчанию
     */
    T get(std::size_t column) {
        auto predicate = [column](const element& elem) {return elem.first == column;};
        auto iter = std::find_if(_elements.begin(), _elements.end(), predicate);
        if(iter != _elements.end())
            return iter->second;
        else
            return T{def_value};
    }

    /**
     * @brief Позволяет сохранить значение, если сохраняется значение по умолчанию, то элемент удаляется
     * @param column - столбец
     * @param value - сохраняемое значение
     */
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
