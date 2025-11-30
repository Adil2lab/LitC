package com.controller;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.LinearLayout;
import android.view.Gravity;
import android.widget.TextView;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("controller");
    }

    private native boolean sendMessage(String serverIp, String message);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Create UI programmatically (no XML needed!)
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setPadding(50, 50, 50, 50);
        layout.setGravity(Gravity.CENTER);

        TextView title = new TextView(this);
        title.setText("Virtual Controller Test");
        title.setTextSize(24);
        layout.addView(title);

        EditText ipInput = new EditText(this);
        ipInput.setHint("Server IP Address");
        ipInput.setText("192.168.1.50"); // Change to your Linux IP
        ipInput.setGravity(Gravity.CENTER);
        layout.addView(ipInput);

        Button sendButton = new Button(this);
        sendButton.setText("Send Test Message");
        sendButton.setOnClickListener(v -> {
            String ip = ipInput.getText().toString();
            
            new Thread(() -> {
                boolean success = sendMessage(ip, "Hello from Android!");
                
                runOnUiThread(() -> {
                    Toast.makeText(this, 
                        success ? "Sent!" : "Failed", 
                        Toast.LENGTH_SHORT).show();
                });
            }).start();
        });
        layout.addView(sendButton);

        setContentView(layout);
    }
}