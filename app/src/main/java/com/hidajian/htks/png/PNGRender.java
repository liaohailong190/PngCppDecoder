package com.hidajian.htks.png;

import android.graphics.Bitmap;

/**
 * Describe as : 使用libpng渲染bitmap
 * Created by Liaohailong on 2018/10/26.
 */
public class PNGRender {
    static {
        System.loadLibrary("PNGRender");
    }

    public native boolean renderBitmap(Bitmap bitmap, String filePath);
}