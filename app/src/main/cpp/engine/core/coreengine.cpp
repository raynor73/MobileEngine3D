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

extern vector<unsigned int> g_indices;
/*static string loadShader(const string &path, const string &name)
{
	string shaderText;
	ifstream inputStream(path + name);
	string line;

	while (getline(inputStream, line)) {
		regex re("#include \"([a-z\\.]+)\"");
		smatch match;
		if (regex_search(line, match, re)) {
			shaderText.append(loadShader(path, match[1]));
		} else {
			shaderText.append(line).append("\n");
		}
		shaderText.append(line).append("\n");
	}

	inputStream.close();

	return shaderText;
}
void compileShader(const string &text, GLenum type)
{
	GLuint shaderReference = glCreateShader(type);
	if (shaderReference == 0) {
		Log::e("Shader", "Error creating shader");
		throw new runtime_error("Error creating shader");
	}

	GLint length = (GLint) text.length();
	const GLchar *rawText = (GLchar *) text.c_str();
	glShaderSource(shaderReference, 1, &rawText, &length);
	glCompileShader(shaderReference);
	if (RenderUtils::glGetShader(shaderReference, GL_COMPILE_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetShaderInfoLog(shaderReference));
		throw new runtime_error(RenderUtils::_glGetShaderInfoLog(shaderReference));
	}

	glAttachShader(g_programReference, shaderReference);
}
void linkProgram()
{
	glLinkProgram(g_programReference);
	if (RenderUtils::glGetProgram(g_programReference, GL_LINK_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetProgramInfoLog(g_programReference));
		throw new runtime_error(RenderUtils::_glGetProgramInfoLog(g_programReference));
	}

	glValidateProgram(g_programReference);
	if (RenderUtils::glGetProgram(g_programReference, GL_VALIDATE_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetProgramInfoLog(g_programReference));
		throw new runtime_error(RenderUtils::_glGetProgramInfoLog(g_programReference));
	}
}*/
void CoreEngine::onOpenGLReady()
{
	m_isOpenGLReady = true;

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
		//m_scene->rootGameObject().renderAll();
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
