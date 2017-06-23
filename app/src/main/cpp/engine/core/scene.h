#ifndef SCENE_H
#define SCENE_H


class CoreEngine;

class Scene
{
public:
	virtual void makeOpenGLDependentSetup() = 0;
	virtual void onOpenGLResized(int width, int height) = 0;
	virtual void update() = 0;
	virtual void setEngine(CoreEngine *) = 0;
};


#endif // SCENE_H
