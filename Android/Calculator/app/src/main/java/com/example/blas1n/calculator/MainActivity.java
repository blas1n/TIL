package com.example.blas1n.calculator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.util.LinkedList;
import java.util.Queue;

public class MainActivity extends AppCompatActivity {

    TextView text;
    Queue<Integer> numQueue;
    Queue<Character> operatorQueue;
    String tempNum;
    boolean bFigure;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        text = findViewById(R.id.textView);
        numQueue = new LinkedList<>();
        operatorQueue = new LinkedList<>();
        tempNum = "";
        bFigure = false;
    }

    public void numClick(View v) {
        if (bFigure) {
            tempNum = "";
            bFigure = false;
        }

        String value = ((Button)v).getText().toString();

        if (operatorQueue.isEmpty() || operatorQueue.peek() != '/' || (Integer.parseInt(tempNum + value) != 0)) {
            if ((tempNum + value).length() < 10) {
                tempNum += value;
                text.setText(tempNum);
            }

            else
                Toast.makeText(getApplicationContext(), "10자리 이상은 지원하지 않습니다", Toast.LENGTH_SHORT).show();
        }

        else
            Toast.makeText(getApplicationContext(), "0으로 나눌 수 없습니다", Toast.LENGTH_SHORT).show();
    }

    public void operatorClick(View v) {
        if (v.getId() == R.id.Clear) {
            numQueue.clear();
            operatorQueue.clear();

            tempNum = "";
            text.setText("");
        }

        else {
            if (numQueue.isEmpty() && tempNum.equals("")) return;

            if (v.getId() == R.id.Equal) {
                if (!tempNum.equals(""))
                    numQueue.offer(Integer.parseInt(tempNum));

                tempNum = String.valueOf(figure());
                text.setText(tempNum);
                bFigure = true;
            }

            else {
                String operator = ((Button)v).getText().toString();

                numQueue.offer(Integer.parseInt(tempNum));
                operatorQueue.offer(operator.charAt(0));
                tempNum = "";

                text.setText(operator);
                bFigure = false;
            }
        }
    }

    private int figure() {
        int result = numQueue.poll();

        while (!numQueue.isEmpty()) {
            int operand = numQueue.poll();

            switch (operatorQueue.poll()) {
                case '+':
                    result += operand;
                    break;

                case '-':
                    result -= operand;
                    break;

                case '*':
                    result *= operand;
                    break;

                case '/':
                    float temp = (float)result / (float)operand;
                    result = (int)temp;

                    if ((float)result != temp)
                        Toast.makeText(getApplicationContext(), "소수점을 버렸습니다.", Toast.LENGTH_SHORT).show();

                    break;
            }
        }

        if (result >= 1000000000) {
            result = 999999999;
            Toast.makeText(getApplicationContext(), "이 계산기의 최댓값으로 치환됩니다", Toast.LENGTH_SHORT).show();
        }

        return result;
    }
}
