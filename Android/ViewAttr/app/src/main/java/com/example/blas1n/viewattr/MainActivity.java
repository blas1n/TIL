package com.example.blas1n.viewattr;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class MainActivity extends AppCompatActivity {

    ImageView image1, image2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void btnClick(View v) {
        image1 = findViewById(R.id.imageView1);
        image2 = findViewById(R.id.imageView2);

        switch (v.getId()) {
            case R.id.button :
                image1.setVisibility(View.VISIBLE);
                image2.setVisibility(View.INVISIBLE);
                break;

            case R.id.button2 :
                image1.setVisibility(View.INVISIBLE);
                image2.setVisibility(View.VISIBLE);
                break;
        }
    }
}
