#include "infinite_matrix.h"
#include <iostream>
#include <cassert>

/**
 * @brief Entry point
 *
 * Execution of the program
 * starts here.
 *
 * @return Program exit status
 */

int main (int, char **)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix; // бесконечная матрица int заполнена значениями -1
    assert(matrix.size() == 0); // все ячейки свободны

    auto a = matrix[0][0];
    assert(a == default_value);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    matrix[100][100] = default_value;
    assert(matrix[100][100] == default_value);
    assert(matrix.size() == 0);

//    // выведется одна строка
//    // 100100314
//    for(auto c: matrix)
//    {
//        int x;
//        int y;
//        int v;
//        std::tie(x, y, v) = c;
//        std::cout << x << y << v << std::endl;
//    }
    return 0;
}


