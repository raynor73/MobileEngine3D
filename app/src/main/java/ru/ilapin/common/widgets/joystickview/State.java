package ru.ilapin.common.widgets.joystickview;

import android.view.MotionEvent;

interface State {

	void onEnter();

	boolean onMotionEventDown(final MotionEvent event);

	boolean onMotionEventMove(final MotionEvent event);

	boolean onMotionEventUp(final MotionEvent event);

	boolean onMotionEventCancel(final MotionEvent event);
	
	void onHorizontalRecoilFinished();
	
	void onVerticalRecoilFinished();
}