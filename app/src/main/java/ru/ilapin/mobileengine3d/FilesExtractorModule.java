package ru.ilapin.mobileengine3d;

import android.content.Context;
import dagger.Module;
import dagger.Provides;

import javax.inject.Singleton;

@Module
public class FilesExtractorModule {

	@Provides
	@Singleton
	public FilesExtractor provideFilesExtractor(final Context context) {
		return new FilesExtractor(context);
	}
}
