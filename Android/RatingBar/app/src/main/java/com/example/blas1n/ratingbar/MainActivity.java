package com.example.blas1n.ratingbar;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.RatingBar;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    RatingBar star;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        star = findViewById(R.id.ratingBar);
        star.setOnRatingBarChangeListener(new RatingBar.OnRatingBarChangeListener() {
            @Override
            public void onRatingChanged(RatingBar ratingBar, float v, boolean b) {
                check();
            }
        });
    }

    public void checkStar(View v) { check(); }

    public void change(View v) {
        float rating = star.getRating();

        if (v.getId() == R.id.Plus) rating += 0.5f;
        else rating -= 0.5f;

        star.setRating(rating);
    }

    private void check() {
        String rating = "레이팅: " + String.valueOf(star.getRating());
        Toast.makeText(getApplicationContext(), rating, Toast.LENGTH_LONG).show();
    }
}