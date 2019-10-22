package com.example.blas1n.drawcar;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.graphics.Rect;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_main);
        MyView m = new MyView(MainActivity.this);
        setContentView(m);
    }
}

class MyView extends View {
    public MyView(Context context) {
        super((context));
    }

    @Override
    protected void onDraw(Canvas canvas) {
        Paint paint = new Paint();
        paint.setColor(Color.GREEN);
        paint.setStyle(Paint.Style.FILL);
        paint.setStrokeWidth(5);

        // 몸체
        canvas.drawRect(150, 400, 550, 600, paint);

        // 바퀴
        canvas.drawCircle(250, 650, 50, paint);
        canvas.drawCircle(450, 650, 50, paint);

        // 배기통
        canvas.drawCircle(580, 510, 30, paint);
        canvas.drawCircle(580, 570, 30, paint);

        // 팔?
        canvas.drawLine(50,450,150, 450, paint);
        canvas.drawLine(50,550,150, 550, paint);

        // 뚜껑
        Point point1 = new Point(400, 300);
        Point point2 = new Point(320, 400);
        Point point3 = new Point(480, 400);

        Path path = new Path();
        path.reset();

        path.moveTo(point1.x, point1.y);
        path.lineTo(point2.x, point2.y);
        path.lineTo(point3.x, point3.y);
        path.lineTo(point1.x, point1.y);
        canvas.drawPath(path, paint);

        path.close();
    }
}