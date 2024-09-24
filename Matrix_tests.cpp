#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test1)
{
    Matrix mat;
    const int width = 3;
    const int height = 5;
    const int value = 42;
    Matrix_init(&mat, width, height);
    ASSERT_EQUAL(Matrix_width(&mat), width);
    ASSERT_EQUAL(Matrix_height(&mat), height);
    Matrix_fill(&mat, value);

    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
        }
    }
    
    Matrix_fill_border(&mat, 24);
    cout << endl;
    ostringstream output;

    Matrix_print(&mat, output);

    string expected = "3 5\n24 24 24 \n24 42 24 \n24 42 24 \n24 42 24 \n24 24 24 \n";
    ASSERT_EQUAL(output.str(), expected);

    const int c = *Matrix_at(&mat, 0, 0);
    ASSERT_EQUAL(c, 24);

    ASSERT_EQUAL(*Matrix_at(&mat, 0, 0), 24);
    ASSERT_EQUAL(*Matrix_at(&mat, 4, 2), 24);
    ASSERT_EQUAL(*Matrix_at(&mat, 1, 1), 42);

    *Matrix_at(&mat, 3, 2) = 43;

    ASSERT_EQUAL(Matrix_max(&mat), 43);

    ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 1, 0, 1), 0);

    ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 1, 0, 1), 24);
}
TEST(test_matrix_print){
    Matrix mat;
    Matrix_init(&mat, 1, 1);
    *Matrix_at(&mat, 0, 0) = 42;
    ostringstream expected;
    expected << "1 1\n"
             << "42 \n";
    ostringstream actual;
    Matrix_print(&mat, actual);
    ASSERT_EQUAL(expected.str(), actual.str());
}
TEST(test_matrix_init){
    Matrix mat1, mat2;
    Matrix_init(&mat1, 1, 1);
    Matrix_init(&mat2, 10, 100);
    ASSERT_EQUAL(Matrix_width(&mat1), 1);
    ASSERT_EQUAL(Matrix_height(&mat1), 1);
    ASSERT_EQUAL(Matrix_width(&mat2), 10);
    ASSERT_EQUAL(Matrix_height(&mat2), 100);
}
TEST(test_matrix_at){
    Matrix mat;
    Matrix_init(&mat,5, 3);
    Matrix_fill(&mat, 10);
    Matrix_fill_border(&mat, 42);
    ASSERT_EQUAL(*Matrix_at(&mat, 1, 1), 10);
    ASSERT_EQUAL(*Matrix_at(&mat, 0, 0), 42);
    ASSERT_EQUAL(*Matrix_at(&mat, 2, 2), 42);
}
TEST(test_matrix_fill_border){
    Matrix mat;
    Matrix_init(&mat, 3, 5);
    Matrix_fill(&mat, 10);
    Matrix_fill_border(&mat, 42);
    for (size_t r = 0; r < Matrix_height(&mat); r++){
        ASSERT_EQUAL(*Matrix_at(&mat, r, 0), 42);
        ASSERT_EQUAL(*Matrix_at(&mat, r, 2), 42);
        }
    
    for (size_t c = 0; c < Matrix_width(&mat); c++){
        ASSERT_EQUAL(*Matrix_at(&mat, 0, c), 42);
        ASSERT_EQUAL(*Matrix_at(&mat, 4, c), 42);
        }
}
TEST(test_matrix_max){
    Matrix mat;
    Matrix_init(&mat, 3, 5);
    Matrix_fill(&mat, 10);
    *Matrix_at(&mat, 4, 2) = 11;
    ASSERT_EQUAL(Matrix_max(&mat), 11);
    Matrix_fill(&mat, -6);
    Matrix_fill_border(&mat, -2);
    ASSERT_EQUAL(Matrix_max(&mat), -2);
}
TEST(test_matrix_column_of_min_value_in_row){
    Matrix mat;
    Matrix_init(&mat, 3, 5);
    Matrix_fill(&mat, 10);
    ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 3, 0, 2), 0);
    Matrix_fill_border(&mat, 50);
    ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 3, 0, 2), 1);
}
TEST(test_matrix_min_value_in_row) {
    Matrix mat;
    Matrix_init(&mat, 3, 5);
    Matrix_fill(&mat, 10);
    ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 3, 0, 2), 10);
    Matrix_fill_border(&mat, 5);
    ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 3, 0, 2), 5);
}
TEST_MAIN() // Do NOT put a semicolon here
