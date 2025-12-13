package com.controller;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.MotionEvent;
import android.view.View;

public class JoystickView extends View {
  private Paint basePaint;
  private Paint stickPaint;
  private float baseRadius = 150f;
  private float stickRadius = 60f;
  private float centerX;
  private float centerY;
  private float stickX;
  private float stickY;
  private int maxDistance = 100;

  private OnJoystickMoveListener listener;

  public interface OnJoystickMoveListener {
    void onJoystickMove(int x, int y);
  }

  public JoystickView(Context context) {
    super(context);
    init();
  }

  private void init() {
    basePaint = new Paint();
    basePaint.setColor(Color.DKGRAY);
    basePaint.setStyle(Paint.Style.FILL);
    basePaint.setAlpha(128);

    stickPaint = new Paint();
    stickPaint.setColor(Color.LTGRAY);
    stickPaint.setStyle(Paint.Style.FILL);

    // Start at center
    stickX = 0;
    stickY = 0;
  }

  public void setOnJoystickMoveListener(OnJoystickMoveListener listener) {
    this.listener = listener;
  }

  @Override
  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    super.onSizeChanged(w, h, oldw, oldh);
    centerX = w / 2f;
    centerY = h / 2f;
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);

    // Draw base circle
    canvas.drawCircle(centerX, centerY, baseRadius, basePaint);

    // Draw stick
    canvas.drawCircle(centerX + stickX, centerY + stickY, stickRadius, stickPaint);
  }

  @Override
  public boolean onTouchEvent(MotionEvent event) {
    float touchX = event.getX() - centerX;
    float touchY = event.getY() - centerY;

    switch (event.getAction()) {
      case MotionEvent.ACTION_DOWN:
      case MotionEvent.ACTION_MOVE:
        // Calculate distance from center
        float distance = (float) Math.sqrt(touchX * touchX + touchY * touchY);

        // Limit stick movement to maxDistance
        if (distance > maxDistance) {
          float ratio = maxDistance / distance;
          stickX = touchX * ratio;
          stickY = touchY * ratio;
        } else {
          stickX = touchX;
          stickY = touchY;
        }

        // Notify listener with normalized values (-32768 to 32767)
        if (listener != null) {
          int normalizedX = (int) ((stickX / maxDistance) * 32767);
          int normalizedY = (int) (-(stickY / maxDistance) * 32767); // Invert Y for standard joystick
          listener.onJoystickMove(normalizedX, normalizedY);
        }
        break;

      case MotionEvent.ACTION_UP:
      case MotionEvent.ACTION_CANCEL:
        // Reset to center
        stickX = 0;
        stickY = 0;
        if (listener != null) {
          listener.onJoystickMove(0, 0);
        }
        break;
    }

    invalidate();
    return true;
  }

  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    int size = (int) (baseRadius * 2 + 100);
    setMeasuredDimension(size, size);
  }
}
