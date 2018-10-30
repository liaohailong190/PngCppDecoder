//
// Created by liaohailong on 2018/10/30.
//

#include "PNGUtil.h"

#define  LOG_TAG    "jniTag"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

void readFileCallback(png_structp png_ptr, png_bytep destination, png_size_t bytesToRead) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);

    if (io_ptr == 0) {
        return;
    }

    png_voidp a = png_get_io_ptr(png_ptr);
    //Cast the pointer to std::istream* and read 'bytesToRead' bytes into 'destination'
    ((std::istream *) a)->read((char *) destination, bytesToRead);
}

unsigned char *LoadFromFilePath(const char *fileName, int *width, int *height) {
#if 1
    std::ifstream fin(fileName);
    //创建字符串流对象
    std::ostringstream sin;
    //把文件流中的字符输入到字符串流中
    sin << fin.rdbuf();
    std::string str = sin.str();
    std::istringstream pngDataStream(str);
    unsigned char *pixel = LoadFromStream(pngDataStream, width, height);
    fin.close();
    fin.clear();
    return pixel;
#endif
}

unsigned char *LoadFromStream(std::istream &pngDataStream, int *width, int *height) {

    const int PNG_SIG_BYTES = 8;
    char pngSignature[PNG_SIG_BYTES];
    pngDataStream.read(pngSignature, PNG_SIG_BYTES * sizeof(char));

    if (!png_check_sig((png_bytep) pngSignature, PNG_SIG_BYTES)) {
        return nullptr;
    }

    /**
     * Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return nullptr;
    }

    /**
     * Allocate/initialize the memory
     * for image information.  REQUIRED. */
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return nullptr;
    }

    /**
     * Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return nullptr;
    }


    /**
     * Set custom input stream READER / handler
     */
    png_set_read_fn(png_ptr, (void *) &pngDataStream, readFileCallback);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    png_byte bit_depth_ = png_get_bit_depth(png_ptr, info_ptr);
    png_uint_32 color_type_ = png_get_color_type(png_ptr, info_ptr);

    //CCLOG("color type %u", color_type);

    //调色板格式的png图片，转化为RGB888的像素格式
    // force palette images to be expanded to 24-bit RGB
    // it may include alpha channel
    if (color_type_ == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    //像素格式少于1字节长度的灰度图，将其转为每像素占1字节的像素格式
    // low-bit-depth grayscale images are to be expanded to 8 bits
    if (color_type_ == PNG_COLOR_TYPE_GRAY && bit_depth_ < 8) {
        bit_depth_ = 8;
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    //将tRNS块数据信息扩展为完整的ALPHA通道信息
    // expand any tRNS chunk data into a full alpha channel
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    //将16位输入降为8位
    // reduce images with 16-bit samples to 8 bits
    if (bit_depth_ == 16) {
        png_set_strip_16(png_ptr);
    }

    // Expanded earlier for grayscale, now take care of palette and rgb
    if (bit_depth_ < 8) {
        png_set_packing(png_ptr);
    }
    //更新png数据的详细信息
    // update info
    png_read_update_info(png_ptr, info_ptr);

    int _width, _height, _depth;
    int color_type, interlace_type;

    png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) &_width, (png_uint_32 *) &_height, &_depth,
                 &color_type, &interlace_type, NULL, NULL);

#if 1
    png_size_t cols = png_get_rowbytes(png_ptr, info_ptr);
    png_bytepp row_pp = new png_bytep[_height];
    unsigned char *bitmapData = new unsigned char[cols * _height];
#if 1
    for (int i = 0; i < _height; ++i) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        //    row_pp[_height - i - 1] = (png_bytep)&((char *)bitmapData)[ i * cols ];
        row_pp[i] = (png_bytep) &(bitmapData)[i * cols];
    }
#endif
    png_read_image(png_ptr, row_pp);
    png_read_end(png_ptr, info_ptr);

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);

    delete[] row_pp;
    *width = _width;
    *height = _height;
#endif
    return bitmapData;
}