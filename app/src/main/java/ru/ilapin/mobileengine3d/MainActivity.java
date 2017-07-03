package ru.ilapin.mobileengine3d;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.ViewGroup;
import butterknife.BindView;
import butterknife.ButterKnife;
import ru.ilapin.common.widgets.joystickview.JoystickPosition;
import ru.ilapin.common.widgets.joystickview.JoystickView;

import javax.inject.Inject;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.io.File;

import static android.opengl.GLSurfaceView.DEBUG_CHECK_GL_ERROR;

public class MainActivity extends AppCompatActivity {

	private static final String TAG = "MainActivity";

	static {
		System.loadLibrary("native-main");
	}

	@BindView(R.id.container)
	ViewGroup mContainerViewGroup;
	@BindView(R.id.rightJoystick)
	JoystickView mRightJoystickView;
	@BindView(R.id.leftJoystick)
	JoystickView mLeftJoystickView;

	@Inject
	FilesExtractor mFilesExtractor;

	GLSurfaceView mGLSurfaceView;

	private boolean mIsRendererSet;

	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		ButterKnife.bind(this);
		App.getApplicationComponent().inject(this);

		mFilesExtractor.getStateObservable().subscribe(state -> {
			switch (state) {
				case CHECKING:
				case EXTRACTING:
					if (mGLSurfaceView != null) {
						mContainerViewGroup.removeView(mGLSurfaceView);
						mGLSurfaceView = null;
						mIsRendererSet = false;
					}
					break;

				case COMPLETED:
					mGLSurfaceView = new GLSurfaceView(this);
					mContainerViewGroup.addView(mGLSurfaceView, 0);

					final ActivityManager activityManager =
							(ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
					final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

					if (configurationInfo.reqGlEsVersion >= 0x20000) {
						initEngine(new String[] {
								mFilesExtractor.getShadersDir().getPath() + File.separator,
								mFilesExtractor.getTexturesDir().getPath() + File.separator + "bricks2.jpg",
								mFilesExtractor.getTexturesDir().getPath() + File.separator + "test.jpg",
								mFilesExtractor.getModelsDir().getPath() + File.separator + "monkey2.obj",
								mFilesExtractor.getTexturesDir().getPath() + File.separator + "bricks2_normal.jpg",
								mFilesExtractor.getModelsDir().getPath() + File.separator + "plane3.obj",
								mFilesExtractor.getTexturesDir().getPath() + File.separator + "default_normal.jpg",
								mFilesExtractor.getTexturesDir().getPath() + File.separator + "default_displacement.jpg",
								mFilesExtractor.getTexturesDir().getPath() + File.separator + "bricks2_displacement.jpg",
						});

						mGLSurfaceView.setEGLContextClientVersion(2);
						mGLSurfaceView.setDebugFlags(DEBUG_CHECK_GL_ERROR);
						mGLSurfaceView.setRenderer(new GLSurfaceView.Renderer() {
							@Override
							public void onSurfaceCreated(final GL10 gl10, final EGLConfig eglConfig) {
								MainActivity.this.onSurfaceCreated();
							}

							@Override
							public void onSurfaceChanged(final GL10 gl10, final int width, final int height) {
								MainActivity.this.onSurfaceChanged(width, height);
							}

							@Override
							public void onDrawFrame(final GL10 gl10) {
								MainActivity.this.onDrawFrame();
							}
						});
						mIsRendererSet = true;

						mRightJoystickView.getPositionObservable().subscribe(joystickPosition ->
								mGLSurfaceView.queueEvent(() -> onRightJoystickPositionChanged(joystickPosition)));

						mLeftJoystickView.getPositionObservable().subscribe(joystickPosition ->
								mGLSurfaceView.queueEvent(() -> onLeftJoystickPositionChanged(joystickPosition)));
					}
					break;
			}
		});
	}

	@Override
	protected void onResume() {
		super.onResume();

		if (mIsRendererSet)
			mGLSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();

		if (mIsRendererSet)
			mGLSurfaceView.onPause();
	}

	public native void initEngine(String[] paths);

	public native void onRightJoystickPositionChanged(JoystickPosition position);

	public native void onLeftJoystickPositionChanged(JoystickPosition position);

	public native void onSurfaceCreated();

	public native void onSurfaceChanged(int width, int height);

	public native void onDrawFrame();
}
