package com.example.blas1n.toggleimage;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class MainActivity extends AppCompatActivity {

    ImageView starImage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        starImage = findViewById(R.id.star);
    }

    public void btnClick(View v) {
        if (starImage.getVisibility() == View.VISIBLE)
            starImage.setVisibility(View.INVISIBLE);
        else
            starImage.setVisibility(View.VISIBLE);
    }
}
