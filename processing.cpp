#include <cassert>
#include <vector>
#include "processing.hpp"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image *img)
{

    // for convenience
    int width = Image_width(img);
    int height = Image_height(img);

    // auxiliary image to temporarily store rotated image
    Image aux;
    Image_init(&aux, height, width); // width and height switched

    // iterate through pixels and place each where it goes in temp
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
        }
    }

    // Copy data back into original
    *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image *img)
{

    // for convenience
    int width = Image_width(img);
    int height = Image_height(img);

    // auxiliary image to temporarily store rotated image
    Image aux;
    Image_init(&aux, height, width); // width and height switched

    // iterate through pixels and place each where it goes in temp
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
        }
    }

    // Copy data back into original
    *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2)
{
    int dr = p2.r - p1.r;
    int dg = p2.g - p1.g;
    int db = p2.b - p1.b;
    // Divide by 100 is to avoid possible overflows
    // later on in the algorithm.
    return (dr * dr + dg * dg + db * db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// ------------------------------------------------------------------
// You may change code below this line!

// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image *img, Matrix *energy)
{
    Matrix_init(energy, Image_width(img), Image_height(img));
    int maxEnergy = 0;

    // computing for the interior cells + saving the maximum energy
    for (int r = 1; r < Matrix_height(energy) - 1; r++)
    {
        for (int c = 1; c < Matrix_width(energy) - 1; c++)
        {
            int e = squared_difference(
                        Image_get_pixel(img, r - 1, c),
                        Image_get_pixel(img, r + 1, c)) +
                    squared_difference(
                        Image_get_pixel(img, r, c - 1),
                        Image_get_pixel(img, r, c + 1));
            if (e > maxEnergy)
            {
                maxEnergy = e;
            }
            *Matrix_at(energy, r, c) = e;
        }
    }

    // set the boarder cells equal to maximum energy
    for (int r = 0; r < Matrix_height(energy); r++)
    {
        for (int c = 0; c < Matrix_width(energy); c++)
        {
            if (r == 0 || c == 0 ||
                r == Matrix_height(energy) - 1 || c == Matrix_width(energy) - 1)
            {
                *Matrix_at(energy, r, c) = maxEnergy;
            }
        }
    }
}

// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix *energy, Matrix *cost)
{
    Matrix_init(cost, Matrix_width(energy), Matrix_height(energy));

    // for the first row, the cost is equal to the energy
    for (int c = 0; c < Matrix_width(energy); c++)
    {
        *Matrix_at(cost, 0, c) = *Matrix_at(energy, 0, c);
    }
    // the cost is equal to the energy + minimum cost of its northern neighbors
    // (for rows after the first row)
    for (int r = 1; r < Matrix_height(energy); r++)
    {
        for (int c = 0; c < Matrix_width(energy); c++)
        {
            int minimum = 100000000;

            if (c != 0 && *Matrix_at(cost, r - 1, c - 1) < minimum)
            {
                minimum = *Matrix_at(cost, r - 1, c - 1);
            }
            if (*Matrix_at(cost, r - 1, c) < minimum)
            {
                minimum = *Matrix_at(cost, r - 1, c);
            }
            if (c != Matrix_width(energy) - 1 && *Matrix_at(cost, r - 1, c + 1) < minimum)
            {
                minimum = *Matrix_at(cost, r - 1, c + 1);
            }
            *Matrix_at(cost, r, c) = *Matrix_at(energy, r, c) + minimum;
        }
    }
}

// REQUIRES: cost points to a valid Matrix
// EFFECTS:  Returns the vertical seam with the minimal cost according to the given
//           cost matrix, represented as a vector filled with the column numbers for
//           each pixel along the seam, starting with the lowest numbered row (top
//           of image) and progressing to the highest (bottom of image). The length
//           of the returned vector is equal to Matrix_height(cost).
//           While determining the seam, if any pixels tie for lowest cost, the
//           leftmost one (i.e. with the lowest column number) is used.
//           See the project spec for details on computing the minimal seam.
vector<int> find_minimal_vertical_seam(const Matrix *cost)
{
    vector<int> seam = vector<int>(Matrix_height(cost));

    // start with the column with the minimum cost in the bottom row
    int c = Matrix_column_of_min_value_in_row(cost,
                     Matrix_height(cost) - 1, 0, Matrix_width(cost));

    // computing the cost from the bottom row up
    for (int i = Matrix_height(cost) - 1; i > 0; i--)
    {
        seam[i] = c;
        int leftNeighbor = c == 0 ? 0 : c - 1;
        int rightNeighbor = c == Matrix_width(cost) - 1 ? c : c + 1;
        c = Matrix_column_of_min_value_in_row(
            cost, i - 1, leftNeighbor, rightNeighbor + 1);
    }
    seam[0] = c;

    return seam;
}

// REQUIRES: img points to a valid Image with width >= 2
//           seam.size() == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Declare a new variable to hold the smaller Image, and
//           then do an assignment at the end to copy it back into the
//           original image.
void remove_vertical_seam(Image *img, const vector<int> &seam)
{
    // create and initialize a temporary image that is one pixel narrower
    Image tempImage;
    Image_init(&tempImage, Image_width(img) - 1, Image_height(img));

    // copy all pixels except for vertical seam from img to tempImage
    for (int r = 0; r < Image_height(img); r++)
    {
        // copy pixels before the seam into tempImage
        for (int c = 0; c < seam[r]; c++)
        {
            Pixel p = Image_get_pixel(img, r, c);
            Image_set_pixel(&tempImage, r, c, p);
        }

        // copy pixels after the seam into tempImage
        for (int c = seam[r] + 1; c < Image_width(img); c++)
        {
            Pixel p = Image_get_pixel(img, r, c);
            Image_set_pixel(&tempImage, r, c - 1, p);
        }
    }

    // resize the original image to be one pixel narrower
    Image_init(img, Image_width(img) - 1, Image_height(img));

    // copy all pixels from tempImage into Image
    for (int r = 0; r < Image_height(img); r++)
    {
        for (int c = 0; c < Image_width(img); c++)
        {
            Pixel p = Image_get_pixel(&tempImage, r, c);
            Image_set_pixel(img, r, c, p);
        }
    }
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use a vector to hold the seam, and make sure that it has
//           the right size. You can use .data() on a vector to get
//           the underlying array.
void seam_carve_width(Image *img, int newWidth)
{
    int numberOfColumnsToRemove = Image_width(img) - newWidth;

    Matrix energy, cost;

    Matrix_init(&energy, Image_width(img), Image_height(img));
    Matrix_init(&cost, Image_width(img), Image_height(img));

    // repeatedly removing vertical seams

    for (int i = 0; i < numberOfColumnsToRemove; i++)
    {
        compute_energy_matrix(img, &energy);
        compute_vertical_cost_matrix(&energy, &cost);

        // dynamically allocating the array for the seam
        vector<int> seam = find_minimal_vertical_seam(&cost);
        remove_vertical_seam(img, seam);
    }
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight)
{
    rotate_left(img);
    seam_carve_width(img, newHeight);
    rotate_right(img);
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight)
{
    seam_carve_width(img, newWidth);
    seam_carve_height(img, newHeight);
}
