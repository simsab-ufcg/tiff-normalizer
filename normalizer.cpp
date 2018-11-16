#include <bits/stdc++.h>
#include "tiffio.h"
using namespace std;

TIFF* openFile(string filename, int width, int height){
    TIFF* file = TIFFOpen(filename.c_str(), "w8m");
    TIFFSetField(file, TIFFTAG_IMAGEWIDTH     , width); 
    TIFFSetField(file, TIFFTAG_IMAGELENGTH    , height);
    TIFFSetField(file, TIFFTAG_BITSPERSAMPLE  , 64);
    TIFFSetField(file, TIFFTAG_SAMPLEFORMAT   , 3);
    TIFFSetField(file, TIFFTAG_COMPRESSION    , 1);
    TIFFSetField(file, TIFFTAG_PHOTOMETRIC    , 1);
    TIFFSetField(file, TIFFTAG_ORIENTATION    , 1);
    TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(file, TIFFTAG_ROWSPERSTRIP   , 8);
    TIFFSetField(file, TIFFTAG_RESOLUTIONUNIT , 1);
    TIFFSetField(file, TIFFTAG_XRESOLUTION    , 1);
    TIFFSetField(file, TIFFTAG_YRESOLUTION    , 1);
    TIFFSetField(file, TIFFTAG_PLANARCONFIG   , PLANARCONFIG_CONTIG   );
    return file;
}

void normalize(string inputFile, string outputFile){

    TIFF *input = TIFFOpen(inputFile.c_str(), "rm");

    int width = 0, height = 0;
    TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(input, TIFFTAG_IMAGELENGTH, &height);

    TIFF *output = openFile(outputFile, width, height);

    double *line = new double[width];
    double *outline = new double[width];

    double minValue, maxValue;
    bool foundSomething = false;

    for (int i = 0; i < height; i++) {
        TIFFReadScanline(input, line, i);
        for (int j = 0; j < width; j++) {
            if (!isnan(line[j])) {
                if (foundSomething) {
                    if (minValue > line[j])
                        minValue = line[j];
                    if (maxValue < line[j])
                        maxValue = line[j];
                } else {
                    minValue = line[j];
                    maxValue = line[j];
                    foundSomething = true;
                }
            }
        }
    }

    for (int i = 0; i < height; i++) {
        TIFFReadScanline(input, line, i);
        for (int j = 0; j < width; j++) {
            if (!isnan(line[j])) {
                outline[j] = (line[j] - minValue) / (maxValue - minValue);
            } else {
                outline[j] = -sqrt(-1.0);
            }
        }
        TIFFWriteScanline(output, outline, i);
    }

    TIFFClose(output);
}

int main(int argc, char* argv[]){

    if (argc != 2) {
        cout << "Wrong parameters!!" << endl;
    } else {
        string file = string(argv[1]);
        string tmpFile = "tmp." + file;
        normalize(file, tmpFile);
        rename(tmpFile.c_str(), file.c_str());
    }  
    return 0;
}