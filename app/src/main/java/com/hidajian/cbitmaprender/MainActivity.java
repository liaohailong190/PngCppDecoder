package com.hidajian.cbitmaprender;

import android.Manifest;
import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.ValueAnimator;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.widget.ImageView;

import com.hidajian.htks.png.PNGRender;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private String frameNames[] = {
            "love1.png",
            "love2.png",
            "love3.png",
            "love4.png"

//            "kiss1.png",
//            "kiss2.png",
//            "kiss3.png",
//            "kiss4.png",
//            "kiss5.png",
//            "kiss6.png",
//            "kiss7.png",
//            "kiss8.png",
//            "kiss9.png",
//            "kiss10.png",
//            "kiss11.png",
//            "kiss12.png",
//            "kiss13.png",
//            "kiss14.png",
//            "kiss15.png",
//            "kiss16.png",

//            "number1.png",
//            "number2.png",
//            "number3.png",
//            "number4.png",
//            "number5.png",
//            "number6.png",
//            "number7.png",
//            "number8.png",
//            "number9.png",
//            "number10.png",
//            "number11.png",
//            "number12.png",
//            "number13.png",
//            "number14.png",
//            "number15.png"

//            "cucumber1.png",
//            "cucumber2.png",
//            "cucumber3.png",
//            "cucumber4.png",
//            "cucumber5.png",
//            "cucumber6.png",
//            "cucumber7.png",
//            "cucumber8.png",
//            "cucumber9.png",
//            "cucumber10.png",
//            "cucumber11.png",
//            "cucumber12.png",
//            "cucumber13.png",
//            "cucumber14.png",
//            "cucumber15.png",
//            "cucumber16.png",
//            "cucumber17.png",
//            "cucumber18.png",
//            "cucumber19.png",
//            "cucumber20.png",
//            "cucumber21.png",
//            "cucumber22.png",
//            "cucumber23.png",
//            "cucumber24.png",
//            "cucumber25.png",
//            "cucumber26.png"
    };
    private int index = 0;
    private ImageView frameImg;
    private Bitmap inBitmap;
    private PNGRender pngRender;
    private ValueAnimator valueAnimator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        pngRender = new PNGRender();
        frameImg = findViewById(R.id.frame_img);

        if (PermissionUtil.requestPerssions(this, 0, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
            File file = new File(Environment.getExternalStorageDirectory(), frameNames[0]);
            inBitmap = BitmapFactory.decodeFile(file.getAbsolutePath());

//            frameImg.setImageBitmap(inBitmap);
//            frameImg.postDelayed(new Runnable() {
//                @Override
//                public void run() {
//                    File file1 = new File(Environment.getExternalStorageDirectory(), frameNames[0]);
//                    int byteCount = inBitmap.getByteCount();
//                    ByteBuffer byteBuffer = ByteBuffer.allocateDirect(byteCount);
//                    pngRender.readByteBuffer(file1.getAbsolutePath(), byteBuffer);
//
//                    inBitmap.copyPixelsFromBuffer(byteBuffer);
//                    frameImg.setImageBitmap(inBitmap);
//                }
//            }, 4000);

            valueAnimator = ValueAnimator.ofInt(0, 1);
            valueAnimator.setDuration(64);
            valueAnimator.setRepeatCount(ValueAnimator.INFINITE);
            valueAnimator.addListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationRepeat(Animator animation) {
                    File file = new File(Environment.getExternalStorageDirectory(), frameNames[index % frameNames.length]);
                    if (pngRender.renderBitmap(inBitmap, file.getAbsolutePath())) {
                        frameImg.setImageBitmap(inBitmap);
                    }
                    frameImg.setImageBitmap(inBitmap);
                    index++;
                }
            });
            valueAnimator.start();
        }
    }

    @Override
    protected void onDestroy() {
        valueAnimator.cancel();
        super.onDestroy();
    }
}
