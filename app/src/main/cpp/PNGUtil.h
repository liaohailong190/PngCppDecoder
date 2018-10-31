//
// Created by liaohailong on 2018/10/30.
//

#ifndef CBITMAPRENDER_PNGUTIL_H
#define CBITMAPRENDER_PNGUTIL_H

#include "lpng1635/png.h"
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <stdio.h>


unsigned char *readPixel(const char *filepath, int *width, int *height);

#endif //CBITMAPRENDER_PNGUTIL_H
