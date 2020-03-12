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
class matrix_row : ISizable
{
    using element = std::pair<std::size_t, T>;
public:
    matrix_row() : _default_value(def_value) {}
    std::size_t size() const override {
        return _elements.size();
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
            return _default_value;
    }

private:
    std::vector<element> _elements;
    T _default_value;
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

    const matrix_row<T, def_value>& operator[](std::size_t index) const {
        return brackets_impl<true>(index);
    }

    matrix_row<T, def_value>& operator[](std::size_t index) {
        return brackets_impl<false>(index);
    }

private:
    template<bool is_const>
    typename std::conditional<is_const,
                              const matrix_row<T, def_value>&,
                              matrix_row<T, def_value>&>::type brackets_impl(std::size_t index)
    {
        auto predicate = [index](const element& elem) {return elem.first == index;};

        auto iter = std::find_if(_elements.begin(), _elements.end(), predicate);
        if(iter != _elements.end())
            return iter->second;
        else
            return _dummy;
    }

private:
    std::vector<element> _elements;
    matrix_row<T, def_value> _dummy;
};

#endif // INFINITE_MATRIX_H
