#include "infinite_matrix.h"
#include <gtest/gtest.h>

TEST(SIZE, INIT_EMPTY)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;

    ASSERT_EQ(matrix.size(), 0);
}

TEST(SIZE, ADDED_NOT_EMPTY)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;
    matrix[10][10] = 15;

    ASSERT_EQ(matrix.size(), 1);
}

TEST(SIZE, ADDED_AND_REMOVED)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;

    matrix[10][10] = 15;
    ASSERT_EQ(matrix.size(), 1);

    matrix[10][10] = default_value;
    ASSERT_EQ(matrix.size(), 0);
}

TEST(VALUES, GET_DEFAULT_VALUE)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;

    ASSERT_TRUE(matrix[10][10] == default_value);
    ASSERT_TRUE(matrix[20][20] == default_value);
    ASSERT_EQ(matrix.size(), 0);
}

TEST(VALUES, SET_VALUES)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;
    int value1 = 56;
    int value2 = 78;

    matrix[10][10] = value1;
    matrix[20][20] = value2;

    ASSERT_TRUE(matrix[10][10] == value1);
    ASSERT_TRUE(matrix[20][20] == value2);
    ASSERT_EQ(matrix.size(), 2);
}

TEST(VALUES, SET_VALUES_AND_RESET)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;
    int value1 = 56;
    int value2 = 78;

    matrix[10][10] = value1;
    matrix[20][20] = value2;

    ASSERT_TRUE(matrix[10][10] == value1);
    ASSERT_TRUE(matrix[20][20] == value2);
    ASSERT_EQ(matrix.size(), 2);

    matrix[10][10] = default_value;
    matrix[20][20] = default_value;

    ASSERT_TRUE(matrix[10][10] == default_value);
    ASSERT_TRUE(matrix[20][20] == default_value);
    ASSERT_EQ(matrix.size(), 0);
}

TEST(ITERATOR, EMPTY_ITERATION)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;

    ASSERT_TRUE(matrix.begin() == matrix.end());
}

TEST(ITERATOR, NOT_EMPTY_ITERATION)
{
    const int default_value = -1;
    infinite_matrix<int, default_value> matrix;
    std::size_t index1 = 10;
    int value1 = 56;

    std::size_t index2 = 20;
    int value2 = 78;

    matrix[index1][index1] = value1;
    matrix[index2][index2] = value2;


    std::vector<infinite_matrix<int, default_value>::iterator_value> values;
    for(const auto value : matrix)
        values.push_back(value);

    ASSERT_TRUE(values.size() == 2);
    const auto[x1, y1, v1] = values[0];
    ASSERT_TRUE(x1 == index1);
    ASSERT_TRUE(y1 == index1);
    ASSERT_TRUE(v1 == value1);

    const auto[x2, y2, v2] = values[1];
    ASSERT_TRUE(x2 == index2);
    ASSERT_TRUE(y2 == index2);
    ASSERT_TRUE(v2 == value2);
}


