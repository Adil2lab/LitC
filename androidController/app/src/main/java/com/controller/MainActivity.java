package com.controller;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.LinearLayout;
import android.view.Gravity;
import android.widget.TextView;
import android.os.Handler;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("controller");
    }

    private native boolean initNetwork(String serverIp);

    private native boolean sendControllerData(int buttons, int leftX, int leftY, int rightX, int rightY,
            int leftTrigger, int rightTrigger);

    private native void closeNetwork();

    private Handler handler = new Handler();
    private boolean isSending = false;

    // Joystick state
    private int leftStickX = 0;
    private int leftStickY = 0;
    private int rightStickX = 0;
    private int rightStickY = 0;
    private int currentButtons = 0;

    // Button Flags ( Must match C++ Enum )
    private static final int A = 1 << 0;
    private static final int B = 1 << 1;
    private static final int X = 1 << 2;
    private static final int Y = 1 << 3;
    private static final int L1 = 1 << 4;
    private static final int R1 = 1 << 5;
    private static final int L2 = 1 << 6;
    private static final int R2 = 1 << 7;
    private static final int SELECT = 1 << 8;
    private static final int START = 1 << 9;
    private static final int LEFT = 1 << 10;
    private static final int RIGHT = 1 << 11;
    private static final int UP = 1 << 12;
    private static final int DOWN = 1 << 13;
    private static final int LEFT_STICK_DOWN = 1 << 14;
    private static final int RIGHT_STICK_DOWN = 1 << 15;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Create UI programmatically (no XML needed!)
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setPadding(50, 50, 50, 50);
        layout.setGravity(Gravity.CENTER);

        TextView title = new TextView(this);
        title.setText("Virtual Controller : V0.0.2");
        title.setTextSize(24);
        layout.addView(title);

        EditText ipInput = new EditText(this);
        ipInput.setHint("Server IP Address");
        ipInput.setText("192.168.0.11"); // Change to your Linux IP
        ipInput.setGravity(Gravity.CENTER);
        layout.addView(ipInput);

        Button connectBTN = new Button(this);
        connectBTN.setText("Connect");
        connectBTN.setOnClickListener(v -> {
            String ip = ipInput.getText().toString();
            if (initNetwork(ip)) {
                Toast.makeText(this, "Connected!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this, "Failed to connect!", Toast.LENGTH_SHORT).show();
            }
        });
        layout.addView(connectBTN);

        Button disconnectBTN = new Button(this);
        disconnectBTN.setText("Disconnect");
        disconnectBTN.setOnClickListener(v -> {
            closeNetwork();
            Toast.makeText(this, "Disconnected!", Toast.LENGTH_SHORT).show();
        });
        layout.addView(disconnectBTN);

        Button BTN_A = new Button(this);
        BTN_A.setText("A");
        BTN_A.setOnClickListener(v -> sendButtonPress(A));

        layout.addView(BTN_A);

        Button BTN_B = new Button(this);
        BTN_B.setText("B");
        BTN_B.setOnClickListener(v -> sendButtonPress(B));

        layout.addView(BTN_B);

        Button BTN_X = new Button(this);
        BTN_X.setText("X");
        BTN_X.setOnClickListener(v -> sendButtonPress(X));

        layout.addView(BTN_X);

        Button BTN_Y = new Button(this);
        BTN_Y.setText("Y");
        BTN_Y.setOnClickListener(v -> sendButtonPress(Y));

        layout.addView(BTN_Y);

        Button BTN_L1 = new Button(this);
        BTN_L1.setText("L1");
        BTN_L1.setOnClickListener(v -> sendButtonPress(L1));

        layout.addView(BTN_L1);

        Button BTN_R1 = new Button(this);
        BTN_R1.setText("R1");
        BTN_R1.setOnClickListener(v -> sendButtonPress(R1));

        layout.addView(BTN_R1);

        Button BTN_L2 = new Button(this);
        BTN_L2.setText("L2");
        BTN_L2.setOnClickListener(v -> sendButtonPress(L2));

        layout.addView(BTN_L2);

        Button BTN_R2 = new Button(this);
        BTN_R2.setText("R2");
        BTN_R2.setOnClickListener(v -> sendButtonPress(R2));

        layout.addView(BTN_R2);

        Button BTN_SELECT = new Button(this);
        BTN_SELECT.setText("SELECT");
        BTN_SELECT.setOnClickListener(v -> sendButtonPress(SELECT));

        layout.addView(BTN_SELECT);

        Button BTN_START = new Button(this);
        BTN_START.setText("START");
        BTN_START.setOnClickListener(v -> sendButtonPress(START));

        layout.addView(BTN_START);

        Button BTN_LEFT = new Button(this);
        BTN_LEFT.setText("LEFT");
        BTN_LEFT.setOnClickListener(v -> sendButtonPress(LEFT));

        layout.addView(BTN_LEFT);

        Button BTN_RIGHT = new Button(this);
        BTN_RIGHT.setText("RIGHT");
        BTN_RIGHT.setOnClickListener(v -> sendButtonPress(RIGHT));

        layout.addView(BTN_RIGHT);

        Button BTN_UP = new Button(this);
        BTN_UP.setText("UP");
        BTN_UP.setOnClickListener(v -> sendButtonPress(UP));

        layout.addView(BTN_UP);

        Button BTN_DOWN = new Button(this);
        BTN_DOWN.setText("DOWN");
        BTN_DOWN.setOnClickListener(v -> sendButtonPress(DOWN));

        layout.addView(BTN_DOWN);

        Button BTN_LEFT_STICK_DOWN = new Button(this);
        BTN_LEFT_STICK_DOWN.setText("LS");
        BTN_LEFT_STICK_DOWN.setOnClickListener(v -> sendButtonPress(LEFT_STICK_DOWN));

        layout.addView(BTN_LEFT_STICK_DOWN);

        Button BTN_RIGHT_STICK_DOWN = new Button(this);
        BTN_RIGHT_STICK_DOWN.setText("RS");
        BTN_RIGHT_STICK_DOWN.setOnClickListener(v -> sendButtonPress(RIGHT_STICK_DOWN));

        layout.addView(BTN_RIGHT_STICK_DOWN);

        // Joystick section
        TextView joystickTitle = new TextView(this);
        joystickTitle.setText("Joysticks");
        joystickTitle.setTextSize(18);
        joystickTitle.setPadding(0, 20, 0, 10);
        layout.addView(joystickTitle);

        // Container for joysticks side by side
        LinearLayout joystickContainer = new LinearLayout(this);
        joystickContainer.setOrientation(LinearLayout.HORIZONTAL);
        joystickContainer.setGravity(Gravity.CENTER);

        // Left joystick
        LinearLayout leftJoystickLayout = new LinearLayout(this);
        leftJoystickLayout.setOrientation(LinearLayout.VERTICAL);
        leftJoystickLayout.setGravity(Gravity.CENTER);
        leftJoystickLayout.setPadding(10, 10, 10, 10);

        TextView leftJoystickLabel = new TextView(this);
        leftJoystickLabel.setText("Left Stick");
        leftJoystickLayout.addView(leftJoystickLabel);

        JoystickView leftJoystick = new JoystickView(this);
        leftJoystick.setOnJoystickMoveListener((x, y) -> {
            leftStickX = x;
            leftStickY = y;
        });
        leftJoystickLayout.addView(leftJoystick);

        joystickContainer.addView(leftJoystickLayout);

        // Right joystick
        LinearLayout rightJoystickLayout = new LinearLayout(this);
        rightJoystickLayout.setOrientation(LinearLayout.VERTICAL);
        rightJoystickLayout.setGravity(Gravity.CENTER);
        rightJoystickLayout.setPadding(10, 10, 10, 10);

        TextView rightJoystickLabel = new TextView(this);
        rightJoystickLabel.setText("Right Stick");
        rightJoystickLayout.addView(rightJoystickLabel);

        JoystickView rightJoystick = new JoystickView(this);
        rightJoystick.setOnJoystickMoveListener((x, y) -> {
            rightStickX = x;
            rightStickY = y;
        });
        rightJoystickLayout.addView(rightJoystick);

        joystickContainer.addView(rightJoystickLayout);

        layout.addView(joystickContainer);

        // Start continuous sending loop (60Hz)
        startSendingLoop();

        setContentView(layout);
    }

    private void startSendingLoop() {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                // Send current state at 60Hz
                sendControllerData(currentButtons, leftStickX, leftStickY,
                        rightStickX, rightStickY, 0, 0);
                handler.postDelayed(this, 16); // ~60 FPS
            }
        }, 16);
    }

    private void sendButtonPress(int button) {
        currentButtons = button;
        sendControllerData(currentButtons, leftStickX, leftStickY,
                rightStickX, rightStickY, 0, 0);
        // Clear button after short delay
        handler.postDelayed(() -> currentButtons = 0, 100);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        handler.removeCallbacksAndMessages(null);
        closeNetwork();
    }
}