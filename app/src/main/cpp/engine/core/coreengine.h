#ifndef COREENGINE_H
#define COREENGINE_H

#include <chrono>
#include <memory>
#include <string>

using namespace std;

//class RenderingEngine;
class SceneWithRootObject;
class UserInput;

class CoreEngine
{
public:
	CoreEngine(const string &shaderDirPath, UserInput &);

	CoreEngine(const CoreEngine &) = delete;
	CoreEngine &operator =(const CoreEngine &) = delete;

	UserInput &userInput() { return m_userInput; }
	//RenderingEngine &renderingEngine() { return *m_renderingEngine; }
	SceneWithRootObject *scene() { return m_scene; }
	float fps() const { return m_fps; }
	void setScene(SceneWithRootObject *);

	void onOpenGLReady();
	void onOpenGLResized(int width, int height);
	void onRender();

private:
	//shared_ptr<RenderingEngine> m_renderingEngine;
	SceneWithRootObject *m_scene;
	UserInput &m_userInput;
	string m_shadersDirPath;
	float m_fps;
	bool m_isOpenGLReady;
	bool m_isOpenGLSizeKnown;
	int m_openGLWidth;
	int m_openGLHeight;
	bool m_hasPrevTimePoint;
	std::chrono::high_resolution_clock::time_point m_prevTimePoint;
};


#endif //COREENGINE_H
