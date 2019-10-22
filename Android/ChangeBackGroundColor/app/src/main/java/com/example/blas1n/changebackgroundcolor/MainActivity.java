package com.example.blas1n.changebackgroundcolor;

import android.graphics.Color;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.RadioGroup;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    ConstraintLayout back;
    TextView hello;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        back = findViewById(R.id.BackGround);
        hello = findViewById(R.id.Hello);

        RadioGroup backRadio = findViewById(R.id.radioGroup1);
        backRadio.setOnCheckedChangeListener(backCheck);

        RadioGroup helloRadio = findViewById(R.id.radioGroup2);
        helloRadio.setOnCheckedChangeListener(helloCheck);
    }

    RadioGroup.OnCheckedChangeListener backCheck =
            new RadioGroup.OnCheckedChangeListener() {
                public void onCheckedChanged(RadioGroup group, int checkedId) {

                    switch (checkedId){
                        case R.id.radioButtonRed1:
                            back.setBackgroundColor(Color.RED);
                            break;

                        case R.id.radioButtonGreen1:
                            back.setBackgroundColor(Color.GREEN);
                            break;

                        case R.id.radioButtonBlue1:
                            back.setBackgroundColor(Color.BLUE);
                            break;
                    }
                }
            };

    RadioGroup.OnCheckedChangeListener helloCheck =
            new RadioGroup.OnCheckedChangeListener() {
                public void onCheckedChanged(RadioGroup group, int checkedId) {

                    switch (checkedId){
                        case R.id.radioButtonRed2:
                            hello.setTextColor(Color.RED);
                            break;

                        case R.id.radioButtonGreen2:
                            hello.setTextColor(Color.GREEN);
                            break;

                        case R.id.radioButtonBlue2:
                            hello.setTextColor(Color.BLUE);
                            break;
                    }
                }
            };
}
