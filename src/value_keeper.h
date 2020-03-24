#ifndef VALUE_KEEPER_H
#define VALUE_KEEPER_H

#include <cstddef>
#include <algorithm>

// forward declaration
template <typename T, T def_value>
class infinite_matrix;

// forward declaration
template <typename T, T def_value>
class matrix_row;

/**
 * @brief Класс, позволяющий управлять значением в матрице
 */
template <typename T, T def_value>
class value_keeper
{
public:
    /**
     * @brief Конструктор
     * @param row - номер строки
     * @param column - номер столбца
     * @param matrix - ссылка на матрицу
     */
    value_keeper(std::size_t row, std::size_t column, infinite_matrix<T, def_value>& matrix) :
        _row(row), _column(column), _matrix(matrix) {}

    /**
     * @brief Оператор приведения типа к типу хранимого матрицей значения
     * @return хранимое значение или значение по умолчанию
     */
    operator T() {
        auto predicate = [this](const typename infinite_matrix<T, def_value>::element& elem) {return elem.first == _row;};
        auto iter = std::find_if(_matrix.elems_begin(), _matrix.elems_end(), predicate);
        if(iter == _matrix.elems_end())
            return T{def_value};
        else
            return iter->second.get(_column);
    }

    /**
     * @brief Оператор присваивания от хранимого в матрице типа. Позволяет сохранить значение в матрице
     * @param value - сохраняемое значение
     */
    value_keeper& operator=(const T& value)
    {
        auto predicate = [this](const typename infinite_matrix<T, def_value>::element& elem) {return elem.first == _row;};
        auto iter = std::find_if(_matrix.elems_begin(), _matrix.elems_end(), predicate);

        if(iter == _matrix.elems_end())
        {
            if(value == def_value)
                return *this;

            auto row = matrix_row<T, def_value>(_row, std::make_pair(_column, value));
            _matrix.insert_row(std::make_pair(_row, std::move(row)));
        }
        else
        {
            iter->second.set(_column, value);
            if(iter->second.size() == 0)
                _matrix.erase_row(iter);
        }

        return *this;
    }

private:
    std::size_t _row;
    std::size_t _column;
    infinite_matrix<T, def_value>& _matrix;
};

/**
 * @brief Класс доступа к строке, позволяющий сгенерировать объект value_keeper'a
 */
template <typename T, T def_value>
class row_accessor
{
public:
    /**
     * @brief Конструктор
     * @param row - номер строки
     * @param matrix - ссылка на матрицу
     */
    row_accessor(std::size_t row, infinite_matrix<T, def_value>& matrix) :
        _row(row), _matrix(matrix) {}

    /**
     * @brief Оператор квадратные скобки. Позволяет диспетчиризировать вызов к value_keeper'у путем его создания
     * @param column - номер столбца
     * @return value_keeper управляющий значением по заданным параметрам row и column
     */
    value_keeper<T, def_value> operator[](std::size_t column) {
        return value_keeper<T, def_value>(_row, column, _matrix);
    }

private:
    std::size_t _row;
    infinite_matrix<T, def_value>& _matrix;
};

#endif // VALUE_KEEPER_H
