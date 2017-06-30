package ru.ilapin.common.widgets.joystickview;

import android.animation.Animator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import io.reactivex.Observable;
import io.reactivex.subjects.BehaviorSubject;

public class JoystickView extends View {

	private static final String TAG = "JoystickView";

	private final NewState mNewState = new NewState();
	private final IdleState mIdleState;
	private final DraggingState mDraggingState;
	private final RecoilingState mRecoilingState;

	private State mState;

	private int mStickSize;

	private final Paint mBackgroundPaint = new Paint();
	private final Paint mStickPaint = new Paint();

	private JoystickPosition mJoystickPosition = new JoystickPosition();

	private final ValueAnimator mVerticalRecoilAnimator;
	private final ValueAnimator mHorizontalRecoilAnimator;

	private final BehaviorSubject<JoystickPosition> mPositionObservable = BehaviorSubject.create();

	public JoystickView(final Context context) {
		this(context, null);
	}

	public JoystickView(final Context context, @Nullable final AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public JoystickView(final Context context, @Nullable final AttributeSet attrs, final int defStyleAttr) {
		super(context, attrs, defStyleAttr);

		mBackgroundPaint.setColor(0x80ffffff);
		mBackgroundPaint.setStyle(Paint.Style.FILL);
		mStickPaint.setColor(0x80000000);
		mStickPaint.setStyle(Paint.Style.FILL);

		mVerticalRecoilAnimator = new ValueAnimator();
		mHorizontalRecoilAnimator = new ValueAnimator();

		mIdleState = new IdleState(this);
		mDraggingState = new DraggingState(this);
		mRecoilingState = new RecoilingState(this);

		changeState(mNewState);

		mHorizontalRecoilAnimator.addUpdateListener(animation -> {
			final JoystickPosition oldPosition = getJoystickPositon();
			final JoystickPosition newPosition = new JoystickPosition(
					((Float) animation.getAnimatedValue()).intValue(),
					oldPosition.getY()
			);
			setJoystickPosition(newPosition);
		});
		mHorizontalRecoilAnimator.addListener(new Animator.AnimatorListener() {
			@Override
			public void onAnimationStart(final Animator animation) {
				// do nothing
			}

			@Override
			public void onAnimationEnd(final Animator animation) {
				mState.onHorizontalRecoilFinished();
			}

			@Override
			public void onAnimationCancel(final Animator animation) {
				// do nothing
			}

			@Override
			public void onAnimationRepeat(final Animator animation) {
				// do nothing
			}
		});
		mVerticalRecoilAnimator.addUpdateListener(animation -> {
			final JoystickPosition oldPosition = getJoystickPositon();
			final JoystickPosition newPosition = new JoystickPosition(
					oldPosition.getX(),
					((Float) animation.getAnimatedValue()).intValue()
			);
			setJoystickPosition(newPosition);
		});
		mVerticalRecoilAnimator.addListener(new Animator.AnimatorListener() {
			@Override
			public void onAnimationStart(final Animator animation) {
				// do nothing
			}

			@Override
			public void onAnimationEnd(final Animator animation) {
				mState.onVerticalRecoilFinished();
			}

			@Override
			public void onAnimationCancel(final Animator animation) {
				// do nothing
			}

			@Override
			public void onAnimationRepeat(final Animator animation) {
				// do nothing
			}
		});
	}

	public Observable<JoystickPosition> getPositionObservable() {
		return mPositionObservable;
	}

	@Override
	protected void onMeasure(final int widthMeasureSpec, final int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);

		final int width = getMeasuredWidth();
		final int height = getMeasuredHeight();

		if (width != height)
			throw new RuntimeException("Width and height are not equal: " + width + "; " + height);

		mStickSize = width / 3;

		if (mState == mNewState) {
			changeState(mIdleState);
		}
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		final int action = event.getActionMasked();

		switch (action) {
			case MotionEvent.ACTION_DOWN:
				return mState.onMotionEventDown(event);

			case MotionEvent.ACTION_MOVE:
				return mState.onMotionEventMove(event);

			case MotionEvent.ACTION_UP:
				return mState.onMotionEventUp(event);

			case MotionEvent.ACTION_CANCEL:
				return mState.onMotionEventCancel(event);
		}

		return true;
	}

	@Override
	protected void onDraw(final Canvas canvas) {
		super.onDraw(canvas);

		canvas.drawRect(0, 0, getWidth() - 1, getWidth() - 1, mBackgroundPaint);
		canvas.drawRect(
				mJoystickPosition.getX(),
				mJoystickPosition.getY(),
				mJoystickPosition.getX() + mStickSize,
				mJoystickPosition.getY() + mStickSize,
				mStickPaint
		);
	}

	ValueAnimator getVerticalRecoilAnimator() {
		return mVerticalRecoilAnimator;
	}

	ValueAnimator getHorizontalRecoilAnimator() {
		return mHorizontalRecoilAnimator;
	}

	void moveStickToCenter() {
		mJoystickPosition.setX(calculateCenterX());
		mJoystickPosition.setY(calculateCenterY());

		notifyStickMoved();

		invalidate();
	}

	int calculateCenterY() {
		return getMeasuredHeight() / 2 - mStickSize / 2;
	}

	int calculateCenterX() {
		return getMeasuredWidth() / 2 - mStickSize / 2;
	}

	JoystickPosition getJoystickPositon() {
		return new JoystickPosition(mJoystickPosition);
	}

	void setJoystickPosition(final JoystickPosition position) {
		mJoystickPosition = new JoystickPosition(position);

		if (mJoystickPosition.getX() < 0)
			mJoystickPosition.setX(0);
		else if (mJoystickPosition.getX() + mStickSize >= getWidth())
			mJoystickPosition.setX(getWidth() - mStickSize);

		if (mJoystickPosition.getY() < 0)
			mJoystickPosition.setY(0);
		else if (mJoystickPosition.getY() + mStickSize >= getHeight())
			mJoystickPosition.setY(getHeight() - mStickSize);

		notifyStickMoved();

		invalidate();
	}

	DraggingState getDraggingState() {
		return mDraggingState;
	}

	IdleState getIdleState() {
		return mIdleState;
	}

	RecoilingState getRecoilingState() {
		return mRecoilingState;
	}

	void changeState(final State newState) {
		if (mState == newState)
			throw new RuntimeException("Already has state: " + mState);

		mState = newState;
		mState.onEnter();
	}

	private void notifyStickMoved() {
		if (mState == mNewState) {
			return;
		}

		final float actualWidth = getWidth();
		final float width = actualWidth == 0 ? getMeasuredWidth() : actualWidth;

		final float actualHeight = getHeight();
		final float height = actualHeight == 0 ? getMeasuredHeight() : actualHeight;

		final float alphaX = (mJoystickPosition.getX() / (width - mStickSize)) * 2 - 1;
		final float alphaY = (mJoystickPosition.getY() / (height - mStickSize)) * 2 - 1;
		mPositionObservable.onNext(new JoystickPosition(alphaX, alphaY));
	}
}
