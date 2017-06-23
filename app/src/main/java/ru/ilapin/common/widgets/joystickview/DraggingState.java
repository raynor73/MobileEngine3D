package ru.ilapin.common.widgets.joystickview;

import android.view.MotionEvent;

class DraggingState implements State {

	private static final String TAG = "DraggingState";

	private final JoystickView mJoystickView;

	private boolean mHasPrevCoordinate;
	private float mPrevX;
	private float mPrevY;

	DraggingState(final JoystickView joystickView) {
		mJoystickView = joystickView;
	}

	@Override
	public void onEnter() {
		mJoystickView.getVerticalRecoilAnimator().cancel();
		mJoystickView.getHorizontalRecoilAnimator().cancel();
		mHasPrevCoordinate = false;
	}

	@Override
	public boolean onMotionEventDown(final MotionEvent event) {
		return false;
	}

	@Override
	public boolean onMotionEventMove(final MotionEvent event) {
		final float x = event.getX();
		final float y = event.getY();

		if (mHasPrevCoordinate) {
			final float dx = x - mPrevX;
			final float dy = y - mPrevY;

			final JoystickPosition position = mJoystickView.getJoystickPositon();
			mJoystickView.setJoystickPosition(new JoystickPosition(position.getX() + dx, position.getY() + dy));
		}

		mHasPrevCoordinate = true;
		mPrevX = x;
		mPrevY = y;

		return true;
	}

	@Override
	public boolean onMotionEventUp(final MotionEvent event) {
		mJoystickView.changeState(mJoystickView.getRecoilingState());

		return true;
	}

	@Override
	public boolean onMotionEventCancel(final MotionEvent event) {
		mJoystickView.changeState(mJoystickView.getRecoilingState());

		return true;
	}

	@Override
	public void onHorizontalRecoilFinished() {
		// do nothing
	}

	@Override
	public void onVerticalRecoilFinished() {
		// do nothing
	}

	@Override
	public String toString() {
		return "DraggingState";
	}
}
