package ru.ilapin.common.widgets.joystickview;

public class JoystickPosition {

	private float mX;
	private float mY;

	public JoystickPosition() {
		mX = 0;
		mY = 0;
	}

	public JoystickPosition(final float x, final float y) {
		mX = x;
		mY = y;
	}

	public JoystickPosition(final JoystickPosition other) {
		mX = other.mX;
		mY = other.mY;
	}

	public float getX() {
		return mX;
	}

	public float getY() {
		return mY;
	}

	public void setX(final float x) {
		mX = x;
	}

	public void setY(final float y) {
		mY = y;
	}
}
