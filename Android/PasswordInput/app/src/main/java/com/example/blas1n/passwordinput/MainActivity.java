package com.example.blas1n.passwordinput;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    EditText text1, text2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        text1 = findViewById(R.id.password1);
        text2 = findViewById(R.id.password2);

        text2.setImeOptions(EditorInfo.IME_ACTION_DONE);
        text2.setOnEditorActionListener(new TextView.OnEditorActionListener()  {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                if(actionId == EditorInfo.IME_ACTION_DONE) {
                    String word = text2.getText().toString();

                    if (!word.equals(text1.getText().toString())) {
                        Toast.makeText(getApplicationContext(), "비밀번호가 서로 다릅니다", Toast.LENGTH_SHORT).show();
                        return false;
                    }

                    if (!word.contains("!")) {
                        Toast.makeText(getApplicationContext(), "특수 문자가 필요합니다", Toast.LENGTH_SHORT).show();
                        return false;
                    }

                    Toast.makeText(getApplicationContext(), word, Toast.LENGTH_SHORT).show();
                }

                return true;
            }
        });
    }
}
