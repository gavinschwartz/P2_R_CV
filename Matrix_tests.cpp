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
TEST(test_fill_basic)
{
    Matrix mat;
    const int width = 3;
    const int height = 5;
    const int value = 42;
    Matrix_init(&mat, 3, 5);
    ASSERT_EQUAL(Matrix_width(&mat), 5);
    ASSERT_EQUAL(Matrix_height(&mat), 3);
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

    string expected = string("3 5 \n24 24 24 \n24 42 24 \n24 42 24 \n24 42 24 \n24 24 24 ");
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

TEST_MAIN() // Do NOT put a semicolon here
