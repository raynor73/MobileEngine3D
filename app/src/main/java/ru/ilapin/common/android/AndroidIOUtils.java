package ru.ilapin.common.android;

import android.content.Context;

import java.io.*;

public class AndroidIOUtils {

	public static void copyFileFromAssetsToFileSystem(final Context context, final String srcPath,
			final String destPath) throws IOException {
		final File destFile = new File(destPath);
		final InputStream is = context.getAssets().open(srcPath);
		final OutputStream os = new FileOutputStream(destFile);
		final byte[] buffer = new byte[102400];

		int bytesRead;
		while ((bytesRead = is.read(buffer)) > 0) {
			os.write(buffer, 0, bytesRead);
		}

		is.close();
		os.close();
	}

	public static long calculateFileSizeFromAssets(final Context context, final String path) throws IOException {
		long size = 0;
		final InputStream is = context.getAssets().open(path);
		final byte[] buffer = new byte[102400];

		int bytesRead;
		while ((bytesRead = is.read(buffer)) > 0) {
			size += bytesRead;
		}

		is.close();
		return size;
	}
}
