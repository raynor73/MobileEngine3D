package ru.ilapin.mobileengine3d;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.util.Log;
import io.reactivex.subjects.BehaviorSubject;
import ru.ilapin.common.android.AndroidIOUtils;

import javax.inject.Inject;
import java.io.File;
import java.io.IOException;

public class FilesExtractor {

	private static final String TAG = "FilesExtractor";
	private static final String SHADERS_DIR_NAME = "shaders";
	private static final String TEXTURES_DIR_NAME = "textures";
	private static final String MODELS_DIR_NAME = "models";

	public enum State { CHECKING, EXTRACTING, COMPLETED }

	private final Context mContext;
	private final File mShadersDir;
	private final File mTexturesDir;
	private final File mModelsDir;
	private final BehaviorSubject<State> mStateObservable = BehaviorSubject.create();

	private State mState;

	@Inject
	FilesExtractor(final Context context) {
		mContext = context;
		mShadersDir = mContext.getDir(SHADERS_DIR_NAME, Context.MODE_PRIVATE);
		mTexturesDir = mContext.getDir(TEXTURES_DIR_NAME, Context.MODE_PRIVATE);
		mModelsDir = mContext.getDir(MODELS_DIR_NAME, Context.MODE_PRIVATE);

		checkFiles();
	}

	public BehaviorSubject<State> getStateObservable() {
		return mStateObservable;
	}

	public File getShadersDir() {
		return mShadersDir;
	}

	public File getTexturesDir() {
		return mTexturesDir;
	}

	public File getModelsDir() {
		return mModelsDir;
	}

	private void changeState(final State newState) {
		if (newState == mState)
			throw new RuntimeException("Already has " + mState + " state");

		mState = newState;
		mStateObservable.onNext(mState);
	}

	private void checkFiles() {
		changeState(State.CHECKING);
		new CheckingFilesTask().execute();
	}

	private void extractFiles() {
		changeState(State.EXTRACTING);
		new ExtractingFilesTask().execute();
	}

	private class CheckingFilesTask extends AsyncTask<Void, Void, Boolean> {

		@Override
		protected Boolean doInBackground(final Void... params) {
			try {
				return isExtractionRequiredForDirName(SHADERS_DIR_NAME, getShadersDir()) ||
						isExtractionRequiredForDirName(TEXTURES_DIR_NAME, getTexturesDir()) ||
						isExtractionRequiredForDirName(MODELS_DIR_NAME, getModelsDir());
			} catch (final IOException e) {
				throw new RuntimeException(e);
			}
		}

		@Override
		protected void onPostExecute(final Boolean extractionRequired) {
			if (extractionRequired)
				extractFiles();
			else
				changeState(State.COMPLETED);
		}

		private boolean isExtractionRequiredForDirName(final String dirName, final File destFilesDir) throws IOException {
			final AssetManager assetManager = mContext.getAssets();
			final String[] filenames = assetManager.list(dirName);

			for (final String filename : filenames) {
				final File destFile = new File(destFilesDir, filename);

				final String srcPath = dirName + File.separator + filename;
				final long assetsFileSize = ru.ilapin.common.android.AndroidIOUtils.calculateFileSizeFromAssets(mContext, srcPath);
				if (!destFile.exists() || destFile.length() != assetsFileSize) {
					Log.d(TAG, "Check failed for file " + filename);
					return true;
				}
			}

			return false;
		}
	}

	private class ExtractingFilesTask extends AsyncTask<Void, Void, Void> {

		@Override
		protected Void doInBackground(final Void... params) {
			try {
				extractDirToDir(SHADERS_DIR_NAME, mShadersDir);
				extractDirToDir(TEXTURES_DIR_NAME, mTexturesDir);
				extractDirToDir(MODELS_DIR_NAME, mModelsDir);
			} catch (final IOException e) {
				throw new RuntimeException(e);
			}

			return null;
		}

		@Override
		protected void onPostExecute(final Void aVoid) {
			changeState(State.COMPLETED);
		}

		private void extractDirToDir(final String srcDirName, final File destDir) throws IOException {
			final AssetManager assetManager = mContext.getAssets();
			final String[] srcFilenames = assetManager.list(srcDirName);

			for (final String filename : srcFilenames) {
				Log.d(TAG, "Extracting " + filename);
				final File destFile =  new File(destDir, filename);
				AndroidIOUtils.copyFileFromAssetsToFileSystem(
						mContext,
						srcDirName + File.separator + filename,
						destFile.getPath()
				);
			}
		}
	}
}
