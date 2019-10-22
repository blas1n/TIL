package com.example.blas1n.hello;

import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    EditText phoneText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void calling(View v) {
        phoneText = findViewById(R.id.editText);

        Toast.makeText(getApplicationContext(), "전화 연결", Toast.LENGTH_LONG).show();
        String phoneNum = "tel:" + phoneText.getText().toString();
        Intent in = new Intent(Intent.ACTION_DIAL, Uri.parse(phoneNum));
        startActivity(in);
    }
}
