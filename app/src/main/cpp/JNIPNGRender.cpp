//
// Created by Administrator on 2018/10/26.
//

#include <jni.h>
#include "PNGUtil.h"
#include<android/bitmap.h>
#include<android/log.h>
#include <cstring>

#define TAG "LibPNG"
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
    unsigned char *pixels = readPixel(filePath, &width, &height);
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
    //内存复制，速度最快
    memcpy(srcData, pixels, bmpinfo.width * BLOCK_SIZE * bmpinfo.height);

    if (AndroidBitmap_unlockPixels(env, bitmap) < 0) {
        LOGI("bitmap unlock fail");
        delete[] pixels;
        return JNI_FALSE;
    }
    delete[] pixels;
    return JNI_TRUE;
}