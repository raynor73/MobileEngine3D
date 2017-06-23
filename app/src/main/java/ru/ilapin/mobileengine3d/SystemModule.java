package ru.ilapin.mobileengine3d;

import android.content.Context;
import dagger.Module;
import dagger.Provides;

import javax.inject.Singleton;

@Module
public class SystemModule {

	private final Context mContext;

	public SystemModule(final Context context) {
		mContext = context;
	}

	@Provides
	@Singleton
	public Context provideContext() {
		return mContext;
	}
}
