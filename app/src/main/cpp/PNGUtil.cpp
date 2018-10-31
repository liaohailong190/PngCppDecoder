//
// Created by liaohailong on 2018/10/30.
//

#include "PNGUtil.h"

#define  LOG_TAG    "jniTag"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define PNG_BYTES_TO_CHECK 4

unsigned char *readPixel(const char *filepath, int *_width, int *_height) {
    png_structp png_ptr;
    png_infop info_ptr;
    FILE *fp;

    if ((fp = fopen(filepath, "rb")) == NULL) {
        return nullptr;
    }
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp);
        return nullptr;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return nullptr;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a problem reading the file */
        return nullptr;
    }
    png_uint_32 width, height;
    int bit_depth, color_type;
    /* Set up the input control if you are using standard C streams */
    png_init_io(png_ptr, fp);
    //读取png文件
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
    //获取png图片相关信息
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL,
                 NULL);

    //获得所有png数据
    png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);
    unsigned int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    //申请堆空间
    unsigned char *buffer = new unsigned char[rowbytes * height];
    long pos = 0;
    if (color_type == PNG_COLOR_TYPE_RGB) {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width * 3; j += 3) {
                buffer[pos++] = row_pointers[i][j + 0];
                buffer[pos++] = row_pointers[i][j + 1];
                buffer[pos++] = row_pointers[i][j + 2];
            }
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width * 4; j += 4) {
                buffer[pos++] = row_pointers[i][j + 0] * row_pointers[i][j + 3] / 255;
                buffer[pos++] = row_pointers[i][j + 1] * row_pointers[i][j + 3] / 255;
                buffer[pos++] = row_pointers[i][j + 2] * row_pointers[i][j + 3] / 255;
                buffer[pos++] = row_pointers[i][j + 3];
            }
    } else {
        return nullptr;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fclose(fp);
    *_width = width;
    *_height = height;
    return buffer;
}