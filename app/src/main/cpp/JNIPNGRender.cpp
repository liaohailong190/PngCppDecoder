//
// Created by Administrator on 2018/10/26.
//

#include <jni.h>
#include "PNGUtil.h"
#include<android/bitmap.h>
#include<android/log.h>
#include <cstring>

#define TAG "LIBPNG"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hidajian_htks_png_PNGRender_renderBitmap(JNIEnv *env, jobject instance, jobject bitmap,
                                                  jstring filePath_) {
    const char *filePath = env->GetStringUTFChars(filePath_, JNI_FALSE);

    //检查传入渲染的bitmap是否有问题
    AndroidBitmapInfo bmpinfo = {0};
    if (AndroidBitmap_getInfo(env, bitmap, &bmpinfo) < 0) {
        LOGI("bitmap read fail");
        return JNI_FALSE;
    }

    int width = 0;
    int height = 0;
    unsigned char *pixels = LoadFromFilePath(filePath, &width, &height);
    env->ReleaseStringUTFChars(filePath_, filePath);
    if (pixels == nullptr) return JNI_FALSE;

    if (bmpinfo.width != width || bmpinfo.height != height) {
        LOGI("lockPixels failed");
        delete[] pixels;
        return JNI_FALSE;
    }

    void *bmpFromJObject = NULL;
    if (AndroidBitmap_lockPixels(env, bitmap, &bmpFromJObject)) {
        LOGI("lockPixels failed");
        delete[] pixels;
        return JNI_FALSE;
    }
    unsigned char *srcData = (unsigned char *) bmpFromJObject;
    const int BLOCK_SIZE = 4;    //(rgba 4 bytes)
    long pos = 0;
    for (int y = 0; y < bmpinfo.height; y++) {
        long data_offset = static_cast<long>(bmpinfo.width * BLOCK_SIZE * y);
        for (int x = 0; x < bmpinfo.width * BLOCK_SIZE; x += BLOCK_SIZE) {
            srcData[pos++] = pixels[data_offset + x + 0];        //	R
            srcData[pos++] = pixels[data_offset + x + 1];        //	G
            srcData[pos++] = pixels[data_offset + x + 2];        //	B
            srcData[pos++] = pixels[data_offset + x + 3];        //	A
        }
    }

    if (AndroidBitmap_unlockPixels(env, bitmap) < 0) {
        LOGI("bitmap unlock fail");
        delete[] pixels;
        return JNI_FALSE;
    }
    delete[] pixels;
    return JNI_TRUE;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hidajian_htks_png_PNGRender_readByteBuffer(JNIEnv *env, jobject instance,
                                                    jstring filePath_, jobject byteBuffer) {
    const char *filePath = env->GetStringUTFChars(filePath_, JNI_FALSE);
    int width = 0;
    int height = 0;
    unsigned char *pixels = LoadFromFilePath(filePath, &width, &height);
    jbyte *pData = (jbyte *) env->GetDirectBufferAddress(byteBuffer);

    const int BLOCK_SIZE = 4;    //(rgba 4 bytes)
    long pos = 0;
    for (int y = 0; y < height; y++) {
        long data_offset = width * BLOCK_SIZE * y;
        for (int x = 0; x < width * BLOCK_SIZE; x += BLOCK_SIZE) {
            pData[pos++] = pixels[data_offset + x + 0];        //	R
            pData[pos++] = pixels[data_offset + x + 1];        //	G
            pData[pos++] = pixels[data_offset + x + 2];        //	B
            pData[pos++] = pixels[data_offset + x + 3];        //	A
        }
    }
    delete[]pixels;
    env->ReleaseStringUTFChars(filePath_, filePath);
}