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

public class MainActivity extends AppCompatActivity {

	static {
		System.loadLibrary("native-main");
	}

	@BindView(R.id.container)
	ViewGroup mContainerViewGroup;
	@BindView(R.id.joystick)
	JoystickView mJoystickView;

	@Inject
	FilesExtractor mFilesExtractor;

	GLSurfaceView mGlSurfaceView;

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
					if (mGlSurfaceView != null) {
						mContainerViewGroup.removeView(mGlSurfaceView);
						mGlSurfaceView = null;
						mIsRendererSet = false;
					}
					break;

				case COMPLETED:
					mGlSurfaceView = new GLSurfaceView(this);
					mContainerViewGroup.addView(mGlSurfaceView, 0);

					final ActivityManager activityManager =
							(ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
					final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

					if (configurationInfo.reqGlEsVersion >= 0x20000) {
						mGlSurfaceView.setEGLContextClientVersion(2);
						mGlSurfaceView.setRenderer(new GLSurfaceView.Renderer() {
							@Override
							public void onSurfaceCreated(final GL10 gl10, final EGLConfig eglConfig) {
								initEngine(
										/*mFilesExtractor.getShadersDir().getPath() + File.separator,
										mFilesExtractor.getTexturesDir().getPath() + File.separator + "brick.jpg"*/
								);

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

						mJoystickView.getPositionObservable().subscribe(this::onJoystickPositionChanged);
					}
					break;
			}
		});
	}

	@Override
	protected void onResume() {
		super.onResume();

		if (mIsRendererSet)
			mGlSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();

		if (mIsRendererSet)
			mGlSurfaceView.onPause();
	}

	public native void initEngine(/*String shaderDirPath, String brickTexturePath*/);

	public native void onJoystickPositionChanged(JoystickPosition position);

	public native void onSurfaceCreated();

	public native void onSurfaceChanged(int width, int height);

	public native void onDrawFrame();
}
