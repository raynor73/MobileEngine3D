#include <glwrapper.h>
#include <logwrapper.h>
#include <fstream>
#include <engine/rendering/renderutils.h>
#include <engine/rendering/vertex.h>
#include <vector>
#include "coreengine.h"
#include <engine/rendering/renderingengine.h>
#include "scenewithrootobject.h"
#include "matrix4f.h"
#include <engine/core/gameobject.h>
#include <engine/rendering/mesh.h>
#include <engine/rendering/shader.h>

using namespace std;

CoreEngine::CoreEngine(const string &shadersDirPath, UserInput &userInput) :
	m_scene(nullptr),
	m_userInput(userInput),
	m_shadersDirPath(shadersDirPath),
	m_fps(0),
	m_isOpenGLReady(false),
	m_isOpenGLSizeKnown(false),
	m_openGLWidth(0),
	m_openGLHeight(0),
	m_hasPrevTimePoint(false)
{}

void CoreEngine::onOpenGLReady()
{
	Mesh::clearLoadedModels();
	Shader::clearLoadedShaders();
	// TODO Clear loaded textures

	m_isOpenGLReady = true;

	// Call destructors with glDelete* calls before allocating new OpenGL resources which possibly have the same
	// indexes/references.
	// TODO Improve that stinky code
	m_renderingEngine.reset();
	m_renderingEngine = make_shared<RenderingEngine>(m_shadersDirPath);

	if (m_scene != nullptr)
		m_scene->makeOpenGLDependentSetup();
}

void CoreEngine::onOpenGLResized(int width, int height)
{
	m_isOpenGLSizeKnown = true;
	m_openGLWidth = width;
	m_openGLHeight = height;

	if (m_scene != nullptr) {
		m_renderingEngine->onOpenGLResized(m_scene->rootGameObject(), m_openGLWidth, m_openGLHeight);
		m_scene->onOpenGLResized(width, height);
	}
}

void CoreEngine::onRender()
{
	auto currentTime = std::chrono::high_resolution_clock::now();

	if (m_hasPrevTimePoint) {
		std::chrono::duration<float> dt;
		dt = currentTime - m_prevTimePoint;
		m_prevTimePoint = currentTime;
		m_fps = 1.0f / dt.count();
	} else {
		m_hasPrevTimePoint = true;
		m_prevTimePoint = currentTime;
	}

	if (m_scene != nullptr) {
		m_scene->update();
		m_renderingEngine->render(m_scene->rootGameObject());
	}
}

void CoreEngine::setScene(SceneWithRootObject *scene)
{
	m_scene = scene;
	if (m_scene == nullptr)
		return;

	m_scene->setEngine(this);

	if (m_isOpenGLReady)
		m_scene->makeOpenGLDependentSetup();

	if (m_isOpenGLSizeKnown) {
		m_renderingEngine->onOpenGLResized(m_scene->rootGameObject(), m_openGLWidth, m_openGLHeight);
		m_scene->onOpenGLResized(m_openGLWidth, m_openGLHeight);
	}
}
