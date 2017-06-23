package ru.ilapin.common.widgets.joystickview;

import android.view.MotionEvent;

class IdleState implements State {

	private final JoystickView mJoystickView;
	//private final float mTouchSlopSquared;

	IdleState(final JoystickView joystickView) {
		mJoystickView = joystickView;
		//mTouchSlopSquared = (float) Math.pow(ViewConfiguration.get(mJoystickView.getContext()).getScaledTouchSlop(), 2);
	}

	@Override
	public void onEnter() {
		mJoystickView.getVerticalRecoilAnimator().cancel();
		mJoystickView.getHorizontalRecoilAnimator().cancel();
		mJoystickView.moveStickToCenter();
	}

	@Override
	public boolean onMotionEventDown(final MotionEvent event) {
		mJoystickView.changeState(mJoystickView.getDraggingState());
		return true;
	}

	@Override
	public boolean onMotionEventMove(final MotionEvent event) {
		return false;
		/*if (event.getHistorySize() >= 1) {
			final float x = event.getX();
			final float y = event.getY();
			final float prevX = event.getHistoricalX(0);
			final float prevY = event.getHistoricalY(0);

			final float dx = x - prevX;
			final float dy = y - prevY;

			if (dx * dx + dy * dy > mTouchSlopSquared)
				mJoystickView.getStateMachine().changeState(new DraggingState());
		}*/
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
		// do nothing
	}

	@Override
	public void onVerticalRecoilFinished() {
		// do nothing
	}

	@Override
	public String toString() {
		return "IdleState";
	}
}