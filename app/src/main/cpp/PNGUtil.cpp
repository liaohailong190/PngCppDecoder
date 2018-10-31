//
// Created by liaohailong on 2018/10/30.
//

#include "PNGUtil.h"

#define  LOG_TAG    "jniTag"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define PNG_BYTES_TO_CHECK 4

//获取每一行所用的字节数，需要凑足4的倍数
int getRowBytes(int width) {
    //刚好是4的倍数
    if ((width * 3) % 4 == 0) {
        return width * 3;
    } else {
        return ((width * 3) / 4 + 1) * 4;
    }
}

unsigned char *LoadFromFilePath(const char *filepath, int *_width, int *_height) {
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
    printf("width[%d], height[%d], bit_depth[%d], color_type[%d]\n",
           width, height, bit_depth, color_type);

    //获得所有png数据
    png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);
    //计算buffer大小
    unsigned int bufSize = 0;
    if (color_type == PNG_COLOR_TYPE_RGB) {
        bufSize = getRowBytes(width) * height;
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        bufSize = width * height * 4;
    } else {
        return nullptr;
    }
    //申请堆空间
    unsigned char *buffer = new unsigned char[bufSize];
    int i;
    for (i = 0; i < height; i++) {
        //拷贝每行的数据到buffer，
        //opengl原点在下方，拷贝时要倒置一下
        if (color_type == PNG_COLOR_TYPE_RGB) {
            memcpy(buffer + getRowBytes(width) * i, row_pointers[i], width * 3);
        } else if (color_type == PNG_COLOR_TYPE_RGBA) {
            memcpy(buffer + i * width * 4, row_pointers[i], width * 4);
        }
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fclose(fp);
    *_width = width;
    *_height = height;
    return buffer;
}