#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include "Image.hpp"
#include "processing.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4 && argc != 5)
    {

        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
             << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        exit(1);
    }

    char *inputFileName = argv[1];
    ifstream input;
    input.open(inputFileName);
    if (!input.is_open())
    {
        cout << "Error opening file: " << inputFileName << endl;
    }

    Image img;
    Image_init(&img, input);

    char *outputFileName = argv[2];
    ofstream output;
    output.open(outputFileName);
    if (!output.is_open())
    {
        cout << "Error opening file: " << outputFileName << endl;
    }

    int width = stoi(string(argv[3]));

    int height;
    if (argc == 4)
    {
        height = img.height;
    }
    else
    {
        height = stoi(string(argv[4]));
    }

    seam_carve(&img, width, height);
    Image_print(&img, output);

    return 0;
}
