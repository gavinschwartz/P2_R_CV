#include <cassert>
#include "Matrix.hpp"

using namespace std;
// REQUIRES: mat points to a Matrix
//           0 < width && 0 < height
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height,
//           with all elements initialized to 0.
void Matrix_init(Matrix *mat, int width, int height)
{
    // set width & height of matrix
    mat->height = height;
    mat->width = width;

    // creating vector by resizing
    mat->data.resize(width * height);
    for (int y = 0; y < mat->data.size(); y++)
    {
        mat->data[y] = 0;
    }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix *mat, std::ostream &os)
{
    os << mat->width << " " << mat->height << "\n";
    for (int y = 0; y < mat->height; y++)
    {
        for (int x = 0; x < mat->width; x++)
        {
            os << mat->data[y * mat->width + x] << " ";
        }
        os << "\n";
    }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix *mat)
{
    return mat->width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix *mat)
{
    return mat->height;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int *Matrix_at(Matrix *mat, int row, int column)
{
    assert(0 <= row && row < mat->height);
    assert(0 <= column && column < mat->width);

    int index = row * mat->width + column;

    return &mat->data[index];
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int *Matrix_at(const Matrix *mat, int row, int column)
{
    assert(0 <= row && row < mat->height);
    assert(0 <= column && column < mat->width);

    int index = row * mat->width + column;

    return &mat->data[index];
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix *mat, int value)
{
    for (int y = 0; y < mat->data.size(); y++)
    {
        mat->data[y] = value;
    }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix *mat, int value)
{
    for (int y = 0; y < mat->height; y++)
    {
        for (int x = 0; x < mat->width; x++)
        {
            if (x == 0 || y == 0 || x == mat->width - 1 || y == mat->height - 1)
            {
                *Matrix_at(mat, y, x) = value;
            }
        }
    }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix *mat)
{
    int maximum = mat->data[0];
    for (int i = 1; i < mat->data.size(); i++)
    {
        if (maximum < mat->data[i])
        {
            maximum = mat->data[i];
        }
    }
    return maximum;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix *mat, int row,
                                      int column_start, int column_end)
{
    int minimum_column = column_start;
    int minimum = *Matrix_at(mat, row, minimum_column);

    for (int x = column_start + 1; x < column_end; x++)
    {
        int current_value = *Matrix_at(mat, row, x);

        if (current_value < minimum)
        {
            minimum = current_value;
            minimum_column = x;
        }
    }
    return minimum_column;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix *mat, int row,
                            int column_start, int column_end)
{
    int minimum_column = Matrix_column_of_min_value_in_row(
        mat, row, column_start, column_end
    );

    return *Matrix_at(mat, row, minimum_column);
}
