package ru.ilapin.common.widgets.joystickview;

import android.view.MotionEvent;

class RecoilingState implements State {

	private final JoystickView mJoystickView;

	public RecoilingState(final JoystickView joystickView) {
		mJoystickView = joystickView;
	}

	@Override
	public void onEnter() {
		mJoystickView.getHorizontalRecoilAnimator().cancel();
		mJoystickView.getVerticalRecoilAnimator().cancel();

		mJoystickView.getHorizontalRecoilAnimator().setFloatValues(
				mJoystickView.getJoystickPositon().getX(),
				mJoystickView.calculateCenterX()
		);
		mJoystickView.getVerticalRecoilAnimator().setFloatValues(
				mJoystickView.getJoystickPositon().getY(),
				mJoystickView.calculateCenterY()
		);

		mJoystickView.getHorizontalRecoilAnimator().start();
		mJoystickView.getVerticalRecoilAnimator().start();
	}

	@Override
	public boolean onMotionEventDown(final MotionEvent event) {
		mJoystickView.changeState(mJoystickView.getDraggingState());
		return true;
	}

	@Override
	public boolean onMotionEventMove(final MotionEvent event) {
		return false;
	}

	@Override
	public boolean onMotionEventUp(final MotionEvent event) {
		return false;
	}

	@Override
	public boolean onMotionEventCancel(final MotionEvent event) {
		return false;
	}

	@Override
	public void onHorizontalRecoilFinished() {
		if (!mJoystickView.getVerticalRecoilAnimator().isStarted())
			mJoystickView.changeState(mJoystickView.getIdleState());
	}

	@Override
	public void onVerticalRecoilFinished() {
		if (!mJoystickView.getHorizontalRecoilAnimator().isStarted())
			mJoystickView.changeState(mJoystickView.getIdleState());
	}

	@Override
	public String toString() {
		return "RecoilingState";
	}
}
