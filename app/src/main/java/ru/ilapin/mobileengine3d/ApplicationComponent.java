package ru.ilapin.mobileengine3d;

import dagger.Component;

import javax.inject.Singleton;

@Singleton
@Component(modules = {SystemModule.class, FilesExtractorModule.class})
public interface ApplicationComponent {

	void inject(MainActivity activity);
}
