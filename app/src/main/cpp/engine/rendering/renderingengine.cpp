#include "renderingengine.h"
#include <engine/components/baselight.h>
#include <logwrapper.h>

const string RenderingEngine::TAG = "RenderingEngine";

RenderingEngine::RenderingEngine(const string &shadersDirPath) :
		m_mainCamera(nullptr),
		m_shadersDirPath(shadersDirPath)
{
	m_samplerMap["diffuse"] = 0;
	m_samplerMap["normalMap"] = 1;
	m_vectors3f["ambient"] = Vector3f(0.1, 0.1, 0.1);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
#ifndef __ANDROID__
	glEnable(GL_DEPTH_CLAMP);
#endif

#ifdef __ANDROID__
	m_forwardAmbientShader = make_shared<Shader>(m_shadersDirPath, "forwardambient");
#else
	m_forwardAmbientShader = make_shared<Shader>(m_shadersDirPath, "forwardambient", m_vertexArrayName);
	glGenVertexArrays(1, &m_vertexArrayName);
#endif
}

RenderingEngine::~RenderingEngine()
{
#ifndef __ANDROID__
	glDeleteVertexArrays(1, &m_vertexArrayName);
#endif
}

void RenderingEngine::updateUniformStruct(Transform &, Material &, Shader &, const string &uniformType,
										  const string &uniformName) {
	Log::e("RenderingEngine", "Uniform " + uniformName + " of type " + uniformType + " is not supported");
	throw new runtime_error("Uniform " + uniformName + " of type " + uniformType + " is not supported");
}

void RenderingEngine::onOpenGLResized(GameObject &gameObject, int width, int height)
{
	gameObject.onOpenGLResizedAll(width, height);
}

void RenderingEngine::render(GameObject &gameObject)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gameObject.renderAll(*m_forwardAmbientShader, *this);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);
	glBlendFunc(GL_ONE, GL_ONE);

	for (auto light : m_lights) {
		m_activeLight = light;
		gameObject.renderAll(*m_activeLight->shader(), *this);
	}

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
}

Camera &RenderingEngine::mainCamera() const
{
	if (m_mainCamera == nullptr) {
		Log::e(TAG, "No main camera");
		throw new runtime_error("No main camera");
	}

	return *m_mainCamera;
}

void RenderingEngine::setClearColor(const Vector3f &color)
{
	glClearColor(color.x(), color.y(), color.z(), 1);
}

void RenderingEngine::enableTextures(bool isEnabled)
{
	if (isEnabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

string RenderingEngine::openGLVersion()
{
	return string(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}

void RenderingEngine::unbindTextures()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
