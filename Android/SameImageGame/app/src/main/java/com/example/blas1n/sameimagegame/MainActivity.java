package com.example.blas1n.sameimagegame;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private boolean bAdd, bMulti;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void add(View v) {
        if (bAdd) {
            Toast.makeText(getApplicationContext(), "정답!", Toast.LENGTH_LONG).show();
            bAdd = false;
        }

        else if (bMulti) {
            Toast.makeText(getApplicationContext(), "오답..", Toast.LENGTH_LONG).show();
            bMulti = false;
        }

        else bAdd = true;
    }

    public void multi(View v) {
        if (bMulti) {
            Toast.makeText(getApplicationContext(), "정답!", Toast.LENGTH_LONG).show();
            bMulti = false;
        }

        else if (bAdd) {
            Toast.makeText(getApplicationContext(), "오답..", Toast.LENGTH_LONG).show();
            bAdd = false;
        }

        else bMulti = true;
    }
}
