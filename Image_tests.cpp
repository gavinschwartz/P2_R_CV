#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic)
{
    Image img;
    const Pixel red = {255, 0, 0};
    const Pixel green = {0, 255, 0};
    const Pixel blue = {0, 0, 255};
    const Pixel white = {255, 255, 255};

    Image_init(&img, 2, 2);
    ASSERT_EQUAL(Image_width(&img), 2);
    ASSERT_EQUAL(Image_height(&img), 2);
    Image_set_pixel(&img, 0, 0, red);
    Image_set_pixel(&img, 0, 1, green);
    Image_set_pixel(&img, 1, 0, blue);
    Image_set_pixel(&img, 1, 1, white);

    // Capture our output
    ostringstream s;

    string expected = "P3\n2 2\n255\n255 0 0 0 255 0 \n0 0 255 255 255 255 \n";
    Image_print(&img, s);
    // Image_print(&img,cout);
    ASSERT_EQUAL(expected, s.str());

    // Correct output
    ostringstream correct;
    correct << "P3\n2 2\n255\n";
    correct << "255 0 0 0 255 0 \n";
    correct << "0 0 255 255 255 255 \n";
    ASSERT_EQUAL(s.str(), correct.str());
}

TEST(test_print_advanced)
{
    Image img;              // create an Image object in local memory
    Image_init(&img, 5, 5); // initialize it as a 5x5 image

    Pixel um_blue = {0, 46, 98};
    Pixel um_maize = {251, 206, 51};
    Image_fill(&img, um_blue); // fill with blue

    // fill every other column with maize to make stripes
    for (int c = 0; c < Image_width(&img); ++c)
    {
        if (c % 2 == 0)
        { // only even columns
            for (int r = 0; r < Image_height(&img); ++r)
            {
                Image_set_pixel(&img, r, c, um_maize);
            }
        }
    }
    // Capture our output
    ostringstream s;
    Image_print(&img, s);

    // Correct output
    ostringstream correct;
    correct << "P3\n5 5\n255\n";
    correct << "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n";
    correct << "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n";
    correct << "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n";
    correct << "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n";
    correct << "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n";
    ASSERT_EQUAL(s.str(), correct.str());
}

TEST(test_init)
{

    Image img; // create an Image object in local memory

    istringstream is;
    string input = string("P3\n5 5\n255\n") + "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n" + "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n" + "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n" + "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n" + "251 206 51 0 46 98 251 206 51 0 46 98 251 206 51 \n";

    is.str(input);

    Image_init(&img, is);

    for (int c = 0; c < Image_width(&img); ++c)
    {
        for (int r = 0; r < Image_height(&img); ++r)
        {
            Pixel pixel = Image_get_pixel(&img, r, c);
            if (c % 2 == 0)
            { // only even columns
                ASSERT_EQUAL(251, pixel.r)
                ASSERT_EQUAL(206, pixel.g)
                ASSERT_EQUAL(51, pixel.b)
            }
            else
            {
                ASSERT_EQUAL(0, pixel.r)
                ASSERT_EQUAL(46, pixel.g)
                ASSERT_EQUAL(98, pixel.b)
            }
        }
    }
}

// IMPLEMENT YOUR TEST FUNCTIONS HERE
// You are encouraged to use any functions from Image_test_helpers.hpp as needed.

TEST_MAIN() // Do NOT put a semicolon here
