#ifndef TESTSCENE_H
#define TESTSCENE_H


#include <engine/core/scenewithrootobject.h>

class TestScene : public SceneWithRootObject
{
	virtual void makeOpenGLDependentSetup() override;
	virtual void onOpenGLResized(int width, int height) override;
};


#endif //TESTSCENE_H
