package ru.ilapin.common.widgets.joystickview;

import android.view.MotionEvent;

class NewState implements State {

	@Override
	public void onEnter() {
		// do nothing
	}

	@Override
	public boolean onMotionEventDown(final MotionEvent event) {
		return false;
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
		// do nothing
	}

	@Override
	public void onVerticalRecoilFinished() {
		// do nothing
	}

	@Override
	public String toString() {
		return "NewState";
	}
}
